
// tui_menu.h — Menüsáv és legördülő menük az egyszerű TUI kerethez
#ifndef TUI_MENU_H
#define TUI_MENU_H

#include "tui.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *label; // megjelenített szöveg
    char        key;   // gyorsbillentyű (nagybetű); 0 = nincs
    int         id;    // visszatérési kód
} TuiMenuItem;

typedef struct {
    const char *title;     // menü neve a menüsávban
    TuiMenuItem *items;    // tételek
    int          count;    // tételek száma
} TuiMenu;

// Menüsáv kirajzolása és fókusz kezelése; Enterre megnyitja az aktuális menüt
// Visszatér: -1 = ESC főszinten; egyébként 0 folytatásra
int tui_menubar_loop(const char **headers, int hno, int *inout_index, TuiMenu *menus, int menu_count);

// Egyetlen legördülő menü futtatása (kurzorral/gyorsbillentyűvel) egy téglalapban
// Visszatér: kiválasztott TuiMenuItem.id; vagy -1 (ESC)
int tui_menu_run(TuiMenu *menu, int start_x, int start_y);

#ifdef __cplusplus
}
#endif

#endif // TUI_MENU_H
