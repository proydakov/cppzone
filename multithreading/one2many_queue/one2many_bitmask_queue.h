#include <cmath>
#include <atomic>
#include <memory>
#include <optional>
#include <type_traits>

enum : std::uint64_t { QUEUE_CPU_CACHE_LINE_SIZE = 64 };

template<typename T>
struct one2many_bitmask_queue_impl
{
    enum : T { MIN_READER_ID = 0 };
    enum : T { MAX_READER_ID = sizeof(T) * 8 - 2 };
    enum : T { MIN_EVENT_SEQ_NUM = 1 };
    enum : T { DUMMY_EVENT_SEQ_NUM = 0 };
    enum : T { EMPTY_DATA_MASK = 0 };
    enum : T { CONSTRUCTED_MASK = T(1) << (MAX_READER_ID + 1) };
};

// predeclaration

// queue
template<class event_t, typename bitmask_t>
class one2many_bitmask_queue;

// reader
template<class event_t, typename bitmask_t>
class one2many_bitmask_reader;

// guard
template<class event_t, typename bitmask_t>
class one2many_bitmask_guard;

// bucket
template<class event_t, typename bitmask_t>
struct one2many_bitmask_bucket;

// implementation

// guard
template<class event_t, typename bitmask_t>
class one2many_bitmask_guard
{
public:
    // ctor
    one2many_bitmask_guard(one2many_bitmask_bucket<event_t, bitmask_t>& bucket, bitmask_t mask) : m_bucket(&bucket), m_mask(mask)
    {
    }

    // move ctor and assign
    one2many_bitmask_guard(one2many_bitmask_guard&& data) : m_bucket(data.m_bucket), m_mask(data.m_mask)
    {
        data.m_mask = one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK;
    }

    one2many_bitmask_guard& operator=(one2many_bitmask_guard&& data)
    {
        if (&data != this)
        {
            m_bucket = data.m_bucket;
            m_mask = data.m_mask;
            data.m_mask = one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK;
        }
        return *this;
    }

    // copy ctor and assign
    one2many_bitmask_guard(const one2many_bitmask_guard&) = delete;
    one2many_bitmask_guard& operator=(const one2many_bitmask_guard&) = delete;

    // dtor
    ~one2many_bitmask_guard()
    {
        if (m_mask != one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK)
        {
            auto const release_etalon(m_mask | one2many_bitmask_queue_impl<bitmask_t>::CONSTRUCTED_MASK);
            auto const before = m_bucket->m_mask.fetch_and(~m_mask, std::memory_order_consume);

            if (before == release_etalon)
            {
                m_bucket->get_event().~event_t();
                m_bucket->m_mask.store(one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK, std::memory_order_release);
            }
        }
    }

    event_t const& get_event() const
    {
        return m_bucket->get_event();
    }

private:
    one2many_bitmask_bucket<event_t, bitmask_t>* m_bucket;
    bitmask_t m_mask;
};

// bucket
template<class event_t, typename bitmask_t>
struct alignas(QUEUE_CPU_CACHE_LINE_SIZE) one2many_bitmask_bucket
{
    using storage_t = typename std::aligned_storage<sizeof(event_t), alignof(event_t)>::type;

    one2many_bitmask_bucket()
        : m_seqn(one2many_bitmask_queue_impl<bitmask_t>::DUMMY_EVENT_SEQ_NUM)
        , m_mask(one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK)
    {
    }

    // copy ctor and assign
    one2many_bitmask_bucket(const one2many_bitmask_bucket&) = delete;
    one2many_bitmask_bucket& operator=(const one2many_bitmask_bucket&) = delete;

    // move ctor and assign
    one2many_bitmask_bucket(one2many_bitmask_bucket&&) = delete;
    one2many_bitmask_bucket& operator=(one2many_bitmask_bucket&&) = delete;

    ~one2many_bitmask_bucket()
    {
        if (m_mask != one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK)
        {
            get_event().~event_t();
            m_mask.store(one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK, std::memory_order_relaxed);
        }
    }

    event_t& get_event()
    {
        return reinterpret_cast<event_t&>(m_storage);
    }

    std::atomic<bitmask_t> m_seqn;
    std::atomic<bitmask_t> m_mask;
    storage_t m_storage;
};

// reader
template<class event_t, typename bitmask_t>
class alignas(QUEUE_CPU_CACHE_LINE_SIZE) one2many_bitmask_reader
{
public:
    using bucket_type = one2many_bitmask_bucket<event_t, bitmask_t>;
    using guard_type = one2many_bitmask_guard<event_t, bitmask_t>;
    using event_type = event_t;
    using bitmask_type = bitmask_t;

public:
    // ctor
    one2many_bitmask_reader(std::shared_ptr<bucket_type> storage, std::size_t storage_size, bitmask_t read_from, bitmask_t mask)
        : m_next_read_index(read_from)
        , m_storage_size(storage_size)
        , m_storage(storage)
        , m_mask(mask)
    {
    }

    // copy ctor and assign
    one2many_bitmask_reader(const one2many_bitmask_reader&) = delete;
    one2many_bitmask_reader& operator=(const one2many_bitmask_reader&) = delete;

    // move ctor and assign
    one2many_bitmask_reader(one2many_bitmask_reader&&) = default;
    one2many_bitmask_reader& operator=(one2many_bitmask_reader&&) = default;

