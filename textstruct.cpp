#include "textstruct.h"
#include "servicestruct.h"

// Инициализация структуры текста
void initText(TextStruct * ts, WindowStruct * ws){
    ts->lines = 1;
    LL xCharCount = ws->cxClient / ws->cxChar;
    LL curCharCount = 0;
    bool is_end = false;

    // Подсчёт кол-ва строк
    for(LL i = 0; i < ts->length; ++i){
        if(ts->text[i] == '\n' ||
           (ws->mode == LAYOUT && curCharCount >= xCharCount - 1)){
            is_end = true;
            curCharCount = 0;
        }
        else{
            curCharCount++;

            if(is_end){
                is_end = false;
                ts->lines++;
            }
        }
    }

    ts->start_size = ts->lines + 1;
    ts->start = (LL *) malloc(ts->start_size * sizeof(LL));
    ts->start[0] = 0;
    ts->start[ts->start_size - 1] = ts->length;

    LL line = 1;
    is_end = false;
    // Инициализация массива указателей на начала строк
    for(LL i = 0; i < ts->length; ++i){
        if(ts->text[i] == '\n' ||
           (ws->mode == LAYOUT && curCharCount >= xCharCount - 1)){
            is_end = true;
            curCharCount = 0;
        }
        else{
            curCharCount++;

            if(is_end){
                is_end = false;
                ts->start[line] = i;
                line++;
            }
        }
    }

    // Подсчёт максимальной длины строки
    for(LL i = 0; i < ts->lines; ++i){
        ts->maxLen = max(ts->maxLen, getLength(*ts, i));
    }
}

// Длина строки
LL getLength(TextStruct ts, int i){
    int len = ts.start[i + 1] - ts.start[i];
    if(ts.text[ts.start[i + 1] - 2] == '\r'){
        len -= 2;
    }

    return len;
}
