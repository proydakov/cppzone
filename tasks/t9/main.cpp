/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <list>
#include <iostream>

#define TEST // DEFINE FOR BUILD TEST EXAMPLE
#define COMMENT

class dictionary
{
public:
    dictionary() :
        m_min_delta_priority(0.00001), 
        m_delta_priority(m_min_delta_priority),
        m_index(0)
    {
    }
    ~dictionary() {};
    
    friend std::ostream& operator<< (std::ostream& ostr, const dictionary& book);
    
    bool add_word(const std::string& string, double priority);
    bool nice(const std::string& string);
    
    void search(std::string& next_symbols, std::string& string);
    void next(std::string& next_string);
    void stop_search();
    
    void clear();
    
private:
    void find_priority_element(std::string& element);
    
private:
    typedef std::pair<std::string, double> information_about_word;
    typedef std::map<std::string, double> collection;
    typedef std::list<std::string> query;
    
    collection m_collection;
    collection m_selection;
    
    query m_query;
    
    double m_min_delta_priority;
    double m_delta_priority;
    
    int m_index;
};

class phone
{
public:
    phone() :
        m_started_writing_word(false),
        m_started_writing_mark(false)
    {
    }
    
    void parse_message(dictionary& i_dictionary, std::string& message);
    
private:
    void phone_keypad_transformer(char symbol, std::string& symbols);  
    
private:
    bool m_started_writing_word;
    bool m_started_writing_mark;
};

void read_input_data(dictionary& data);
void read_input_task(std::string& task);

void create_example_dictionary_1(dictionary& data);
void create_example_dictionary_2(dictionary& data);

void create_example_task_1(std::string& data);
void create_example_task_2(std::string& data);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    dictionary i_dictionary;
    phone i_phone;
    std::string task;
    
#ifdef TEST
    // test 1
    create_example_dictionary_1(i_dictionary);
    std::cout << i_dictionary;
    create_example_task_1(task);
    std::cout << task << "\n" << std::endl;
    
    i_phone.parse_message(i_dictionary, task);
    
    i_dictionary.clear();
    
    // test 2
    create_example_dictionary_2(i_dictionary);
    std::cout << i_dictionary;
    create_example_task_2(task);
    std::cout << task << "\n" << std::endl;
    
    i_phone.parse_message(i_dictionary, task);
    
    i_dictionary.clear();
#else
    read_input_data(i_dictionary);
    std::cout << i_dictionary;
    read_input_task(task);
    std::cout << task << "\n" << std::endl;
    
    i_phone.parse_message(i_dictionary, task);
    
    i_dictionary.clear();
#endif
    
    return 0;
}

std::ostream& operator<< (std::ostream& ostr, const dictionary& book)
{
    ostr << "\nDICTIONARY:" << std::endl;
    dictionary::collection::const_iterator endIt = book.m_collection.end();
    for(dictionary::collection::const_iterator it = book.m_collection.begin(); it != endIt; ++it) {
        ostr << "Word:   " << it->first << "   priority:   " << it->second << std::endl;
    }
    ostr << std::endl;
    return ostr;
}

bool dictionary::add_word(const std::string& string, double priority)
{
    return m_collection.insert(information_about_word(string, priority)).second;
}

bool dictionary::nice(const std::string& string)
{
    collection::iterator it = m_collection.find(string);
    bool res = it != m_collection.end();
    if(res) {
        it->second += 1 + m_delta_priority;
        m_delta_priority += m_min_delta_priority;  
    }
    return res;
}

void dictionary::search(std::string& next_symbols, std::string& string)
{
    if(m_query.empty()) {
        m_query.push_back(next_symbols);
        
        query::const_iterator it = m_query.begin();
        for(int i = 0; i < (*it).size(); ++i) {
            std::string symbol;
            symbol += (*it)[i];
            m_collection.lower_bound(symbol);
            collection::const_iterator itEndCollection = m_collection.upper_bound(symbol);
            for(collection::const_iterator itCollection = m_collection.begin(); itCollection != itEndCollection; ++itCollection) {
                m_selection.insert(*itCollection);
            }
        }
        ++m_index;
    }
    else {
        
    }
    next(string);
}

void dictionary::next(std::string& next_string)
{
    find_priority_element(next_string);
    m_selection.erase(next_string);
}

void dictionary::stop_search()
{
    m_index = 0;
    m_query.clear();
    m_selection.clear();
}

void dictionary::clear()
{
    stop_search();
    m_collection.clear();
}

void dictionary::find_priority_element(std::string& element)
{
    element = m_collection.begin()->first;
    collection::const_iterator element_iterator = m_collection.begin();
    collection::const_iterator endIt = m_collection.end();
    for(collection::const_iterator it = m_collection.begin(); it != endIt; ++it) {
        if(it->second > element_iterator->second) {
            element_iterator = it;
            element = it->first;
        }
    }
}

void phone::parse_message(dictionary& i_dictionary, std::string& message)
{
    char symbol;
    for(int i = 0; i < message.size(); ++i) {
        symbol = message[i];
        if(symbol == ' ') {
            if(m_started_writing_mark) {
                m_started_writing_mark = false;
            }
            else if(m_started_writing_word) {
                m_started_writing_word = false;
            }
        }
        else if(symbol == '*') {
            if(m_started_writing_mark) {
                
            }
            else if(m_started_writing_word) {
                
            }
        }
        else if(symbol == '1') {
            if(m_started_writing_word) {
                m_started_writing_word = false;
            }
        }
        else {
            
        }
    }    
}

void phone::phone_keypad_transformer(char symbol, std::string& symbols)
{
    switch(symbol) {
    case '1':
        symbols = ".,?";
        break;
    case '2':
        symbols = "abc";
        break;
    case '3':
        symbols = "def";
        break;
    case '4':
        symbols = "ghi";
        break;
    case '5':
        symbols = "jkl";
        break;
    case '6':
        symbols = "mno";
        break;
    case '7':
        symbols = "pqrs";
        break;
    case '8':
        symbols = "tuv";
        break;
    case '9':
        symbols = "wxyz";
        break;
    default:
        break;
    }
}

void read_input_data(dictionary &data)
{
#ifdef COMMENT
    std::cout << "Enter the size of the dictionary:  ";
#endif // COMMENT
    int lenght;
    std::cin >> lenght;
    
    std::string string;
    int priority;
    
    for(int i = 1; i <= lenght; ++i) {
#ifdef COMMENT
        std::cout << "Enter data about next word (text, priority): ";
#endif // COMMENT
        std::cin >> string;
        std::cin >> priority;
        data.add_word(string, priority);
    }
}

void read_input_task(std::string& task)
{
#ifdef COMMENT
    std::cout << "Enter a sequence of characters entered by: ";
#endif // COMMENT
    std::cin >> task;
    std::string temp;
    std::getline(std::cin ,temp);
    task += temp;
}

void create_example_dictionary_1(dictionary& data)
{
    data.add_word("ad", 2);
    data.add_word("be", 1);
    data.add_word("not", 10);
    data.add_word("or", 5);
    data.add_word("to", 50);
}

void create_example_dictionary_2(dictionary& data)
{
    data.add_word("act", 1);
    data.add_word("bat", 1);
    data.add_word("cat", 1);
}

void create_example_task_1(std::string& data)
{
    data = "86 23* 67 668 86 231**";
}

void create_example_task_2(std::string& data)
{
    data = "228* 228** 228** 228**1";  
}
