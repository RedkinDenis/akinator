#include "akinator.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "input_output.h"
#include "interface.h"
#include "..\err_codes.h"
#include "UDL.h"
#include "stack.h"

#define CHECK_FOR_CLOSE(ans, run)    \
    do                               \
    {                                \
    if (ans == CLOSE)                \
    {                                \
        *run = 0;                    \
        return SUCCESS;              \
    }                                \
    } while(0)

enum mode
{
    USER = 1,
    GOD = 2
};

static mode choose_game_mode (int argc, char* argv[]);

static int check_password (char* str);

static char* caesar (const char* str);

static err change_tree (Node* tree, int* run);

static err get_node_info (Node* tree);

static err running(Node* tree, int* run);

static err run_describe (Node* tree);

static err run_guess (Node* tree, int* run);

static err check_back_restart (answer ans, Node** tree, Stack* dont_know = NULL);

static err add_node (Node* tree);

static err get_info (Node* tree, char** left_buf, char** parent_buf);

static err ask (char* data);

static char* make_question (char* data);

static err tree_kill (Node* head);

static err deleteNode (Node* head);

static err treeSearch (Node* head, data_t srch, Node** return_t);

static err make_description (Node* tree, const char* obj, char** description);

static err fill_description (Stack* stk, char** description);

static err make_description__ (Node* tree, const char* obj, Stack* stk, int* found);

static void split_str (char** description);

static void other_subtree (Node** tree, Stack* stk);

static void choose_subtree (Node** tree, Stack* stk);

int main(int argc, char* argv[])
{
    FOPEN(read, "rtTree.txt", "rb");

    Node* tree = {};

    CALLOC(tree, Node, 1);
    CALLOC(tree->data, char, DATA_LEN + 1);

    importTree(read, tree);

    fclose(read);

    create_window ();

    int run = 1;

    mode md = choose_game_mode(argc, argv);

    if (md == GOD)
        while (run == 1)
            change_tree(tree, &run);

    else
        while (run == 1)
            running(tree, &run);

    //print_tree(tree);

    /*FOPEN(out, "rtTree.txt", "wb");

    fprint_tree(out, tree);

    fclose(out);*/

    tree_kill(tree);
}

mode choose_game_mode (int argc, char* argv[])
{
    //const char* password = "god";
    if(argc == 1)
        return USER;

    if (argc == 2 && check_password(argv[1]))
        return GOD;

    else if (argc > 2)
    {
        printf ("��������� �� ���� ����������\n");
        mySleep(1500);
    }
    return USER;
}

int check_password (char* str)
{
    const char* password = "ufxx|twi";

    if (strncmp(password, caesar(str), strlen(password)) == 0)
        return 1;

    return 0;
}

char* caesar (const char* str)
{
    char* code = (char*)calloc(strlen(str), sizeof(char));

    for (int i = 0; i < strlen(str); i++)
        code[i] = str[i] + 5;

    return code;
}

err change_tree (Node* tree, int* run)
{
    answer ans = ERR;
    while (1)
    {
        ask(tree->data);
        ans = check_answer(YNBR);

        CHECK_FOR_CLOSE (ans, run);

        if (ans == YES)
            tree = tree->left;

        else if (ans == NO)
            tree = tree->right;

        if (check_back_restart(ans, &tree) == FAIL)
            return SUCCESS;

        if (tree->right == NULL && tree->left == NULL)
        {
            put_answer(tree->data, THINKING);
            ans = check_add ();

            CHECK_FOR_CLOSE (ans, run);

            if (check_back_restart(ans, &tree) == FAIL)
                return SUCCESS;

            if (ans == ADD)
                get_node_info(tree);
        }
    }
    return SUCCESS;
}

err get_node_info (Node* tree)
{
    answer ans = ERR;
    char* right_buf = 0;
    char* left_buf = 0;

    CALLOC(left_buf, char, DATA_LEN + 1);
    CALLOC(right_buf, char, DATA_LEN + 1);

    InputBox(left_buf, "������� �������� ���� �� ����������� ��", DATA_LEN);
    if (ans == ERR)
    {
        free(left_buf);
        return FAIL;
    }

    InputBox(right_buf, "������� �������� ���� �� ����������� ���", DATA_LEN);
    if (ans == ERR)
    {
        free(left_buf);
        free(right_buf);
        return FAIL;
    }

    CALLOC(tree->right, Node, 1);
    CALLOC(tree->left, Node, 1);

    Node* right = tree->right;
    Node* left = tree->left;

    CALLOC(right->data, char, DATA_LEN + 1);
    CALLOC(left->data, char, DATA_LEN + 1);

    right->parent = tree;
    left->parent = tree;

    strcpy(right->data, right_buf);
    strcpy(left->data, left_buf);

    free(left_buf);
    free(right_buf);

    return SUCCESS;
}

