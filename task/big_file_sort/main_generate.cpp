#include <ctime>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cstdio>
#include <string>
#include <vector>
#include <thread>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <condition_variable>

constexpr double GB_SIZE = 1024.0 * 1024.0 * 1024.0;

std::string pretty_size(double size)
{
    size /= GB_SIZE;
    return std::to_string(size) + " GB";
}

template<class T>
class cstorage
{
public:
    cstorage()
        : done_(false)
    {
    }

    bool push(T&& data)
    {
        {
            std::lock_guard<std::mutex> guard(mutex_);
            if (vector_.size() > 32 * 1024)
            {
                return false;
            }
            vector_.emplace_back(std::move(data));
        }
        cv_.notify_one();
        return true;
    }

    void swap(std::vector<T>& vector)
    {
        std::unique_lock<std::mutex> guard(mutex_);
        cv_.wait(guard, [this](){
            return !vector_.empty() || is_done();
        });
        vector_.swap(vector);
    }

    void done()
    {
        done_.store(true);
        cv_.notify_one();
    }

    bool is_done() const
    {
        return done_.load(std::memory_order_relaxed);
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    std::vector<T> vector_;
    std::atomic<bool> done_;
};

void producer(cstorage<std::vector<std::string>>& storage, double segment_size)
{
    constexpr size_t BATCH_SIZE = 4096;
    std::vector<std::string> data;

    double generated = 0;
    while(generated < segment_size) {
        int email = rand() % 10000000;
        int count = rand() % 10000;

        std::string buffer;
        buffer.append(std::to_string(email));
        buffer.append("@yandex.ru ");
        buffer.append(std::to_string(count));

        // consumer is going to write \n symbol
        generated += double(buffer.size()) + 1;

        data.emplace_back(buffer);
        if(data.size() >= BATCH_SIZE) {
            while(!storage.push(std::move(data)))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            data.reserve(BATCH_SIZE);
        }
    }
    storage.push(std::move(data));
}

void consumer(const std::string& file, cstorage<std::vector<std::string>>& storage)
{
    std::ofstream output(file);
    std::vector<std::vector<std::string>> buffer;

    auto save = [&storage, &output, &buffer](){
        storage.swap(buffer);

        if(!buffer.empty()) {
            for(auto const& active_buffer : buffer) {
                for(auto const& active_element : active_buffer) {
                    output << active_element << "\n";
                }
            }
            buffer.clear();
        }
    };

    while(!storage.is_done()) {
        save();
    }

    save();
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " <path/to/file> <size> (GB)" << std::endl;
        return 1;
    }

    std::string file(argv[1]);
    std::string size(argv[2]);

    std::string::size_type sz;
    double target_gb = std::stod(size, &sz);
    double target = target_gb * GB_SIZE;

    srand(static_cast<unsigned>(time(NULL)));

    std::vector<std::thread> producers;

    const size_t hardware_concurrency = std::thread::hardware_concurrency();
    double segment_size = target / double(hardware_concurrency);

    cstorage<std::vector<std::string>> storage;

    for(size_t i = 0; i < hardware_concurrency; i++) {
        producers.push_back(std::thread([&storage, segment_size](){
            producer(storage, segment_size);
        }));
    }

    std::thread consumer_thread([&file, &storage](){
        consumer(file, storage);
    });

    for(auto& iproducer : producers) {
        iproducer.join();
    }

    storage.done();

    consumer_thread.join();

    std::cout << "generated: " << file << " size: " << pretty_size(target) << std::endl;

    return 0;
}
