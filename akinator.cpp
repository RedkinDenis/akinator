#include "akinator.h"

static err printTree__ (Node* head, int* tab);

static err make_description__ (Node* tree, const char* obj, Stack* stk, int* found);

static err fprintTree__ (FILE* out, Node* head, int* tab);

static void draw_tree_1 (FILE* save, Node* tree, int* node_num);

static void draw_tree_2 (FILE* save, Node* tree);


int main()
{
    FOPEN(read, "treeSave1.txt", "rb");

    Node* tree = {};

    CALLOC(tree, Node, 1);
    CALLOC(tree->data, char, DATA_LEN + 1);

    importTree(read, tree);

    fclose(read);

    create_window ();

    int run = 1;
    while (run == 1)
        running(tree, &run);

    //printTree(tree);

    /*FOPEN(out, "treeSave1.txt", "wb");

    fprintTree(out, tree);

    fclose(out);*/

    treeKill(tree);
}

err draw_tree (Node* tree)
{
    FOPEN(save, "drawTree.txt", "wb");

    fprintf(save, "digraph List {\n");

    int node_num = 0;
    draw_tree_1(save, tree, &node_num);
    draw_tree_2(save, tree);

    fprintf(save, "}");

    fclose(save);

    system("iconv -f WINDOWS-1251 -t UTF-8 drawTree.txt > buffer.txt");
    system("dot buffer.txt -Tpng -o drawTree.png");
    system("start drawTree.png");
    return SUCCESS;
}

void draw_tree_1 (FILE* save, Node* tree, int* node_num)
{
    char* buffer = (char*)calloc(strlen(tree->data) + 3, sizeof(char));
    buffer[0] = '"';
    strcat(buffer, tree->data);
    buffer[strlen(tree->data) + 1] = '"';

    tree->num_in_tree = *node_num;

    fprintf(save, "    %d [shape = Mrecord, style = filled, fillcolor = red, label = %s ];\n", *node_num, buffer);

    if (tree->right == NULL && tree->left == NULL)
        fprintf(save, "    %d [shape = Mrecord, style = filled, fillcolor = cyan, label = %s ];\n", *node_num, buffer);
    else
        fprintf(save, "    %d [shape = Mrecord, style = filled, fillcolor = lightgoldenrod1, label = %s ];\n", *node_num, buffer);


    if (tree->left != NULL)
    {
        *node_num += 1;
        draw_tree_1(save, tree->left, node_num);
    }

    if (tree->right != NULL)
    {
        *node_num += 1;
        draw_tree_1(save, tree->right, node_num);
    }

    return;
}

void draw_tree_2 (FILE* save, Node* tree)
{
    if (tree->left != NULL)
    {
        fprintf(save, "    %d -> %d [ color=green label=Да fontcolor=green ];\n", tree->num_in_tree, (tree->left)->num_in_tree);
        draw_tree_2(save, tree->left);
    }

    if (tree->right != NULL)
    {
        fprintf(save, "    %d -> %d [ color=red label=Нет fontcolor=red ];\n", tree->num_in_tree, (tree->right)->num_in_tree);
        draw_tree_2(save, tree->right);
    }

    return;
}


char* make_question (char* data)
{
    char* question = (char*)calloc(strlen(data) + strlen("Это  ?"), sizeof(char));
    strcpy(question, "Это ");
    strcat(question, data);
    strcat(question, " ?");

    return question;
}

err make_description (Node* tree, const char* obj, char** description)
{
    CHECK_PTR(tree);
    CHECK_PTR(obj);

    Stack stk = {};
    stack_ctor(&stk, 1);

    int found = 0;
    err res = make_description__(tree, obj, &stk, &found);

    if (res == SUCCESS)
    {
        int descr_size = 0;

        for (size_t i = 0; i < stk.size; i++)
            descr_size += (strlen(stk.data[i]) + 1);

        CALLOC(*(description), char, descr_size);

        for (size_t i = 0; i < stk.size; i++)
        {
            strcat(*(description), stk.data[i]);

            if (i == (stk.size - 2))
                strcat(*(description), "и");

            strcat(*(description), " ");
        }

        size_t cnt = 0;
        for (int i = 1; i < descr_size; i++)
        {
            if ((*description)[i] == '?')
            {
                cnt++;
                if (cnt == stk.size)
                    (*description)[i] = '.';
                else if (cnt == (stk.size - 1))
                    (*description)[i] = ' ';
                else
                    (*description)[i] = ',';
            }
            (*description)[i] = (char)tolower((*description)[i]);
        }
    }
    else
    {
        const char* descr_temp = "Не нашел ничего по вашему запросу(((";
        *description = (char*)calloc(strlen(descr_temp), sizeof(char));
        strcpy(*description, descr_temp);
    }

    stack_dtor(&stk);
    return res;
}