err check_back_restart (answer ans, Node** tree, Stack* dont_know)
{
    if (ans == BACK)
    {
        if ((*tree)->parent != NULL)
        {
            (*tree) = (*tree)->parent;
            if (dont_know != NULL)
                if (dont_know->data[dont_know->size - 1] == (*tree))
                    stack_pop(dont_know, NULL);
        }
        else
            return FAIL;
    }
    else if (ans == RESTART)
    {
        while ((*tree)->parent != NULL)
            (*tree) = (*tree)->parent;

        if (dont_know != NULL)
        {
            stack_dtor(dont_know);
            stack_ctor(dont_know, 0);
        }
    }
    return SUCCESS;
}

err running(Node* tree, int* run)
{
    answer ans = ERR;

    fill_window(BASE);
    draw_mode_bt();
    ans = choose_mode();

    CHECK_FOR_CLOSE (ans, run);

    if (ans == GUESS)
    {
        run_guess(tree, run);
        if (*run == 0)
            return SUCCESS;
    }

    else if (ans == DESCR)
        run_describe(tree);

    else if (ans == SHOW)
        draw_tree(tree);

    put_question((char*)"������ �����?", BASE);
    ans = check_answer(YN);

    if (ans == NO)
    {
        put_answer("������� �� ����!", BASE);
        *run = 0;
    }

    CHECK_FOR_CLOSE (ans, run);

    return SUCCESS;
}

err run_guess (Node* tree, int* run)
{
    Stack dont_know = {};
    stack_ctor(&dont_know, 1);

    answer ans = ERR;

    while (tree->right != NULL && tree->left != NULL)
    {
        ask(tree->data);

        ans = check_answer(YNDN);

        CHECK_FOR_CLOSE (ans, run);

        if (ans == YES)
            tree = tree->left;

        else if (ans == NO)
            tree = tree->right;

        else if (ans == SKIP)
            choose_subtree(&tree, &dont_know);

         if (check_back_restart(ans, &tree, &dont_know) == FAIL)
            return SUCCESS;

        if (tree->right == NULL && tree->left == NULL)
        {
            char* question = make_question(tree->data);
            put_question (question, UNDERSTAND);
            free(question);

            ans = check_answer(YN);
            CHECK_FOR_CLOSE (ans, run);

            if (ans == YES)
                put_answer("�� ���� ������ �� ��������", PROUD);

            else if (ans == NO)
            {
                if (dont_know.size > 0)
                {
                    other_subtree(&tree, &dont_know);
                }
                else
                {
                    err res = add_node(tree);

                    if (res == FAIL)
                        return SUCCESS;

                    put_answer("��� ������� �������� ����.", CONFUSED);

                    break;
                }
            }
        }
    }
    stack_dtor(&dont_know);
    mySleep(5000);
    return SUCCESS;
}

err run_describe (Node* tree)
{
    char* description = 0;
    char* srch = 0;
    CALLOC(srch, char, DATA_LEN);

    //txWaveData_t tam_blya = txWaveLoadWav ("description.wav");

    answer ans = InputBox(srch, "��� �������� �� ������ �������� ?\n( ����������, ������� � ������������ ������ )", DATA_LEN);
    if (ans == ERR)
    {
        free(srch);
        return FAIL;
    }

    make_description (tree, srch, &description);
    put_answer(description, UNDERSTAND, DATA_LEN);

    //txWaveOut (tam_blya);

    free(description);
    free(srch);
    mySleep(5000);

    return SUCCESS;
}

err tree_kill (Node* head)
{
    CHECK_PTR(head);

    if (head->left != NULL)
        tree_kill(head->left);

    if (head->right != NULL)
        tree_kill(head->right);

    free(head);
    return SUCCESS;
}

char* make_question (char* data)
{
    char* question = (char*)calloc(strlen(data) + strlen("���  ?"), sizeof(char));
    strcpy(question, "��� ");
    strcat(question, data);
    strcat(question, " ?");

    return question;
}

err ask (char* data)
{
    char* qst = 0;

    CALLOC(qst, char, (strlen("��� ������ ") + strlen(data)));
    strcat(qst, "��� ������ ");
    strcat(qst, data);
    for (int i = 1; i < strlen(qst); i++)
        qst[i] = (char)tolower(qst[i]);

    put_question (qst, THINKING);

    return SUCCESS;
}

