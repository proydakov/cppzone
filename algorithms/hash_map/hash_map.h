#pragma once

#include <list>
#include <vector>
#include <utility>

#include <cmath>
#include <algorithm>

#include <hash_functor.h>
#include <equal_functor.h>



template<class key, class value, class hash = hash_functor<key>, class equal = equal_functor<key>>
class hash_map
{
public:
    typedef size_t                      size_type;

    typedef key                         key_type;
    typedef value                       mapped_type;
    typedef std::pair<const key, value> value_type;

    typedef hash                        hasher;
    typedef equal                       key_equal;

    typedef typename std::list<value_type>::iterator       iterator;
    typedef typename std::list<value_type>::const_iterator const_iterator;

    typedef typename std::list<value_type>::iterator       local_iterator;
    typedef typename std::list<value_type>::const_iterator const_local_iterator;

private:
    enum { min_buckets_count = 8 };

public:
    hash_map(size_type n = min_buckets_count,
             const hasher& hf = hasher(),
             const key_equal& eql = key_equal());

    hash_map(std::initializer_list<std::pair<const key, const value>> l,
             size_type n = min_buckets_count,
             const hasher& hf = hasher(),
             const key_equal& eql = key_equal());

    ~hash_map();

    local_iterator begin(size_type n);
    const_local_iterator begin(size_type n) const;

    local_iterator end(size_type n);
    const_local_iterator end(size_type n) const;

    bool insert(const value_type& v);
    bool erase(const key_type& k);

    value& operator[](const key_type& k);

    size_type size() const;
    size_type bucket_count() const;
    
    float load_factor() const;
    float max_load_factor() const;

    hasher hash_function() const;
    key_equal  key_eq() const;

    void clear();

private:
    size_type get_index(const key_type& k);
    static size_type get_next_pow_2(int val);

private:
    std::vector<std::list<value_type>> m_data;
    float  m_max_load_factor;
    size_type m_size;

    const hasher    m_hf;
    const key_equal m_eql;
};

template<class key, class value, class hash, class equal>
hash_map<key, value, hash, equal>::hash_map(size_type n, const hasher& hf, const key_equal& eql) :
    m_data(std::max(get_next_pow_2(n), size_t(min_buckets_count))),
    m_max_load_factor(1.0f),
    m_size(0),
    m_hf(hf),
    m_eql(eql)
{
}

template<class key, class value, class hash, class equal>
hash_map<key, value, hash, equal>::hash_map(std::initializer_list<std::pair<const key, const value>> l, size_type n, const hasher& hf, const key_equal& eql) :
    m_data(std::max(get_next_pow_2(std::max(l.size(), n)), size_t(min_buckets_count))),
    m_max_load_factor(1.0f),
    m_size(0)
{
    for (auto it = l.begin(); it != l.end(); ++it) {
        insert((*it));
    }
    int i = 0;
}

template<class key, class value, class hash, class equal>
hash_map<key, value, hash, equal>::~hash_map()
{
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::local_iterator hash_map<key, value, hash, equal>::begin(size_type n)
{
    return m_data[n].begin();
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::const_local_iterator hash_map<key, value, hash, equal>::begin(size_type n) const
{
    return m_data[n].cbegin();
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::local_iterator hash_map<key, value, hash, equal>::end(size_type n)
{
    return m_data[n].end();
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::const_local_iterator hash_map<key, value, hash, equal>::end(size_type n) const
{
    return m_data[n].end();
}

template<class key, class value, class hash, class equal>
bool hash_map<key, value, hash, equal>::insert(const value_type& v)
{
    const key& k = v.first;
    const size_type index = get_index(k);
    std::list<value_type>& list = m_data[index];
    auto it = std::find_if(list.begin(), list.end(), [=](const value_type& v) {
        return m_eql(v.first, k);
    });
    if (it != list.end()) {
        return false;
    }
    list.push_back(v);
    m_size++;
    return true;
}

template<class key, class value, class hash, class equal>
bool hash_map<key, value, hash, equal>::erase(const key_type& k)
{
    const size_t index = get_index(k);
    std::list<value_type>& list = m_data[index];
    auto it = std::find(list.begin(), list.end(), [=](const value_type& v) {
        return m_eql(v.first, k);
    });
    if (it == list.end()) {
        return false;
    }
    m_size--;
    list.erase(it);
    return true;
}

template<class key, class value, class hash, class equal>
value& hash_map<key, value, hash, equal>::operator[](const key& k)
{
    const size_type index = get_index(k);
    std::list<value_type>& list = m_data[index];
    auto it = std::find_if(list.begin(), list.end(), [=](const value_type& v) {
        return m_eql(v.first, k);
    });
    if (it != list.end()) {
        return it->second;
    }
    value_type pair(k, value());
    list.push_back(std::move(pair));
    m_size++;
    return list.rbegin()->second;
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::size_type hash_map<key, value, hash, equal>::size() const
{
    return m_size;
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::size_type hash_map<key, value, hash, equal>::bucket_count() const
{
    return m_data.size();
}

template<class key, class value, class hash, class equal>
float hash_map<key, value, hash, equal>::load_factor() const
{
    return static_cast<float>(size()) / bucket_count();
}

template<class key, class value, class hash, class equal>
float hash_map<key, value, hash, equal>::max_load_factor() const
{
    return m_max_load_factor;
}

template<class key, class value, class hash, class equal>
hash hash_map<key, value, hash, equal>::hash_function() const
{
    return m_hf;
}

template<class key, class value, class hash, class equal>
equal hash_map<key, value, hash, equal>::key_eq() const
{
    return m_eql;
}

template<class key, class value, class hash, class equal>
void hash_map<key, value, hash, equal>::clear()
{
    m_size = 0;
    return m_data.clear();
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::size_type hash_map<key, value, hash, equal>::get_index(const key_type& k)
{
    const size_type raw_index = m_hf(k);
    const size_type res_index = raw_index % m_data.size();
    return res_index;
}

template<class key, class value, class hash, class equal>
typename hash_map<key, value, hash, equal>::size_type hash_map<key, value, hash, equal>::get_next_pow_2(int val)
{
    return std::pow(2, std::ceil(std::log2(val)));
}
