#include "searchable_array_bag.hpp"
searchable_array_bag::searchable_array_bag(){}
searchable_array_bag::searchable_array_bag(const searchable_array_bag&o):array_bag(o){}
searchable_array_bag&searchable_array_bag::operator=(const searchable_array_bag&o){if(this!=&o)array_bag::operator=(o);return*this;}
bool searchable_array_bag::has(int v)const{for(int i=0;i<this->size;i++)if(this->data[i]==v)return true;return false;}
searchable_array_bag::~searchable_array_bag(){}