#ifndef AKINATOR_H
#define AKINATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "input_output.h"
#include "C:\Users\vp717\Desktop\ilab\err_codes.h"
#include "C:\Users\vp717\Desktop\ilab\processor\redkin_processor\DSL.h"
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
};

int check_answer ();

char* make_question (char* data);

void create_window ();

void put_answer (char* data);

err running(Node* tree);

void input_name (char* data, int data_len);

void input_qst (char* data, int data_len);

err printTree (Node* head);

err printTree__ (Node* head, int* tab);

err fprintTree (FILE* out, Node* head);

err fprintTree__ (FILE* out, Node* head, int* tab);

err treeKill (Node* head);

err deleteNode (Node* head);

err treeSearch (Node* head, data_t srch, Node** return_t);

err importTree (FILE* read ,Node* head);

err importTree (FILE* read ,Node* head);

err fill_buffer (FILE* read, char** buf);

void get_data (char* buf, int* ptr, Node* tree);

#endif //AKINATOR_H
