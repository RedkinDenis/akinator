#include "akinator.h"

int main()
{
    FOPEN(read, "treeSave1.txt", "rb");

    Node* tree = {};

    CALLOC(tree, Node, 1);
    CALLOC(tree->data, char, DATA_LEN + 1);

    importTree(read, tree);

    printf("Here\n");

    fclose(read);

    create_window ();

    running(tree);

    printTree(tree);

    FOPEN(out, "treeSave1.txt", "wb");

    fprintTree(out, tree);

    fclose(out);

    treeKill(tree);
}

char* make_question (char* data)
{
    char* question = (char*)calloc(strlen(data) + strlen("Это  ?"), sizeof(char));
    strcpy(question, "Это ");
    strcat(question, data);
    strcat(question, " ?");

    return question;
}

err running(Node* tree)
{
    answer ans;

    system("cls");

    while(tree->right != NULL && tree->left != NULL)
    {
        put_answer (tree->data);

        ans = check_answer();

        if (ans == YES)
            tree = tree->left;
        else if (ans == NO)
            tree = tree->right;
    }

    char* question = make_question(tree->data);
    put_answer (question);
    ans = check_answer();

    if (ans == YES)
    {
        put_answer("От меня ничего не укроется");
        return SUCCESS;
    }
    else if (ans == NO)
    {
        CALLOC(tree->right, Node, 1);
        CALLOC(tree->left, Node, 1);

        Node* right = tree->right;
        Node* left = tree->left;

        right->parent = tree;
        left->parent = tree;

        CALLOC(right->data, char, DATA_LEN + 1);
        CALLOC(left->data, char, DATA_LEN + 1);

        InputBox(left->data, "Кого или что вы имели в виду?", DATA_LEN);

        strcpy(right->data, tree->data);

        char* what_different = 0;
        CALLOC(what_different, char, (strlen("Чем  отличается от  ?") + strlen(left->data) + strlen(right->data)));

        sprintf(what_different, "Чем %s отличается от %s ?", left->data, right->data);

        InputBox(tree->data, what_different, DATA_LEN);
        strcat(tree->data, "?");

        put_answer("Вам удалось победить меня.");

    }

    return SUCCESS;
}

err fill_buffer (FILE* read, char** buf)
{
    CHECK_PTR(read);
    CHECK_PTR(buf);

    int fsize = GetFileSize(read);

    CALLOC(*buf, char, (fsize + 2));

    fread(*buf, sizeof(char), fsize, read);

    return SUCCESS;
}

err importTree (FILE* read, Node* tree)
{
    CHECK_PTR(read);
    CHECK_PTR(tree);

    char* buf = 0;
    fill_buffer(read, &buf);

    int level = 0, ptr = 0;

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
            CALLOC(tree->data, char, DATA_LEN + 1);

            get_data(buf, &ptr, tree);
        }
        else if (buf[ptr] == ')')
        {
            tree = tree->parent;
            level--;

            if (level == 0)
                break;

            ptr++;

            goto_prace(buf, &ptr);

            if (buf[ptr] == '(')
            {
                ptr++;

                CHANGE_NODE(tree, tree->right);
                CALLOC(tree->data, char, DATA_LEN + 1);

                get_data(buf, &ptr, tree);
            }
        }
    }

    return SUCCESS;
}

void goto_prace (char* buf, int* ptr)
{
    while((buf[*ptr] != '(') && (buf[*ptr] != ')'))
        *ptr += 1;
}

void get_data(char* buf, int* ptr, Node* tree)
{
    int i = 0;
    while (buf[*ptr - 1] != '*')
        *ptr += 1;

    while ((buf[*ptr] != '*') && i <= DATA_LEN)
    {
        tree->data[i] = buf[*ptr];
        *ptr += 1;
        i++;
    }

    goto_prace(buf, ptr);
}

err printTree (Node* head)
{
    CHECK_PTR(head);

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
    CHECK_PTR(out);
    CHECK_PTR(head);

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

