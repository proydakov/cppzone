/*
 *  Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
 */

#include <map>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>

#define TEST // DEFINE FOR BUILD TEST EXAMPLE
#define COMMENT

typedef long long length_type;
typedef int index_type;

class node
{
public:
	node() :
	    parent(NULL), 
	    length_to_the_parent(0), 
	    parent_name(0)
	{
	}
	
	bool is_head() const {
		return parent == NULL;
	}
	
	friend bool operator==(const node& a_node, const node& b_node) {
		return a_node.parent == b_node.parent &&
				a_node.length_to_the_parent == b_node.length_to_the_parent &&
				a_node.parent_name == b_node.parent_name;
	}
	
	node* parent;
	index_type length_to_the_parent;
	index_type parent_name;
};
typedef std::vector<node> graph;

typedef std::pair<index_type, length_type> way_element;
typedef std::map<index_type, length_type> way;

typedef std::pair<index_type, index_type> task_pair;
typedef std::list<task_pair> task;

void read_input_data(graph& data);
void read_input_task(task& data);

length_type search_distances_on_the_graph(const graph& data, const task_pair& pair);

void create_example_graph(graph& data);
void create_example_task(task& data);

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    graph my_graph;
#ifndef TEST
    read_input_data(my_graph);
#else
    create_example_graph(my_graph);
#endif
    
    task my_task;
#ifndef TEST
    read_input_task(my_task);
#else
    create_example_task(my_task);
#endif
    
    task::const_iterator endIt = my_task.end();
    for(task::const_iterator it = my_task.begin(); it != endIt; ++it) {
#ifdef COMMENT
        std::cout << "PAIR:  " << it->first << " " << it->second << std::endl;
        std::cout << "LENGTH:  ";
#endif
        std::cout << search_distances_on_the_graph(my_graph, *it) << std::endl;
    }
    return 0;
}

void read_input_data(graph& data)
{
#ifdef COMMENT
	std::cout << "Enter number of graph vertices: ";
#endif
	int input_size = 0;
	std::cin >> input_size;
	data.resize(input_size);
	
	index_type graph_parent_name;
	index_type graph_node_name;
	length_type length_to_the_parent;
	
	node my_node;
	data[0] = my_node;
	
	for(int i = 1; i < input_size; ++i) {
#ifdef COMMENT
		std::cout << "Enter data graph for the element " << i << " (parent, element, length_to_the_parent):  ";
#endif
		std::cin >> graph_parent_name;
		std::cin >> graph_node_name;
		std::cin >> length_to_the_parent;
		
		my_node.length_to_the_parent = length_to_the_parent;
		my_node.parent_name = graph_parent_name;
		my_node.parent = &data[graph_parent_name - 1];
		
		data[graph_node_name - 1] = my_node;
	}	
}

void read_input_task(task& data)
{
#ifdef COMMENT
	std::cout << "Enter the number of pairs to find: ";
#endif
	int size = 0;
	std::cin >> size;
	
	index_type first;
	index_type second;
	
	for(int i = 1; i <= size; ++i) {
#ifdef COMMENT
		std::cout << "Enter to find a pair " << i << " (element a, element b):  ";
#endif
		std::cin >> first;
		std::cin >> second;
		data.push_back(task_pair(first, second));
	}
}

length_type search_distances_on_the_graph(const graph& data, const task_pair& pair)
{
	if(data.empty())
		return -1;
	
	index_type min_element = std::min(pair.first, pair.second);
	index_type max_element = std::max(pair.first, pair.second);	
	
	if(min_element < 0 && max_element >= data.size())
		return -1;
	
	length_type length = 0;
	
	way my_way;
	my_way.insert(way_element(max_element, length));
	
	const node* element = &data[max_element - 1];
	
	while(!element->is_head()) {
		length += element->length_to_the_parent;
		my_way.insert(way_element(element->parent_name, length));
		element = element->parent;
	}
	
	length = 0;
	index_type next_element = min_element;
	
	element = &data[min_element - 1];
	
	while(my_way.insert(way_element(next_element, length)).second) {
		length += element->length_to_the_parent;
		next_element = element->parent_name;
		element = element->parent;
	}
	length_type main_len = my_way.find(next_element)->second + length;
	
	return main_len;
}

void create_example_graph(graph& data)
{
    node my_node;
    index_type graph_parent_name;
    
    data.resize(6);
    
    // node 1
    index_type graph_node_name = 1;
    data[0] = my_node;
    
    // node 2
    graph_node_name = 2;
    graph_parent_name = 1;
    my_node.length_to_the_parent = 7;
    my_node.parent = &data[graph_parent_name - 1];
    my_node.parent_name = graph_parent_name;
    data[1] = my_node;
    
    // node 3
    graph_node_name = 3;
    graph_parent_name = 2;
    my_node.length_to_the_parent = 3;
    my_node.parent = &data[graph_parent_name - 1];
    my_node.parent_name = graph_parent_name;
    data[2] = my_node;
    
    // node 4
    graph_node_name = 4;
    graph_parent_name = 2;
    my_node.length_to_the_parent = 6;
    my_node.parent = &data[graph_parent_name - 1];
    my_node.parent_name = graph_parent_name;
    data[3] = my_node;
    
    // node 5
    graph_node_name = 5;
    graph_parent_name = 4;
    my_node.length_to_the_parent = 3;
    my_node.parent = &data[graph_parent_name - 1];
    my_node.parent_name = graph_parent_name;
    data[4] = my_node;
    
    // node 6
    graph_node_name = 6;
    graph_parent_name = 5;
    my_node.length_to_the_parent = 1;
    my_node.parent = &data[graph_parent_name - 1];
    my_node.parent_name = graph_parent_name;
    data[5] = my_node;
}

void create_example_task(task& data)
{
    data.push_back(task_pair(1, 6));
    data.push_back(task_pair(1, 3));
    data.push_back(task_pair(6, 3));
    data.push_back(task_pair(2, 5));
    data.push_back(task_pair(4, 3));
    data.push_back(task_pair(1, 2));
    data.push_back(task_pair(1, 3));
    data.push_back(task_pair(4, 4));
    data.push_back(task_pair(1, 1));
    data.push_back(task_pair(6, 6));
}