err make_description__ (Node* tree, const char* obj, Stack* stk, int* found)
{
    if (tree->left != NULL)
    {
        stack_push(stk, &(tree->data));
        make_description__(tree->left, obj, stk, found);
        if (*found == 1)
            return SUCCESS;
        stack_pop(stk, NULL);
    }

    if (tree->right != NULL)
    {
        make_description__(tree->right, obj, stk, found);
        if (*found == 1)
            return SUCCESS;
    }

    if (strcmp(tree->data, obj) == 0)
    {
        *found = 1;
        return SUCCESS;
    }
    return FAIL;
}

err running(Node* tree, int* run)
{
    answer ans = ERR;

    fill_window();
    draw_mode_bt();
    ans = choose_mode();

    if (ans == GUESS)
    {
        while(tree->right != NULL && tree->left != NULL)
        {
            put_question (tree->data);

            ans = check_answer();

            if (ans == YES)
                tree = tree->left;
            else if (ans == NO)
                tree = tree->right;
        }

        char* question = make_question(tree->data);
        put_question (question);
        ans = check_answer();

        if (ans == YES)
            put_answer("От меня ничего не укроется");

        else if (ans == NO)
        {
            char* parent_buf = 0;
            char* left_buf = 0;
            CALLOC(parent_buf, char, DATA_LEN + 1);
            CALLOC(left_buf, char, DATA_LEN + 1);

            ans = InputBox(left_buf, "Кого или что вы имели в виду?", DATA_LEN);
            if (ans == ERR)
                goto END;

            char* what_different = 0;
            CALLOC(what_different, char, (strlen("Чем  отличается от  ?") + strlen(left_buf) + strlen(tree->data)));

            sprintf(what_different, "Чем %s отличается от %s ?", left_buf, tree->data);

            InputBox(parent_buf, what_different, DATA_LEN);
            if (ans == ERR)
                goto END;

            strcat(parent_buf, "?");

            CALLOC(tree->right, Node, 1);
            CALLOC(tree->left, Node, 1);

            Node* right = tree->right;
            Node* left = tree->left;

            right->parent = tree;
            left->parent = tree;

            CALLOC(right->data, char, DATA_LEN + 1);
            CALLOC(left->data, char, DATA_LEN + 1);

            strcpy(left->data, left_buf);

            strcpy(right->data, tree->data);

            strcpy(tree->data, parent_buf);

            put_answer("Вам удалось победить меня.");
        }
        Sleep(3000);
    }

    else if (ans == DESCR)
    {
        char* description = 0;
        char* srch = 0;
        CALLOC(srch, char, DATA_LEN);

        InputBox(srch, "Чье описание вы хотите получить ?\n( Пожалуйста, вводите в именительном падеже )", DATA_LEN);
        if (ans == ERR)
            goto END;

        make_description (tree, srch, &description);
        put_answer(description);
        Sleep(5000);
    }

    else if (ans == REVIEW)
        draw_tree(tree);

END:
    put_question("Играть снова?");
    ans = check_answer();

    if (ans == YES)
        *run = 1;

    else if (ans == NO)
    {
        put_answer("Спасибо за игру!");
        *run = 0;
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

        get_data(buf, &ptr, tree, DATA_LEN);
    }

    while (level > 0)
    {
        if (buf[ptr] == '(')
        {
            ptr++;

            CHANGE_NODE(tree, tree->left);
            CALLOC(tree->data, char, DATA_LEN + 1);

            get_data(buf, &ptr, tree, DATA_LEN);
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

                get_data(buf, &ptr, tree, DATA_LEN);
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

void get_data(char* buf, int* ptr, Node* tree, int data_len)
{
    int i = 0;
    while (buf[*ptr - 1] != '*')
        *ptr += 1;

    while ((buf[*ptr] != '*') && i <= data_len)
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

