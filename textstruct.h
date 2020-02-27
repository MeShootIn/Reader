#ifndef TEXTSTRUCT_H_INCLUDED
#define TEXTSTRUCT_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "windowstruct.h"

/*
Структура, отвечающая только за содержимое и служебную информацию о тексте
*/

struct TextStruct{
    char * text; // Текст
    LL length; // Кол-во символов
    LL lines; // Кол-во строк
    LL maxLen; // Максимальная длина строки

    LL * start; // Массив индексов начал строк
    LL start_size; // Размер массива start
};

void initText(TextStruct * ts, WindowStruct * ws);
LL getLength(TextStruct ts, int i);

#endif
