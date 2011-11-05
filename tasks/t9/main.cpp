/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <stdint.h>
#include <assert.h>

//#define READ_FROM_FILE

typedef uint64_t        index;
typedef long double     hash;
typedef long long       loop_index;
typedef long double     priority;
typedef char            symbol;
typedef std::string     string;

class dictionary
{
private:
    static const index MAX_NUM_WORDS = 50000;
    static const index MAX_WORD_SIZE = 20;    
    
    static const priority MAX_WORD_PRIORITY = 3000;
    static const priority MIN_PRIORITY =  0.000001;
    
public:
    dictionary() :
        m_query_size(0),
        m_word_position(0),
        m_delta_priority(MIN_PRIORITY)
    {
        m_data.resize(MAX_NUM_WORDS);
        m_query.resize(MAX_WORD_SIZE);
    }
    
    bool add_word(const string& word, priority word_priority);
    
    void search(const symbol& key);
    void next_word();
    void stop_search(string& word);
    
private:
    hash hash_fun(const string& word) const;
    
    inline index hash_index(hash word_hash) const {
        return static_cast<index>(word_hash) % MAX_NUM_WORDS;
    }
    
    inline index index_from_num_symbol(symbol s) const {
        return (index)s - (index)'0';
    }
    
    inline index index_from_text_symbol(symbol s) const;
    
    void get_priority_word(string& word);
    
private:
    typedef std::multimap<priority, string> data_element;
    typedef std::map<hash, data_element> data_container;
    typedef std::vector<data_container> container;
    typedef std::vector<index> query;
    
    container m_data;
    
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
    void print_mark(const string& marks, index& current_mark_index, bool& mark_enter_state);
    void print_word(bool& word_enter_state);
    
private:
    dictionary* m_dictionary;
    
    states m_state;
};

void read_input_data(dictionary& data, string& task);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    dictionary i_dictionary;
    phone i_phone(&i_dictionary);
    string task;
    
    read_input_data(i_dictionary, task);
    
    i_phone.parse_message(task);
    
    return 0;
}

bool dictionary::add_word(const string& word, priority word_priority)
{
    bool res = true;
    
    hash word_hash = hash_fun(word);
    index position = hash_index(word_hash);
    
    bool insert = true;
    if(!m_data[position].empty()) {
        data_container::iterator it = m_data[position].find(word_hash);
        data_container::iterator endIt = m_data[position].end();
        if(it != endIt) {
            it->second.insert(data_element::value_type(MAX_WORD_PRIORITY - word_priority, word));
            insert = false;
        }
    }
    if(insert) {
        data_element element;
        element.insert(data_element::value_type(MAX_WORD_PRIORITY - word_priority, word));
        res = m_data[position].insert(data_container::value_type(word_hash, element)).second;
    }
    return res;
}

void dictionary::search(const symbol& key)
{
    m_query[m_query_size] = index_from_num_symbol(key);
    ++m_query_size;
}

void dictionary::next_word()
{
    ++m_word_position;
}

void dictionary::stop_search(string& word)
{    
    get_priority_word(word);
    
    m_query_size = 0;
    m_word_position = 0;
}

hash dictionary::hash_fun(const string &word) const
{
    hash k = 1;
    hash word_hash = 0;
    for(loop_index  i = word.size() - 1; i >= 0; --i) {
        index t = index_from_text_symbol(word[i]);
        word_hash += t * k;
        k *= 10;
    }
    return word_hash;
}

index dictionary::index_from_text_symbol(symbol s) const
{
    if(s >= 'a' && s <= 'c') {
        return 2;
    }
    else if(s >= 'd' && s <= 'f') {
        return 3;
    }
    else if(s >= 'g' && s <= 'i') {
        return 4;
    }
    else if(s >= 'j' && s <= 'l') {
        return 5;
    }
    else if(s >= 'm' && s <= 'o') {
        return 6;
    }
    else if(s >= 'p' && s <= 's') {
        return 7;
    }
    else if(s >= 't' && s <= 'v') {
        return 8;
    }
    else if(s >= 'w' && s <= 'z') {
        return 9;
    }
    assert(!"invalid input data");
}

void dictionary::get_priority_word(string& word)
{
    hash word_hash = 0;
    hash k = 1;
    for(loop_index i = m_query_size - 1; i >= 0; --i) {
        word_hash += m_query[i] * k;
        k *= 10;
    }
    index position = hash_index(word_hash);
    data_container::iterator data_it = m_data[position].find(word_hash);
    data_element::iterator word_it = data_it->second.begin();
    
    for(index i = 0; i < m_word_position; ++i) {
        ++word_it;
    }
    word = word_it->second;
    priority word_priority = word_it->first - 1 - m_delta_priority;
    
    data_it->second.erase(word_it);
    data_it->second.insert(data_element::value_type(word_priority, word));
    
    m_delta_priority += MIN_PRIORITY;
}

void phone::parse_message(const string& message)
{   
    m_state = state_start;
    
    string marks = ".,?";
    index current_mark_index = 0;
    
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
            m_dictionary->search(message[i]);
            break;
            
        case state_next_element:
            if(started_writing_word) {
                m_dictionary->next_word();
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

void read_input_data(dictionary &data, string& task)
{
#ifdef READ_FROM_FILE
    std::ifstream stream("data.txt", std::ios::in);
#define data_stream stream
#else
#define data_stream std::cin
#endif // READ_FROM_FILE
    
    // read data
    index lenght;
    data_stream >> lenght;
    
    string word;
    priority word_priority;
    string line;
    
    std::getline(data_stream, line);
    
    for(index i = 0; i < lenght; ++i) {
        std::getline(data_stream, line);
        index p = line.find(' ');
        word = line.substr(0, p);
        word_priority = atoi(line.substr(p, line.size()).c_str());
        data.add_word(word, word_priority);
    }
    
    // read task
    std::getline(data_stream, task);
}
