#include "interface.h"

#define YES_BUTTON 250, 400, 450, 500

#define GUESS_BUTTON 250, 410, 450, 490

#define DESCR_BUTTON 550, 410, 750, 490

#define NO_BUTTON 550, 400, 750, 500

#define SHOW_BUTTON 350, 280, 650, 360

void create_window ()
{
    _txConsole = -1;

    txCreateWindow (1000, 550);
}

void fill_window ()
{
    HDC background_CopiedFromHelp = txLoadImage ("background.bmp");

    //HDC wizard = txLoadImage ("wizard.bmp");

    if (!background_CopiedFromHelp)
      txMessageBox ("Не могу загрузить фон из background.bmp");

    txBitBlt (txDC(), 0, 0, 1000, 550, background_CopiedFromHelp, 0, 0);

    //txTransparentBlt (txDC(), 400, 0, 100, 100, wizard, 300, 100);

    txDeleteDC (background_CopiedFromHelp);
}

void draw_YN_bt ()
{
    txSetFillColor (TX_ORANGE);

    txSetColor (TX_RED);
    txSelectFont ("Times New Roman", 80);

    txRectangle (YES_BUTTON);
    txTextOut (320, 410, "Да");

    txRectangle (NO_BUTTON);
    txTextOut (600, 410, "Нет");
}

void draw_mode_bt ()
{
    txSetFillColor (TX_ORANGE);

    txSetColor (TX_RED);
    txSelectFont ("Times New Roman", 60);

    txRectangle (GUESS_BUTTON);
    txTextOut (260, 415, "Отгадать");

    txRectangle (DESCR_BUTTON);
    txTextOut (560, 415, "Описать");

    txRectangle (SHOW_BUTTON);
    txTextOut (370, 285, "Выдать базу");
}

void put_question (const char* data)
{
    txClear();
    fill_window();
    draw_YN_bt();

    txSetFillColor (TX_BLUE);

    size_t data_len = strlen(data);
    int wide_coeff = 0;
    if (data_len < 10)
        wide_coeff = 10;
    else
        wide_coeff = 9;

    txRectangle (txGetExtentX() / 2 - data_len * wide_coeff, 160, txGetExtentX() / 2 + data_len * wide_coeff, 250);
    txFloodFill (400, 200);

    txSetColor (TX_WHITE);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(txGetExtentX() / 2 - data_len * wide_coeff, 150, txGetExtentX() / 2 + data_len * wide_coeff, 250, data);
}

enum answer check_answer ()
{
    BUTTON_ yes = { YES_BUTTON };
    BUTTON_ no  = { NO_BUTTON };

    while(1)
    {
        while (txMouseButtons() != 1);
        if (mouse_in(&yes))
        {
            while (txMouseButtons() != 0);
            return YES;
        }
        else if (mouse_in(&no))
        {
            while (txMouseButtons() != 0);
            return NO;
        }
    }
    return ERR;
}

void put_answer (const char* data)
{
    txClear();
    fill_window();

    txSetFillColor (TX_BLUE);

    size_t data_len = strlen(data);
    int wide_coeff = 0;
    if (data_len < 10)
        wide_coeff = 10;
    else
        wide_coeff = 9;

    txRectangle (txGetExtentX() / 2 - data_len * wide_coeff, 160, txGetExtentX() / 2 + data_len * wide_coeff, 250);
    txFloodFill (400, 200);

    txSetColor (TX_WHITE);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(txGetExtentX() / 2 - data_len * wide_coeff, 150, txGetExtentX() / 2 + data_len * wide_coeff, 250, data);
}

enum answer choose_mode ()
{
    put_answer("Выберите режим:");
    draw_mode_bt();

    BUTTON_ guess = { GUESS_BUTTON };
    BUTTON_ describe = { DESCR_BUTTON };
    BUTTON_ review = { SHOW_BUTTON };

    while(1)
    {
        while (txMouseButtons() != 1);
        if (mouse_in(&guess))
        {
            while (txMouseButtons() != 0);
            return GUESS;
        }
        else if (mouse_in(&describe))
        {
            while (txMouseButtons() != 0);
            return DESCR;
        }
        else if (mouse_in(&review))
        {
            while (txMouseButtons() != 0);
            return REVIEW;
        }
    }
    return ERR;
}

int mouse_in (BUTTON_* bt)
{
    if ( (bt->x1 <= txMouseX() && txMouseX() <= bt->x2) && (bt->y1 <= txMouseY() && txMouseY() <= bt->y2) )
        return 1;
    return 0;
}

answer InputBox (char* data, const char* message, int data_len)
{
    const char* temp = txInputBox(message, "Помогите мне стать лучше");

    char* try_again = (char*)calloc(strlen("Это поле не может быть пустым!\n  \nПоследняя попытка!!!\nПотом игра прервется") + strlen(message), sizeof(char));
    strcpy(try_again, "Это поле не может быть пустым!\n");
    strcat(try_again, message);

    int tryings = 2;

    while (strlen(temp) == 0)
    {
        if (tryings == 0)
            return ERR;
        if (tryings == 1)
            strcat(try_again, "\nПоследняя попытка!!!\nПотом игра прервется");

        temp = txInputBox(try_again, "Помогите мне стать лучше");
        tryings--;
    }

    strncpy(data, temp, data_len - 1);
    return YES;
}
