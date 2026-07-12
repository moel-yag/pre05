#include "searchable_bag.hpp"
#include "searchable_array_bag.hpp"
#include "searchable_tree_bag.hpp"
#include "set.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
  if (argc == 1)
    return 1;
  searchable_bag *t = new searchable_tree_bag;
  searchable_bag *a = new searchable_array_bag;

  for (int i = 1; i < argc; i++)
  {
    t->insert(std::atoi(argv[i]));
    a->insert(std::atoi(argv[i]));
  }
  t->print();
  a->print();

  for (int i = 1; i < argc; i++)
  {
    std::cout << t->has(std::atoi(argv[i])) << std::endl;
    std::cout << a->has(std::atoi(argv[i])) << std::endl;
    std::cout << t->has(std::atoi(argv[i]) - 1) << std::endl;
    std::cout << a->has(std::atoi(argv[i]) - 1) << std::endl;
  }

  t->clear();
  a->clear();

  const searchable_array_bag tmp(static_cast<searchable_array_bag &>(*a));
  tmp.print();
  tmp.has(1);

  set sa(*a);
  set st(*t);
  for (int i = 1; i < argc; i++)
  {
    st.insert(std::atoi(argv[i]));
    sa.insert(std::atoi(argv[i]));

    sa.has(std::atoi(argv[i]));
    sa.print();
    sa.get_bag().print();
    st.print();
    sa.clear();
    int values[] = {1, 2, 3, 4};
    sa.insert(values, 4);
    std::cout << std::endl;
  }

  return 0;
}
