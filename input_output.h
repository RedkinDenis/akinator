#ifndef INPUT_OTPUT_H
#define INPUT_OTPUT_H

#include "akinator.h"
#include "UDL.h"
#include "..\err_codes.h"

struct line
{
    size_t len;
    char* str;
};

struct advertisement
{
    struct  line* banners = 0;
    int ptr = 0;
    int qant = 0;
}; 

advertisement input_ad ();

struct line* InputData(FILE* fp, int fsize, int* n);

void input_name (char* data, int data_len);

void input_qst (char* data, int data_len);

int GetFileSize(FILE* fp);

err draw_tree (Node* tree);

err print_tree (Node* head);

err fprint_tree (FILE* out, Node* head);

err importTree (FILE* read ,Node* head);

#endif // INPUT_OTPUT_H
