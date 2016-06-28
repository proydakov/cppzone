#pragma once

template<class key>
class equal_functor
{
public:
    bool operator()(key k1, key k2) const
    {
        return k1 == k2;
    }
};
