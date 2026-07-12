#ifndef SET_HPP
#define SET_HPP

#include "searchable_bag.hpp"

class set
{
private:
    searchable_bag* bag_;

public:
    set();
    set(searchable_bag& s_bag);
    set(searchable_bag* s_bag);
    set(const set& source);
    set& operator=(const set& source);
    ~set();

    bool has(int value) const;
    searchable_bag& get_bag() const;
    void insert(int value);
    void insert(int* data, int size);
    void print() const;
    void clear();
};

#endif