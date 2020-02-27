#ifndef WINDOWSTRUCT_H_INCLUDED
#define WINDOWSTRUCT_H_INCLUDED

#define LAYOUT 1
#define NON_LAYOUT 2

typedef long long LL;

/*
Структура параметров окна
*/

struct WindowStruct{
    HDC hdc; // Дескриптор контекста устройства
    PAINTSTRUCT ps; // Дескриптор устройства отображения
    TEXTMETRIC tm; // Дексриптор параметров шрифта

    int mode; // Режим отображения

    int cxChar, cyChar; // Размеры одной буквы по x и y
    int cxClient, cyClient; // Размеры рабочей области по x и y

    int iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax, iVscrollInc, iHscrollInc; // Координаты ползунков

    int linePos, lineInc;

    int hCaretPos, vCaretPos, prevPos;
};

#endif
