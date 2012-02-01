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

#ifndef I_BINARY_TREE
#define I_BINARY_TREE

#include <list>
#include <iostream>

template<class T>
class binary_tree
{
public:
    typedef unsigned long size_type;
    typedef T value_type;
    
    struct node;
    
public:
    binary_tree();
    binary_tree(const binary_tree<T>& src);
    binary_tree& operator=(const binary_tree<T>& src);
    ~binary_tree();
    
    void print() const;
    static void print_node(const node* elem);
    
    const node* successor(const node* elem) const;
    const node* predecessor(const node* elem) const;
    
    const node* min() const;
    const node* max() const;
    
    bool insert(const value_type& value);
    bool erase(const value_type& value);
    void clear();
    
    size_type depth() const;
    size_type size() const;
    bool empty() const;
    
    size_type count(const value_type& value) const;
    
private:
    void inorder_tree_walk_iter(const node* elem) const;
    void copy(const binary_tree<T>& src);
    node* copy_tree(node* src);
    void destroy_tree(node* root);
    node* find_node(const value_type& value) const;
    
    size_type   m_size;
    node*       m_root;
    
    node*       m_min;
    node*       m_max;
};

#define NULL_PTR 0

template<class T>
struct binary_tree<T>::node
{   
    typedef T value_type;
    typedef unsigned int number;
    
    node() : m_counter(0), m_parent(NULL_PTR), m_left(NULL_PTR), m_right(NULL_PTR) { }
    node(const value_type& data, node* parent, node* left, node* right) : 
        m_data(data), m_counter(1), m_parent(parent), m_left(left), m_right(right) { }
    node(const value_type& data, number counter, node* parent, node* left, node* right) : 
        m_data(data), m_counter(counter), m_parent(parent), m_left(left), m_right(right) { }
    
    value_type  m_data;
    number      m_counter;
    node*       m_parent;
    node*       m_left;
    node*       m_right;
};

template<class T>
binary_tree<T>::binary_tree() :
    m_size(0),
    m_root(NULL_PTR),
    m_min(NULL_PTR),
    m_max(NULL_PTR)
{
}

template<class T>
binary_tree<T>::binary_tree(const binary_tree<T>& src)
{
    copy(src);
}

template<class T>
binary_tree<T>& binary_tree<T>::operator=(const binary_tree<T>& src)
{
    if(this == &src)
        return *this;
    
    clear();
    copy(src);
    return *this;
}

template<class T>
binary_tree<T>::~binary_tree()
{
    destroy_tree(m_root);
}

template<class T>
void binary_tree<T>::print() const
{
    std::cout << "PRINT TREE" << std::endl;
    std::cout << "SIZE : " << m_size << std::endl;
    
    typedef std::list<node*> print_list;
    print_list* to_print = new print_list;
    print_list* temp_print = new print_list;
    if(m_root != NULL_PTR)
        to_print->push_back(m_root);
    while(!to_print->empty()) {
        typename print_list::const_iterator end_it = to_print->end();
        for(typename print_list::const_iterator it = to_print->begin(); it != end_it; ++it) {
            print_node(*it);
            
            if((*it)->m_left != NULL_PTR) {
                temp_print->push_back((*it)->m_left);
            }
            
            if((*it)->m_right != NULL_PTR) {
                temp_print->push_back((*it)->m_right);
            }
        }
        std::cout << std::endl;
        to_print->clear();
        std::swap(to_print, temp_print);
    }
    delete to_print;
    delete temp_print;
}

template<class T>
void binary_tree<T>::print_node(const node* elem)
{
    std::cout << "[E(" << elem->m_data << ")  C(" << elem->m_counter << ")  P(";
    if(elem->m_parent != NULL_PTR)
        std::cout << elem->m_parent->m_data;
    else
        std::cout << "NULL_PTR";
    std::cout << ")]   ";
}

template<class T>
const typename binary_tree<T>::node* binary_tree<T>::successor(const node* elem) const
{
    node* current = elem->m_right;
    if(current != NULL_PTR) {
        while(current->m_left != NULL_PTR)
            current = current->m_left;
        return current;
    }
    current = const_cast<node*>(elem);
    node* parent = elem->m_parent;
    while(parent != NULL_PTR && current == parent->m_right) {
        current = parent;
        parent = parent->m_parent;
    }
    return parent;
}
template<class T>
const typename binary_tree<T>::node* binary_tree<T>::predecessor(const node* elem) const
{
    node* current = elem->m_left;
    if(current != NULL_PTR) {
        while(current->m_right != NULL_PTR) 
            current = current->m_right;
        return current;
    }
    current = const_cast<node*>(elem);
    node* parent = elem->m_parent;
    while(parent != NULL_PTR && current == parent->m_left) {
        current = parent;
        parent = parent->m_parent;
    }
    return parent;
}

template<class T>
const typename binary_tree<T>::node* binary_tree<T>::min() const
{
    return m_min;
}

template<class T>
const typename binary_tree<T>::node* binary_tree<T>::max() const
{
    return m_max;
}

