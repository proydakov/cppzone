#include <stack>
#include <string>
#include <iostream>

template<class T>
class buffer_guard
{
public:
    buffer_guard(T& buffer, size_t size) : buffer_(buffer), size_(size)
    {
    }

    ~buffer_guard()
    {
        buffer_.resize(size_);
    }

private:
    T& buffer_;
    const size_t size_;
};

void generate_step(const std::string& origin, size_t opos, std::string& buffer)
{
    const buffer_guard<std::string> guard(buffer, buffer.size());

    const size_t openpos = origin.find('{', opos);
    if(openpos == std::string::npos) {
        for(size_t i = opos; i < origin.size(); i++) {
            buffer.push_back( origin[ i ] );
        }
        std::cout << buffer << std::endl;
        return;
    }
    for(size_t i = opos; i < openpos; i++) {
        buffer.push_back( origin[i] );
    }

    const size_t closepos = origin.find('}', openpos);
    bool empty = true;
    for(size_t i = openpos + 1; i < closepos; i++) {
        empty = false;
        buffer.push_back(origin[i]);
        generate_step(origin, closepos + 1, buffer);
        buffer.pop_back();
    }
    if(empty) {
        generate_step(origin, closepos + 1, buffer);
    }
}

void generate(const std::string& origin)
{
    std::cout << "generate:" << std::endl;

    if(origin.empty()) {
        return;
    }
    std::string buffer;
    buffer.reserve(origin.size());

    generate_step(origin, 0, buffer);
}

bool validate(const std::string& origin)
{
    std::stack<char> validator;

    for(size_t i = 0; i < origin.size(); i++) {
        char c = origin[i];
        switch(c) {
            case '}':
            {
                if(validator.empty()) {
                    return false;
                }
                validator.pop();
            }
                break;

            case '{':
            {
                if(!validator.empty()) {
                    return false;
                }
                validator.push(c);
            }
                break;
        }
    }

    return validator.empty();
}

int main( int argc, char* argv[] )
{
    if(argc != 2) {
        std::cout << argv[0] << " <regex_string>" << std::endl;
        return 1;
    }

    const std::string regex( argv[1] );
    const bool valid = validate(regex);
    std::cout << "regex: " << regex << " valid: " << valid <<  std::endl;

    generate(regex);

    return 0;
}
