#ifndef AKINATOR_H
#define AKINATOR_H

#include <cstddef>

#define DATA_LEN 30

typedef char* data_t;

enum sub_tree
{
    right = 0,
    left = 1,
    Default = 2
};

struct Node
{
    data_t data = 0;
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;

    int num_in_tree = 0;
    sub_tree st = Default;
};

typedef struct Node* elem_t;

#endif //AKINATOR_H
