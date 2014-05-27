/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <iostream>

//#define READ_FROM_FILE

typedef std::string     hash;
typedef unsigned int    loop_index;
typedef long double     priority;
typedef char            symbol;
typedef std::string     string;

class dictionary
{
private:
    static const size_t MAX_NUM_WORDS;
    static const size_t MAX_WORD_SIZE;
    
    static const priority MAX_WORD_PRIORITY;
    static const priority MIN_PRIORITY;
    
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
    struct word_info
    {
        friend bool operator<(const word_info& T1, const word_info& T2) {
            if(T1.m_priority < T2.m_priority)
                return true;
            else if(T1.m_priority > T2.m_priority)
                return false;
            else
                return T1.m_word < T2.m_word;
        }
        priority m_priority;
        string m_word;
    };
    
    typedef std::set<word_info> data_element;
    //typedef std::multimap<priority, string> data_element;
    typedef std::map<hash, data_element> data_container;
    typedef std::vector<data_container> container;
    typedef std::vector<symbol> query;
    
private:
    void hash_fun(const string& word, hash& word_hash) const;
    void hash_fun(const query& word_query, size_t query_size, hash& word_hash) const;
    
    inline size_t hash_index(hash word_hash) const {
        size_t word_index = 0;
        for(loop_index i = 0; i < word_hash.size(); ++i) {
            word_index = (word_index << 3) + word_hash[i];
        }
        return word_index % MAX_NUM_WORDS;
    }
    
    inline size_t index_from_num_symbol(symbol s) const {
        return (size_t)s - (size_t)'0';
    }
    
    inline symbol num_symbol_from_text_symbol(symbol s) const;
    
    void get_priority_word(string& word);
    
private:
    container m_data;
    
    query m_query;
    size_t m_query_size;
    size_t m_word_position;
    
    priority m_delta_priority;
};


const size_t dictionary::MAX_NUM_WORDS = 50000;
const size_t dictionary::MAX_WORD_SIZE = 25;

const priority dictionary::MAX_WORD_PRIORITY = 3000;
const priority dictionary::MIN_PRIORITY      =  0.00001;

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
    void print_mark(const string& marks, size_t& current_mark_index, bool& mark_enter_state);
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
    
    hash word_hash;
    hash_fun(word, word_hash);
    size_t position = hash_index(word_hash);
    
    word_info new_word;
    
    bool insert = true;
    if(!m_data[position].empty()) {
        data_container::iterator it = m_data[position].find(word_hash);
        data_container::iterator endIt = m_data[position].end();
        if(it != endIt) {
            new_word.m_priority = MAX_WORD_PRIORITY - word_priority;
            new_word.m_word = word;
            it->second.insert(new_word);
            insert = false;
        }
    }
    if(insert) {
        data_element element;
        new_word.m_priority = MAX_WORD_PRIORITY - word_priority;
        new_word.m_word = word;
        element.insert(new_word);
        res = m_data[position].insert(data_container::value_type(word_hash, element)).second;
    }
    return res;
}

void dictionary::search(const symbol& key)
{
    m_query[m_query_size] = key;
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

void dictionary::hash_fun(const string &word, hash& word_hash) const
{
    word_hash = "";
    for(loop_index  i = 0; i < word.size(); ++i) {
        word_hash += num_symbol_from_text_symbol(word[i]);
    }
}

void dictionary::hash_fun(const query& word_query, size_t query_size, hash& word_hash) const
{
    word_hash = "";
    for(loop_index i = 0; i < query_size; ++i) {
        word_hash += word_query[i];
    }  
}

symbol dictionary::num_symbol_from_text_symbol(symbol s) const
{
    if(s >= 'a' && s <= 'c') {
        return '2';
    }
    else if(s >= 'd' && s <= 'f') {
        return '3';
    }
    else if(s >= 'g' && s <= 'i') {
        return '4';
    }
    else if(s >= 'j' && s <= 'l') {
        return '5';
    }
    else if(s >= 'm' && s <= 'o') {
        return '6';
    }
    else if(s >= 'p' && s <= 's') {
        return '7';
    }
    else if(s >= 't' && s <= 'v') {
        return '8';
    }
    else if(s >= 'w' && s <= 'z') {
        return '9';
    }
    assert(!"invalid input data");
    return '0';
}

void dictionary::get_priority_word(string& word)
{
    hash word_hash("");
    hash_fun(m_query, m_query_size, word_hash);
    size_t position = hash_index(word_hash);
    data_container::iterator data_it = m_data[position].find(word_hash);
    data_element::iterator word_it = data_it->second.begin();
    
    for(size_t i = 0; i < m_word_position; ++i) {
        ++word_it;
    }
    word = word_it->m_word;
    priority word_priority = word_it->m_priority - 1 - m_delta_priority;
    
    data_it->second.erase(word_it);
    word_info new_word;
    new_word.m_word = word;
    new_word.m_priority = word_priority;
    data_it->second.insert(new_word);
    
    m_delta_priority += MIN_PRIORITY;
}

void phone::parse_message(const string& message)
{   
    m_state = state_start;
    
    string marks = ".,?";
    size_t current_mark_index = 0;
    size_t message_size = message.size();
    
    symbol current_symbol;
    
    bool started_writing_word = false;
    bool started_writing_mark = false;
    
    for(size_t i = 0; i < message_size; ++i) {
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

void phone::print_mark(const string& marks, size_t& current_mark_index, bool& mark_enter_state)
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
    size_t lenght;
    data_stream >> lenght;
    
    string word;
    priority word_priority;
    string line;
    
    for(size_t i = 0; i < lenght; ++i) {
        data_stream >> word;
        data_stream >> word_priority;
        data.add_word(word, word_priority);
    }
    std::getline(data_stream, line);
    
    // read task
    std::getline(data_stream, task);
}
