#include "servicestruct.h"
#include <limits.h>



/*
????????? ????????? ??????
?????????????? ?????? ?? ??????, ??????? ????? ? ???????? ??????? ???????
*/

void showText(TextStruct ts, WindowStruct ws) {
  LL  y, pos, len, lastLine;
  int xCharCount = ws.cxClient / ws.cxChar,
  yCharCount = ws.cyClient / ws.cyChar;

  SelectObject(ws.hdc, GetStockObject(SYSTEM_FIXED_FONT));
  GetTextMetrics(ws.hdc, &ws.tm);

  lastLine = min(ws.linePos + yCharCount + 1, ts.lines);

  for (int i = ws.linePos; i < lastLine; ++i) {
    pos = ts.start[i] + ws.iHscrollPos;
    len = min(xCharCount + 1, max(getLength(ts, i) - ws.iHscrollPos, 0));
    y = ws.cyChar * (-ws.linePos + i);

    TextOut(ws.hdc, 0, y, ts.text + pos, len);
  }
}

// ???????? ???????????: ??????? ??????? -> ????????? ??????
int getMap(TextStruct ts, WindowStruct ws, int scrollPos) {
  return ( (scrollPos * (ts.lines - ws.cyClient / ws.cyChar)) / ws.iVscrollMax )
  + ( ((
  (scrollPos * (ts.lines - ws.cyClient / ws.cyChar)) % ws.iVscrollMax
) > 0) ? 1 : 0 );
}

// ???????? ???????????: ?????? -> ??????? ???????
int getInverseMap(TextStruct ts, WindowStruct ws, int linePos) {
  return ((LL) linePos * (LL) ws.iVscrollMax) /
  ((LL) ts.lines - ws.cyClient / ws.cyChar);
}

// ???????? ???????
void setCaret(HWND hwnd, WindowStruct ws) {
  CreateCaret(hwnd, NULL, ws.cxChar, ws.cyChar);
  SetCaretPos(ws.hCaretPos * ws.cxChar, ws.vCaretPos * ws.cyChar);
  ShowCaret(hwnd);
}

// ?????? ???????
void deleteCaret(HWND hwnd) {
  HideCaret(hwnd);
  DestroyCaret();
}

// ???????? ?? ???????
void goToCaret(HWND hwnd, TextStruct ts, WindowStruct * ws) {
  LL xCharCount = ws->cxClient / ws->cxChar;
  LL yCharCount = ws->cyClient / ws->cyChar;

  int symbPos = prevSymbPos(ts, *ws);

  // ???? ??????? ?? ????????? ??????? ??????? ?? ???????????
  if (!(0 <= ws->hCaretPos && ws->hCaretPos <= xCharCount)) {
    if (xCharCount < ws->hCaretPos) {
      ws->iHscrollInc = ws->hCaretPos - xCharCount + 1;
    } else {
      ws->iHscrollInc = ws->hCaretPos;
    }

    scrollH(hwnd, ws);
    ws->hCaretPos -= ws->iHscrollInc;
  }

  // ???? ??????? ?? ????????? ??????? ??????? ?? ?????????
  if (!(0 <= ws->vCaretPos && ws->vCaretPos <= yCharCount)) {
    if (yCharCount < ws->vCaretPos) {
      scrollToLine(hwnd, ts, ws, ws->linePos + ws->vCaretPos - yCharCount + 1);
    } else {
      scrollToLine(hwnd, ts, ws, ws->linePos + ws->vCaretPos);
    }

    ws->vCaretPos -= ws->lineInc;
  }
}

// ???????? ????? ? ????????????? ????????
void fileOpen(HWND hwnd, TextStruct * ts, WindowStruct * ws, char * fileName) {
  initStructs(ts, ws);

  char * buf = readFile(fileName, ts);
  if (buf == NULL) {
    return;
  }

  ts->text = buf;
  initText(ts, ws);

  disableMenu(hwnd, IDM_OPEN);
}

// ???????, ???????????? ????? ?? ?????
char * readFile(char * fileName, TextStruct * ts) {
  char * buf = NULL;
  FILE * file = fopen(fileName, "rb");

  if (file != NULL) {
    fseek(file, 0, SEEK_END);
    LL sz = ftell(file);
    // + 2 - ????????? '\r' ? '\n' ? ????? ????????? ??????
    ts->length = sz + 2;
    fseek(file, 0, SEEK_SET);

    buf = (char *) calloc(sz + 2, sizeof(char));

    if (buf != NULL) {
      fread(buf, sizeof(char), sz, file);
      buf[sz] = '\r';
      buf[sz + 1] = '\n';
    } else {
      error("Cannot allocate buffer!");
    }
  } else {
    error("Cannot open file!");
  }

  fclose(file);

  return buf;
}

// ??????? ?????? ??? ??? ?????????
void initStructs(TextStruct * ts, WindowStruct * ws) {
  if (ts->text != NULL) {
    free(ts->text);
    ts->text = NULL;
  }

  ws->linePos = 0;
  ws->lineInc = 0;

  ws->hCaretPos = 0;
  ws->vCaretPos = 0;
  ws->prevPos = 0;

  ts->length = 0;
  initMode(ts, ws);
}

// ??????? ?????? ??? ????????? ??? ????? ??????
void initMode(TextStruct * ts, WindowStruct * ws) {
  if (ts->start != NULL) {
    free(ts->start);
    ts->start = NULL;
  }

  ts->lines = 0;
  ts->maxLen = 0;
  ts->start_size = 0;
}

