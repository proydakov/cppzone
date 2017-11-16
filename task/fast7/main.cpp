#include <limits>
#include <vector>
#include <cstdint>
#include <iostream>

enum class protocol_error {
    none,
    value_overflow,
    spent_space,
    tail_error
};

std::ostream& operator<<(std::ostream& os, protocol_error error)
{
    switch(error) {
        case protocol_error::none:
        os << "protocol_error::none";
        break;

        case protocol_error::value_overflow:
        os << "protocol_error::value_overflow";
        break;

        case protocol_error::spent_space:
        os << "protocol_error::spent_space";
        break;

        case protocol_error::tail_error:
        os << "protocol_error::tail_error";
        break;
    }
    return os;
}

protocol_error decode(const std::vector<uint8_t>& memory, std::vector<uint32_t>& data)
{
    data.reserve(memory.size());
    data.resize(0);

    uint64_t buffer{0};
    int counter = 0;

    for(const auto byte : memory) {
        counter++;

        const bool flag   = byte & 0b10000000;
        const uint8_t val = byte & 0b01111111;

        buffer <<= 7;
        buffer += val;

        if(counter > 5 || buffer > std::numeric_limits<uint32_t>::max()) {
            return protocol_error::value_overflow;
        }

        if(flag) {
            int real_counter = buffer == 0 ? 1 : 0;
            uint64_t temp_buffer = buffer;
            while(temp_buffer > 0) {
                temp_buffer >>= 7;
                real_counter++;
            }
            if(real_counter != counter) {
                return protocol_error::spent_space;
            }
            data.push_back(static_cast<uint32_t>(buffer));
            buffer  = 0;
            counter = 0;
        }
    }

    return (buffer == 0 && counter == 0) ? protocol_error::none : protocol_error::tail_error;
}

void test(const std::vector<uint8_t>& memory, const protocol_error result)
{
    std::vector<uint32_t> data;
    const protocol_error test_result = decode(memory, data);

    if(result != test_result) {
        std::cout << "!!! bad result. need: " << result << " got: " << test_result << std::endl;
        return;
    }

    if(test_result != protocol_error::none) {
        std::cout << "invalid input. error: " << test_result << std::endl;
        return;
    }

    std::cout << "values: ";
    for(size_t i = 0; i < data.size(); i++) {
        std::cout << data[i];
        if(i != data.size() - 1) std::cout << ", ";
    }

    std::cout << std::endl;
}

class scope
{
public:
    scope(const std::string& str)
    {
        std::cout << str << "\n" << std::endl;
    }

    ~scope()
    {
        std::cout << std::endl;
    }
};

int main()
{
    {
        scope s("// single value");

        test({ 0b00000001, 0b10000000 }, protocol_error::none);
        test({ 0b00000001, 0b00000000, 0b10000000 }, protocol_error::none);
        test({ 0b00000001, 0b01111111, 0b11111111 }, protocol_error::none);
        test({ 0b00000001, 0b01111111, 0b01111111, 0b11111111 }, protocol_error::none);
        test({ 0b00001111, 0b01111111, 0b01111111, 0b01111111, 0b11111111 }, protocol_error::none);
    }

    {
        scope s("// multiple values");

        test({ 0b10000000,
               0b10000001,
               0b10000010
        }, protocol_error::none);

        test({ 0b10000000,
               0b00000001, 0b01111111, 0b01111111, 0b11111111,
               0b10000010
        }, protocol_error::none);

        test({ 0b00000001, 0b01111111, 0b11111111, 0b10000010 }, protocol_error::none);
        test({ 0b00001111, 0b01111111, 0b01111111, 0b01111111, 0b11111111,
               0b00001111, 0b01111111, 0b01111111, 0b01111111, 0b11111111,
               0b00001111, 0b01111111, 0b01111111, 0b01111111, 0b11111111,
               0b00001111, 0b01111111, 0b01111111, 0b01111111, 0b11111111,
               0b00001111, 0b01111111, 0b01111111, 0b01111111, 0b11111111,
        }, protocol_error::none);
    }

    {
        scope s("// value_overflow");

        test({ 0b00011111, 0b01111111, 0b01111111, 0b01111111, 0b11111111 }, protocol_error::value_overflow);
        test({ 0b01111111, 0b01111111, 0b01111111, 0b01111111, 0b11111111 }, protocol_error::value_overflow);
        test({ 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 }, protocol_error::value_overflow );
    }

    {
        scope s("// spent_space");

        test({ 0b00000000, 0b10000000 }, protocol_error::spent_space);
        test({ 0b00000000, 0b00000001, 0b10000000 }, protocol_error::spent_space);
        test({ 0b00000000, 0b00000000, 0b00000000, 0b10000000 }, protocol_error::spent_space );
        test({ 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b10000000 }, protocol_error::spent_space );
    }

    {
        scope s("// tail_error");

        test({ 0b00000000 }, protocol_error::tail_error);
        test({ 0b00000000, 0b00000000 }, protocol_error::tail_error);
        test({ 0b10000000, 0b10000001, 0b10000010, 0b00000000 }, protocol_error::tail_error);
        test({ 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000 }, protocol_error::tail_error );
    }

    return 0;
}
