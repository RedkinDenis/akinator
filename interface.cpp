#define  TX_USE_SPEAK
#define _TX_SKIP_EXCEPTION EXCEPTION_CPP_MSC
#include "..\TXLib.h"

#include "interface.h"

#include "TXWave.h"
#include <time.h>
//#include "input_output.h"

#define YES_BUTTON txGetExtentX() / 2 - 250, txGetExtentY() * 6 / 10, txGetExtentX() / 2 - 50, txGetExtentY() * 6 / 10 + 100

#define NO_BUTTON txGetExtentX() / 2 + 50, txGetExtentY() * 6 / 10, txGetExtentX() / 2 + 250, txGetExtentY() * 6 / 10 + 100

#define SKIP_BUTTON txGetExtentX() / 2 - 150, txGetExtentY() * 3 / 4, txGetExtentX() / 2 + 150, txGetExtentY() * 3 / 4 + 80

#define GUESS_BUTTON txGetExtentX() / 2 - 250, txGetExtentY() * 7 / 10, txGetExtentX() / 2 - 50, txGetExtentY() * 7 / 10 + 100

#define DESCR_BUTTON txGetExtentX() / 2 + 50, txGetExtentY() * 7 / 10, txGetExtentX() / 2 + 250, txGetExtentY() * 7 / 10 + 100

#define SHOW_BUTTON txGetExtentX() / 2 - 150, txGetExtentY() / 2, txGetExtentX() / 2 + 150, txGetExtentY() / 2 + 80

#define CLOSE_BUTTON txGetExtentX() - 50, 0, txGetExtentX(), 37

#define BACK_BUTTON txGetExtentX() - 300, 450, txGetExtentX() - 100, 550

#define RESTART_BUTTON txGetExtentX() - 300, 600, txGetExtentX() - 100, 700

#define ADD_BUTTON txGetExtentX() / 2 - 150, txGetExtentY() * 3 / 4, txGetExtentX() / 2 + 150, txGetExtentY() * 3 / 4 + 80

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
    HDC background = txLoadImage ("interface\\background.bmp");
    if (!background)
        printf("error in open of background");

    txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), background, 0, 0);
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
    advertisement* advert = (advertisement*)Advert;
    srand ( (unsigned)time(NULL) );

    if (GetKeyState(VK_CAPITAL))
        return;

    int rnd = rand() % 2;
    // if (rnd == 1)
    //     return;

    char* ad_name = (char*)calloc(strlen(advert->banners[advert->ptr].str) + strlen("advert\\") + 1, sizeof(char));
    strcpy(ad_name, "advert\\");
    strcat(ad_name, advert->banners[advert->ptr].str);

    HDC adv = txLoadImage (ad_name);

    free(ad_name);

    HDC save = txCreateCompatibleDC (1520, 780);

    int x = rand() % (1520 - txGetExtentX(adv)) + 5, y = rand() % (780 - txGetExtentY(adv)) + 5;
    int m = min(x, y);
    x -= m; y -= m;

    int dx = rand() % 4 + 1, dy = rand() % 4 + 1;
    
    txBitBlt (save, 0, 0, 1520, 780, txDC(), 0, 0);
    txBegin();
    while (1)
    {   
        txBitBlt (txDC(), x, y, 0, 0, adv, 0, 0);
        txRedrawWindow ();

        if (txMouseButtons() == 1)
            break;

        txSleep(10); 
        txBitBlt (txDC(), 0, 0, 1520, 780, save, 0, 0);

        x += dx;    y += dy;
        if (x >= 1520 - txGetExtentX(adv) || x < 0)
            dx = -dx;
        if (y >= 780 - txGetExtentY(adv) || y < 0)
            dy = -dy;
    }
    txEnd();
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

void put_answer (const char* data, wizard mood, int symb_lim)
{   
    fill_window(mood);
    //txClear();
    txSetFillColor (FILD_COLOR);

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

    txSetColor (TX_BLACK);
    txSelectFont ("Comic Sans MS", 40);
    txDrawText(txGetExtentX() / 2 - data_len * wide_coeff, txGetExtentY() / 3, txGetExtentX() / 2 + data_len * wide_coeff, txGetExtentY() / 3 + 90, data);
}

void put_question (char* data, wizard mood)
{
    //_beginthread (say, 0, data);

    fill_window(mood);
    // printf("%llu \n", sizeof(RGBQUAD) * 1520 * 780);
    //txClear();
    draw_YN_bt();

    txSetFillColor (FILD_COLOR);

    int data_len = (int)strlen(data);
    int wide_coeff = 0;
    if (data_len < 10)
        wide_coeff = 10;
    else
        wide_coeff = 9;

    txRectangle (txGetExtentX() / 2 - data_len * wide_coeff, txGetExtentY() / 3, txGetExtentX() / 2 + data_len * wide_coeff, txGetExtentY() / 3 + 90);
    txFloodFill (txGetExtentX() / 2, txGetExtentY() / 3 + 10);

    txSetColor (TX_BLACK);
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

    char* try_again = (char*)calloc(
            strlen("абра кадабра!\n  \nабра кадабра!!!\nабра кадабра") + strlen(message), sizeof(char));
    strcpy(try_again, "абра кадабра!\n");
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