// ????? ?????? ???????????
void changeMode(HWND hwnd, TextStruct * ts, WindowStruct * ws, int mode) {
  ws->mode = mode;

  if (ts->text != NULL) {
    initMode(ts, ws);
    initText(ts, ws);
  }

  checkMenu(hwnd, mode);
}

// ??????? ?? ???? ?? ??????? ???? ??? ????? ??????
void changeModeClick(HWND hwnd, TextStruct * ts, WindowStruct * ws, int mode) {
  if (ws->mode == mode) {
    return;
  }

  int startPos;
  int symbPos;

  if (ts->text != NULL) {
    startPos = ts->start[ws->linePos];
    symbPos = prevSymbPos(*ts, *ws);
  }

  changeMode(hwnd, ts, ws, mode);

  if (ts->text != NULL) {
    goToLine(hwnd, *ts, ws, startPos);
    goToSymbPos(hwnd, *ts, ws, symbPos);
  }
}

// ???? ?????? ? ???????? ?? ???
void goToLine(HWND hwnd, TextStruct ts, WindowStruct * ws, int startPos) {
  int pos = 0;
  while(ts.start[pos] < startPos) {
    ++pos;
  }
  if (ts.start[pos] > startPos) {
    --pos;
  }

  scrollToLine(hwnd, ts, ws, pos);
}

// ?????? ??????? ?? ???????????? ???????
void goToSymbPos(HWND hwnd, TextStruct ts, WindowStruct * ws, int symbPos) {
  for (int i = 0; i < ts.start_size; ++i) {
    for (int j = 0; j < getLength(ts, i) + 1; ++j) {
      if (ts.start[i] + j == symbPos) {
        ws->hCaretPos = j - ws->iHscrollPos;
        ws->vCaretPos = i - ws->linePos;

        ws->prevPos = prevSymbPos(ts, *ws);
        SetCaretPos(ws->hCaretPos * ws->cxChar, ws->vCaretPos * ws->cyChar);

        return;
      }
    }
  }
}

// ???????????? ??????? ?? ???????????? ??????
void scrollToLine(HWND hwnd, TextStruct ts, WindowStruct * ws, int linePos) {
  int prevLinePos = ws->linePos;
  ws->linePos = linePos;
  ws->lineInc = ws->linePos - prevLinePos;

  ws->iVscrollPos = getInverseMap(ts, *ws, ws->linePos);

  ScrollWindow(hwnd, 0, -ws->lineInc * ws->cyChar, NULL, NULL);
  SetScrollPos(hwnd, SB_VERT, ws->iVscrollPos, TRUE);

  UpdateWindow(hwnd);
}

// ???????????? ?????????
void scrollV(HWND hwnd, TextStruct ts, WindowStruct * ws) {
  ws->iVscrollInc = max(-ws->iVscrollPos, min(
                          ws->iVscrollInc, ws->iVscrollMax - ws->iVscrollPos
                        ));

  if (ws->iVscrollInc != 0) {
    ws->iVscrollPos += ws->iVscrollInc;

    ws->lineInc = ws->linePos;
    ws->linePos = getMap(ts, *ws, ws->iVscrollPos);
    ws->lineInc = ws->linePos - ws->lineInc;

    ScrollWindow(hwnd, 0, -ws->lineInc * ws->cyChar, NULL, NULL);
    SetScrollPos(hwnd, SB_VERT, ws->iVscrollPos, TRUE);

    UpdateWindow(hwnd);
  }
}

// ?????????????? ?????????
void scrollH(HWND hwnd, WindowStruct * ws) {
  ws->iHscrollInc = max(-ws->iHscrollPos, min(
                          ws->iHscrollInc, ws->iHscrollMax - ws->iHscrollPos
                        ));

  if (ws->iHscrollInc != 0) {
    ws->iHscrollPos += ws->iHscrollInc;

    ScrollWindow(hwnd, -ws->iHscrollInc * ws->cxChar, 0, NULL, NULL);
    SetScrollPos(hwnd, SB_HORZ, ws->iHscrollPos, TRUE);

    UpdateWindow(hwnd);
  }
}

int prevSymbPos(TextStruct ts, WindowStruct ws) {
  return ts.start[ws.linePos + ws.vCaretPos] + ws.iHscrollPos + ws.hCaretPos;
}

void setClientSize(HWND hwnd, WindowStruct * ws) {
  RECT rect;
  GetClientRect(hwnd, &rect);

  ws->cxClient = rect.right - rect.left;
  ws->cyClient = rect.bottom - rect.top;
}

void clean(HWND hwnd) {
  InvalidateRect(hwnd, NULL, TRUE);
  UpdateWindow(hwnd);
}

void info(char * str, char * caption) {
  MessageBox(NULL, str, caption, MB_ICONINFORMATION);
}

void error(char * str) {
  MessageBox(NULL, str, "Error", MB_ICONERROR);
}

void debug(char * str) {
  HANDLE Output = GetStdHandle(STD_OUTPUT_HANDLE);
  unsigned long num;
  WriteConsole(Output, str, strlen(str), &num, NULL);
}

void debugInt(int x) {
  char integer[100];
  itoa(x, integer, 10);
  debug(integer);
}

void debugChar(char ch) {
  char s[2] = {ch, '\0'};
  debug(s);
}
