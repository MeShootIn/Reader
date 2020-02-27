#ifndef SERVICESTRUCT_H_INCLUDED
#define SERVICESTRUCT_H_INCLUDED

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include "textstruct.h"
#include "windowstruct.h"
#include "menu.h"

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

void scrollToLine(HWND hwnd, TextStruct ts, WindowStruct * ws, int linePos);
void showText(TextStruct ts, WindowStruct ws);
int getMap(TextStruct ts, WindowStruct ws, int scrollPos);
int getInverseMap(TextStruct ts, WindowStruct ws, int linePos);
void setCaret(HWND hwnd, WindowStruct ws);
void deleteCaret(HWND hwnd);
void goToCaret(HWND hwnd, TextStruct ts, WindowStruct * ws);
void fileOpen(HWND hwnd, TextStruct * ts, WindowStruct * ws, char * fileName);
char * readFile(char * fileName, TextStruct * ts);
void changeMode(HWND hwnd, TextStruct * ts, WindowStruct * ws, int mode);
void changeModeClick(HWND hwnd, TextStruct * ts, WindowStruct * ws, int mode);
void initStructs(TextStruct * ts, WindowStruct * ws);
void initMode(TextStruct * ts, WindowStruct * ws);
void scrollV(HWND hwnd, TextStruct ts, WindowStruct * ws);
void scrollH(HWND hwnd, WindowStruct * ws);
void goToLine(HWND hwnd, TextStruct ts, WindowStruct * ws, int startPos);
void goToSymbPos(HWND hwnd, TextStruct ts, WindowStruct * ws, int symbPos);
int prevSymbPos(TextStruct ts, WindowStruct ws);
void setClientSize(HWND hwnd, WindowStruct * ws);
void clean(HWND hwnd);
void info(char * str, char * caption);
void error(char * str);
void debug(char * str);
void debugInt(int x);
void debugChar(char ch);

#endif
