/*
 *  Copyright (c) 2020 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <vector>
#include <chrono>
#include <string>
#include <bitset>
#include <variant>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string_view>

class parser_exception : public std::runtime_error
{
public:
    parser_exception(std::size_t position, const std::string& what_arg)
        : std::runtime_error(what_arg)
        , m_position(position)
    {
    }

    parser_exception(std::size_t position, const char* what_arg)
        : std::runtime_error(what_arg)
        , m_position(position)
    {
    }

    std::size_t position() const noexcept
    {
        return m_position;
    }

private:
    std::size_t m_position;
};

class token final
{
public:
    static std::bitset<256> valid_symbols_mask;

    // List of supported token types
    enum class type {
        none,
        number,
        plus = '+',
        minus = '-',
        multiply = '*',
        divide = '/',
        opening_bracket = '(',
        closing_bracket = ')'
    };

    // Number type for used in calculator
    using number_t = long;

    // Value storage type
    using storage_t = std::variant<std::monostate, char, number_t>;

    explicit token(const char symbol, std::size_t position)
    {
        if (valid_symbols_mask[static_cast<std::size_t>(symbol)])
        {
            m_value = symbol;
            m_type = static_cast<type>(symbol);
            m_position = position;
        }
        else
        {
            throw parser_exception(position, "Can't parse token. Unknown symbol.");
        }
    }

    explicit token(std::string_view const text, std::size_t position)
    {
        if (!text.empty())
        {
            m_value = text_2_number(text, position);
            m_type = type::number;
            m_position = position;
        }
        else
        {
            throw parser_exception(position, "Can't parse token. Invalid symbol.");
        }
    }

    storage_t get_value() const noexcept
    {
        return m_value;
    }

    number_t get_number() const noexcept
    {
        return std::get<number_t>(m_value);
    }

    type get_type() const noexcept
    {
        return m_type;
    }

    std::size_t get_position() const noexcept
    {
        return m_position;
    }

    bool is_number() const noexcept
    {
        return m_type == type::number;
    }

    bool is_operator() const noexcept
    {
        return m_type == type::plus ||
               m_type == type::minus ||
               m_type == type::multiply ||
               m_type == type::divide
        ;
    }

    bool is_bracket() const noexcept
    {
        return m_type == type::opening_bracket ||
               m_type == type::closing_bracket
        ;
    }

    static int get_operator_priority(const type& t)
    {
        int priority = 0;
        switch (t) {
            case type::minus:
                priority = 0;
                break;

            case type::plus:
                priority = 0;
                break;

            case type::multiply:
                priority = 1;
                break;

            case type::divide:
                priority = 1;
                break;

            default:
                throw std::runtime_error("Invalid token type");
                break;
        }
        return priority;
    }

private:
    static number_t text_2_number(std::string_view text, std::size_t position)
    {
        number_t result{};
        for (auto const symbol : text)
        {
            if (symbol >= '0' && symbol <= '9')
            {
                result *= 10;
                result += static_cast<number_t>(symbol - '0');
            }
            else
            {
                throw parser_exception(position, "Can't parse token. Invalid input.");
            }
        }
        return result;
    }

private:
    type m_type;
    storage_t m_value;
    std::size_t m_position;
};


std::bitset<256> build_token_bitmask()
{
    std::bitset<256> mask;
    mask['+'] = true;
    mask['-'] = true;
    mask['*'] = true;
    mask['/'] = true;
    mask['('] = true;
    mask[')'] = true;
    return mask;
}

std::bitset<256> token::valid_symbols_mask = build_token_bitmask();

std::ostream& operator<<(std::ostream& os, const token::type type)
{
    switch (type) {
    case token::type::number:
        os << "number";
        break;
    case token::type::minus:
        os << "minus";
        break;
    case token::type::plus:
        os << "plus";
        break;
    case token::type::multiply:
        os << "multiply";
        break;
    case token::type::divide:
        os << "divide";
        break;
    case token::type::opening_bracket:
        os << "opening_bracket";
        break;
    case token::type::closing_bracket:
        os << "closing_bracket";
        break;
    case token::type::none:
        os << "none";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const token::storage_t storage)
{
    std::visit([&os](auto&& arg) {
        os << arg;
    }, storage);
    return os;
}

class expression_calculator final
{
public:
    expression_calculator() = default;

    expression_calculator(std::string_view input)
    {
        reserve(input);
    }

    // preallocate internal buffers
    void reserve(std::string_view input)
    {
        m_raw_tokens.reserve(input.size());
        m_rpn_tokens.reserve(input.size());
        m_tmp_tokens_stack.reserve(input.size());
        m_calc_stack.reserve(input.size());
    }

    // reset calculator for next expression
    void reset()
    {
        m_raw_tokens.clear();
        m_rpn_tokens.clear();
        m_tmp_tokens_stack.clear();
        m_calc_stack.clear();
    }

    template<bool trace>
    token::number_t calculate(const std::string_view input)
    {
        to_raw_tokens(input);
        if (trace)
        {
            std::cout << "tokens:" << std::endl;
            std::for_each(m_raw_tokens.begin(), m_raw_tokens.end(), [](const token& t)
            {
                std::cout << "value: '" << t.get_value() << "' type: " << t.get_type() << std::endl;
            });
            std::cout << std::endl;
        }

        if (m_raw_tokens.empty())
        {
            throw std::runtime_error("Invalid tokens: empty input string.");
        }

        to_rpn_tokens();
        if (trace)
        {
            std::cout << "Reverse Polish notation:" << std::endl;
            std::for_each(m_rpn_tokens.begin(), m_rpn_tokens.end(), [](const token& t)
            {
                std::cout << "value: '" << t.get_value() << "' type: " << t.get_type() << std::endl;
            });
            std::cout << std::endl;
        }

        if (m_rpn_tokens.empty())
        {
            throw std::runtime_error("Invalid rpn: empty input string.");
        }

        return calculate<trace>();
    }

private:
    // Convert input string into vector of primitive tokens
    void to_raw_tokens(const std::string_view input)
    {
        std::size_t number_size = 0;

        bool allow_negative_number = true;

        auto try_to_flush_number = [&number_size, cptr = input.data(), &raw_tokens = m_raw_tokens, &allow_negative_number](std::size_t current_index)
        {
            if (number_size > 0)
            {
                auto const start_index = current_index - number_size;
                raw_tokens.emplace_back(std::string_view(cptr + start_index, number_size), start_index);
                number_size = 0;
                allow_negative_number = false;
            }
        };

        for(std::size_t index = 0; index < input.size(); index++)
        {
            auto const symbol = input[index];

            switch (symbol)
            {
                case ' ':
                {
                    try_to_flush_number(index);
                }
                    break;

                case '-':
                {
                    try_to_flush_number(index);
                    auto const next = index + 1;
                    if (allow_negative_number && next < input.size() && std::isdigit(input[next]))
                    {
                        ++number_size;
                        continue;
                    }
                }
                [[fallthrough]];
                case '+':
                case '*':
                case '/':
                case '(':
                case ')':
                {
                    try_to_flush_number(index);
                    m_raw_tokens.emplace_back(symbol, index);
                    allow_negative_number = (')' != symbol);
                }
                    break;

                default:
                {
                    if (std::isdigit(symbol))
                    {
                        ++number_size;
                    }
                    else
                    {
                        std::stringstream error;
                        error << "Can't convert unknown symbol: '" << symbol << "' to token.";
                        throw parser_exception(index, error.str());
                    }
                }
                    break;
            }
        };

        try_to_flush_number(input.size());
   }

    // Reverse Polish notation
    // https://en.wikipedia.org/wiki/Reverse_Polish_notation
    void to_rpn_tokens()
    {
        token::type last_type = token::type::none;
        for (auto const& t : m_raw_tokens)
        {
            switch (t.get_type())
            {
                case token::type::number:
                {
                    m_rpn_tokens.emplace_back(t);
                }
                    break;

                case token::type::opening_bracket:
                {
                    if (last_type == token::type::closing_bracket)
                    {
                        throw parser_exception(t.get_position(), "Invalid tokens: missed operator.");
                    }
                    m_tmp_tokens_stack.emplace_back(t);
                }
                    break;

                case token::type::closing_bracket:
                {
                    auto temp_token = t;
                    while (temp_token.get_type() != token::type::opening_bracket)
                    {
                        if (m_tmp_tokens_stack.empty())
                        {
                            throw parser_exception(t.get_position(), "Invalid tokens: incorrectly placed separator or not agreed brackets.");
                        }
                        temp_token = m_tmp_tokens_stack.back();
                        m_tmp_tokens_stack.pop_back();
                        if (!temp_token.is_bracket())
                        {
                            m_rpn_tokens.emplace_back(temp_token);
                        }
                    }
                }
                    break;

                default:
                {
                    auto priority = token::get_operator_priority(t.get_type());
                    while (!m_tmp_tokens_stack.empty())
                    {
                        auto temp_token = m_tmp_tokens_stack.back();
                        if (temp_token.is_operator())
                        {
                            auto temp_priority = token::get_operator_priority(temp_token.get_type());
                            if (priority <= temp_priority)
                            {
                                m_rpn_tokens.emplace_back(temp_token);
                                m_tmp_tokens_stack.pop_back();
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    m_tmp_tokens_stack.emplace_back(t);
                }
                    break;
            }
            last_type = t.get_type();
        }

        while (!m_tmp_tokens_stack.empty())
        {
            auto temp_token = m_tmp_tokens_stack.back();
            m_tmp_tokens_stack.pop_back();
            if (token::type::opening_bracket == temp_token.get_type() || token::type::closing_bracket == temp_token.get_type())
            {
                throw parser_exception(temp_token.get_position(), "Invalid tokens: not agreed brackets.");
            }
            m_rpn_tokens.emplace_back(temp_token);
        }
    }

    template<bool trace>
    token::number_t calculate()
    {
        if (trace)
        {
            std::cout << "calculate:" << std::endl;
        }
        for (auto t : m_rpn_tokens)
        {
            if (t.is_number())
            {
                m_calc_stack.emplace_back(t.get_number());
            }
            else
            {
                if (m_calc_stack.size() < 2)
                {
                    throw parser_exception(t.get_position(), "Invalid RPN tokens: not found elements in calc stack");
                }

                token::number_t const right_element = m_calc_stack.back();
                m_calc_stack.pop_back();

                token::number_t const left_element = m_calc_stack.back();
                m_calc_stack.pop_back();

                token::number_t calc_result = 0;
                if (trace)
                {
                    std::cout << "action: " << t.get_type() << std::endl;
                }
                switch (t.get_type())
                {
                    case token::type::minus:
                        calc_result = left_element - right_element;
                        break;
                    case token::type::plus:
                        calc_result = left_element + right_element;
                        break;
                    case token::type::multiply:
                        calc_result = left_element * right_element;
                        break;
                    case token::type::divide:
                        calc_result = left_element / right_element;
                        break;
                    default:
                        throw parser_exception(t.get_position(), "Invalid RPN token in calc operations");
                        break;
                    }
                    m_calc_stack.emplace_back(calc_result);
            }
            if (trace)
            {
                std::cout << "Calculation stack: ";
                std::for_each(m_calc_stack.begin(), m_calc_stack.end(), [](const token::number_t& value) {
                    std::cout << value << " ";
                });
                std::cout << std::endl;
            }
        }

        if (trace)
        {
            std::cout << std::endl;
        }

        if (m_calc_stack.size() != 1)
        {
            std::stringstream error;
            error << "Invalid RPN tokens: missed operator. Calculation stack contain numbers: ";
            bool first = true;
            for (auto const number : m_calc_stack)
            {
                if (!first)
                {
                    error << ", ";
                }
                error << number;
                first = false;
            }
            throw std::runtime_error(error.str());
        }

        return m_calc_stack.back();
    }

private:
    std::vector<token> m_raw_tokens;
    std::vector<token> m_rpn_tokens;
    std::vector<token> m_tmp_tokens_stack;
    std::vector<token::number_t> m_calc_stack;
};

///////////////////////////////////////////////////////////////////////////////

// https://misc.flogisoft.com/bash/tip_colors_and_formatting
enum class bash_text_color
{
    reset = 0,
    black = 90,
    red = 91,
    green = 92,
    yellow = 93,
    blue = 94,
    magenta = 95,
    cyan = 96,
    white = 97
};

std::ostream& operator<<(std::ostream& os, bash_text_color color)
{
    os << "\x1B[" << static_cast<int>(color) << 'm';
    return os;
}

int main(int argc, char *argv[])
{
    bool const trace = (2 == argc) && std::string_view("trace") == std::string_view(argv[1]);

    std::cout << "Enter string for calculation: ";
    std::string input;
    std::getline(std::cin, input);
    std::cout << std::endl;

    expression_calculator calculator(input);

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    //long total = 0;

    //for(int i = 0; i < 10'000'000; i++)
    try
    {
        auto const result = trace ?
            calculator.calculate<true>(input) :
            calculator.calculate<false>(input);
        //total += result;
        std::cout << "result: " << result << std::endl;
        calculator.reset();
    }
    catch (parser_exception const& error)
    {
        std::cerr << bash_text_color::red << "Parser error. " << bash_text_color::reset << error.what() << std::endl;
        std::cerr << input << std::endl;
        std::cerr << std::string(error.position(), ' ') << bash_text_color::green << "^" << bash_text_color::reset << std::endl;
    }
    catch (std::exception const& error)
    {
        std::cerr << "Internal error. " << error.what() << std::endl;
    }

    auto end = clock.now();
    std::cout << "Input size: " << input.size() << " calculate: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;

    //std::cout << "Total: " << total << std::endl;

    return 0;
}
