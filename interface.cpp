#define  TX_USE_SPEAK
#define _TX_SKIP_EXCEPTION EXCEPTION_CPP_MSC
#include "..\TXLib.h"

#include "interface.h"

#include "TXWave.h"
#include <time.h>
//#include "input_output.h"

#define YES_BUTTON WINDOW_WIDE / 2 - 250, WINDOW_HIGHT * 6 / 10, WINDOW_WIDE / 2 - 50, WINDOW_HIGHT * 6 / 10 + 100

#define NO_BUTTON WINDOW_WIDE / 2 + 50, WINDOW_HIGHT * 6 / 10, WINDOW_WIDE / 2 + 250, WINDOW_HIGHT * 6 / 10 + 100

#define SKIP_BUTTON WINDOW_WIDE / 2 - 150, WINDOW_HIGHT * 3 / 4, WINDOW_WIDE / 2 + 150, WINDOW_HIGHT * 3 / 4 + 80

#define GUESS_BUTTON WINDOW_WIDE / 2 - 250, WINDOW_HIGHT * 7 / 10, WINDOW_WIDE / 2 - 50, WINDOW_HIGHT * 7 / 10 + 100

#define DESCR_BUTTON WINDOW_WIDE / 2 + 50, WINDOW_HIGHT * 7 / 10, WINDOW_WIDE / 2 + 250, WINDOW_HIGHT * 7 / 10 + 100

#define SHOW_BUTTON WINDOW_WIDE / 2 - 150, WINDOW_HIGHT / 2, WINDOW_WIDE / 2 + 150, WINDOW_HIGHT / 2 + 80

#define CLOSE_BUTTON WINDOW_WIDE - 50, 0, WINDOW_WIDE, 37

#define BACK_BUTTON WINDOW_WIDE - 300, 450, WINDOW_WIDE - 100, 550

#define RESTART_BUTTON WINDOW_WIDE - 300, 600, WINDOW_WIDE - 100, 700

#define ADD_BUTTON WINDOW_WIDE / 2 - 150, WINDOW_HIGHT * 3 / 4, WINDOW_WIDE / 2 + 150, WINDOW_HIGHT * 3 / 4 + 80

#define FILD_COLOR RGB(177, 71, 74)

#define BT_COLOR RGB(247, 4, 56)

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

static void draw_ADD_bt ();

static void draw_back_bt ();

static int count_lines (const char* data);

void mySleep (int time)
{
    txSleep(time);
}

void create_window ()
{
    _txConsole = -1;

    txCreateWindow (WINDOW_WIDE, WINDOW_HIGHT);
}

void fill_window (wizard mood)
{ 
    HDC background = txLoadImage ("interface\\background.bmp");
    if (!background)
        printf("error in open of background");

    txBitBlt (txDC(), 0, 0, WINDOW_WIDE, WINDOW_HIGHT, background, 0, 0);
    txDeleteDC (background);

    char* wizard_type = (char*)calloc(DATA_LEN + 1, sizeof(char));

    if (mood == UNDERSTAND)
        strcpy(wizard_type, "yakov\\understand_yakov.bmp");

    else if (mood == THINKING)
        strcpy(wizard_type, "yakov\\thinking_wizard.bmp");

    else if (mood == BASE)
        strcpy(wizard_type, "yakov\\base_yakov.bmp");

    else if (mood == CONFUSED)
        strcpy(wizard_type, "yakov\\confused_wizard.bmp");

    else if (mood == PROUD)
        strcpy(wizard_type, "yakov\\proud_yakov.bmp");

    else if (mood == FISH)
        strcpy(wizard_type, "yakov\\fish_yakov.bmp");

    HDC wizard = txLoadImage (wizard_type);
    free(wizard_type);
    if (!wizard)
        printf("error in open of wizard");

    txTransparentBlt (txDC(), 0, 90, 0, 0, wizard, 0, 0);
    txDeleteDC (wizard);

    HDC Close = txLoadImage ("interface\\close.bmp");
    if (!Close)
        printf("error in open of Close");

    BUTTON_ close = { CLOSE_BUTTON };
    txTransparentBlt (txDC(), close.x1, 0, 0, 0, Close, 0, 0);
    txDeleteDC (Close);
}

