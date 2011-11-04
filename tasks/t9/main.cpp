/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <vector>
#include <string>
#include <cstdlib>
//#include <stdio.h>
#include <iostream>
#include <assert.h>

//#include <benchmark.h>

//#define COMMENT

typedef unsigned int    index;
typedef double          priority;
typedef char            symbol;
typedef std::string     string;

const index MAX_NUM_WORDS   = 100000;
const priority MIN_PRIORITY = (priority) 1 / MAX_NUM_WORDS;

const index NUMBER_OF_KEYS = 8;
const index SHIFT = 2;
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
        m_data.resize(MAX_WORD_SIZE);
        for(index i = 0; i < MAX_WORD_SIZE; ++i) {
            m_data[i].resize(NUMBER_OF_KEYS);
        }
        m_query_data.resize(MAX_WORD_SIZE);
        m_query_index.resize(MAX_WORD_SIZE);
    }
    
    bool add_word(const string& word, priority word_priority);
    
    void search(const string& symbols);
    void next();
    void stop_search(string& word);
    
    friend std::ostream& operator<< (std::ostream& ostr, const dictionary& book);
    
private:
    index get_symbol_index(const symbol word_symbol);
    
    void parsing_query();
    void get_priority_word(string& word);
    
private:
    typedef std::map<string, priority> collection;
    typedef std::pair<priority, priority*> priority_info;
    typedef std::multimap<priority_info, string> selection;
    typedef std::vector<string> query_data;
    typedef std::vector<index> query_index;
    
    typedef std::vector<std::vector<collection> > megadata;
    
    megadata m_data;
    selection m_selection;
    
    query_data m_query_data;
    query_index m_query_index;
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
    
    inline index index_from_symbol(symbol s)
    {
        return (index)s - 48;
    }
    
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
    
    /*
    i_dictionary.add_word("act", 1);
    i_dictionary.add_word("bat", 1);
    i_dictionary.add_word("cat", 1);
    
    std::cout << i_dictionary << std::endl;
    
    task = "228* 228** 228** 228**1 ";
    */
    
    read_input_data(i_dictionary);
    read_input_task(task);
    
    i_phone.parse_message(task);
    
    return 0;
}

std::ostream& operator<< (std::ostream& ostr, const dictionary& book)
{
    ostr << "\nDICTIONARY:" << std::endl;
    for(index i = 0; i < MAX_WORD_SIZE; ++i) {
        for(index j = 0; j < NUMBER_OF_KEYS; ++j) {
            dictionary::collection::const_iterator endIt = book.m_data[i][j].end();
            for(dictionary::collection::const_iterator it = book.m_data[i][j].begin(); it != endIt; ++it) {
                ostr << "Word:   " << it->first << "   priority:   " << it->second << std::endl;
            }
        }
    }
    return ostr;
}

bool dictionary::add_word(const string& word, priority word_priority)
{
    index word_size_position = word.size() - 1;
    index symbol_position = get_symbol_index(word[0]) - SHIFT;
    
    return m_data[word_size_position][symbol_position].insert(collection::value_type(word, word_priority)).second;
}

void dictionary::search(const string& symbols)
{
    m_query_data[m_query_size] = symbols;
    m_query_index[m_query_size] = get_symbol_index(symbols[0]) - SHIFT;
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
        std::cout << "Word:   " << it->second << "   priority:   " << it->first.first << std::endl;
    }
    */
    
    get_priority_word(word);
    
    m_selection.clear();
    m_query_size = 0;
    m_word_position = 0;
}

index dictionary::get_symbol_index(const symbol word_symbol)
{
    index  i = 0;
    if(word_symbol >= 'a' && word_symbol <= 'c') {
        i = 2;
    }
    else if(word_symbol >= 'd' && word_symbol <= 'f') {
        i = 3;
    }
    else if(word_symbol >= 'g' && word_symbol <= 'i') {
        i = 4;
    }
    else if(word_symbol >= 'j' && word_symbol <= 'l') {
        i = 5;
    }
    else if(word_symbol >= 'm' && word_symbol <= 'o') {
        i = 6;
    }
    else if(word_symbol >= 'p' && word_symbol <= 's') {
        i = 7;
    }
    else if(word_symbol >= 't' && word_symbol <= 'v') {
        i = 8;
    }
    else if(word_symbol >= 'w' && word_symbol <= 'z') {
        i = 9;
    }
    else {
        assert(!"Invalid data");
    }
    return i;
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

void dictionary::parsing_query()
{
    collection::iterator itCollection = m_data[m_query_size - 1][m_query_index[0]].begin();
    collection::iterator endCollection = m_data[m_query_size - 1][m_query_index[0]].end();
    
    string first = itCollection->first;
    
    index lower_bound_test = 1;
    index upper_bound_test = m_query_size;
    
    string word;
    
    bool insert_word;
    while(itCollection != endCollection) {
        insert_word = true;
        
        for(index i = lower_bound_test; i < upper_bound_test; ++i) {
            if(m_query_data[i].find_first_of(itCollection->first[i]) == -1) {
                insert_word = false;
                break;
            }
        }
        
        if(insert_word) {
            priority_info info(MAX_PRIORITY - itCollection->second, &itCollection->second);
            m_selection.insert(selection::value_type(info, itCollection->first));
            word = itCollection->first;
        }
        ++itCollection;
    }
}

void dictionary::get_priority_word(string& word)
{
    if(m_selection.empty()) {
        assert(!"selection data empty");
    }
    selection::const_iterator it = m_selection.begin();
    for(index i = 0; i < m_word_position; ++i) {
        ++it;
    }
    word = it->second;
    *it->first.second += 1 + m_delta_priority;
    m_delta_priority += MIN_PRIORITY;
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
    
    string word;
    std::getline(std::cin, word);
    priority word_priority;
    
    string line;
    //char str[30];
    
    //benchmark::benchmark ben;
    //ben.start();
    
    for(index i = 0; i < lenght; ++i) {
#ifdef COMMENT
        std::cout << "Enter data about next word (text, priority): ";
#endif // COMMENT
        //gets(str);
        //line = str;
        std::getline(std::cin, line);
        index p = line.find(' ');
        word = line.substr(0, p);
        word_priority = atoi(line.substr(p, line.size()).c_str());
        //line = "";
        data.add_word(word, word_priority);
    }
    
    //ben.stop();
    //std::cout << "READ DATA:  " << ben.get_last_interval() << std::endl;
}

void read_input_task(string& task)
{
#ifdef COMMENT
    std::cout << "Enter a sequence of characters entered by: ";
#endif // COMMENT
    
    //benchmark::benchmark ben;
    //ben.start();
    
    //char str[100000];
    //gets(str);
    //task = str;
    std::getline(std::cin, task);
    
    //ben.stop();
    //std::cout << "READ TASK:  " << ben.get_last_interval() << std::endl;
}
