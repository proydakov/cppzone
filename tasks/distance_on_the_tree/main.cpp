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

#include <map>
#include <list>
#include <vector>
#include <cassert>
#include <iostream>

//#define COMMENT_OUTPUT
//#define WAIT_EXIT

typedef unsigned int uint;
typedef long long distance;

class tree
{
public:
    tree(uint size);
    
    bool insert_arc(uint child_id, uint parent_id, uint weight);
    distance calculate_distance(uint first_node_id, uint second_node_id);
    
private:
    struct node;
    typedef std::vector<node> container;
    typedef std::map<uint, distance> way;
    
    container m_tree;
};

typedef std::pair<uint, uint> task_pair;
typedef std::list<task_pair> task;

void read_input_task(task& data);

int main()
{
    uint data_size = 0;
#ifdef COMMENT_OUTPUT
    std::cout << "Enter number of graph arc: ";
#endif // COMMENT_OUTPUT
    std::cin >> data_size;
    
    tree i_tree(data_size);
    
    uint graph_node_name;
    uint graph_parent_name;
    uint length_to_the_parent;
    
    for(uint i = 1; i < data_size; ++i) {
        std::cin >> graph_node_name;
        std::cin >> graph_parent_name;
        std::cin >> length_to_the_parent;
        
        i_tree.insert_arc(graph_node_name, graph_parent_name, length_to_the_parent);
    }
    
    task i_task;
    read_input_task(i_task);
    task::const_iterator endIt = i_task.end();
    for(task::const_iterator it = i_task.begin(); it != endIt; ++it) {
        std::cout << i_tree.calculate_distance(it->first, it->second) << std::endl;
    }
#ifdef WAIT_EXIT
    char exit;
    std::cin >> exit;
#endif
    return 0;
}

struct tree::node
{
    node() : m_node_id(1), m_parent(parent_node(0, nullptr)) ,m_exist(false) {}
    node(uint node_id) : m_node_id(node_id), m_parent(parent_node(0, nullptr)) ,m_exist(true) {}
    
    bool is_head() const {
        return m_parent.second == nullptr;
    }
    
    bool is_exist() const {
        return m_exist;
    }
    
    void enable() {
        m_exist = true;
    }
    
    typedef std::pair<uint, node*>  parent_node;
    typedef std::list<node*>		child_nodes;
    
    uint		m_node_id;
    parent_node m_parent;
    child_nodes m_childs;
    
    bool		m_exist;
};

tree::tree(uint size)
{
    m_tree.resize(size);
    if(size == 1) {
        m_tree[0].enable();
    }
}

bool tree::insert_arc(uint child_id, uint parent_id, uint weight)
{
    bool exist_child = m_tree[child_id - 1].is_exist();
    bool exist_parent = m_tree[parent_id - 1].is_exist();
    
    node* child_ptr = !exist_child ? nullptr : &m_tree[child_id - 1];
    node* parent_ptr = !exist_parent ? nullptr : &m_tree[parent_id - 1];
    
    if(!exist_child) {
        node child(child_id);
        m_tree[child_id - 1] = child;
        child_ptr = &m_tree[child_id - 1];
    }
    if(!exist_parent) {
        node parent(parent_id);
        m_tree[parent_id - 1] = parent;
        parent_ptr = &m_tree[parent_id - 1];
    }
    
    child_ptr->m_parent = node::parent_node(weight, parent_ptr);
    parent_ptr->m_childs.push_back(child_ptr);
    
    return true;
}

distance tree::calculate_distance(uint first_node_id, uint second_node_id)
{
    distance length_way = 0;
    
    uint min_element = std::min(first_node_id, second_node_id);
    uint max_element = std::max(first_node_id, second_node_id);
    
    if(min_element < 0 && max_element >= m_tree.size()) {
        return -1;
    }
    
    distance length = 0;
    
    way my_way;
    my_way.insert(way::value_type(max_element, length));
    
    node* element = &m_tree[max_element - 1];
    
    while(!element->is_head()) {
        length += element->m_parent.first;
        my_way.insert(way::value_type(element->m_parent.second->m_node_id, length));
        element = element->m_parent.second;
    }
    
    length = 0;
    uint next_element = min_element;
    
    element = &m_tree[min_element - 1];
    
    while(my_way.insert(way::value_type(next_element, length)).second) {
        length += element->m_parent.first;
        next_element = element->m_parent.second->m_node_id;
        element = element->m_parent.second;
    }
    length_way = my_way.find(next_element)->second + length;
    
    return length_way;
}

void read_input_task(task& data)
{
#ifdef COMMENT_OUTPUT
    std::cout << "Enter the number of pairs to find: ";
#endif // COMMENT_OUTPUT
    uint size = 0;
    std::cin >> size;
    
    uint first;
    uint second;
    
    for(uint i = 0; i < size; ++i) {
#ifdef COMMENT_OUTPUT
        std::cout << "Enter to find a pair " << i + 1 << " (element a, element b):  ";
#endif // COMMENT_OUTPUT
        std::cin >> first;
        std::cin >> second;
        data.push_back(task_pair(first, second));
    }
}
