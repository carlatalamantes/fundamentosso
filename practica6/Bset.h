#ifndef TREE_BSET_H
#define TREE_BSET_H

#include <stdbool.h>

typedef struct stSet* Set;

typedef void* Type;
typedef int (*CompareFunc) (Type, Type);


Set set_create(CompareFunc);
int set_size(Set s);
bool set_add(Set s, Type t);

int compareInts(Type, Type);

bool set_containsValue(Type X, Set s);

bool set_removeValueminder(Type t, Set s);

bool set_removeValuemaxizq(Type t, Set s);

void set_printPreOrder( Set s);

void set_printPostOrder (Set s);

void set_printInOrder (Set s);

bool set_Destroy_Bset(Set s);


#endif //TREE_BSET_H
