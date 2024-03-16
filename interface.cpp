#include "interface.h"

#define  TX_USE_SPEAK
#include "..\TXLib.h"

#include "TXWave.h"
#include <time.h>

#define YES_BUTTON txGetExtentX() / 2 - 250, txGetExtentY() * 6 / 10, txGetExtentX() / 2 - 50, txGetExtentY() * 6 / 10 + 100

#define NO_BUTTON txGetExtentX() / 2 + 50, txGetExtentY() * 6 / 10, txGetExtentX() / 2 + 250, txGetExtentY() * 6 / 10 + 100

#define SKIP_BUTTON txGetExtentX() / 2 - 150, txGetExtentY() * 3 / 4, txGetExtentX() / 2 + 150, txGetExtentY() * 3 / 4 + 80

#define GUESS_BUTTON txGetExtentX() / 2 - 250, txGetExtentY() * 7 / 10, txGetExtentX() / 2 - 50, txGetExtentY() * 7 / 10 + 100

#define DESCR_BUTTON txGetExtentX() / 2 + 50, txGetExtentY() * 7 / 10, txGetExtentX() / 2 + 250, txGetExtentY() * 7 / 10 + 100

#define SHOW_BUTTON txGetExtentX() / 2 - 150, txGetExtentY() / 2, txGetExtentX() / 2 + 150, txGetExtentY() / 2 + 80

#define CLOSE_BUTTON txGetExtentX() - 50, 0, txGetExtentX(), 37

#define BACK_BUTTON txGetExtentX() - 300, 100, txGetExtentX() - 100, 200

#define RESTART_BUTTON txGetExtentX() - 300, 300, txGetExtentX() - 100, 400

struct BUTTON_
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

static void draw_YN_bt ();

static void draw_AKIN_bt ();

static void say (void* data);

static int mouse_in (BUTTON_* bt);

void mySleep (int time)
{
    txSleep(time);
}

void create_window ()
{
    _txConsole = -1;

    txCreateWindow (1520, 780);
}

void fill_window (wizard mood)
{
    HDC background = txLoadImage ("background.bmp");

    const char* wizard_type = 0;

    if (mood == UNDERSTAND)
        wizard_type = "understanding_wizard.bmp";

    else if (mood == THINKING)
        wizard_type = "thinking_wizard.bmp";

    else if (mood == BASE)
        wizard_type = "base_wizard.bmp";

    else if (mood == CONFUSED)
        wizard_type = "confused_wizard.bmp";

    else if (mood == PROUD)
        wizard_type = "proud_wizard.bmp";

    HDC wizard = txLoadImage (wizard_type);

    HDC Close = txLoadImage ("close.bmp");

    txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), background, 0, 0);

    txTransparentBlt (txDC(), 0, 0, 0, 0, wizard, 0, 0);

    BUTTON_ close = { CLOSE_BUTTON };
    txTransparentBlt (txDC(), close.x1, 0, 0, 0, Close, 0, 0);

    txDeleteDC (background);
}

void draw_YN_bt ()
{
    BUTTON_ yes = { YES_BUTTON };
    BUTTON_ no  = { NO_BUTTON };

    txSetFillColor (TX_ORANGE);

    txSetColor (TX_RED);
    txSelectFont ("Times New Roman", 80);

    txRectangle (YES_BUTTON);
    txTextOut (yes.x1 + 60, yes.y1 + 10, "Да");

    txRectangle (NO_BUTTON);
    txTextOut (no.x1 + 50, no.y1 + 10, "Нет");
}

void draw_AKIN_bt ()
{
    BUTTON_ skip = { SKIP_BUTTON };
    BUTTON_ Back = { BACK_BUTTON };
    BUTTON_ restart = { RESTART_BUTTON };

    txSetFillColor (TX_ORANGE);

    txSetColor (TX_RED);
    txSelectFont ("Times New Roman", 80);

    txRectangle (SKIP_BUTTON);
    txTextOut (skip.x1 + 30, skip.y1 + 5, "Не знаю");

    txRectangle (BACK_BUTTON);
    txTextOut (Back.x1 + 20, Back.y1 + 5, "Назад");

    txSelectFont ("Times New Roman", 60);
    txRectangle (RESTART_BUTTON);
    txTextOut (restart.x1 + 5, restart.y1 + 15, "В начало");
}

