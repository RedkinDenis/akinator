#ifndef INTERFACE_H
#define INTERFACE_H

#include "..\TXLib.h"

enum answer
{
    YES = 1,
    NO = 0,
    ERR = 2,
    GUESS = 3,
    DESCR = 4,
    REVIEW = 5
};

struct BUTTON_
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

enum answer check_answer ();

enum answer choose_mode ();

int mouse_in (BUTTON_* bt);

answer InputBox (char* data, const char* message, int data_len);

void create_window ();

void fill_window ();

void draw_YN_bt ();

void draw_mode_bt ();

void put_question (const char* data);

void put_answer (const char* data);

#endif //INTERFACE_H
