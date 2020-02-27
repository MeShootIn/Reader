#ifndef TEXTSTRUCT_H_INCLUDED
#define TEXTSTRUCT_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "windowstruct.h"

/*
���������, ���������� ������ �� ���������� � ��������� ���������� � ������
*/

struct TextStruct{
    char * text; // �����
    LL length; // ���-�� ��������
    LL lines; // ���-�� �����
    LL maxLen; // ������������ ����� ������

    LL * start; // ������ �������� ����� �����
    LL start_size; // ������ ������� start
};

void initText(TextStruct * ts, WindowStruct * ws);
LL getLength(TextStruct ts, int i);

#endif