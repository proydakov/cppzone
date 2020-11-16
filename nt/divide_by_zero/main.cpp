#include <memory>
#include <iostream>

class stack_object
{
public:
    stack_object(char const* const comment)
        : m_common(comment)
    {
        std::cout << "ctor: " << m_common << std::endl;
    }

    ~stack_object()
    {
        std::cout << "dtor: " << m_common << std::endl;
    }

private:
    char const* const m_common;
};

int main(int argc, char*[])
{
    auto i = argc - 1;

    try {
        stack_object object("stack_object");

        i = 5 / i;
    }
    catch (...) {
        std::cerr << "Catch SEH error...\n";
    }

    return 0;
}
