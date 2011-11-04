/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <list>
#include <vector>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <assert.h>

#include <benchmark.h>

//#define COMMENT

typedef unsigned int    index;
typedef double          priority;
typedef char            symbol;
typedef std::string     string;

const index MAX_NUM_WORDS   = 100000;
const priority MIN_PRIORITY = (priority) 1 / MAX_NUM_WORDS;

const index MAX_WORD_SIZE = 20;
const priority MAX_PRIORITY = 2000;

class dictionary
{
public:
    dictionary() :
        m_query_size(0),
        m_word_position(0),
        m_delta_priority(MIN_PRIORITY)
    {
        m_query.resize(MAX_WORD_SIZE);
    }
    
    bool add_word(const string& word, priority word_priority);
    
    void search(const string& next_symbols);
    void next();
    void stop_search(string& word);
    
    friend std::ostream& operator<< (std::ostream& ostr, const dictionary& book);
    
private:
    void parsing_query();
    void find_priority_word(string& word);
    bool nice(const string& word);
    
    void clear();
    
private:
    typedef std::map<string, priority> collection;
    typedef std::multimap<priority, string> selection;
    typedef std::vector<string> query;
    
    collection m_collection;
    selection m_selection;
    
    query m_query;
    index m_query_size;
    index m_word_position;
    
    priority m_delta_priority;
};

class phone
{
public:
    phone(dictionary* phone_dictionary) :
        m_dictionary(phone_dictionary),
        m_state(state_end)
    {
    }
    
    void parse_message(const string& message);
    
private:
    // state machine
    enum states {
        state_start,
        state_enter_space,
        state_enter_mark,
        state_enter_word,
        state_next_element,
        state_end
    };
    
private:
    void phone_keypad_transformer(symbol key, string& symbols);  
    
    void print_mark(const string& marks, index& current_mark_index, bool& mark_enter_state);
    void print_word(bool& word_enter_state);
    
private:
    dictionary* m_dictionary;
    
    states m_state;
};

void read_input_data(dictionary& data);
void read_input_task(string& task);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    dictionary i_dictionary;
    phone i_phone(&i_dictionary);
    string task;
    
    read_input_data(i_dictionary);
    read_input_task(task);
    
    benchmark::benchmark parsing;
    parsing.start();
    i_phone.parse_message(task);
    parsing.stop();
    std::cout << "PARSING:  " << parsing.get_last_interval() << "  " << parsing.get_unit() << std::endl;
    
    return 0;
}

std::ostream& operator<< (std::ostream& ostr, const dictionary& book)
{
    ostr << "\nDICTIONARY:" << std::endl;
    dictionary::collection::const_iterator endIt = book.m_collection.end();
    for(dictionary::collection::const_iterator it = book.m_collection.begin(); it != endIt; ++it) {
        ostr << "Word:   " << it->first << "   priority:   " << it->second << std::endl;
    }
    return ostr;
}

bool dictionary::add_word(const string& word, priority word_priority)
{
    return m_collection.insert(collection::value_type(word, word_priority)).second;
}

void dictionary::search(const string& next_symbols)
{
    m_query[m_query_size] = next_symbols;
    ++m_query_size;
}

void dictionary::next()
{
    ++m_word_position;
}

void dictionary::stop_search(string& word)
{
    parsing_query();
    /*
    std::cout << "\nSELECTION:" << std::endl;
    selection::const_iterator endIt = m_selection.end();
    for(selection::const_iterator it = m_selection.begin(); it != endIt; ++it) {
        std::cout << "Word:   " << it->second << "   priority:   " << it->first << std::endl;
    }
    */
    find_priority_word(word);
    
    m_selection.clear();
    m_query_size = 0;
    m_word_position = 0;
    
    nice(word);
}

void dictionary::parsing_query()
{
    string symbols = m_query[0];
    
    symbol end_symbol = symbols[symbols.size() - 1];
    
    collection::const_iterator endCollection = m_collection.end();
    collection::const_iterator itCollection;
    
    for(index i = 0; i < symbols.size(); ++i) {
        std::string search_symbol;
        search_symbol = symbols[i];
        itCollection = m_collection.lower_bound(search_symbol);
        if(itCollection != endCollection)
            break;
    }
    
    //string first = itCollection->first;
    
    index lower_bound_test = 1;
    index upper_bound_test = m_query_size;
    
    string word;
    
    bool insert_word;
    while(itCollection != endCollection && itCollection->first[0] <= end_symbol) {
        insert_word = true;
        
        //string iter_word = itCollection->first;
        //(void)iter_word;
        
        if(itCollection->first.size() != upper_bound_test) {
            insert_word = false;
        }
        else {
            for(index i = lower_bound_test; i < upper_bound_test; ++i) {
                if(m_query[i].find_first_of(itCollection->first[i]) == -1) {
                    insert_word = false;
                    break;
                }
            }
        }
        if(insert_word) {
            m_selection.insert(selection::value_type(MAX_PRIORITY - itCollection->second, itCollection->first));
            word = itCollection->first;
        }
        ++itCollection;
    }
}

