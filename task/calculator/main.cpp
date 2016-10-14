/*
 *  Copyright (c) 2014 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <list>
#include <cmath>
#include <locale>
#include <chrono>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

class token
{
public:
    enum class type { number, plus, minus, multiply, divide, involution, opening_bracket, closing_bracket, none };

    token(const std::string& text) :
        m_value(text) {
        if(text.size() == 1) {
            switch(text[0]) {
            case '+':
                m_type = type::plus;
                break;
            case '-':
                m_type = type::minus;
                break;
            case '*':
                m_type = type::multiply;
                break;
            case '/':
                m_type = type::divide;
                break;
            case '^':
                m_type = type::involution;
                break;
            case '(':
                m_type = type::opening_bracket;
                break;
            case ')':
                m_type = type::closing_bracket;
                break;
            default:
                m_type = type::none;
                break;
            }
        }
		std::locale loc;
		if (std::isdigit(text[0], loc) || (text[0] == '-' && std::isdigit(text[1], loc))) {
            m_type = type::number;
        }
    }

    std::string get_value() const {
        return m_value;
    }

    type get_type() const {
        return m_type;
    }

    bool is_number() const {
        return m_type == type::number;
    }

    bool is_operator() const {
        return m_type == type::plus || m_type == type::minus ||
                m_type == type::multiply || m_type == type::divide ||
                m_type == type::involution;
    }

    bool is_bracket() const {
        return m_type == type::opening_bracket || m_type == type::closing_bracket;
    }

    static int get_operator_priority(const type& t) {
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
        case type::involution:
            priority = 2;
            break;
        default:
            throw std::runtime_error("Invalid token type");
            break;
        }
        return priority;
    }

private:
    std::string m_value;
    type m_type;
};

std::ostream& operator<<(std::ostream& os, const token::type& type)
{
    std::string result;
    switch (type) {
    case token::type::number:
        result = "number";
        break;
    case token::type::minus:
        result = "minus";
        break;
    case token::type::plus:
        result = "plus";
        break;
    case token::type::multiply:
        result = "multiply";
        break;
    case token::type::divide:
        result = "divide";
        break;
    case token::type::involution:
        result = "involution";
        break;
    case token::type::opening_bracket:
        result = "opening_bracket";
        break;
    case token::type::closing_bracket:
        result = "closing_bracket";
        break;
    case token::type::none:
        result = "none";
        break;
    default:
        throw std::runtime_error("Invalid token type");
        break;
    }
    os << result;
    return os;
}

///////////////////////////////////////////////////////////////////////////////

std::list<token> to_tokens(const std::string& input)
{
    std::locale loc;
    std::list<token> tokens;
    std::string value;
    bool first = true;
    bool operation = false;
    bool skip = false;
    std::for_each(input.begin(), input.end(), [&](const char symbol) {
        switch (symbol) {
        case ' ':
            break;
        case '-':
        {
            if(first || operation) {
                value.push_back(symbol);
                skip = true;
            }
        }
        case '+':
        case '*':
        case '/':
        case '(':
        case ')':
        case '^':
        {
            if(!skip) {
                if(value.length()) {
                    tokens.push_back(token(value));
                    value.clear();
                }
                value = symbol;
                tokens.push_back(token(value));
                value.clear();
            }
            operation = true;
            skip = false;
        }
            break;
        default:
			if (std::isdigit(symbol, loc) || '.' == symbol) {
                value.push_back(symbol);
                operation = false;
            }
            else {
                std::stringstream error;
                error << "Invalid symbol: " << symbol;
                throw std::runtime_error(error.str());
            }
            break;
        }
        first = false;
    });
    if(value.length()) {
        tokens.push_back(token(value));
    }
    return tokens;
}

///////////////////////////////////////////////////////////////////////////////

std::list<token> to_rpn(const std::list<token>& tokens)
{
    std::list<token> rpn_tokens;
    std::list<token> temp_stack;
    token::type last_type = token::type::none;
    std::for_each(tokens.begin(), tokens.end(), [&](const token& t) {
        switch (t.get_type()) {
        case token::type::number:
        {
            rpn_tokens.push_back(t);
        }
            break;
        case token::type::opening_bracket:
        {
            if(last_type == token::type::closing_bracket) {
                throw std::runtime_error("Invalid tokens: missed operator");
            }
            temp_stack.push_back(t);
        }
            break;
        case token::type::closing_bracket:
        {
            auto temp_t = t;
            while(temp_t.get_type() != token::type::opening_bracket) {
                if(temp_stack.empty()) {
                    throw std::runtime_error("Invalid tokens: incorrectly placed separator or not agreed brackets");
                }
                temp_t = temp_stack.back();
                temp_stack.pop_back();
                if(!temp_t.is_bracket()) {
                    rpn_tokens.push_back(temp_t);
                }
            }
        }
            break;
        default:
            if(!t.is_operator()) {
                throw std::runtime_error("Invalid token type");
            }
            auto priority = token::get_operator_priority(t.get_type());
			while (!temp_stack.empty()) {
				auto temp_t = temp_stack.back();
				if (temp_t.is_operator()) {
					auto temp_priority = token::get_operator_priority(temp_t.get_type());
					if (priority <= temp_priority) {
						rpn_tokens.push_back(temp_t);
						temp_stack.pop_back();
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
            temp_stack.push_back(t);
            break;
        }
        last_type = t.get_type();
    });
    while(!temp_stack.empty()) {
        auto temp_t = temp_stack.back();
        temp_stack.pop_back();
        if(temp_t.get_type() == token::type::opening_bracket || temp_t.get_type() == token::type::closing_bracket) {
            throw std::runtime_error("Invalid tokens: not agreed brackets");
        }
        rpn_tokens.push_back(temp_t);
    }
    return rpn_tokens;
}

///////////////////////////////////////////////////////////////////////////////

template<class T, class string2T>
T calculate(const std::list<token>& rpn, string2T fun)
{
    std::list<T> calc_stack;
    std::cout << "calculate:" << std::endl;
    std::for_each(rpn.begin(), rpn.end(), [&](const token& t) {
        if(t.is_number()) {
            T element = static_cast<T>(fun(t.get_value().c_str()));
            calc_stack.push_back(element);
        }
        else {
            if(calc_stack.size() < 2) {
                throw std::runtime_error("Invalid RPN tokens: not found elements in calc stack");
            }
            T right_element = calc_stack.back();
            calc_stack.pop_back();
            T left_element = calc_stack.back();
            calc_stack.pop_back();
            T calc_result = 0;
            switch (t.get_type()) {
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
            case token::type::involution:
                calc_result = std::pow(left_element, right_element);
                break;
            default:
                throw std::runtime_error("Invalid RPN token in calc operations");
                break;
            }
            calc_stack.push_back(calc_result);
        }
        std::cout << "calc stack: ";
        std::for_each(calc_stack.begin(), calc_stack.end(), [&](const T& value) {
            std::cout << value << " ";
        });
        std::cout << std::endl;
    });
    std::cout << std::endl;
    if(calc_stack.size() != 1) {
        throw std::runtime_error("Invalid RPN tokens: missed operator");
    }
    return static_cast<T>(calc_stack.back());
}

///////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] )
{
    (void) argc;
    (void) argv;

    std::cout << "Enter string for calculation: ";
    std::string input;
    std::getline(std::cin, input);
    std::cout << std::endl;

    std::chrono::high_resolution_clock clock;
    auto start = clock.now();

    try {
        const auto& tokens = to_tokens(input);
        std::cout << "tokens:" << std::endl;
        std::for_each(tokens.begin(), tokens.end(), [&tokens](const token& t) {
            std::cout << "value: " << t.get_value() << " type: " << t.get_type() << std::endl;
        });
        std::cout << std::endl;

        if(tokens.empty()) {
            throw std::runtime_error("Invalid tokens: empty input string");
        }

        const auto& rpn = to_rpn(tokens);
        std::cout << "RPN:" << std::endl;
        std::for_each(rpn.begin(), rpn.end(), [&tokens](const token& t) {
            std::cout << "value: " << t.get_value() << " type: " << t.get_type() << std::endl;
        });
        std::cout << std::endl;

        if(rpn.empty()) {
            throw std::runtime_error("Invalid rpn: empty input string");
        }

        const auto& result = calculate<double>(rpn, std::atof);
        std::cout << "result: " << result << std::endl;
    }
    catch(const std::exception& error) {
        std::cout << "Error: " << error.what() << std::endl;
    }

    auto end = clock.now();
    std::cout << "Input size: " << input.size() << " calculate: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us" << std::endl;
    return 0;
}