void draw_advert (void* Advert)
{
    srand ( (unsigned)time(NULL) );
    int rnd = rand() % 10;

    advertisement* advert = (advertisement*)Advert;
    if (advert->qant == 0 ||
        GetKeyState(VK_CAPITAL) ||
        advert->running == 1 ||
        rnd <= 4)
        return;

    BUTTON_ yes = { YES_BUTTON };
    BUTTON_ no  = { NO_BUTTON };
    BUTTON_ close = { CLOSE_BUTTON };
    BUTTON_ Back = { BACK_BUTTON };
    BUTTON_ restart = { RESTART_BUTTON };
    BUTTON_ skip  = { SKIP_BUTTON };

    advert->running = 1;
    char* ad_name = (char*)calloc(strlen(advert->banners[advert->ptr]) + strlen("advert\\") + 1, sizeof(char));
    strcpy(ad_name, "advert\\");
    strcat(ad_name, advert->banners[advert->ptr]);

    HDC adv = txLoadImage (ad_name);

    free(ad_name);

    HDC save = txCreateCompatibleDC (1520, 780);

    int x = 100, y = 300;
    int m = min(x, y);
    x -= m; y -= m;

    int t = 1;
    int dy = 5;
    
    txBitBlt (save, 0, 0, 1520, 780, txDC(), 0, 0);
    txBegin();
    while (1)
    {   
        txBitBlt (txDC(), x + (t%5-1) * 3, y + (t%3-1) * 5, 0, 0, adv, 0, 0);
        txRedrawWindow ();

        txSleep(30); 
        txBitBlt (txDC(), 0, 0, 1520, 780, save, 0, 0);

        if (txMouseButtons() == 1)
        {
            // mySleep(100);
            // txBitBlt (save, 0, 0, 1520, 780, txDC(), 0, 0);
            if (mouse_in(&yes) || mouse_in(&no) || mouse_in(&close) || mouse_in(&Back) || mouse_in(&restart) || mouse_in(&skip))
                break;
        }

        y += dy;
        if (y >= 780)
            break;
        t++;
    }
    txEnd();
    advert->running = 0;
    
    txDeleteDC (adv);
    txDeleteDC(save);

    advert->ptr++;
    if (advert->ptr == advert->qant)
        advert->ptr = 0;

    return;
}

void draw_YN_bt ()
{
    BUTTON_ yes = { YES_BUTTON };
    BUTTON_ no  = { NO_BUTTON };

    txSetFillColor (BT_COLOR);

    txSetColor (TX_BLACK);
    txSelectFont ("Times New Roman", 80);

    txRectangle (YES_BUTTON);
    txTextOut (yes.x1 + 60, yes.y1 + 10, "Да");

    txRectangle (NO_BUTTON);
    txTextOut (no.x1 + 50, no.y1 + 10, "Нет");
}

void draw_AKIN_bt ()
{
    BUTTON_ skip = { SKIP_BUTTON };

    txSetFillColor (BT_COLOR);

    txSetColor (TX_BLACK);
    txSelectFont ("Times New Roman", 80);

    txRectangle (SKIP_BUTTON);
    txTextOut (skip.x1 + 30, skip.y1 + 5, "Не знаю");

    draw_back_bt();
}

void draw_back_bt ()
{
    BUTTON_ Back = { BACK_BUTTON };
    BUTTON_ restart = { RESTART_BUTTON };

    txSetFillColor (BT_COLOR);

    txSetColor (TX_BLACK);
    txSelectFont ("Times New Roman", 80);

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

    txSetFillColor (BT_COLOR);

    txSetColor (TX_BLACK);
    txSelectFont ("Times New Roman", 60);

    txRectangle (GUESS_BUTTON);
    txTextOut (guess.x1 + 10, guess.y1 + 15, "Угадать");

    txRectangle (DESCR_BUTTON);
    txTextOut (describe.x1 + 10, describe.y1 + 15, "Описать");

    txRectangle (SHOW_BUTTON);
    txTextOut (show.x1 + 25, show.y1 + 10, "Выдать базу");
}

void draw_ADD_bt ()
{
    BUTTON_ add = { ADD_BUTTON };

    txSetFillColor (BT_COLOR);

    txSetColor (TX_BLACK);
    txSelectFont ("Times New Roman", 60);

    txRectangle (ADD_BUTTON);
    txTextOut (add.x1 + 25, add.y1 + 10, "Добавить");
}

