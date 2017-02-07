#include <cmath>
#include <thread>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <fstream>

struct data
{
    int height;
    int weight;
};


std::ostream& operator<<(std::ostream& stream, const data& obj)
{
    stream << "[ height: " << std::setw(5) << obj.height << " weight: " << std::setw(5) << obj.weight << " ]";
    return stream;
}

int read_input(std::vector<data>& candies, size_t& tindex)
{
    size_t count = 0;
    int jump = 0;

    //std::ifstream input("/home/eproydakov/repository/cppzone/tasks/zeptocode_rush_2014_a/input1.txt");
    //auto& src = input;

    auto& src = std::cin;

    src >> count;
    src >> jump;

    candies.resize(count);

    size_t findex = 0;
    size_t lindex = count - 1;

    for(size_t i = 0; i < count; i++) {
        data d;
        int type = 0;
        src >> type;
        src >> d.height;
        src >> d.weight;
        if(!type) {
            candies[findex] = d;
            findex++;
        }
        else {
            candies[lindex] = d;
            lindex--;
        }
    }

    tindex = findex;

    return jump;
}

void height_sort(std::vector<data>& candies, size_t tindex)
{
    std::sort(candies.begin(), candies.begin() + tindex, [](const data& d1, const data& d2){
        if(d1.height == d2.height) {
            return d1.weight > d2.weight;
        }
        return d1.height < d2.height;
    });

    std::sort(candies.begin() + tindex, candies.end(), [](const data& d1, const data& d2){
        if(d1.height == d2.height) {
            return d1.weight > d2.weight;
        }
        return d1.height < d2.height;
    });
}

void eat_step(std::vector<data>& candies, size_t head, size_t neck, int jump)
{
    size_t eat_index = head;
    int eat_weght = candies[eat_index].weight;
    size_t temp_index = eat_index + 1;

    while(temp_index < neck && candies[temp_index].height <= jump) {
        if(candies[temp_index].weight > eat_weght) {
            eat_weght = candies[temp_index].weight;
            eat_index = temp_index;
        }
        temp_index++;
    }
    std::swap( candies[head], candies[eat_index] );
}

int eat_impl(std::vector<data>& candies, size_t from1, size_t to1, size_t from2, size_t to2, int jump)
{
    int candi_counter = 0;
    size_t i1 = from1;
    size_t i2 = from2;
    int type = 2;
    for(; i1 < to1 || i2 < to2; i1++, i2++) {
        if(type == 2 && i1 < to1 && candies[i1].height <= jump) {
            eat_step(candies, i1, to1, jump);
            //std::cout << "eat 0: " << candies[i1] << std::endl;
            jump += candies[i1].weight;
            candi_counter++;
            type = 1;
        }

        if(type == 1 && i2 < to2 && candies[i2].height <= jump) {
            eat_step(candies, i2, to2, jump);
            //std::cout << "eat 1: " << candies[i2] << std::endl;
            jump += candies[i2].weight;
            candi_counter++;
            type = 2;
        }
    }
    /*
    if(i1 < to1 && type == 2 && jump >= candies[i1].height) {
        candi_counter++;
    }
    if(i2 < to2 && type == 1 && jump >= candies[i2].height) {
        candi_counter++;
    }
    */
    return candi_counter;
}

int eat(const std::vector<data>& candies, size_t tindex, int jump)
{
    if(tindex == 0 || tindex == candies.size()) {
        return jump >= candies[0].height ? 1 : 0;
    }

    size_t from1_t1 = 0;
    size_t to1_t1 = tindex;
    size_t from2_t1 = tindex;
    size_t to2_t1 = candies.size();

    size_t from1_t2 = tindex;
    size_t to1_t2 = candies.size();
    size_t from2_t2 = 0;
    size_t to2_t2 = tindex;

    int max1 = 0;
    int max2 = 0;

    std::vector<data> candies1(candies);

    std::thread t1([&max1, &candies1, from1_t1, to1_t1, from2_t1, to2_t1, jump](){
        //std::cout << "eat1" << std::endl;
        max1 = eat_impl(candies1, from1_t1, to1_t1, from2_t1, to2_t1, jump);
    });

    std::vector<data> candies2(candies);

    std::thread t2([&max2, &candies2, from1_t2, to1_t2, from2_t2, to2_t2, jump](){
        //std::cout << "eat2" << std::endl;
        max2 = eat_impl(candies2, from1_t2, to1_t2, from2_t2, to2_t2, jump);
    });

    t1.join();
    t2.join();

    //std::cout << "max1: " << max1 << std::endl;
    //std::cout << "max2: " << max2 << std::endl;

    return std::max(max1, max2);
}

int main()
{
    std::vector<data> candies;
    size_t t1index = 0;
    const int jump = read_input(candies, t1index);

    if(candies.empty()) {
        std::cout << 0 << std::endl;
        return 0;
    }
/*
    std::cout << "jump: " << jump << std::endl;
    std::cout << "t1index: " << t1index << std::endl;

    std::cout << "read" << std::endl;
    std::for_each(candies.begin(), candies.end(), [](const data& obj){
        std::cout << obj << std::endl;
    });
*/
    height_sort(candies, t1index);

/*
    std::cout << "sort" << std::endl;
    size_t index = 0;
    std::for_each(candies.begin(), candies.end(), [&index, t1index](const data& obj){
        if(index == t1index) {
            std::cout << std::endl;
        }
        index++;
        std::cout << obj << std::endl;
    });
*/

    int eat_cand = eat(candies, t1index, jump);

    std::cout << eat_cand << std::endl;

    return 0;
}
