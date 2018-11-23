#include <cmath>
#include <atomic>
#include <memory>
#include <type_traits>

#include <iostream>

namespace one2many_bitmask_queue_impl
{
    enum : std::uint64_t { CPU_CACHE_LINE_SIZE = 64 };
    enum : std::uint64_t { MIN_READER_ID = 0 };
    enum : std::uint64_t { MAX_READER_ID = 62 };
    enum : std::uint64_t { MIN_EVENT_SEQ_NUM = 1 };
    enum : std::uint64_t { DUMMY_EVENT_SEQ_NUM = 0 };
    enum : std::uint64_t { EMPTY_DATA_MASK = std::uint64_t(0) };
    enum : std::uint64_t { CONSTRUCTED_MASK = std::uint64_t(1) << MAX_READER_ID + 1 };
}

// predeclaration

// queue
template<class event_t>
class one2many_bitmask_queue;

// reader
template<class event_t>
class one2many_bitmask_reader;

// guard
template<class event_t>
class one2many_bitmask_guard;

// bucket
template<class event_t>
struct one2many_bitmask_bucket;

// implementation

// guard
template<class event_t>
class one2many_bitmask_guard
{
public:
    // ctor
    one2many_bitmask_guard(one2many_bitmask_bucket<event_t>& bucket, std::uint64_t mask) : m_bucket(&bucket), m_mask(mask)
    {
    }

    // move ctor and assign
    one2many_bitmask_guard(one2many_bitmask_guard&& data) : m_bucket(data.m_bucket), m_mask(data.m_mask)
    {
        data.m_mask = one2many_bitmask_queue_impl::EMPTY_DATA_MASK;
    }

    one2many_bitmask_guard& operator=(one2many_bitmask_guard&& data)
    {
        if (&data != this)
        {
            m_bucket = data.m_bucket;
            m_mask = data.m_mask;
            data.m_mask = one2many_bitmask_queue_impl::EMPTY_DATA_MASK;
        }
        return *this;
    }

    // copy ctor and assign
    one2many_bitmask_guard(const one2many_bitmask_guard&) = delete;
    one2many_bitmask_guard& operator=(const one2many_bitmask_guard&) = delete;

    // dtor
    ~one2many_bitmask_guard()
    {
        if (m_mask != one2many_bitmask_queue_impl::EMPTY_DATA_MASK)
        {
            auto const before = m_bucket->m_mask.fetch_and(~m_mask, std::memory_order_release);
            auto const release_etalon(m_mask | one2many_bitmask_queue_impl::CONSTRUCTED_MASK);

            if (before == release_etalon)
            {
                m_bucket->get_event().~event_t();
                m_bucket->m_mask.store(one2many_bitmask_queue_impl::EMPTY_DATA_MASK, std::memory_order_release);
            }
        }
    }

    event_t const& get_event() const
    {
        return m_bucket->m_event;
    }

private:
    one2many_bitmask_bucket<event_t>* m_bucket;
    std::uint64_t m_mask;
};

// bucket
template<class event_t>
struct alignas(one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE) one2many_bitmask_bucket
{
    using storage_t = typename std::aligned_storage<sizeof(event_t), alignof(event_t)>::type;

    one2many_bitmask_bucket() :
        m_seqn(one2many_bitmask_queue_impl::DUMMY_EVENT_SEQ_NUM),
        m_mask(one2many_bitmask_queue_impl::EMPTY_DATA_MASK)
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
        if (m_mask != one2many_bitmask_queue_impl::EMPTY_DATA_MASK)
        {
            get_event().~event_t();
            m_mask.store(one2many_bitmask_queue_impl::EMPTY_DATA_MASK, std::memory_order_relaxed);
        }
        //std::clog << "~one2many_bucket()\n";
    }

    event_t& get_event()
    {
        return reinterpret_cast<event_t&>(m_storage);
    }

    std::atomic<std::uint64_t> m_seqn;
    std::atomic<std::uint64_t> m_mask;
    storage_t m_storage;
};

static_assert(sizeof(one2many_bitmask_bucket<void*>) == one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE, "Performance optimization");

// reader
template<class event_t>
class alignas(one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE) one2many_bitmask_reader
{
public:
    // ctor
    one2many_bitmask_reader(std::shared_ptr<one2many_bitmask_queue<event_t>> queue, one2many_bitmask_bucket<event_t>* storage, std::uint64_t storage_size, std::uint64_t read_from, std::uint64_t mask) :
        m_next_read_index(read_from), m_storage_size(storage_size), m_storage(storage), m_mask(mask), m_queue(std::move(queue))
    {
        std::clog << "one2many_reader() id: [" + std::to_string(get_id()) + "]\n";
    }

    // copy ctor and assign
    one2many_bitmask_reader(const one2many_bitmask_reader&) = delete;
    one2many_bitmask_reader& operator=(const one2many_bitmask_reader&) = delete;

    // move ctor and assign
    one2many_bitmask_reader(one2many_bitmask_reader&&) = delete;
    one2many_bitmask_reader& operator=(one2many_bitmask_reader&&) = delete;

    // dtor
    ~one2many_bitmask_reader()
    {
        std::clog << "~one2many_reader() id: [" + std::to_string(get_id()) + "]\n";
    }

    std::pair<bool, one2many_bitmask_guard<event_t>> try_read() noexcept
    {
        bool result = false;
        auto& bucket = m_storage[get_bounded_index(m_next_read_index)];
        if (bucket.m_seqn.load(std::memory_order_acquire) == m_next_read_index)
        {
            m_next_read_index++;
            result = true;
        }
        return std::make_pair(result, one2many_bitmask_guard<event_t>(bucket, result ? m_mask : one2many_bitmask_queue_impl::EMPTY_DATA_MASK));
    }

