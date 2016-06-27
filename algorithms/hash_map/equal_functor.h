#pragma once

template<class key>
class equal_functor
{
public:
    bool operator()(const key& k1, const key& k2)
    {
        return k1 == k2;
    }
};
