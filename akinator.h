#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input_output.h"
#include "interface.h"
#include "stack.h"
#include "C:\Users\vp717\Desktop\ilab\err_codes.h"
#include "C:\Users\vp717\Desktop\ilab\processor\redkin_processor\DSL.h"
#include <conio.h>
#include <ctype.h>
#include "..\TXLib.h"


#include <windows.h>

#define DATA_LEN 30

#define CHANGE_NODE(from, to)        \
    do                               \
    {                                \
    Node* tree_temp_ = 0;            \
    CALLOC(to, Node, 1);             \
    tree_temp_ = from;               \
    from = to;                       \
    from->parent = tree_temp_;       \
    level++;                         \
    } while(0)

typedef char* data_t;

struct Node
{
    data_t data = 0;
    Node* left = NULL;
    Node* right = NULL;
    Node* parent = NULL;

    int num_in_tree = 0;
};

char* make_question (char* data);

err running(Node* tree, int* run);

err draw_tree (Node* tree);

err printTree (Node* head);

err fprintTree (FILE* out, Node* head);

err treeKill (Node* head);

err deleteNode (Node* head);

err treeSearch (Node* head, data_t srch, Node** return_t);

err importTree (FILE* read ,Node* head);

err fill_buffer (FILE* read, char** buf);

void get_data (char* buf, int* ptr, Node* tree, int data_len);

void goto_prace (char* buf, int* ptr);

err make_description (Node* tree, const char* obj, char** description);

#endif //AKINATOR_H
