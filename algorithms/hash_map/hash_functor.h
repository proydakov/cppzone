#pragma once

template<class key>
class hash_functor
{
public:
    size_t operator()(const key&) const;
};

template<>
class hash_functor<size_t>
{
public:
    size_t hash_functor::operator()(const size_t& k) const
    {
        return k;
    }
};

template<>
class hash_functor<int>
{
public:
    size_t hash_functor::operator()(const int& k) const
    {
        return static_cast<size_t>(k);
    }
};
