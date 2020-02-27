#include "menu.h"

// Пометить "галочкой" пункт меню
void checkMenu(HWND hwnd, int item){
    HMENU hMenu = GetMenu(hwnd);

    switch(item){
        case LAYOUT:{
            CheckMenuItem(hMenu, IDM_LAYOUT, MF_CHECKED);
            CheckMenuItem(hMenu, IDM_NON_LAYOUT, MF_UNCHECKED);

            break;
        }

        case NON_LAYOUT:{
            CheckMenuItem(hMenu, IDM_NON_LAYOUT, MF_CHECKED);
            CheckMenuItem(hMenu, IDM_LAYOUT, MF_UNCHECKED);

            break;
        }

        default:{
            CheckMenuItem(hMenu, item, MF_CHECKED);
        }
    }

    SendMessage(hwnd, WM_SIZE, 0L, 0L);
}

// Сделать недоступным определённый пункт меню
void disableMenu(HWND hwnd, int item){
    HMENU hMenu = GetMenu(hwnd);

    switch(item){
        case IDM_OPEN:{
            EnableMenuItem(hMenu, IDM_OPEN, MF_DISABLED);
            EnableMenuItem(hMenu, IDM_CLOSE, MF_ENABLED);

            break;
        }

        case IDM_CLOSE:{
            EnableMenuItem(hMenu, IDM_OPEN, MF_ENABLED);
            EnableMenuItem(hMenu, IDM_CLOSE, MF_DISABLED);

            break;
        }

        default:{
            EnableMenuItem(hMenu, item, MF_DISABLED);
        }
    }

    SendMessage(hwnd, WM_SIZE, 0L, 0L);
}