void say (void* data)
{
    txSpeak("%s", (const char*)data);
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
    else if (mode == YNBR)
    {
        draw_YN_bt();
        draw_back_bt();
    }

    txWaveData_t shlepa = txWaveLoadWav ("interface\\shlepa.wav");

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
        else if ((mode == YNDN || mode == YNBR) && mouse_in(&Back))
        {
            while (txMouseButtons() != 0);
            return BACK;
        }
        else if ((mode == YNDN || mode == YNBR) && mouse_in(&restart))
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

answer check_add ()
{
    BUTTON_ add = { ADD_BUTTON };
    BUTTON_ close = { CLOSE_BUTTON };
    BUTTON_ Back = { BACK_BUTTON };
    BUTTON_ restart = { RESTART_BUTTON };

    draw_ADD_bt();
    draw_back_bt();

    while(1)
    {
        while (txMouseButtons() != 1);

        if (mouse_in(&add))
        {
            while (txMouseButtons() != 0);
            return ADD;
        }
        else if (mouse_in(&Back))
        {
            while (txMouseButtons() != 0);
            return BACK;
        }
        else if (mouse_in(&restart))
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

void draw_picture (Node* tree)
{
    if (tree->picture == NULL)
        return;

    char* pict = (char*)calloc(strlen(tree->picture) + strlen("pictures\\"), sizeof(char));

    strcpy(pict, "pictures\\");
    strcat(pict, tree->picture);

    HDC picture = txLoadImage (pict);

    txBitBlt  (txDC(), 1020, 50, 0, 0, picture, 0, 0);

    txDeleteDC (picture);
}

int count_lines (const char* data)
{
    int lines = 1;
    for (size_t i = 0; i < strlen(data); i++)
        if (data[i] == '\n')
            lines++;
    return lines;
}

void put_answer (const char* data, wizard mood, int symb_lim)
{   
    fill_window(mood);
    txSetFillColor (FILD_COLOR);

    int data_len = (int)strlen(data);
    if(symb_lim != 0)
        data_len = symb_lim;

    int wide_coeff = 0;
    if (data_len >= 30)
        wide_coeff = 7;
    else
        wide_coeff = 10 - data_len / 10;

    int line_wide = 90;

    if (symb_lim != 0)
    {
        int lines = count_lines(data);
        line_wide = 60 * lines;
    }
    txRectangle (WINDOW_WIDE / 2 - data_len * wide_coeff, WINDOW_HIGHT / 3, WINDOW_WIDE / 2 + data_len * wide_coeff, WINDOW_HIGHT / 3 + line_wide);
    txFloodFill (WINDOW_WIDE / 2, WINDOW_HIGHT / 3 + 10);

    txSetColor (TX_BLACK);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(WINDOW_WIDE / 2 - data_len * wide_coeff, WINDOW_HIGHT / 3, WINDOW_WIDE / 2 + data_len * wide_coeff, WINDOW_HIGHT / 3 + line_wide, data);
}

void put_question (char* data, wizard mood)
{
    _beginthread (say, 0, data);

    fill_window(mood);
    draw_YN_bt();

    txSetFillColor (FILD_COLOR);

    int data_len = (int)strlen(data);
    int wide_coeff = 0;
    if (data_len < 10)
        wide_coeff = 10;
    else
        wide_coeff = 9;

    txRectangle (WINDOW_WIDE / 2 - data_len * wide_coeff, WINDOW_HIGHT / 3, WINDOW_WIDE / 2 + data_len * wide_coeff, WINDOW_HIGHT / 3 + 90);
    txFloodFill (WINDOW_WIDE / 2, WINDOW_HIGHT / 3 + 10);

    txSetColor (TX_BLACK);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(WINDOW_WIDE / 2 - data_len * wide_coeff, WINDOW_HIGHT / 3, WINDOW_WIDE / 2 + data_len * wide_coeff, WINDOW_HIGHT / 3 + 90, data);
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

    char* try_again = (char*)calloc(
            strlen("Это поле не может быть пустым!\nПоследняя попытка!!!\nДалее игра прервется") + strlen(message), sizeof(char));
    strcpy(try_again, "Это поле не может быть пустым!\n");
    strcat(try_again, message);

    int tryings = 2;

    while (strlen(temp) == 0)
    {
        if (tryings == 0)
            return ERR;
        if (tryings == 1)
            strcat(try_again, "\nПоследняя попытка!!!\nДалее игра прервется");

        temp = txInputBox(try_again, "Помогите мне стать лучше");
        tryings--;
    }

    strncpy(data, temp, data_len - 1);
    return YES;
}
