#include <stdlib.h>
#include <stdio.h>
#include "C:\Users\vp717\Desktop\ilab\err_codes.h"


typedef int data_t;

struct Node
{
    data_t data = 0;
    Node* left = NULL;
    Node* right = NULL;
};

err NodeInsert (Node* head, data_t num);

err printTree (Node* head);

err treeKill (Node* head);

err deleteNode (Node* head);

err treeSearch (Node* head, data_t srch, Node** return_t);

int main()
{
    Node* tree = {};

    Node* tree_temp = (Node*)calloc(1, sizeof(Node));
    if (tree_temp == NULL)
        return CALLOC_ERROR;
    tree = tree_temp;

    //NodeInit(&tree);

    tree->data = 100;

    NodeInsert(tree, 50);
    NodeInsert(tree, 60);
    NodeInsert(tree, 40);
    NodeInsert(tree, 44);

    /*Node* ret;
    data_t srch = 45;

    if (treeSearch(tree, srch, &ret) == SUCCESS)
        printf("\n%d\n", ret->data);
    else
        printf("\nUNFOUND");*/

    printTree(tree);

    treeKill(tree);
}

err NodeInsert (Node* head, data_t num)
{
    if (head == NULL)
        return NULL_INSTEAD_PTR;

    Node* node_temp = {};

    Node* node_temp1 = (Node*)calloc(1, sizeof(Node));
    if (node_temp1 == NULL)
        return CALLOC_ERROR;
    node_temp = node_temp1;

    node_temp->data = num;

    while(1)
    {
        if (num >= head->data)
        {
            if (head->right == NULL)
            {
                head->right = node_temp;
                break;
            }
            else
                head = head->right;
        }
        else
        {
            if (head->left == NULL)
            {
                head->left = node_temp;
                break;
            }
            else
                head = head->left;
        }
        //printf("here");
    }

    return SUCCESS;
}

err printTree (Node* head)
{
    if (head == NULL)
        return NULL_INSTEAD_PTR;

    if (head->left != NULL)
    {
        printf("(");
        printTree(head->left);
    }

    if (head->right != NULL)
    {
        printTree(head->right);
        printf(")");
    }

    printf(" %d ", head->data);

    return SUCCESS;
}

err deleteNode (Node* node)
{
    if (node == NULL)
        return NULL_INSTEAD_PTR;

    free(node->right);
    free(node->left);
    free(node);
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

err treeSearch (Node* head, data_t srch, Node** return_t)
{
    if (head == NULL || return_t == NULL)
        return NULL_INSTEAD_PTR;

    while(1)
    {
        if (head->data == srch)
        {
            *return_t = head;
            return SUCCESS;
        }

        if (head->data > srch)
        {
            if (head->left != NULL)
            {
                return treeSearch(head->left, srch, return_t);
            }
            else
                return UNFOUND;
        }

        if (head->data < srch)
        {
            if (head->right != NULL)
            {
                return treeSearch(head->right, srch, return_t);
            }
            else
                return UNFOUND;
        }
    }
}
