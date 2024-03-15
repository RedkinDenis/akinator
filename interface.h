#ifndef INTERFACE_H
#define INTERFACE_H

enum answer
{
    YES = 1,
    NO = 0,
    ERR = 2,
    GUESS = 3,
    DESCR = 4,
    SHOW = 5,
    CLOSE = 6,
    SKIP = 7
};

enum wizard
{
    THINKING = 1,
    UNDERSTAND = 2,
    CONFUSED = 3,
    BASE = 4,
    PROUD = 5
};

enum ans_mode
{
    YN = 1,
    YNDN = 2
};

void mySleep (int time);

void draw_mode_bt ();

enum answer check_answer (ans_mode mode);

enum answer choose_mode ();

answer InputBox (char* data, const char* message, int data_len);

void create_window ();

void fill_window (wizard mood);

void put_question (char* data, wizard mood);

void put_answer (const char* data, wizard mood);

#endif //INTERFACE_H