void draw_mode_bt ()
{
    BUTTON_ guess = { GUESS_BUTTON };
    BUTTON_ describe = { DESCR_BUTTON };
    BUTTON_ show = { SHOW_BUTTON };

    txSetFillColor (TX_ORANGE);

    txSetColor (TX_RED);
    txSelectFont ("Times New Roman", 60);

    txRectangle (GUESS_BUTTON);
    txTextOut (guess.x1 + 10, guess.y1 + 15, "Отгадать");

    txRectangle (DESCR_BUTTON);
    txTextOut (describe.x1 + 10, describe.y1 + 15, "Описать");

    txRectangle (SHOW_BUTTON);
    txTextOut (show.x1 + 25, show.y1 + 10, "Выдать базу");
}

void say (void* data)
{
    txSpeak("%s", (const char*)data);
}

void put_question (char* data, wizard mood)
{
    _beginthread (say, 0, data);

    txClear();
    fill_window(mood);
    draw_YN_bt();

    txSetFillColor (TX_BLUE);

    int data_len = (int)strlen(data);
    int wide_coeff = 0;
    if (data_len < 10)
        wide_coeff = 10;
    else
        wide_coeff = 9;

    txRectangle (txGetExtentX() / 2 - data_len * wide_coeff, txGetExtentY() / 3, txGetExtentX() / 2 + data_len * wide_coeff, txGetExtentY() / 3 + 90);
    txFloodFill (txGetExtentX() / 2, txGetExtentY() / 3 + 10);

    txSetColor (TX_WHITE);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(txGetExtentX() / 2 - data_len * wide_coeff, txGetExtentY() / 3, txGetExtentX() / 2 + data_len * wide_coeff, txGetExtentY() / 3 + 90, data);
}

enum answer check_answer (ans_mode mode)
{
    BUTTON_ yes = { YES_BUTTON };
    BUTTON_ no  = { NO_BUTTON };
    BUTTON_ close = { CLOSE_BUTTON };
    BUTTON_ Back = { BACK_BUTTON };
    BUTTON_ restart = { RESTART_BUTTON };

    BUTTON_ skip  = { SKIP_BUTTON };

    if (mode == YNDN)
        draw_AKIN_bt();

    txWaveData_t shlepa = txWaveLoadWav ("shlepa.wav");

    time_t start = time(NULL);
    while(1)
    {
        while (txMouseButtons() != 1)
        {
            if ((time(NULL) - start) == 10)
            {
                txWaveOut (shlepa);
                start = time(NULL);
            }
        }

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
        else if (mode == YNDN && mouse_in(&skip))
        {
            while (txMouseButtons() != 0);
            return SKIP;
        }
        else if (mode == YNDN && mouse_in(&Back))
        {
            while (txMouseButtons() != 0);
            return BACK;
        }
        else if (mode == YNDN && mouse_in(&restart))
        {
            while (txMouseButtons() != 0);
            return RESTART;
        }
        else if (mouse_in(&close))
        {
            while (txMouseButtons() != 0);
            return CLOSE;
        }
    }
    return ERR;
}

void put_answer (const char* data, wizard mood, int symb_lim)
{
    txClear();
    fill_window(mood);

    txSetFillColor (TX_BLUE);

    int data_len = (int)strlen(data);
    int wide_coeff = 0;
    if (data_len < 10)
        wide_coeff = 10;
    else if (data_len < 20)
        wide_coeff = 9;
    else if (data_len < 30)
        wide_coeff = 8;
    else
        wide_coeff = 7;

    if(symb_lim != 0)
        data_len = symb_lim;

    txRectangle (txGetExtentX() / 2 - data_len * wide_coeff, txGetExtentY() / 3, txGetExtentX() / 2 + data_len * wide_coeff, txGetExtentY() / 3 + 90);
    txFloodFill (txGetExtentX() / 2, txGetExtentY() / 3 + 10);

    txSetColor (TX_WHITE);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(txGetExtentX() / 2 - data_len * wide_coeff, txGetExtentY() / 3, txGetExtentX() / 2 + data_len * wide_coeff, txGetExtentY() / 3 + 90, data);
}

enum answer choose_mode ()
{
    put_answer("Выберите режим:", BASE);
    draw_mode_bt();

    BUTTON_ guess = { GUESS_BUTTON };
    BUTTON_ describe = { DESCR_BUTTON };
    BUTTON_ show = { SHOW_BUTTON };
    BUTTON_ close = { CLOSE_BUTTON };

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
        else if (mouse_in(&show))
        {
            while (txMouseButtons() != 0);
            return SHOW;
        }
        else if (mouse_in(&close))
        {
            while (txMouseButtons() != 0);
            return CLOSE;
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
