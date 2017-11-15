#include <vector>
#include <iostream>

template<class T>
class zigzag_iterator
{
public:
    zigzag_iterator(std::vector<T> const& v1, std::vector<T> const& v2) :
        index_(-1),
        v1_it_(v1.begin()),
        v2_it_(v2.begin()),
        v1_end_(v1.end()),
        v2_end_(v2.end())
    {
        it_ = v1.begin();
        index_ = 1;
        if(it_ == v1_end_) {
            index_ = 2;
            it_ = v2.begin();
        }
        if(it_ == v2_end_) {
            index_ = 0;
        }
    }

    T next()
    {
        T solution = *it_;

        if(index_ == 1) {
            v1_it_++;
            it_ = v2_it_;
            index_ = 2;
            if(it_ == v2_end_) {
                it_ = v1_it_;
                index_ = 1;
                if(it_ == v1_end_) {
                    index_ = 0;
                }
            }
        }
        else if(index_ == 2) {
            v2_it_++;
            it_ = v1_it_;
            index_ = 1;
            if(it_ == v1_end_) {
                it_ = v2_it_;
                index_ = 2;
                if(it_ == v2_end_) {
                    index_ = 0;
                }
            }
        }

        return solution;
    }

    bool has_next() const
    {
        return index_ != 0;
    }

private:
    int index_;
    typename std::vector<T>::const_iterator it_;

    typename std::vector<T>::const_iterator v1_it_;
    typename std::vector<T>::const_iterator v2_it_;

    typename std::vector<T>::const_iterator v1_end_;
    typename std::vector<T>::const_iterator v2_end_;
};

void test(std::string comment, const std::vector<int>& v1, const std::vector<int>& v2)
{
    std::cout << "test: " << comment << std::endl;
    zigzag_iterator<int> it(v1, v2);
    while(it.has_next()) {
        std::cout << it.next() << ' ';
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

int main()
{
    std::ios::sync_with_stdio(false);

    test(
        "zero size",
        { },
        { }
    );

    test(
        "eq size",
        { 1,  2,  3},
        {-1, -2, -3}
    );

    test(
        "diff size_1",
        { 1,  2,  3, 4},
        {-1, -2, -3}
    );

    test(
        "diff size_2",
        { 1,  2,  3, 4, 5},
        {-1, -2, -3}
    );

    test(
        "diff size_3",
        { 1,  2,  3},
        {-1, -2, -3, -4}
    );

    test(
        "diff size_4",
        { 1,  2,  3},
        {-1, -2, -3, -4, -5}
    );

    test(
        "diff size_5",
        { 1,  2,  3, 4},
        { }
    );

    test(
        "diff size_6",
        { },
        {-1, -2, -3, -4}
    );

    return 0;
}