err add_node (Node* tree)
{
    char* left_buf = 0;
    char* parent_buf = 0;

    err res = get_info(tree, &left_buf, &parent_buf);
    if (res != SUCCESS)
        return res;

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

    free(left_buf);
    free(parent_buf);

    return SUCCESS;
}

err get_info (Node* tree, char** left_buf, char** parent_buf)
{
    answer ans = ERR;

    CALLOC(*left_buf, char, DATA_LEN + 1);

    ans = InputBox(*left_buf, "���� ��� ��� �� ����� � ����?", DATA_LEN);
    if (ans == ERR)
    {
        free(*left_buf);
        return FAIL;
    }

    char* what_different = 0;
    CALLOC(what_different, char, (strlen("���  ���������� ��  ?") + strlen(*left_buf) + strlen(tree->data)));
    sprintf(what_different, "��� %s ���������� �� %s ?", *left_buf, tree->data);

    CALLOC(*parent_buf, char, DATA_LEN + 1);

    ans = InputBox(*parent_buf, what_different, DATA_LEN);
    if (ans == ERR)
    {
        free(*parent_buf);
        free(*left_buf);
        return FAIL;
    }

    strcat(*parent_buf, "?");

    return SUCCESS;
}

void other_subtree (Node** tree, Stack* stk)
{
    stack_pop(stk, tree);

    if ((*tree)->st == left)
    {
        *tree = (*tree)->right;
    }
    else
    {
        *tree = (*tree)->left;
    }
    return;
}

void choose_subtree (Node** tree, Stack* stk)
{
    if (rand() % 2 == 0)
        (*tree)->st = left;
    else
        (*tree)->st = right;

    stack_push(stk, tree);

    if ((*tree)->st == left)
    {
        *tree = (*tree)->left;
    }
    else
    {
        *tree = (*tree)->right;
    }
    return;
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
        fill_description(&stk, description);
    }
    else
    {
        const char* descr_temp = "�� ����� ������ �� ������ �������(((";
        *description = (char*)calloc(strlen(descr_temp), sizeof(char));
        strcpy(*description, descr_temp);
    }

    split_str(description);

    stack_dtor(&stk);
    return res;
}

err fill_description (Stack* stk, char** description)
{
    int descr_size = 0;

    for (size_t i = 0; i < stk->size; i++)
        descr_size += ((int)strlen(stk->data[i]->data) + 5);

    CALLOC(*(description), char, descr_size);

    for (size_t i = 0; i < stk->size; i++)
    {
        if (i < stk->size - 1 && (stk->data[i])->right == stk->data[i + 1])
            strcat(*(description), "�� ");

        strcat(*(description), stk->data[i]->data);

        if (i == (stk->size - 2))
            strcat(*(description), "�");

        strcat(*(description), " ");
    }

    size_t cnt = 0;

    (*description)[0] = (char)toupper((*description)[0]);
    for (int i = 1; i < descr_size; i++)
    {
        if ((*description)[i] == '?')
        {
            cnt++;
            if (cnt == stk->size)
                (*description)[i] = '.';
            else if (cnt == (stk->size - 1))
                (*description)[i] = ' ';
            else
                (*description)[i] = ',';
        }
        (*description)[i] = (char)tolower((*description)[i]);
    }

    return SUCCESS;
}

err make_description__ (Node* tree, const char* obj, Stack* stk, int* found)
{
    CHECK_PTR(tree);

    if (tree->left != NULL)
    {
        stack_push(stk, &tree);
        make_description__(tree->left, obj, stk, found);
        if (*found == 1)
            return SUCCESS;
        stack_pop(stk, NULL);
    }

    if (tree->right != NULL)
    {
        stack_push(stk, &tree);
        make_description__(tree->right, obj, stk, found);
        if (*found == 1)
            return SUCCESS;
        stack_pop(stk, NULL);
    }

    if (strcmp(tree->data, obj) == 0)
    {
        *found = 1;
        return SUCCESS;
    }
    return FAIL;
}

void split_str (char** description)
{
    int line_len = DATA_LEN;

    int ds_len = (int)strlen(*description);
    int lines = ds_len / line_len + 1;
    char* ptr = 0;

    for (int l = 1; l < lines; l++)
    {
        ptr = *description + (l * line_len);
        while (*ptr != ' ')
            ptr--;
        *ptr = '\n';
    }
    return;
}
