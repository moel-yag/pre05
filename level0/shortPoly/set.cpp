#include "set.hpp"
set::set():bag_(0){}
set::set(searchable_bag&b):bag_(&b){}
set::set(searchable_bag*b):bag_(b){}
set::set(const set&o):bag_(o.bag_){}
set&set::operator=(const set&o){if(this!=&o)bag_=o.bag_;return*this;}
set::~set(){}
bool set::has(int v)const{if(!bag_)return false;return bag_->has(v);}
searchable_bag&set::get_bag()const{return*bag_;}
void set::insert(int v){if(!bag_)return;if(!bag_->has(v))bag_->insert(v);}
void set::insert(int*d,int n){if(!bag_||!d||n<=0)return;for(int i=0;i<n;i++)insert(d[i]);}
void set::print()const{if(!bag_)return;bag_->print();}
void set::clear(){if(!bag_)return;bag_->clear();}