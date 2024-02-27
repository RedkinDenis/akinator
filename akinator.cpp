#include <stdlib.h>
#include <stdio.h>
#include "err_codes.h"


typedef int data_t;

struct Node
{
    data_t data = 0;
    Node* left = NULL;
    Node* right = NULL;
};

err NodeInsert (Node* head, data_t num);

/*err NodeInit (Node* node);*/

err printTree (Node* head);

err treeKill (Node* head);

int main()
{
    Node* tree;

    Node* temp = (Node*)calloc(1, sizeof(Node));
    if (temp == NULL)
        return CALLOC_ERROR;
    tree = temp;

    //NodeInit(&tree);

    tree->data = 100;

    NodeInsert(tree, 50);

    NodeInsert(tree, 60);

    printTree(tree);

    treeKill(tree);
}

err NodeInsert (Node* head, data_t num)
{
    if (head == NULL)
        return NULL_INSTEAD_PTR;

    Node* temp;

    Node* temp1 = (Node*)calloc(1, sizeof(Node));
    if (temp1 == NULL)
        return CALLOC_ERROR;
    temp = temp1;

    /*if(result != SUCCESS)
        return result;*/

    temp->data = num;

    while(1)
    {
        if (num >= head->data)
        {
            if (head->right == NULL)
            {
                head->right = temp;
                break;
            }
            else
                head = head->right;
        }
        else
        {
            if (head->left == NULL)
            {
                head->left = temp;
                break;
            }
            else
                head = head->left;
        }
        //printf("here");
    }

    return SUCCESS;
}

/*err NodeInit (Node* node)
{
    if (node == NULL)
        return NULL_INSTEAD_PTR;

    Node* temp = (Node*)calloc(1, sizeof(Node));
    if (temp == NULL)
        return CALLOC_ERROR;
    node = temp;

    node->right = NULL;
    node->left = NULL;

    node->data = 0;

    return SUCCESS;
}*/

err printTree (Node* head)
{
    if (head == NULL)
        return NULL_INSTEAD_PTR;

    if (head->left != NULL)
    {
        //printf("(");
        printTree(head->left);
    }

    else if (head->right != NULL)
    {
        //printf(")");
        printTree(head->right);
    }

    printf(" %d ", head->data);

    return SUCCESS;
}

err treeKill (Node* head)
{
    if (head == NULL)
        return NULL_INSTEAD_PTR;

    if (head->left != NULL)
        treeKill(head->left);

    if (head->right != NULL)
        treeKill(head->right);

    free(head);
    return SUCCESS;
}
