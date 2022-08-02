#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

// ??????????? ??????????
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

// ?????????? ??????????
#include "textstruct.h"
#include "menu.h"
#include "servicestruct.h"



LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("Reader");

// ????? ?????
int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
  HWND hwnd;
  MSG messages;
  WNDCLASSEX wincl;

  wincl.hInstance = hThisInstance;
  wincl.lpszClassName = szClassName;
  wincl.lpfnWndProc = WindowProcedure;
  wincl.style = CS_HREDRAW | CS_VREDRAW; // ?????????????? ??????????? ????? WM_SIZE
  wincl.cbSize = sizeof (WNDCLASSEX);

  // ????????????? ????????
  wincl.hIcon = LoadIcon(hThisInstance, "MYICON");
  wincl.hIconSm = LoadIcon(hThisInstance, "MYICON");
  wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
  wincl.lpszMenuName = "MYMENU";

  wincl.cbClsExtra = 0;
  wincl.cbWndExtra = 0;

  //wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255)); // ????? ???
  wincl.hbrBackground = CreateSolidBrush(RGB(0, 0, 255));

  if (!RegisterClassEx (&wincl))
    return 0;

  hwnd = CreateWindowEx (
      0,
      szClassName,
      _T("Reader"), // ????????? ????
      WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, // WS_VSCROLL | WS_HSCROLL - ????. ? ?????. ???????
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      640, 480, // ????????? ??????? ????
      HWND_DESKTOP,
      NULL,
      hThisInstance,
      lpszArgument
    );

  ShowWindow (hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while (GetMessage (&messages, NULL, 0, 0))
    {
      TranslateMessage(&messages);
      DispatchMessage(&messages);
    }

  return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  // ?????????
  static TextStruct ts;
  static WindowStruct ws;
  char * fileName;

  switch (message) {
    // ???????? ????
    case WM_CREATE: {
      //AllocConsole();

      ws.hdc = GetDC(hwnd);
      SelectObject(ws.hdc, GetStockObject(SYSTEM_FIXED_FONT));
      // ????????? ?????????? ??????
      GetTextMetrics(ws.hdc, &ws.tm);
      ReleaseDC(hwnd, ws.hdc);

      ws.cxChar = ws.tm.tmAveCharWidth;
      ws.cyChar = ws.tm.tmHeight + ws.tm.tmExternalLeading;

      initStructs(&ts, &ws);
      // ?????????? ????? ??????????? - ??? ???????
      ws.mode = NON_LAYOUT;
      disableMenu(hwnd, IDM_CLOSE);
      checkMenu(hwnd, IDM_NON_LAYOUT);
      /*
???????? ????? ????? ????????? ????????? ??????:
* ???? ???? ?????????? ? ??? ????? ???????, ?? ?????????
* ?????, ??????????? ?????? ??????? ???????
            */
      CREATESTRUCT * p = (CREATESTRUCT *) lParam;
      fileName = (char *) p-> lpCreateParams;

      if (strlen(fileName) > 0) {
        fileOpen(hwnd, &ts, &ws, fileName);

        SendMessage(hwnd, WM_SIZE, wParam, lParam);
      }

      break;
    }

    // ????????? ???????? ????
    case WM_SIZE: {
      // ????????? ???????? ????
      setClientSize(hwnd, &ws);

      /*
            ???? ????? ? ????????, ?? ??? ????????? ???????? ????
            ????? ???????????????????? ????????? ??????
            */
      if (ts.text != NULL && ws.mode == LAYOUT) {
        clean(hwnd);

        initMode(&ts, &ws);
        initText(&ts, &ws);

        goToSymbPos(hwnd, ts, &ws, ws.prevPos);
      }

      // ???????
      ws.iVscrollMax = min(SHRT_MAX, max(ts.lines - ws.cyClient / ws.cyChar, 0));
      ws.iVscrollPos = min(ws.iVscrollPos, ws.iVscrollMax);

      SetScrollRange(hwnd, SB_VERT, 0, ws.iVscrollMax, FALSE);
      SetScrollPos(hwnd, SB_VERT, ws.iVscrollPos, TRUE);

      // ???? ????? ??? ???????, ?? ?????? ???????? ??? ??????????????? ???????
      if (ws.mode == NON_LAYOUT) {
        ws.iHscrollMax = max(0, ts.maxLen - ws.cxClient / ws.cxChar + 1);
        ws.iHscrollPos = min(ws.iHscrollPos, ws.iHscrollMax);
      } else {
        ws.iHscrollMax = 0;
        ws.iHscrollPos = 0;
      }

      SetScrollRange(hwnd, SB_HORZ, 0, ws.iHscrollMax, FALSE);
      SetScrollPos(hwnd, SB_HORZ, ws.iHscrollPos, TRUE);

      // ????? ????? ? ????? ????? ?? ??????? ?? ??????? ??????? ???????
      if (ws.iVscrollMax != 0) {
        scrollToLine(hwnd, ts, &ws, min(ws.linePos, max(0, ts.lines - ws.cyClient / ws.cyChar)));

        goToSymbPos(hwnd, ts, &ws, ws.prevPos);
      }

      if (ts.text != NULL && getLength(ts, ws.linePos + ws.vCaretPos) < ws.iHscrollPos + ws.hCaretPos) {
        SendMessage(hwnd, WM_KEYDOWN, VK_END, 0L);
      }

      goToSymbPos(hwnd, ts, &ws, ws.prevPos);

      break;
    }

    // ???? ???? ? ??????, ?? ?????????? ???????
    case WM_SETFOCUS: {
      if (ts.text != NULL) {
        setCaret(hwnd, ws);
      }

      break;
    }

    // ???? ???? ?? ? ??????, ?? ?????? ???????
    case WM_KILLFOCUS: {
      if (ts.text != NULL) {
        deleteCaret(hwnd);
      }

      break;
    }

    // ????????? ??????? ??????
    case WM_KEYDOWN: {
      if (ts.text == NULL) {
        break;
      }

      // ???? ??????? ?? ????????? ??????? ???????, ?? ???????????? ? ???
      goToCaret(hwnd, ts, &ws);

      switch (wParam) {
        case VK_HOME: {
          SendMessage(hwnd, WM_HSCROLL, SB_TOP, 0L);
          ws.hCaretPos = 0;

          break;
        }

        case VK_END: {
          SendMessage(hwnd, WM_HSCROLL, SB_BOTTOM, 0L);
          ws.hCaretPos = min(getLength(ts, ws.linePos + ws.vCaretPos), ws.cxClient / ws.cxChar - 1);

          break;
        }

        case VK_UP: {
          // ?????????? ?????? ??????? ??????? ???????
          if (ws.vCaretPos == 0) {
            SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
          }

          // ?? ?????? ??????
          if (0 < ws.linePos + ws.vCaretPos) {
            ws.vCaretPos -= 1;

            // ????? ?? ??????? ????? ??????
            if (getLength(ts, ws.linePos + ws.vCaretPos) < ws.iHscrollPos + ws.hCaretPos) {
              SendMessage(hwnd, WM_KEYDOWN, VK_END, 0L);
            }
          }

          break;
        }

        case VK_DOWN: {
          // ?????????? ?????? ??????? ??????? ???????
          if (ws.cyClient / ws.cyChar - 1 <= ws.vCaretPos) {
            SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
          }

          // ?? ????????? ??????
          if (ws.linePos + ws.vCaretPos < ts.lines - 1) {
            ws.vCaretPos += 1;

            // ????? ?? ??????? ????? ??????
            if (getLength(ts, ws.linePos + ws.vCaretPos) < ws.iHscrollPos + ws.hCaretPos) {
              SendMessage(hwnd, WM_KEYDOWN, VK_END, 0L);
            }
          }

          break;
        }

        case VK_LEFT: {
          // ?????????? ????? ??????? ??????? ???????
          if (ws.hCaretPos == 0) {
            SendMessage(hwnd, WM_HSCROLL, SB_LINEUP, 0L);
          }

          // ?? ?????? ??????
          if (0 < ws.hCaretPos) {
            ws.hCaretPos -= 1;
          } else {
            // ????????? 0-? ?????? ??????
            if (0 < ws.linePos + ws.vCaretPos) {
              SendMessage(hwnd, WM_KEYDOWN, VK_UP, 0L);
              SendMessage(hwnd, WM_KEYDOWN, VK_END, 0L);
            }
          }

          break;
        }

        case VK_RIGHT: {
          // ?????????? ?????? ??????? ??????? ???????
          if (ws.cxClient / ws.cxChar - 1 <= ws.hCaretPos) {
            SendMessage(hwnd, WM_HSCROLL, SB_LINEDOWN, 0L);
          }

          // ?? ????? ??????
          if (ws.iHscrollPos + ws.hCaretPos < getLength(ts, ws.linePos + ws.vCaretPos)) {
            ws.hCaretPos += 1;
          } else {
            // ????????? ????????? (??????) ?????? ??????
            if (ws.linePos + ws.vCaretPos < ts.lines - 1) {
              SendMessage(hwnd, WM_KEYDOWN, VK_HOME, 0L);
              SendMessage(hwnd, WM_KEYDOWN, VK_DOWN, 0L);
            }
          }

          break;
        }

        case VK_PRIOR: {
          SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);

          ws.vCaretPos += ws.lineInc;

          break;
        }

        case VK_NEXT: {
          SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);

          ws.vCaretPos += ws.lineInc;

          break;
        }
      }

      if (ts.text != NULL && getLength(ts, ws.linePos + ws.vCaretPos) < ws.iHscrollPos + ws.hCaretPos) {
        SendMessage(hwnd, WM_KEYDOWN, VK_END, 0L);
      }

      ws.prevPos = prevSymbPos(ts, ws);
      SetCaretPos(ws.hCaretPos * ws.cxChar, ws.vCaretPos * ws.cyChar);

      break;
    }

    // ?????????????? ??????
    case WM_HSCROLL: {
      switch (LOWORD(wParam)) {
        case SB_TOP: {
          ws.iHscrollInc = -ws.iHscrollPos;

          break;
        }

        case SB_BOTTOM: {
          ws.iHscrollInc = getLength(ts, ws.linePos + ws.vCaretPos) - ws.iHscrollPos - (ws.cxClient / ws.cxChar - 1);

          break;
        }

        case SB_LINEUP: {
          ws.iHscrollInc = -1;

          if (0 < ws.iHscrollPos) {
            ws.hCaretPos -= ws.iHscrollInc;
          }

          break;
        }

        case SB_LINEDOWN: {
          ws.iHscrollInc = 1;

          if (ws.iHscrollPos < ws.iHscrollMax) {
            ws.hCaretPos -= ws.iHscrollInc;
          }

          break;
        }

        case SB_PAGEUP: {
          ws.iHscrollInc = min(-1, -ws.cxClient / ws.cxChar);

          if (ws.iHscrollPos < ws.cxClient / ws.cxChar) {
            ws.hCaretPos -= ws.iHscrollInc;
          }

          break;
        }

        case SB_PAGEDOWN: {
          ws.iHscrollInc = max(1, ws.cxClient / ws.cxChar);

          if (ws.iHscrollPos + ws.cxClient / ws.cxChar < ws.iHscrollMax) {
            ws.hCaretPos -= ws.iHscrollInc;
          }

          break;
        }

        case SB_THUMBTRACK: {
          ws.iHscrollInc = HIWORD(wParam) - ws.iHscrollPos;
          ws.hCaretPos -= ws.iHscrollInc;

          break;
        }

        default: {
          ws.iHscrollInc = 0;

          break;
        }
      }

      scrollH(hwnd, &ws);
      ws.prevPos = prevSymbPos(ts, ws);

      break;
    }

    // ???????????? ??????
    case WM_VSCROLL: {
      switch (LOWORD(wParam)) {
        case SB_LINEUP: {
          if (0 <= ws.linePos - 1) {
            ws.vCaretPos += 1;

            scrollToLine(hwnd, ts, &ws, ws.linePos - 1);
          }

          break;
        }

        case SB_LINEDOWN: {
          if (ws.linePos < ts.lines - ws.cyClient / ws.cyChar) {
            ws.vCaretPos -= 1;

            scrollToLine(hwnd, ts, &ws, ws.linePos + 1);
          }

          break;
        }

        case SB_PAGEUP: {
          scrollToLine(hwnd, ts, &ws, max(0, ws.linePos - ws.cyClient / ws.cyChar));

          ws.vCaretPos -= ws.lineInc;

          break;
        }

        case SB_PAGEDOWN: {
          scrollToLine(hwnd, ts, &ws, min(max(0, ts.lines - ws.cyClient / ws.cyChar), ws.linePos + ws.cyClient / ws.cyChar));

          ws.vCaretPos -= ws.lineInc;

          break;
        }

        case SB_THUMBTRACK: {
          ws.iVscrollInc = HIWORD(wParam) - ws.iVscrollPos;

          int prevLinePos = ws.linePos;
          scrollV(hwnd, ts, &ws);
          ws.vCaretPos -= ws.linePos - prevLinePos;

          break;
        }
      }

      ws.prevPos = prevSymbPos(ts, ws);

      break;
    }

    // ??????? ?? ?????? ????
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {
        case IDM_OPEN: {
          OPENFILENAME ofn;
          TCHAR szFile[MAX_PATH];

          ZeroMemory(&ofn, sizeof(ofn));
          ofn.lStructSize = sizeof(ofn);
          ofn.hwndOwner = hwnd;
          ofn.lpstrFile = szFile;
          ofn.lpstrFile[0] = '\0';
          ofn.nMaxFile = sizeof(szFile);

          // ?????? .txt ?????
          ofn.lpstrFilter = _T("????????? ????? (*.txt)\0*.txt\0");
          ofn.nFilterIndex = 1;
          ofn.lpstrFileTitle = NULL;
          ofn.nMaxFileTitle = 255;
          ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

          if (GetOpenFileName(&ofn) == TRUE) {
            fileName = ofn.lpstrFile;
            fileOpen(hwnd, &ts, &ws, fileName);

            setCaret(hwnd, ws);

            SendMessage(hwnd, WM_SIZE, 0L, 0L);
          }

          break;
        }

        case IDM_CLOSE: {
          initStructs(&ts, &ws);

          disableMenu(hwnd, IDM_CLOSE);
          deleteCaret(hwnd);

          clean(hwnd);

          break;
        }

        case IDM_QUIT: {
          SendMessage(hwnd, WM_DESTROY, 0L, 0L);

          break;
        }

        case IDM_LAYOUT: {
          changeModeClick(hwnd, &ts, &ws, LAYOUT);

          SendMessage(hwnd, WM_SIZE, 0L, 0L);

          break;
        }

        case IDM_NON_LAYOUT: {
          changeModeClick(hwnd, &ts, &ws, NON_LAYOUT);

          SendMessage(hwnd, WM_SIZE, 0L, 0L);

          break;
        }

        case IDM_ABOUT: {
          info("Author: Dmitrii Mishutin\n? All rights reserved", "About...");

          break;
        }
      }

      break;
    }

    // ????????? ????????? ????
    case WM_MOUSEWHEEL: {
      if (ts.text == NULL) {
        break;
      }

      if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
        SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
      } else {
        SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
      }

      break;
    }

    // ?????????
    case WM_PAINT: {
      if (ts.text == NULL) {
        break;
      }

      ws.hdc = BeginPaint(hwnd, &ws.ps);

      showText(ts, ws);

      EndPaint(hwnd, &ws.ps);

      break;
    }

    // ???????? ????
    case WM_DESTROY: {
      // ??????? ??????, ?????????? ??? ?????????
      initStructs(&ts, &ws);
      deleteCaret(hwnd);

      PostQuitMessage(0);

      break;
    }

    default: {
      return DefWindowProc(hwnd, message, wParam, lParam);
    }
  }

  return 0;
}