    // dtor
    ~one2many_bitmask_reader()
    {
    }

    std::optional<guard_type> try_read() noexcept
    {
        bool result = false;
        auto& bucket = m_storage.get()[get_bounded_index(m_next_read_index)];
        if (bucket.m_seqn.load(std::memory_order_consume) == m_next_read_index)
        {
            m_next_read_index++;
            result = true;
        }
        return result ?
            std::optional<guard_type>(guard_type{bucket, m_mask}) :
            std::optional<guard_type>();
    }

    one2many_bitmask_guard<event_t, bitmask_t> read() noexcept
    {
        read_mark:

        auto pair = try_read();
        if (pair.first)
        {
            return std::move(pair.second);
        }

        goto read_mark;
    }

    bitmask_t get_id() const noexcept
    {
        return static_cast<bitmask_t>(std::log2(m_mask));
    }

private:
    std::size_t get_bounded_index(std::size_t seqn) const noexcept
    {
        return seqn % m_storage_size;
    }

private:
    bitmask_t m_next_read_index;
    std::size_t const m_storage_size;
    std::shared_ptr<bucket_type> const m_storage;
    bitmask_t const m_mask;
};

// queue
template<class event_t, typename bitmask_t>
class alignas(QUEUE_CPU_CACHE_LINE_SIZE) one2many_bitmask_queue
{
public:
    using writer_type = one2many_bitmask_queue<event_t, bitmask_t>;
    using reader_type = one2many_bitmask_reader<event_t, bitmask_t>;
    using bucket_type = one2many_bitmask_bucket<event_t, bitmask_t>;
    using guard_type = one2many_bitmask_guard<event_t, bitmask_t>;
    using event_type = event_t;
    using bitmask_type = bitmask_t;

public:
    // ctor
    one2many_bitmask_queue(std::size_t n) : m_local(n)
    {
    }

    // copy ctor and assign
    one2many_bitmask_queue(const one2many_bitmask_queue&) = delete;
    one2many_bitmask_queue& operator=(const one2many_bitmask_queue&) = delete;

    // move ctor and assign
    one2many_bitmask_queue(one2many_bitmask_queue&&) = default;
    one2many_bitmask_queue& operator=(one2many_bitmask_queue&&) = default;

    // dtor
    ~one2many_bitmask_queue()
    {
    }

    reader_type create_reader()
    {
        /// @todo : think about the ability to dynamically create readers

        auto const next_id = m_local.m_next_reader_id++;
        if (next_id > one2many_bitmask_queue_impl<bitmask_t>::MAX_READER_ID)
        {
            throw std::runtime_error("Next reader id overflow: " + std::to_string(next_id) + " > " + std::to_string(one2many_bitmask_queue_impl<bitmask_t>::MAX_READER_ID));
        }

        // read next_seq_num before updating alive mask
        auto const read_from = m_local.m_next_seq_num;

        bitmask_t const mask(bitmask_t(1) << next_id);

        m_local.m_alive_mask |= mask;

        reader_type reader(m_local.m_storage, m_local.m_storage_size, read_from, mask);
        return reader;
    }

    bool try_write(event_t&& event) noexcept
    {
        auto& bucket = m_local.m_storage.get()[get_bounded_index(m_local.m_next_seq_num)];
        if (bucket.m_mask.load(std::memory_order_consume) == one2many_bitmask_queue_impl<bitmask_t>::EMPTY_DATA_MASK)
        {
            auto const alive_mask = m_local.m_alive_mask;

            new (&bucket.m_storage) event_t(std::move(event));
            bucket.m_mask.store(alive_mask, std::memory_order_release);
            bucket.m_seqn.store(m_local.m_next_seq_num++, std::memory_order_seq_cst);

            return true;
        }
        else
        {
            return false;
        }
    }

    void write(event_t&& obj) noexcept
    {
        while (! try_write(std::move(obj)));
    }

    std::size_t size() const noexcept
    {
        return m_local.m_storage_size;
    }

    bitmask_t get_alive_mask() const noexcept
    {
        return m_local.m_alive_mask & (sizeof(bitmask_t) == 8 ? 0x7FFFFFFFFFFFFFFF : 0x7FFFFFFF);
    }

private:
    std::size_t get_bounded_index(std::size_t seqn) const noexcept
    {
        return seqn % m_local.m_storage_size;
    }

private:
    // data used in writer thread
    struct local_t
    {
        using storage_t = std::shared_ptr<bucket_type>;

        local_t(std::size_t n) :
            m_next_seq_num(one2many_bitmask_queue_impl<bitmask_t>::MIN_EVENT_SEQ_NUM),
            m_storage_size(n),
            m_storage(new bucket_type[n], [](bucket_type* ptr) {
                delete[] ptr;
            }),
            m_alive_mask(one2many_bitmask_queue_impl<bitmask_t>::CONSTRUCTED_MASK),
            m_next_reader_id(one2many_bitmask_queue_impl<bitmask_t>::MIN_READER_ID)
        {
        }

        bitmask_t m_next_seq_num;
        std::size_t const m_storage_size;
        storage_t const m_storage;
        bitmask_t m_alive_mask;
        bitmask_t m_next_reader_id;
    };

    local_t  m_local;
};
