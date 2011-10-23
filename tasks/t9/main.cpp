/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <iostream>

#define TEST // DEFINE FOR BUILD TEST EXAMPLE

class dictionary
{
public:
    dictionary() {};
    ~dictionary() {};
    
    void print();
    
    bool add_word(const std::string& string, int priority);
    bool nice(const std::string& string);
    
    void search(char next_symbol, std::string& string);
    void next(std::string& next_string);
    void stop_search();
    
    void clear();
    
private:
    typedef std::pair<std::string, int> information_about_word;
    typedef std::map<std::string, int> collection;
    
    collection   m_collection;
    collection   m_selection;
    int          m_index;
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
    
    dictionary my_dictionary;
    std::string string;
    
#ifdef TEST
    create_example_dictionary_1(my_dictionary);
    my_dictionary.print();
    create_example_task_1(string);
    std::cout << string << "\n" << std::endl;
    my_dictionary.clear();
    
    create_example_dictionary_2(my_dictionary);
    my_dictionary.print();
    create_example_task_2(string);
    std::cout << string << "\n" << std::endl;
    my_dictionary.clear();
#else
    
#endif
    
    return 0;
}

void dictionary::print()
{
    std::cout << "DICTIONARY:" << std::endl;
    collection::const_iterator endIt = m_collection.end();
    for(collection::const_iterator it = m_collection.begin(); it != endIt; ++it) {
        std::cout << "Word:   " << it->first << "   priority:   " << it->second << std::endl;
    }
    std::cout << std::endl;
}

bool dictionary::add_word(const std::string& string, int priority)
{
    return m_collection.insert(information_about_word(string, priority)).second;
}

bool dictionary::nice(const std::string& string)
{
    collection::iterator it = m_collection.find(string);
    bool res = it != m_collection.end();
    if(res) {
        ++(it->second);
    }
    return res;
}

void dictionary::search(char next_symbol, std::string& string)
{
    
}

void dictionary::next(std::string& next_string)
{
    
}

void dictionary::stop_search()
{
    m_index = 0;
    m_selection.clear();
}

void dictionary::clear()
{
    m_collection.clear();
}

void read_input_data(dictionary &data)
{
#ifdef TEST
    std::cout << "Enter the size of the dictionary:  ";
#endif
    int lenght;
    std::cin >> lenght;
    
    std::string string;
    int priority;
    
    for(int i = 1; i <= lenght; ++i) {
#ifdef TEST
        std::cout << "Enter the data dictionary of word" << i <<  ":";
#endif
        std::cin >> string;
        std::cin >> priority;
        data.add_word(string, priority);
    }
}

void read_input_task(std::string& task)
{
#ifdef TEST
    std::cout << "Enter a sequence of characters entered by: ";
#endif
    std::cin >> task;
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