    one2many_bitmask_guard<event_t> read() noexcept
    {
        read_mark:

        auto pair = try_read();
        if (pair.first)
        {
            return std::move(pair.second);
        }

        goto read_mark;
    }

    std::size_t get_id() const noexcept
    {
        return std::log2(m_mask);
    }

private:
    std::uint64_t get_bounded_index(std::uint64_t seqn) const noexcept
    {
        return seqn % m_storage_size;
    }

private:
    std::uint64_t m_next_read_index;
    std::uint64_t const m_storage_size;
    one2many_bitmask_bucket<event_t>* const m_storage;
    std::uint64_t const m_mask;
    std::shared_ptr<one2many_bitmask_queue<event_t>> const m_queue;
};

static_assert(sizeof(one2many_bitmask_reader<void*>) == one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE, "Performance optimization");

// queue
template<class event_t>
class alignas(one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE) one2many_bitmask_queue : public std::enable_shared_from_this<one2many_bitmask_queue<event_t>>
{
public:
    using queue_ptr = std::shared_ptr<one2many_bitmask_queue<event_t>>;
    using reader_ptr = std::unique_ptr<one2many_bitmask_reader<event_t>>;
    using event_type = event_t;

public:
    one2many_bitmask_queue(const one2many_bitmask_queue&) = delete;
    one2many_bitmask_queue& operator=(const one2many_bitmask_queue&) = delete;

    one2many_bitmask_queue(one2many_bitmask_queue&&) = delete;
    one2many_bitmask_queue& operator=(one2many_bitmask_queue&&) = delete;

    ~one2many_bitmask_queue()
    {
        std::clog << "~one2many_queue()\n";
    }

    // allocate new queue.
    static std::shared_ptr<one2many_bitmask_queue<event_t>> make(std::size_t n)
    {
        struct make_shared_enabler : public one2many_bitmask_queue<event_t>
        {
            make_shared_enabler(std::size_t n) : one2many_bitmask_queue<event_t>(n)
            {
            }
        };

        return std::make_shared<make_shared_enabler>(n);
    }

    // create new reader. Max reader id: MAX_READER_ID
    std::unique_ptr<one2many_bitmask_reader<event_t>> create_reader()
    {
        /// @todo : think about the ability to dynamically create readers

        auto const next_id = m_local.m_next_reader_id.fetch_add(1, std::memory_order_seq_cst);;
        if (next_id > one2many_bitmask_queue_impl::MAX_READER_ID)
        {
            throw std::runtime_error("Next reader id overflow: " + std::to_string(next_id) + " > " + std::to_string(one2many_bitmask_queue_impl::MAX_READER_ID));
        }

        // read next_seq_num before updating alive mask
        auto const read_from = m_local.m_next_seq_num;

        std::uint64_t const mask(std::uint64_t(1) << next_id);

        m_local.m_alive_mask.fetch_or(mask, std::memory_order_seq_cst);

        reader_ptr reader(new one2many_bitmask_reader<event_t>(shared_from_this(), m_local.m_storage.get(), m_local.m_storage_size, read_from, mask));
        return reader;
    }

    bool try_write(event_t&& event) noexcept
    {
        auto& bucket = m_local.m_storage[get_bounded_index(m_local.m_next_seq_num)];
        if (bucket.m_mask.load(std::memory_order_acquire) == one2many_bitmask_queue_impl::EMPTY_DATA_MASK)
        {
            auto const alive_mask = m_local.m_alive_mask.load(std::memory_order_acquire);

            new (&bucket.m_storage) event_t(std::move(event));
            bucket.m_mask.store(alive_mask, std::memory_order_release);
            bucket.m_seqn.store(m_local.m_next_seq_num++, std::memory_order_release);

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

    std::uint64_t size() const noexcept
    {
        return m_local.m_storage.size();
    }

    std::uint64_t get_alive_mask() const noexcept
    {
        return m_local.m_alive_mask.load(std::memory_order_acquire) & 0x7FFFFFFFFFFFFFFF;
    }

private:
    one2many_bitmask_queue(std::size_t n) : m_local(n)
    {
        std::clog << "one2many_queue() capacity: [" + std::to_string(n) + "]\n";
    }

    std::uint64_t get_bounded_index(std::uint64_t seqn) const noexcept
    {
        return seqn % m_local.m_storage_size;
    }

private:
    // data used in writer thread
    struct alignas(one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE) local_t
    {
        using storage_t = std::unique_ptr<one2many_bitmask_bucket<event_t>[]>;

        local_t(std::size_t n) :
            m_next_seq_num(one2many_bitmask_queue_impl::MIN_EVENT_SEQ_NUM),
            m_storage_size(n),
            m_storage(new typename storage_t::element_type[n]),
            m_alive_mask(one2many_bitmask_queue_impl::CONSTRUCTED_MASK),
            m_next_reader_id(one2many_bitmask_queue_impl::MIN_READER_ID)
        {
        }

        std::uint64_t m_next_seq_num;
        std::uint64_t const m_storage_size;
        storage_t const m_storage;
        std::atomic<std::uint64_t> m_alive_mask;
        std::atomic<std::uint64_t> m_next_reader_id;
    };

    // enable_shared_from_this data here
    // aligned empty space here
    local_t  m_local;
};

static_assert(sizeof(one2many_bitmask_queue<void*>) == one2many_bitmask_queue_impl::CPU_CACHE_LINE_SIZE * 2, "Performance optimization");
