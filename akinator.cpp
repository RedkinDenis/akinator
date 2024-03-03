#include "akinator.h"

int main()
{
    FOPEN(read, "treeSave.txt", "rb");

    Node* tree = {};

    CALLOC(tree, Node, 1);
    CALLOC(tree->data, char, DATA_LEN);

    importTree(read, tree);

    fclose(read);

    printTree(tree);

    /*FOPEN(out, "treeSave.txt", "wb");

    fprintTree(out, tree);

    fclose(out);*/

    treeKill(tree);
}

err fill_buffer (FILE* read, char** buf)
{
    int fsize = GetFileSize(read);

    CALLOC(*buf, char, (fsize + 1));

    fread(*buf, sizeof(char), fsize, read);

    return SUCCESS;
}

err importTree (FILE* read, Node* tree)
{
    char* buf = 0;
    fill_buffer(read, &buf);

    /*for (int i = 0; i < fsize - 1; i++)
        printf("%c", buf[i]);
    printf("\n");*/

    int level = 0, ptr = 0, i = 0;

    if (buf[ptr] == '(')
    {
        ptr++;
        level++;

        get_data(buf, &ptr, tree);
    }

    while (level > 0)
    {
        if (buf[ptr] == '(')
        {
            ptr++;

            CHANGE_NODE(tree, tree->left);
            CALLOC(tree->data, char, DATA_LEN);

            get_data(buf, &ptr, tree);
        }
        else if (buf[ptr] == ')')
        {
            tree = tree->parent;
            level--;
            ptr++;

            if (buf[ptr] == '(')
            {
                ptr++;

                CHANGE_NODE(tree, tree->right);
                CALLOC(tree->data, char, DATA_LEN);

                get_data(buf, &ptr, tree);
            }
        }
    }

    return SUCCESS;
}

void get_data(char* buf, int* ptr, Node* tree)
{
    int i = 0;
    while (buf[*ptr - 1] != '*')
        *ptr += 1;

    while (buf[*ptr] != '*')
    {
        tree->data[i] = buf[*ptr];
        *ptr += 1;
        i++;
    }
    i = 0;

    while (buf[*ptr] != '(' && buf[*ptr] != ')')
        *ptr += 1;

}

err NodeInsert (Node* head, data_t num)
{
    CHECK_PTR(head);

    Node* node_temp = {};
    void* temp = 0;

    CALLOC(node_temp, Node, 1);

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
    }

    return SUCCESS;
}

err printTree (Node* head)
{
    CHECK_PTR(head);

    printf("(");
    printf("*%s*", head->data);

    if (head->left != NULL)
    {
        printTree(head->left);
    }

    if (head->right != NULL)
    {
        printTree(head->right);
    }

    printf(")");

    return SUCCESS;
}

err fprintTree (FILE* out, Node* head)
{
    CHECK_PTR(head);


    fprintf(out, "(");
    fprintf(out, " %d ", head->data);

    if (head->left != NULL)
    {
        fprintTree(out, head->left);
    }

    if (head->right != NULL)
    {
        fprintTree(out, head->right);
    }

    fprintf(out, ")");

    return SUCCESS;
}

err deleteNode (Node* node)
{
    CHECK_PTR(node);

    free(node->right);
    free(node->left);
    free(node);
}

err treeKill (Node* head)
{
    CHECK_PTR(head);

    if (head->left != NULL)
        treeKill(head->left);

    if (head->right != NULL)
        treeKill(head->right);

    free(head);
    return SUCCESS;
}

err treeSearch (Node* head, data_t srch, Node** return_t)
{
    CHECK_PTR(head);
    CHECK_PTR(return_t);

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
            {
                *return_t = NULL;
                return UNFOUND;
            }
        }
    }
}
