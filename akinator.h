#ifndef AKINATOR_H
#define AKINATOR_H

#include <cstddef>

#define DATA_LEN 30

typedef char* data_t;

struct Node
{
    data_t data = 0;
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;

    int num_in_tree = 0;
};

typedef struct Node* elem_t;

#endif //AKINATOR_H
