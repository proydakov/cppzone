/*
*  Copyright (c) 2016 Evgeny Proydakov <lord.tiran@gmail.com>
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <string>
#include <iostream>
#include <hash_map.h>
#include <unordered_map>

template<class hash_table>
void info()
{
    hash_table table;

    table[0] = 1;
    table[0]++;

    table[8] = 9;
    table[8]--;

    std::cout << "max_bucket_count: " << table.bucket_count() << std::endl;
    std::cout << "size: " << table.size() << std::endl;
    std::cout << "load_factor: " << table.load_factor() << std::endl;
}

template<class hash_table>
void test_operator_access()
{
    hash_table word_map;

    std::vector<std::string> words = {
        "this", "sentence", "is", "not", "a", "sentence",
        "this", "sentence", "is", "a", "hoax"
    };

    for (auto w : words) {
        ++word_map[w];
    }

    int i = 0;
    /*
    for (auto elem : word_map) {
        std::cout << elem.second
        << " occurrences of word '"
        << elem.first << "'\n";
    }
     */
}

template<class hash_table>
void test_iterator()
{
    hash_table mymap{ { "Australia","Canberra" },{ "U.S.","Washington" },{ "France","Paris" } };

    /*
    std::cout << "mymap contains:";
    for (auto it = mymap.begin(); it != mymap.end(); ++it)
        std::cout << " " << it->first << ":" << it->second;
    std::cout << std::endl;
    */
    std::cout << "mymap's buckets contain:\n";
    for (unsigned i = 0; i < mymap.bucket_count(); ++i) {
        std::cout << "bucket #" << i << " contains:";
        for (auto local_it = mymap.begin(i); local_it != mymap.end(i); ++local_it)
            std::cout << " " << local_it->first << ":" << local_it->second;
        std::cout << std::endl;
    }
}

int main()
{
    {
        std::cout << "/// STD ///" << std::endl;
        info< std::unordered_map<int, int> >();

        std::cout << "/// ITD ///" << std::endl;
        info< hash_map<int, int> >();
    }

    std::cout << "///////////////////////////////////////////////////////////////////////////////" << std::endl;

    {
        std::cout << "/// STD ///" << std::endl;
        test_operator_access< std::unordered_map<std::string, size_t> >();

        std::cout << "/// ITD ///" << std::endl;
        test_operator_access< hash_map<std::string, size_t> >();
    }

    std::cout << "///////////////////////////////////////////////////////////////////////////////" << std::endl;

    {
        std::cout << "/// STD ///" << std::endl;
        test_iterator< std::unordered_map<std::string, std::string> >();

        std::cout << "/// ITD ///" << std::endl;
        test_iterator< hash_map<std::string, std::string> >();
    }

    return 0;
};
