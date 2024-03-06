#include "akinator.h"
#include <conio.h>
#include "..\TXLib.h"
#include <windows.h>

int main()
{
    FOPEN(read, "treeSave.txt", "rb");

    Node* tree = {};

    CALLOC(tree, Node, 1);
    CALLOC(tree->data, char, DATA_LEN);

    importTree(read, tree);

    fclose(read);

    create_window ();

    running(tree);

    printTree(tree);

    /*FOPEN(out, "treeSave.txt", "wb");

    fprintTree(out, tree);

    fclose(out);*/


    treeKill(tree);
}

void create_window ()
{
    txCreateWindow (800, 600);

    txSetColor (TX_WHITE);
    txSetFillColor (TX_TRANSPARENT);
    txRectangle (10, 10, 790, 590);


    txTextOut (350, 50, "Akinator");

    txRectangle (100, 400, 200, 500);
    txTextOut (150, 450, "Yes");

    txRectangle (400, 400, 500, 500);
    txTextOut (450, 450, "No");
}

void put_answer (char* data)
{
    txSetFillColor (TX_BLACK);
    txRectangle (0, 150, 800, 250);
    txFloodFill (300, 200);
    txDrawText(0, 150, 800, 250, data);
}

char* make_question (char* data)
{
    char* question = (char*)calloc(40, sizeof(char));
    strcpy(question, "Was it ");
    strcat(question, data);
    strcat(question, " ?");

    return question;
}

err running(Node* tree)
{
    int ans = 0;

    while(tree->right != NULL && tree->left != NULL)
    {
        //txTextOut (300, 150, tree->data);

        put_answer (tree->data);


        //printf("%s\n", tree->data);

        ans = check_answer();

        if (ans == 1)
            tree = tree->left;
        else if (ans == 0)
            tree = tree->right;

        system("cls");
    }
    //printf("\nWas it %s ?", tree->data);

    char* question = make_question(tree->data);
    put_answer (question);

    ans = check_answer();

    if (ans == 1)
    {
        system("cls");
        put_answer("\nI've always known about it.");
        return SUCCESS;
    }
    else if (ans == 0)
    {
        CALLOC(tree->right, Node, 1);
        CALLOC(tree->left, Node, 1);

        Node* right = tree->right;
        Node* left = tree->left;

        right->parent = tree;
        left->parent = tree;

        CALLOC(right->data, char, DATA_LEN);
        CALLOC(left->data, char, DATA_LEN);

        //printf("What did you mean?\n");

        char* temp = (char*)txInputBox("What did you mean?", "Help me become better");
        strcpy(left->data, temp);

        //input_name(left->data, DATA_LEN);

        strcpy(right->data, tree->data);

        char* diff = (char*)calloc(100, sizeof(char));

        sprintf(diff, "What different bitween %s and %s ?\n", left->data, right->data);

        //input_qst(tree->data, DATA_LEN);
        temp = (char*)txInputBox(diff, "Help me become better");
        strcpy(tree->data, temp);

        put_answer("You managed to defeat me.");

    }

    return SUCCESS;
}

void input_qst(char* data, int data_len)
{
    int i = 0;
    //getchar();
    scanf("%c", data + i);

    while((data[i] != '\n') && (i < data_len - 2))
    {
        i++;
        scanf("%c", data + i);
    }
    data[i] = '\?';
    i++;
    data[i] = '\0';
}

void input_name(char* data, int data_len)
{
    int i = 0;
    getchar();
    scanf("%c", data + i);

    while((data[i] != '\n') && (i < data_len - 1))
    {
        i++;
        scanf("%c", data + i);
    }
    data[i] = '\0';

}

int check_answer ()
{
    RECT yes = { 100, 400, 200, 500 };
    RECT no  = { 400, 400, 500, 500 };

    while (txMouseButtons() != 1);

    if (In (txMousePos(), yes))
    {
        while (txMouseButtons() != 0);
        return 1;
    }
    else if (In (txMousePos(), no))
    {
        while (txMouseButtons() != 0);
        return 0;
    }
}

err fill_buffer (FILE* read, char** buf)
{
    int fsize = GetFileSize(read);

    CALLOC(*buf, char, (fsize + 2));

    fread(*buf, sizeof(char), fsize, read);

    return SUCCESS;
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

