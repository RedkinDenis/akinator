#ifndef INTERFACE_H
#define INTERFACE_H

#include "..\TXLib.h"

enum answer
{
    YES = 1,
    NO = 0,
    fail = 2
};

enum answer check_answer ();

void InputBox (char* data, const char* message);

void create_window ();

void put_answer (const char* data);

#endif //INTERFACE_H
