#pragma once
#include "searchable_bag.hpp"
class set{
	searchable_bag* bag_;
public:
	set();
	set(searchable_bag&);
	set(searchable_bag*);
	set(const set&);
	set&operator=(const set&);
	~set();
	searchable_bag&get_bag()const;
	bool has(int)const;
	void insert(int);
	void insert(int*,int);
	void print()const;
	void clear();
};
