#include <limits>
#include <atomic>
#include <memory>
#include <optional>
#include <type_traits>

enum : std::size_t { QUEUE_CPU_CACHE_LINE_SIZE = 64 };

template<typename T>
struct one2many_counter_queue_impl
{
    enum : T { MIN_EVENT_SEQ_NUM = 1 };
    enum : T { DUMMY_EVENT_SEQ_NUM = 0 };
    enum : T { MIN_READER_ID = 0 };
    enum : T { DUMMY_READER_ID = std::numeric_limits<T>::max() };
    enum : T { EMPTY_DATA_MARK = 0 };
    enum : T { CONSTRUCTED_DATA_MARK = 1 };
};

// predeclaration

// queue
template<class event_t, typename counter_t>
class one2many_counter_queue;

// reader
template<class event_t, typename counter_t>
class one2many_counter_reader;

// guard
template<class event_t, typename counter_t>
class one2many_counter_guard;

// bucket
template<class event_t, typename counter_t>
struct one2many_counter_bucket;

// implementation

// guard
template<class event_t, typename counter_t>
class one2many_counter_guard
{
public:
    // ctor
    one2many_counter_guard(one2many_counter_bucket<event_t, counter_t>& bucket, counter_t owner) : m_bucket(&bucket), m_owner(owner)
    {
    }

    // move ctor and assign
    one2many_counter_guard(one2many_counter_guard&& data) : m_bucket(data.m_bucket), m_owner(data.m_owner)
    {
        data.m_owner = one2many_counter_queue_impl<counter_t>::DUMMY_READER_ID;
    }

    one2many_counter_guard& operator=(one2many_counter_guard&& data)
    {
        if (&data != this)
        {
            m_bucket = data.m_bucket;
            m_owner  = data.m_owner;
            data.m_owner = one2many_counter_queue_impl<counter_t>::DUMMY_READER_ID;
        }
        return *this;
    }

    // copy ctor and assign
    one2many_counter_guard(const one2many_counter_guard&) = delete;
    one2many_counter_guard& operator=(const one2many_counter_guard&) = delete;

    // dtor
    ~one2many_counter_guard()
    {
        if (m_owner != one2many_counter_queue_impl<counter_t>::DUMMY_READER_ID)
        {
            auto const release_etalon(one2many_counter_queue_impl<counter_t>::CONSTRUCTED_DATA_MARK + 1);
            auto const before = m_bucket->m_counter.fetch_add(-1, std::memory_order_consume);

            if (before == release_etalon)
            {
                m_bucket->get_event().~event_t();
                m_bucket->m_counter.store(one2many_counter_queue_impl<counter_t>::EMPTY_DATA_MARK, std::memory_order_release);
            }
        }
    }

    event_t const& get_event() const
    {
        return m_bucket->get_event();
    }

private:
    one2many_counter_bucket<event_t, counter_t>* m_bucket;
    counter_t m_owner;
};

// bucket
template<class event_t, typename counter_t>
struct alignas(QUEUE_CPU_CACHE_LINE_SIZE) one2many_counter_bucket
{
    using storage_t = typename std::aligned_storage<sizeof(event_t), alignof(event_t)>::type;

    one2many_counter_bucket() :
        m_seqn(one2many_counter_queue_impl<counter_t>::DUMMY_EVENT_SEQ_NUM),
        m_counter(one2many_counter_queue_impl<counter_t>::EMPTY_DATA_MARK)
    {
    }

    // copy ctor and assign
    one2many_counter_bucket(const one2many_counter_bucket&) = delete;
    one2many_counter_bucket& operator=(const one2many_counter_bucket&) = delete;

    // move ctor and assign
    one2many_counter_bucket(one2many_counter_bucket&&) = delete;
    one2many_counter_bucket& operator=(one2many_counter_bucket&&) = delete;

    ~one2many_counter_bucket()
    {
        if (m_counter != one2many_counter_queue_impl<counter_t>::EMPTY_DATA_MARK)
        {
            get_event().~event_t();
            m_counter.store(one2many_counter_queue_impl<counter_t>::EMPTY_DATA_MARK, std::memory_order_relaxed);
        }
    }

    event_t& get_event()
    {
        return reinterpret_cast<event_t&>(m_storage);
    }

    std::atomic<counter_t> m_seqn;
    std::atomic<counter_t> m_counter;
    storage_t m_storage;
};

// reader
template<class event_t, typename counter_t>
class alignas(QUEUE_CPU_CACHE_LINE_SIZE) one2many_counter_reader
{
public:
    using guard_type = one2many_counter_guard<event_t, counter_t>;
    using event_type = event_t;
    using counter_type = counter_t;

public:
    // ctor
    one2many_counter_reader(std::shared_ptr<one2many_counter_bucket<event_t, counter_t>> storage, std::size_t storage_size, std::size_t read_from, counter_t id) :
        m_next_read_index(read_from), m_storage_size(storage_size), m_storage(storage), m_id(id)
    {
    }

