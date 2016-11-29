#include <ctime>
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

void generate(const std::string& path, double size)
{
    std::ofstream output(path);

    double generated = 0;
    while(generated < size) {
        int email = rand() % 10000000;
        int count = rand() % 10000;

        std::stringstream sstream;
        sstream << email << "@yandex.ru " << count << "\n";

        std::string buffer(sstream.str());
        generated += buffer.size();

        output << buffer;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " path/to/file size (GB)" << std::endl;
        return 1;
    }

    std::string file(argv[1]);
    std::string size(argv[2]);

    std::string::size_type sz;
    double target_gb = std::stod(size, &sz);
    double target = target_gb * GB_SIZE;

    srand (time(NULL));

    std::vector<std::string> files;
    std::vector<std::thread> threads;

    const size_t hardware_concurrency = std::thread::hardware_concurrency();
    double segment_size = target / hardware_concurrency;

    for(size_t i = 0; i < hardware_concurrency; i++) {
        std::string blob_name(file + ".blob" +  std::to_string(i));
        std::cout << "generate: " << blob_name
                  << " size: "    << pretty_size(segment_size)
                  << std::endl;

        threads.push_back(std::move(std::thread([blob_name, segment_size](){
            generate(blob_name, segment_size);
        })));
        files.push_back(std::move(blob_name));
    }

    for(size_t i = 0; i < hardware_concurrency; i++) {
        threads[i].join();
    }

    std::cout << "merge: " << file << " size: " << pretty_size(target) << std::endl;

    std::ofstream output(file);
    for(size_t i = 0; i < files.size(); i++) {
        std::ifstream input(files[i]);
        std::string line;
        while (std::getline(input, line)) {
            output << line << "\n";
        }
    }

    return 0;
}