void dictionary::find_priority_word(string& word)
{
    if(m_selection.empty()) {
        assert(!"selection data empty");
    }
    selection::const_iterator it = m_selection.begin();
    for(index i = 0; i < m_word_position; ++i) {
        ++it;
    }
    word = it->second;
}

bool dictionary::nice(const string& word)
{
    collection::iterator it = m_collection.find(word);
    bool res = it != m_collection.end();
    if(res) {
        it->second += 1 + m_delta_priority;
        m_delta_priority += MIN_PRIORITY;  
    }
    else {
        assert(!"Word not found");
    }
    return res;
}

void dictionary::clear()
{
    m_collection.clear();
    m_selection.clear();
    
    m_query.clear();
    m_word_position = 0;
    
    m_delta_priority = MIN_PRIORITY;
}

void phone::parse_message(const string& message)
{   
    m_state = state_start;
    
    string marks = ".,?";
    index current_mark_index = 0;
    
    string symbols;
    
    index message_size = message.size();
    
    symbol current_symbol;
    
    bool started_writing_word = false;
    bool started_writing_mark = false;
    
    for(index i = 0; i < message_size; ++i) {
        current_symbol = message[i];
        
        if(current_symbol == ' ') {
            m_state = state_enter_space;
        }
        else if(current_symbol == '*') {
            m_state = state_next_element;
        }
        else if(current_symbol == '1') {
            m_state = state_enter_mark;
        }
        else {
            m_state = state_enter_word;
        }
        
        switch(m_state) {
        case state_enter_space:
            if(started_writing_word) {
                print_word(started_writing_word);
            }
            else if(started_writing_mark) {
                print_mark(marks, current_mark_index, started_writing_mark);
            }
            std::cout << ' ';
            break;
            
        case state_enter_mark:
            if(started_writing_word) {
                print_word(started_writing_word);
            }
            else if(started_writing_mark) {
                print_mark(marks, current_mark_index, started_writing_mark);
            }
            started_writing_mark = true;
            current_mark_index = 0;
            break;
            
        case state_enter_word:
            if(started_writing_mark) {
                print_mark(marks, current_mark_index, started_writing_mark);
            }
            started_writing_word = true;
            phone_keypad_transformer(message[i], symbols);
            m_dictionary->search(symbols);
            break;
            
        case state_next_element:
            if(started_writing_word) {
                m_dictionary->next();
            }
            else if(started_writing_mark) {
                ++current_mark_index;
            }
            break;
            
        default:
            assert(!"unknown state");
            break;
        }
    }
    if(started_writing_word) {
        print_word(started_writing_word);
    }
    else if(started_writing_mark) {
        print_mark(marks, current_mark_index, started_writing_mark);
    }
    m_state = state_end;
    std::cout << std::endl;
}

void phone::phone_keypad_transformer(symbol key, string& symbols)
{
    switch(key) {
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
        assert(!"error input data");
        break;
    }
}

void phone::print_mark(const string& marks, index& current_mark_index, bool& mark_enter_state)
{
    mark_enter_state = false;
    std::cout << marks[current_mark_index];
}

void phone::print_word(bool& word_enter_state)
{
    string word;
    m_dictionary->stop_search(word);
    word_enter_state = false;
    std::cout << word;
}

void read_input_data(dictionary &data)
{
#ifdef COMMENT
    std::cout << "Enter the size of the dictionary:  ";
#endif // COMMENT
    index lenght;
    std::cin >> lenght;
    
    benchmark::benchmark parsing;
    parsing.start();
    
    string word;
    std::getline(std::cin, word);
    priority word_priority;
    
    string line;
    
    for(index i = 0; i < lenght; ++i) {
#ifdef COMMENT
        std::cout << "Enter data about next word (text, priority): ";
#endif // COMMENT
        std::getline(std::cin, line);
        index p = line.find(' ');
        word = line.substr(0, p);
        word_priority = atoi(line.substr(p, line.size()).c_str());
        
        data.add_word(word, word_priority);
    }
    parsing.stop();
    std::cout << "DATA READ:  " << parsing.get_last_interval() << "  " << parsing.get_unit() << std::endl;
}

void read_input_task(string& task)
{
    benchmark::benchmark parsing;
    parsing.start();
#ifdef COMMENT
    std::cout << "Enter a sequence of characters entered by: ";
#endif // COMMENT
    std::getline(std::cin, task);
    parsing.stop();
    std::cout << "TASK READ:  " << parsing.get_last_interval() << "  " << parsing.get_unit() << std::endl;
}
