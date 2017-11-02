#include <mutex>
#include <vector>
#include <utility>
#include <iostream>

class mutex_t
{
public:
    void lock()
    {
        std::cout << "lock" << std::endl;
    }

    void unlock()
    {
        std::cout << "unlock" << std::endl;
    }
};

template<class V, class M, size_t CPU_CACHE_LINE_SIZE = 64>
class storage_t
{
private:
    struct alignas(CPU_CACHE_LINE_SIZE) node_impl_t
    {
        V v;
        M m;
    };

    using node_t = node_impl_t;

public:
    storage_t(size_t n) :
        impl_(n, node_t())
    {
        std::cout << "sizeof: " << sizeof(node_t) << "\n";
    }

    std::pair<V&, std::unique_lock<M>> get_data(size_t i)
    {
        auto item = std::pair<V&, std::unique_lock<M>>(impl_[i].v, std::unique_lock<M>(impl_[i].m) );
        return item;
    }

private:
    std::vector<node_t> impl_;
};

struct temp_t
{
    size_t data[9];
};

int main()
{
    std::ios::sync_with_stdio(false);

    {
        std::cout << "sizeof(temp_t): " << sizeof(temp_t) << "\n";
        std::cout << "storage_t<temp_t, mutex_t> ";
        storage_t<temp_t, mutex_t> storage(1024);
    }

    std::cout << std::endl;

    {
        std::cout << "storage_t<size_t, mutex_t> ";
        storage_t<size_t, mutex_t> storage(1024);
        {
            auto item = storage.get_data(9);
            std::cout << "val: " << item.first << std::endl;
        }
    }

    return 0;
}
