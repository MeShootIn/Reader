#ifndef WINDOWSTRUCT_H_INCLUDED
#define WINDOWSTRUCT_H_INCLUDED

#define LAYOUT 1
#define NON_LAYOUT 2

typedef long long LL;

/*
��������� ���������� ����
*/

struct WindowStruct{
    HDC hdc; // ���������� ��������� ����������
    PAINTSTRUCT ps; // ���������� ���������� �����������
    TEXTMETRIC tm; // ���������� ���������� ������

    int mode; // ����� �����������

    int cxChar, cyChar; // ������� ����� ����� �� x � y
    int cxClient, cyClient; // ������� ������� ������� �� x � y

    int iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax, iVscrollInc, iHscrollInc; // ���������� ���������

    int linePos, lineInc;

    int hCaretPos, vCaretPos, prevPos;
};

#endif
