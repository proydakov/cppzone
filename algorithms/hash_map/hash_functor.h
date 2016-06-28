#pragma once

template<class key>
class hash_functor
{
public:
    size_t operator()(key) const;
};

template<>
class hash_functor<size_t>
{
public:
    size_t operator()(size_t k) const
    {
        return k;
    }
};

template<>
class hash_functor<int>
{
public:
    size_t operator()(int k) const
    {
        return static_cast<size_t>(k);
    }
};

template<>
class hash_functor<std::string>
{
public:
    size_t operator()(const std::string& k) const
    {
        size_t hash = 5381;
        for(size_t i = 0; i < k.size(); i++) {
            hash = ((hash << 5) + hash) + k[i];
        }
        return hash;
    }
};
