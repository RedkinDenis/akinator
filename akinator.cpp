#include "akinator.h"
#include <conio.h>

int isLR(char* buf, int ptr);

int check_answer ();

int main()
{
    FOPEN(read, "treeSave.txt", "rb");

    Node* tree = {};

    CALLOC(tree, Node, 1);
    CALLOC(tree->data, char, DATA_LEN);

    importTree(read, tree);

    fclose(read);

    running(tree);

    //printTree(tree);

    /*FOPEN(out, "treeSave.txt", "wb");

    fprintTree(out, tree);

    fclose(out);*/


    treeKill(tree);
}

err running(Node* tree)
{
    int ans = 0;

    while(tree->right != NULL && tree->left != NULL)
    {
        printf("%s\n", tree->data);

        ans = check_answer();

        if (ans == 1)
            tree = tree->left;
        else if (ans == 0)
            tree = tree->right;

        system("cls");
    }
    printf("\nWas it %s ?", tree->data);

    ans = check_answer();
    if (ans == 1)
    {
        system("cls");
        printf("\nI've always known about it.");
        return SUCCESS;
    }
    else if (ans == 0)
    {
        //дописать запись нового элемента
        CALLOC(tree->right, Node, 1);
        CALLOC(tree->left, Node, 1);

        CALLOC((tree->right)->data, char, DATA_LEN);
        CALLOC((tree->left)->data, char, DATA_LEN);

        strcpy((tree->right)->data, tree->data);


    }

    return SUCCESS;
}

int check_answer ()
{
    printf("\nPrint y/n\n");

    int get = 0;
    char ans = 0;

    while (get == 0)
    {
        scanf("%c", &ans);

        if (ans == 'y')
            return 1;

        if (ans == 'n')
            return 0;
    }
}

err fill_buffer (FILE* read, char** buf)
{
    int fsize = GetFileSize(read);

    CALLOC(*buf, char, (fsize + 2));

    fread(*buf, sizeof(char), fsize, read);

    /*for (int i = 0; i < fsize - 1; i++)
        printf(" (%c)[%d] ", (*buf)[i], (*buf)[i]);
    printf("\n");*/

    return SUCCESS;
}

int isLR(char* buf, int ptr)
{
    while(buf[ptr] != '(')
    {
        if(buf[ptr] != ')')
            ptr++;
        else
            return 0;
    }
    return 1;
}

err importTree (FILE* read, Node* tree)
{
    char* buf = 0;
    fill_buffer(read, &buf);

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
        {                                                              // возможно падает из за редактирования базы
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

           /* while (buf[ptr] != '(' && buf[ptr] != ')')
                ptr += 1;*/

            if (buf[ptr] == '(' || ((buf[ptr + 1] == '(') && (buf[ptr] == '\n')) || ((buf[ptr + 2] == '(') && (buf[ptr + 1] == '\n') && (buf[ptr] == '\r')) )
            //if (isLR(buf, ptr))
            {
                //printf("here");
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

/*err NodeInsert (Node* head, data_t num)
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
}*/

err printTree (Node* head)
{
    int tab = 0;
    return printTree__(head, &tab);
}

err printTree__ (Node* head, int* tab)
{
    CHECK_PTR(head);

    printf("(");
    for(int i = 0; i < *tab; i++)
        printf("    ");

    printf("*%s*", head->data);

    if (head->left != NULL)
    {
        *tab += 1;
        printf("\n");
        printTree__(head->left, tab);
    }

    if (head->right != NULL)
    {
        printf("\n");
        printTree__(head->right, tab);
        *tab -= 1;
    }

    printf(")");

    return SUCCESS;
}

err fprintTree (FILE* out, Node* head)
{
    int tab = 0;
    return fprintTree__(out, head, &tab);
}

err fprintTree__ (FILE* out, Node* head, int* tab)
{
    CHECK_PTR(head);

    fprintf(out, "(");
    for(int i = 0; i < *tab; i++)
        fprintf(out, "    ");

    fprintf(out, "*%s*", head->data);

    if (head->left != NULL)
    {
        *tab += 1;
        fprintf(out, "\n");
        fprintTree__(out, head->left, tab);
    }

    if (head->right != NULL)
    {
        fprintf(out, "\n");
        fprintTree__(out, head->right, tab);
        *tab -= 1;
    }

    fprintf(out, ")");

    return SUCCESS;
}

/*err deleteNode (Node* node)
{
    CHECK_PTR(node);

    free(node->right);
    free(node->left);
    free(node);
}
*/

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

/*err treeSearch (Node* head, data_t srch, Node** return_t)
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
}*/
