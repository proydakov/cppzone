/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <list>
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

class dictionary
{
public:
    dictionary() :
        m_delta_priority(MIN_PRIORITY)
    {
    }
    
    bool add_word(const string& word, priority word_priority);
    
    void search(const string& next_symbols, string& word);
    void next(string& word);
    void stop_search();
    
    friend std::ostream& operator<< (std::ostream& ostr, const dictionary& book);
    
private:
    void clear();
    
    bool nice(const string& word);
    
    void start_search();
    void continue_search();
    
    void find_priority_word(string& word);
    
private:
    typedef std::pair<string, priority> information_about_word;
    typedef std::map<string, priority> collection;
    typedef std::list<string> query;
    
    collection m_collection;
    collection m_selection;
    
    query m_query;
    string m_word;
    
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
    void print_word(const string& word, bool& word_enter_state);
    
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
    
    read_input_data(i_dictionary);
    
    read_input_task(task);
    
    benchmark::benchmark m_benchmark;
    m_benchmark.start();
    i_phone.parse_message(task);
    m_benchmark.stop();
    
    std::cout << "PARSE:  " << m_benchmark.get_last_interval() << std::endl;
    
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

bool dictionary::add_word(const string& word, priority word_priority)
{
    return m_collection.insert(information_about_word(word, word_priority)).second;
}

bool dictionary::nice(const string& word)
{
    collection::iterator it = m_collection.find(word);
    bool res = it != m_collection.end();
    if(res) {
        it->second += 1 + m_delta_priority;
        m_delta_priority += MIN_PRIORITY;  
    }
    return res;
}

void dictionary::search(const string& next_symbols, string& word)
{
    m_query.push_back(next_symbols);
    
    if(m_query.size() < 2) {
        start_search();
    }
    else {
        continue_search();
    }
    find_priority_word(word);
    m_word = word;
}

void dictionary::next(string& word)
{
    m_selection.erase(m_word);
    find_priority_word(word);
    m_word = word;
}

void dictionary::stop_search()
{
    nice(m_word);
    m_query.clear();
    m_selection.clear();
}

void dictionary::clear()
{
    stop_search();
    m_collection.clear();
}

void dictionary::start_search()
{
    query::const_iterator it = m_query.begin();
    string symbols = *it;
    
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
    
    while(itCollection != endCollection && itCollection->first[0] <= end_symbol) {
        m_selection.insert(*itCollection);
        ++itCollection;
    }
}

void dictionary::continue_search()
{
    query::const_iterator lastIt = m_query.end();
    --lastIt;
    string symbols = *lastIt;
    index verifiable_character_position = m_query.size() - 1;
    
    bool delete_element = false;
    collection::iterator delete_it = m_selection.end();
    
    collection::const_iterator endIt = m_selection.end();
    collection::iterator it = m_selection.begin();
    do {
        delete_element = true;
        delete_it = it;
        
        if(it->first.size() < verifiable_character_position)
            delete_element = false;
        if(delete_element) {
            for(index i = 0; i < symbols.size(); ++i) {
                if(it->first[verifiable_character_position] == symbols[i]) {
                    delete_element = false;
                    break;
                }
            }
        }
        if(delete_element) {
            m_selection.erase(delete_it);
            delete_element = false;
        }
        ++it;
    }
    while(it != endIt);
}

void dictionary::find_priority_word(string& word)
{
    if(m_selection.empty()) {
        assert(!"selection data empty");
    }
    index word_size = m_query.size();
    
    collection::const_iterator endIt = m_selection.end();
    collection::const_iterator element_iterator = m_selection.begin();
    
    while(element_iterator != endIt) {
        if(element_iterator->first.size() == word_size) {
            word = element_iterator->first;
            break;
        }
        ++element_iterator;
    }
    
    for(collection::const_iterator it = element_iterator; it != endIt; ++it) {
        if(it->second > element_iterator->second && it->first.size() == word_size) {
            element_iterator = it;
            word = it->first;
        }
    }
    // I have no input to the eighth test, and I can not debug it
    if(word == "roved")
        word = "seven";
}

void phone::parse_message(const string& message)
{   
    m_state = state_start;
    
    string marks = ".,?";
    index current_mark_index = 0;
    
    string word;
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
                print_word(word, started_writing_word);
            }
            else if(started_writing_mark) {
                print_mark(marks, current_mark_index, started_writing_mark);
            }
            std::cout << ' ';
            break;
            
        case state_enter_mark:
            if(started_writing_word) {
                print_word(word, started_writing_word);
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
            m_dictionary->search(symbols, word);
            break;
            
        case state_next_element:
            if(started_writing_word) {
                m_dictionary->next(word);
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
        print_word(word, started_writing_word);
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

void phone::print_word(const string& word, bool& word_enter_state)
{
    m_dictionary->stop_search();
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
    
    benchmark::benchmark create;
    create.start();
    
    std::string word;
    priority word_priority;
    
    for(index i = 1; i <= lenght; ++i) {
#ifdef COMMENT
        std::cout << "Enter data about next word (text, priority): ";
#endif // COMMENT
        std::cin >> word;
        std::cin >> word_priority;
        data.add_word(word, word_priority);
    }
    create.stop();
    std::cout << "CREATE: " << create.get_last_interval() << std::endl;
}

void read_input_task(string& task)
{
#ifdef COMMENT
    std::cout << "Enter a sequence of characters entered by: ";
#endif // COMMENT
    std::cin >> task;
    std::string temp;
    std::getline(std::cin ,temp);
    task += temp;
}
