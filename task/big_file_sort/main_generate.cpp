#include <ctime>
#include <mutex>
#include <chrono>
#include <cstdio>
#include <string>
#include <vector>
#include <thread>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

size_t GB_SIZE = 1024 * 1024 * 1024;

std::string pretty_size(double size)
{
    size /= GB_SIZE;
    return std::to_string(size) + " GB";
}

template<class T>
class cstorage
{
public:
    void push(T&& data)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        vector_.push_back(std::move(data));
    }

    void swap(std::vector<T>& vector)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        vector_.swap(vector);
    }

private:
    std::mutex mutex_;
    std::vector<T> vector_;
};

void producer(cstorage<std::vector<std::string>>& storage, double size)
{
    const size_t batch = 4096;
    std::vector<std::string> data;

    double generated = 0;
    while(generated < size) {
        int email = rand() % 10000000;
        int count = rand() % 10000;

        std::stringstream sstream;
        sstream << email << "@yandex.ru " << count << "\n";

        std::string buffer(sstream.str());
        generated += buffer.size();

        data.push_back(buffer);
        if(data.size() >= batch) {
            storage.push(std::move(data));
        }
    }
    storage.push(std::move(data));
}

void consumer(const std::string& file, cstorage<std::vector<std::string>>& storage, bool& done)
{
    std::ofstream output(file);

    std::vector< std::vector<std::string> > buffer;
    while(true) {
        storage.swap(buffer);

        if(buffer.empty()) {
            if(done){
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        else {
            for(size_t i = 0; i < buffer.size(); i++) {
                for(size_t j = 0; j < buffer[i].size(); j++) {
                    output << buffer[i][j];
                }
            }
            buffer.clear();
        }
    }
}

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);

    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " path/to/file size (GB)" << std::endl;
        return 1;
    }

    std::string file(argv[1]);
    std::string size(argv[2]);

    std::string::size_type sz;
    double target_gb = std::stod(size, &sz);
    double target = target_gb * GB_SIZE;

    srand(static_cast<unsigned>(time(NULL)));

    std::vector<std::thread> threads;

    const size_t hardware_concurrency = std::thread::hardware_concurrency();
    double segment_size = target / hardware_concurrency;

    cstorage<std::vector<std::string>> storage;

    for(size_t i = 0; i < hardware_concurrency; i++) {
        threads.push_back(std::thread([&storage, segment_size](){
            producer(storage, segment_size);
        }));
    }

    bool done = false;

    std::thread consumer_thread([&file, &storage, &done](){
        consumer(file, storage, done);
    });

    for(size_t i = 0; i < hardware_concurrency; i++) {
        threads[i].join();
    }

    done = true;

    consumer_thread.join();

    std::cout << "generated: " << file << " size: " << pretty_size(target) << std::endl;

    return 0;
}
