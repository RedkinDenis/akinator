#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "input_output.h"
#include <dirent.h>

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

#define MAX_ADVERT_QANT 10

static err fill_buffer (FILE* read, char** buf);

static void get_data (char* buf, int* ptr, Node* tree, int data_len);

static void goto_prace (char* buf, int* ptr);

static void draw_tree_1 (FILE* save, Node* tree, int* node_num);

static void draw_tree_2 (FILE* save, Node* tree);

static err print_tree__ (Node* head, int* tab);

static err fprint_tree__ (FILE* out, Node* head, int* tab);

void find_advert(advertisement* advert)
{
    // advert = (advertisement*)calloc(1, sizeof(advert));
    advert->banners = (char**)calloc(MAX_ADVERT_QANT, sizeof(char*));

    struct dirent *dp = NULL;
    DIR *dir = opendir(".\\advert");
 
    if (!dir) 
        return; 
 
    int n = 0;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".")  == 0 ||
            strcmp(dp->d_name, "..") == 0 ||
            strstr(dp->d_name, ".bmp") == NULL)
            continue;

        advert->banners[n] = strdup(dp->d_name);
        n++;        
    } 
    advert->qant = n;   
    
    closedir(dir);
    return;
}

void delete_ad (advertisement* advert)
{
    for (int i = 0; i < MAX_ADVERT_QANT; i++)
        free(advert->banners[i]);
    free(advert->banners);
}

struct line* InputData(FILE* fp, int fsize, int* n)    
{
    char* buffer = (char*)calloc(fsize + 1, sizeof(char));
    int nLines = 0;

    int rsize = fread(buffer, sizeof(char), fsize, fp);
    assert(rsize == fsize);

    if(buffer[rsize - 1] != '\n')
        buffer[rsize] = '\n';
    else
        buffer[rsize] = '\0';

    for(int i = 0; i < rsize + 1; i++)
    {
        if(buffer[i] == '\r')
        {
            buffer[i] = '\0';
            i += 1;
            buffer[i] = '\0';
            nLines++;
        }
        else if(buffer[i] == '\n')
        {
            nLines++;
            buffer[i] = '\0';
        }
    }
    struct line* data = (struct line*)calloc(nLines, sizeof(struct line));

    int j = 0;
    int oldind = 0;

    data[j].str = buffer;
    j++;
    for(int i = 0; i < rsize + 1; i++)
    {
        if(buffer[i] == '\0')
        {
            if(j <= nLines)
            {
                data[j].str = buffer + i + 1;
                data[j - 1].len = i - oldind;
                oldind = i + 1;

                if(buffer[i + 1] == '\0')
                {
                    data[j].str++;
                    i++;
                    oldind++;
                }
                j++;
            }
        }
    }

    *n = nLines;
    return data;
}

int GetFileSize(FILE* fp)
{
    int startPos = ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int fsize = ftell(fp);
    fseek(fp, startPos, SEEK_SET);
    return fsize;
}

err draw_tree (Node* tree)
{
    FOPEN(save, "drawTree.txt", "wb");

    fprintf(save, "digraph Tree {\n");

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
    strcat(buffer, tree->data);                                                 //memcpy
    buffer[strlen(tree->data) + 1] = '"';

    tree->num_in_tree = *node_num;

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

err print_tree (Node* head)
{
    CHECK_PTR(head);

    int tab = 0;
    return print_tree__(head, &tab);
}

err print_tree__ (Node* head, int* tab)
{
    CHECK_PTR(head);

    printf("(");

    printf("%*s", *tab * 4, "");

    printf("*%s*", head->data);
    if (head->picture != NULL)
        printf(" *%s*", head->picture);
    

    if (head->left != NULL)
    {
        *tab += 1;
        printf("\n");
        print_tree__(head->left, tab);
    }

    if (head->right != NULL)
    {
        printf("\n");
        print_tree__(head->right, tab);
        *tab -= 1;
    }

    printf(")");

    return SUCCESS;
}

err fprint_tree (FILE* out, Node* head)
{
    CHECK_PTR(out);
    CHECK_PTR(head);

    int tab = 0;
    return fprint_tree__(out, head, &tab);
}

err fprint_tree__ (FILE* out, Node* head, int* tab)
{
    CHECK_PTR(head);

    fprintf(out, "(");

    fprintf(out, "%*s", *tab * 4, "");

    fprintf(out, "*%s*", head->data);
    if (head->picture != NULL)
        fprintf(out, " *%s*", head->picture);

    if (head->left != NULL)
    {
        *tab += 1;
        fprintf(out, "\n");
        fprint_tree__(out, head->left, tab);
    }

    if (head->right != NULL)
    {
        fprintf(out, "\n");
        fprint_tree__(out, head->right, tab);
        *tab -= 1;
    }

    fprintf(out, ")");

    return SUCCESS;
}

err fill_buffer (FILE* read, char** buf)
{
    CHECK_PTR(read);
    CHECK_PTR(buf);

    int fsize = GetFileSize(read);

    CALLOC(*buf, char, (fsize + 1));

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

    goto_prace(buf, &ptr);

    if (buf[ptr] == '(')
    {
        ptr++;
        level++;
        //CALLOC(tree->picture, char, DATA_LEN + 1);
        get_data(buf, &ptr, tree, DATA_LEN);
    }

    while (level > 0)
    {
        if (buf[ptr] == '(')
        {
            ptr++;

            CHANGE_NODE(tree, tree->left);
            CALLOC(tree->data, char, DATA_LEN + 1);
            //CALLOC(tree->picture, char, DATA_LEN + 1);

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
                //CALLOC(tree->picture, char, DATA_LEN + 1);

                get_data(buf, &ptr, tree, DATA_LEN);
            }
        }
    }
    free(buf);

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

    *ptr += 1;
    while((buf[*ptr] != '(') && (buf[*ptr] != ')') && (buf[*ptr] != '*') )
        *ptr += 1;

    if (buf[*ptr] != '*')
        return;
    
    tree->picture = (char*)calloc(data_len + 1, sizeof(char));
    i = 0;
    //printf("%s\n", buf + *ptr);
    *ptr += 1;
    while ((buf[*ptr] != '*') && i <= data_len)
    {
        tree->picture[i] = buf[*ptr];
        *ptr += 1;
        i++;
        //printf("here");
    }
    goto_prace(buf, ptr);
}
