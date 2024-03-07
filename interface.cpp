#include "interface.h"

#define YES_BUTTON 250, 400, 450, 500

#define NO_BUTTON 550, 400, 750, 500

void create_window ()
{
    txCreateWindow (1000, 550);

    HDC  background_CopiedFromHelp = txLoadImage ("background.bmp");

    if (!background_CopiedFromHelp)
      txMessageBox ("Не могу загрузить фон из background.bmp");

    txBitBlt (txDC(), 0, 0, 1000, 550, background_CopiedFromHelp, 0, 0);

    txSetFillColor (TX_ORANGE);

    txSetColor (TX_RED);
    txSelectFont ("Times New Roman", 80);

    txRectangle (YES_BUTTON);
    txTextOut (300, 410, "Yes");

    txRectangle (NO_BUTTON);
    txTextOut (620, 410, "No");

    txDeleteDC (background_CopiedFromHelp);
}

void put_answer (const char* data)
{
    txSetFillColor (TX_BLUE);
    txRectangle (300, 160, 700, 250);
    txFloodFill (400, 200);

    txSetColor (TX_WHITE);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(0, 150, 1000, 250, data);
}

enum answer check_answer ()
{
    const RECT yes = { YES_BUTTON };
    const RECT no  = { NO_BUTTON };

    while(1)
    {
        while (txMouseButtons() != 1);
        if (In (txMousePos(), yes))
        {
            while (txMouseButtons() != 0);
            return YES;
        }
        else if (In (txMousePos(), no))
        {
            while (txMouseButtons() != 0);
            return NO;
        }
        else
            Sleep(1);

        system("cls");
    }
    return fail;
}

void InputBox (char* data, const char* message, int data_len)
{
    const char* temp = txInputBox(message, "Help me become better");
    strncpy(data, temp, data_len - 1);
}
