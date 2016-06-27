#pragma once

#include <list>
#include <vector>
#include <algorithm>

#include <hash_functor.h>
#include <equal_functor.h>

namespace {
    enum { min_buckets_count = 8 };
};

template<class key, class value, class hash = hash_functor<key>, class equal = equal_functor<key>>
class hash_map
{
public:
    typedef key   key_type;
    typedef value mapped_type;
    typedef hash  hasher; 
    typedef equal key_equal;

public:
    hash_map(size_t n = min_buckets_count,
             const hasher& hf = hasher(),
             const key_equal& eql = key_equal());

    ~hash_map();

    size_t size() const;
    size_t bucket_count() const;

    float load_factor() const;
    float max_load_factor() const;

    void clear();

private:
    std::vector<std::list<std::pair<key, value>>> m_data;
    float  m_max_load_factor;
    size_t m_size;

    const hasher    m_hf;
    const key_equal m_eql;
};

template<class key, class value, class hash, class equal>
hash_map<key, value, hash, equal>::hash_map(size_t n, const hasher& hf, const key_equal& eql) : m_data(std::max(n, size_t(min_buckets_count))), m_max_load_factor(1.0f), m_size(0), m_hf(hf), m_eql(eql)
{
}

template<class key, class value, class hash, class equal>
hash_map<key, value, hash, equal>::~hash_map()
{
}

template<class key, class value, class hash, class equal>
size_t hash_map<key, value, hash, equal>::size() const
{
    return m_size;
}

template<class key, class value, class hash, class equal>
size_t hash_map<key, value, hash, equal>::bucket_count() const
{
    return m_data.size();
}

template<class key, class value, class hash, class equal>
float hash_map<key, value, hash, equal>::load_factor() const
{
    return size() / bucket_count();
}

template<class key, class value, class hash, class equal>
float hash_map<key, value, hash, equal>::max_load_factor() const
{
    return m_max_load_factor;
}

template<class key, class value, class hash, class equal>
void hash_map<key, value, hash, equal>::clear()
{
    return m_data.clear();
}