template<class T>
bool binary_tree<T>::insert(const value_type& value)
{
    node* current = m_root;
    node* parent = NULL_PTR;
    node* newnode = NULL_PTR;
    
    while(current != NULL_PTR) {
        parent = current;
        
        if(value < current->m_data) {
            current = current->m_left;
        }
        else if(value > current->m_data) {
            current = current->m_right;
        }
        else {
            ++current->m_counter;
            ++m_size;
            return true;
        }
    }
    
    newnode = new node(value, parent, NULL_PTR, NULL_PTR);
    
    if(parent == NULL_PTR) {
        m_root = newnode;
    }
    else if(value < parent->m_data) {
        parent->m_left = newnode;
    }
    else if(value > parent->m_data) {
        parent->m_right = newnode;
    }
    
    if(m_min == NULL_PTR)
        m_min = newnode;
    else if(value < m_min->m_data)
        m_min = newnode;
    
    if(m_max == NULL_PTR)
        m_max = newnode;
    else if(value > m_max->m_data)
        m_max = newnode;
    
    ++m_size;
    return true;
}

template<class T>
bool binary_tree<T>::erase(const value_type& value)
{  
    node* dnode = find_node(value);
    if(dnode == NULL_PTR)
        return false;
    
    node* pnode = dnode->m_parent;
    node* rnode = NULL_PTR;
    
    if(dnode->m_counter > 1) {
        --dnode->m_counter;
    }
    else {
        if(dnode == m_min)
            m_min = const_cast<node*>(successor(dnode));
        if(dnode == m_max)
            m_max = const_cast<node*>(predecessor(dnode));
        
        if((dnode->m_left == NULL_PTR) && (dnode->m_right == NULL_PTR)) {
            rnode = NULL_PTR;
        }
        else if((dnode->m_left != NULL_PTR) && (dnode->m_right == NULL_PTR)) {
            rnode = dnode->m_left;
            rnode->m_parent = pnode;        
        }
        else if((dnode->m_left == NULL_PTR) && (dnode->m_right != NULL_PTR)) {
            rnode = dnode->m_right;
            rnode->m_parent = pnode;
        }
        else {
            rnode = dnode->m_left;
            while(rnode->m_right != NULL_PTR) {
                rnode = rnode->m_right;
            }
            
            if(dnode->m_left != rnode) {
                rnode->m_parent->m_right = rnode->m_left;
                if(rnode->m_left != NULL_PTR)
                    rnode->m_left->m_parent = rnode->m_parent;
            }
            
            rnode->m_right = dnode->m_right;
            dnode->m_right->m_parent = rnode;
            
            if(rnode != dnode->m_left) {
                dnode->m_left->m_parent = rnode;
                rnode->m_left = dnode->m_left;
            }
            rnode->m_parent = pnode;
        }
        
        if(dnode == m_root)
            m_root = rnode;
        
        if(pnode != NULL_PTR) {
            if(pnode->m_data > dnode->m_data) {
                pnode->m_left = rnode;
            }
            else {
                pnode->m_right = rnode;
            }
        }
        delete dnode;
    }
    
    --m_size;
    return true;
}

template<class T>
void binary_tree<T>::clear()
{
    destroy_tree(m_root);
    m_size = 0;
    m_root = NULL_PTR;
    m_min = NULL_PTR;
    m_max = NULL_PTR;
}

template<class T>
typename binary_tree<T>::size_type binary_tree<T>::depth() const
{
    node* next = m_root;
    size_type depth_left = 0;
    while(next != NULL_PTR) {
        ++depth_left;
        next = next->m_left;
    }
    
    next = m_root;
    size_type depth_right = 0;
    while(next != NULL_PTR) {
        ++depth_right;
        next = next->m_left;  
    }
    
    return std::max(depth_left, depth_right);
}

template<class T>
typename binary_tree<T>::size_type binary_tree<T>::size() const
{
    return m_size;
}

template<class T>
bool binary_tree<T>::empty() const
{
    return m_size == 0;
}

template<class T>
typename binary_tree<T>::node* binary_tree<T>::find_node(const value_type& value) const
{
    node* current = m_root;
    node* parent = NULL_PTR;
    
    while(current != NULL_PTR) {
        if(value == current->m_data) {
            break;
        }
        else {
            parent = current;
            
            if(value < current->m_data)
                current = current->m_left;
            else
                current = current->m_right;
        }
    }
    return current;
}

template<class T>
typename binary_tree<T>::size_type binary_tree<T>::count(const value_type& value) const
{
    node* temp = find(value);
    return temp != NULL_PTR ? temp->m_counter : 0;
}

template<class T>
void binary_tree<T>::copy(const binary_tree<T>& src)
{
    m_size = src.m_size;
    m_root = copy_tree(src.m_root);
}

template<class T>
typename binary_tree<T>::node* binary_tree<T>::copy_tree(node* src)
{
    node* newlnode;
    node* newrnode;
    node* newnode;
    
    if(src == NULL_PTR)
        return NULL_PTR;
    
    newlnode = src->m_left != NULL_PTR ? copy_tree(src->m_left) : NULL_PTR;
    newrnode = src->m_right != NULL_PTR ? copy_tree(src->m_right) : NULL_PTR;
    
    newnode = new node(src->m_data, src->m_counter, NULL_PTR, newlnode, newrnode);
    if(newnode->m_left != NULL_PTR)
        newnode->m_left->m_parent = newnode;
    if(newnode->m_right != NULL_PTR)
        newnode->m_right->m_parent = newnode;
    return newnode;
}

template<class T>
void binary_tree<T>::destroy_tree(node* root)
{
    if(root == NULL_PTR)
        return;
    
    destroy_tree(root->m_left);
    destroy_tree(root->m_right);
    
    delete root;
}

#endif // I_BINARY_TREE
