#include <vector>
#include <utility>
#include <fstream>
#include <cassert>
#include <iostream>

class world
{
public:
    world() :
        m_width(0),
        m_height(0),
        m_spiders(0)
    {
    }

    size_t get_width() const
    {
        return m_width;
    }

    size_t get_height() const
    {
        return m_height;
    }

    size_t get_size() const
    {
        return m_size;
    }

    size_t get_spiders() const
    {
        return m_spiders;
    }

    char get_cell(size_t index) const
    {
        return m_cells[index];
    }

    char get_cell(size_t x, size_t y) const
    {
        size_t index = y * m_width + x;
        if(index < m_size && x < m_width) {
            return m_cells[index];
        }
        return '.';
    }

    friend std::istream& operator>>(std::istream& is, world& w);
    friend std::ostream& operator<<(std::ostream& os, const world& w);

private:
    size_t m_width;
    size_t m_height;
    size_t m_size;
    size_t m_spiders;

    std::vector<char> m_cells;
};

std::istream& operator>>(std::istream& is, world& w)
{
    is >> w.m_height;
    is >> w.m_width;
    is >> w.m_spiders;

    w.m_size = w.m_width * w.m_height;

    w.m_cells.resize( w.m_size );

    char buffer;
    for(size_t i = 0; i < w.m_size; i++) {
        is >> buffer;
        if('R' == buffer || 'L' == buffer || 'U' == buffer || 'D' == buffer) {
            w.m_cells[i] = buffer;
        }
        else {
            w.m_cells[i] = '.';
        }
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const world& w)
{
    os << w.m_width << " " << w.m_height << " " << w.m_spiders << "\n";

    for(size_t hi = 0; hi < w.m_height; hi++) {
        const size_t offset = w.m_width * hi;
        for(size_t wi = 0; wi < w.m_width; wi++) {
            const size_t index = offset + wi;
            os << w.m_cells[ index ];
        }
        os << "\n";
    }
    return os;
}

int has_spider(const world& origin, size_t distance, size_t x, size_t y)
{
    int spiders = 0;
    // up
    {
        size_t up_x = x;
        size_t up_y = y - distance;
        if('D' == origin.get_cell(up_x, up_y)) {
            spiders++;
        }
    }
    // down
    {
        size_t down_x = x;
        size_t down_y = y + distance;
        if('U' == origin.get_cell(down_x, down_y)) {
            spiders++;
        }
    }
    // left
    {
        size_t left_x = x - distance;
        size_t left_y = y;
        if('R' == origin.get_cell(left_x, left_y)) {
            spiders++;
        }
    }
    // right
    {
        size_t right_x = x + distance;
        size_t right_y = y;
        if('L' == origin.get_cell(right_x, right_y)) {
            spiders++;
        }
    }
    return spiders;
}

int simulate(const world& origin, size_t hero_w)
{
    const size_t size = origin.get_size();

    size_t spiders_counter = 0;
    size_t hero_index = hero_w;

    size_t tick = 0;
    do
    {
        spiders_counter += has_spider(origin, tick, hero_w, tick);
        hero_index += origin.get_width();
        tick++;
    }
    while(hero_index < size);

    return spiders_counter;
}

int main()
{
    world origin_world;

    //std::ifstream input("/home/eproydakov/repository/cppzone/tasks/zeptocode_rush_2014_b/input7.txt");
    //auto& src = input;
    auto& src = std::cin;
    src >> origin_world;

    //std::cout << origin_world;
    //std::cout << "-------------------------------------------------------------------------------" << std::endl;

    size_t width = origin_world.get_width();
    for(size_t w = 0; w < width; w++) {
        int spiders = simulate(origin_world, w);
        std::cout << spiders << " ";
    }
    std::cout << std::endl;

    return 0;
}