    // copy ctor and assign
    one2many_counter_reader(const one2many_counter_reader&) = delete;
    one2many_counter_reader& operator=(const one2many_counter_reader&) = delete;

    // move ctor and assign
    one2many_counter_reader(one2many_counter_reader&&) = default;
    one2many_counter_reader& operator=(one2many_counter_reader&&) = default;

    // dtor
    ~one2many_counter_reader()
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
            std::optional<guard_type>(guard_type(bucket, m_id)) :
            std::optional<guard_type>();
    }

    one2many_counter_guard<event_t, counter_t> read() noexcept
    {
        read_mark:

        auto opt = try_read();
        if (opt)
        {
            return std::move(opt.operator*());
        }

        goto read_mark;
    }

    counter_t get_id() const noexcept
    {
        return m_id;
    }

private:
    std::size_t get_bounded_index(std::size_t seqn) const noexcept
    {
        return seqn % m_storage_size;
    }

private:
    std::size_t m_next_read_index;
    std::size_t const m_storage_size;
    std::shared_ptr<one2many_counter_bucket<event_t, counter_t>> const m_storage;
    counter_t const m_id;
};

// queue
template<class event_t, typename counter_t>
class alignas(QUEUE_CPU_CACHE_LINE_SIZE) one2many_counter_queue
{
public:
    using writer_type = one2many_counter_queue<event_t, counter_t>;
    using reader_type = one2many_counter_reader<event_t, counter_t>;
    using bucket_type = one2many_counter_bucket<event_t, counter_t>;
    using guard_type = one2many_counter_guard<event_t, counter_t>;
    using event_type = event_t;
    using counter_type = counter_t;

public:
    // ctor
    one2many_counter_queue(std::size_t n) : m_local(n)
    {
    }

    // copy ctor and assign
    one2many_counter_queue(const one2many_counter_queue&) = delete;
    one2many_counter_queue& operator=(const one2many_counter_queue&) = delete;

    // move ctor and assign
    one2many_counter_queue(one2many_counter_queue&&) = default;
    one2many_counter_queue& operator=(one2many_counter_queue&&) = default;

    // dtor
    ~one2many_counter_queue()
    {
    }

    reader_type create_reader()
    {
        auto const next_id = m_local.m_next_reader_id++;
        if (next_id == one2many_counter_queue_impl<counter_t>::DUMMY_READER_ID)
        {
            throw std::runtime_error("Next reader id overflow: " + std::to_string(next_id) + " > " + std::to_string(one2many_counter_queue_impl<counter_t>::DUMMY_READER_ID));
        }

        // read next_seq_num before updating alive mask
        auto const read_from = m_local.m_next_seq_num;

        return reader_type(m_local.m_storage, m_local.m_storage_size, read_from, next_id);
    }

    bool try_write(event_t&& event) noexcept
    {
        auto& bucket = m_local.m_storage.get()[get_bounded_index(m_local.m_next_seq_num)];
        if (bucket.m_counter.load(std::memory_order_consume) == one2many_counter_queue_impl<counter_t>::EMPTY_DATA_MARK)
        {
            auto const num_readers = m_local.m_next_reader_id;

            new (&bucket.m_storage) event_t(std::move(event));
            bucket.m_counter.store(num_readers + one2many_counter_queue_impl<counter_t>::CONSTRUCTED_DATA_MARK, std::memory_order_release);
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
        while (!try_write(std::move(obj)));
    }

    std::size_t size() const noexcept
    {
        return m_local.m_storage_size;
    }

    counter_t get_alive_mask() const noexcept
    {
        auto const num_readers = m_local.m_next_reader_id;
        counter_t mask{0};
        for (std::size_t i = 0; i < num_readers; i++)
        {
            mask |= (counter_t(1) << i);
        }
        return mask;
    }

private:
    std::size_t get_bounded_index(std::size_t seqn) const noexcept
    {
        return seqn % m_local.m_storage_size;
    }

private:
    // data used in writer thread
    struct alignas(QUEUE_CPU_CACHE_LINE_SIZE) local_t
    {
        using storage_t = std::shared_ptr<bucket_type>;

        local_t(std::size_t n) :
            m_next_seq_num(one2many_counter_queue_impl<counter_t>::MIN_EVENT_SEQ_NUM),
            m_storage_size(n),
            m_storage(new bucket_type[n], [] (bucket_type* ptr) {
                delete[] ptr;
            }),
            m_next_reader_id(one2many_counter_queue_impl<counter_t>::MIN_READER_ID)
        {
        }

        std::size_t m_next_seq_num;
        std::size_t const m_storage_size;
        storage_t const m_storage;
        counter_t m_next_reader_id;
    };

    local_t  m_local;
};