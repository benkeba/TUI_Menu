// tui_menu.c — Egyszerű menüsáv + legördülő menük megvalósítása a TUI-hoz
#include <string.h>
#include <ctype.h>
#include <stdio.h>      // sprintf miatt
#include "tui_menu.h"

static void draw_status_bar(const char **headers, int hno, int idx)
{
    int W,H; tui_size(&W,&H);
    // státuszsor (2. sor) — inverz háttér
    TuiRect r = (TuiRect){0,1,W,1};
    tui_fill(r, ' ');
    int posinc = (W-2) / hno; if (posinc <= 0) posinc = 1;
    int xp0 = 1 + posinc/2; // középre helyezés

    // rajzolás
    for (int j=0;j<hno;++j) {
        tui_goto(xp0 + j*posinc, 1);
        if (j==idx) tui_puts_attr(headers[j], TUI_ATTR_NONE);
        else        tui_puts_attr(headers[j], TUI_ATTR_REVERSE);
    }
}

int tui_menu_run(TuiMenu *menu, int start_x, int start_y)
{
    // popup keret számítása
    int maxlen = 0; 
    for (int i=0;i<menu->count;++i){
        int L=(int)strlen(menu->items[i].label); 
        if(L>maxlen)maxlen=L; 
    }
    TuiRect r = (TuiRect){ start_x, start_y, maxlen + 4, menu->count + 2 };
    tui_draw_box(menu->title, r, TUI_ATTR_NONE, TUI_ATTR_BRIGHT|TUI_ATTR_REVERSE);
    int list_x = r.x + 2, list_y = r.y + 1;

    int j = 0; // kiválasztott index

    for (;;) {
        // lista
        for (int i=0;i<menu->count;++i) {
            tui_goto(list_x, list_y + i);
            if (i==j) tui_puts_attr(menu->items[i].label, TUI_ATTR_BRIGHT);
            else      tui_puts_attr(menu->items[i].label, TUI_ATTR_NONE);
        }
        int key = tui_getkey();
        switch (key) {
            case TUI_KEY_UP:   j = (j>0)? j-1 : menu->count-1; break;
            case TUI_KEY_DOWN: j = (j<menu->count-1)? j+1 : 0; break;
            case TUI_KEY_ESC:  return -1;
            case TUI_KEY_CR:   // Enter
            case TUI_KEY_ENTER:
                return menu->items[j].id;
            default:
                if (key>=0 && key<128) {
                    int K = toupper(key);
                    for (int i=0;i<menu->count;++i) {
                        if (menu->items[i].key && toupper(menu->items[i].key)==K)
                            return menu->items[i].id;
                    }
                }
        }
    }
}

int tui_menubar_loop(const char **headers, int hno, int *inout_index, TuiMenu *menus, int menu_count)
{
    int idx = *inout_index;
    int W,H; tui_size(&W,&H);

    // főkeret
    tui_draw_box(" Highly Portable Menu System ", (TuiRect){0,0,W,H-1}, TUI_ATTR_NONE, TUI_ATTR_BRIGHT|TUI_ATTR_REVERSE);
    draw_status_bar(headers, hno, idx);

    for (;;) {
        int key = tui_getkey();
        switch (key) {
            case TUI_KEY_LEFT:  
                idx = (idx>0)? idx-1 : hno-1; 
                draw_status_bar(headers, hno, idx); 
                break;
            case TUI_KEY_RIGHT: 
                idx = (idx<hno-1)? idx+1 : 0; 
                draw_status_bar(headers, hno, idx); 
                break;
            case TUI_KEY_ESC:   
                *inout_index = idx; 
                return -1; // kilépés
            case TUI_KEY_CR:
            case TUI_KEY_ENTER:
            {
                // almenü felkutatása
                if (idx < menu_count) {
                    // popup pozíció: a menüsáv alatt
                    int posinc = (W-2) / hno; if (posinc<=0) posinc=1;
                    int xp0 = 1 + posinc/2;
                    int start_x = xp0 + idx*posinc - 2; if (start_x<1) start_x=1; 
                    int ret = tui_menu_run(&menus[idx], start_x, 2);
                    // visszajelzés dummy: itt az alkalmazás kezelné a menü ID-ket
                    if (ret >= 0) {
                        // kis üzenet
                        tui_goto(2, H-2);
                        char msg[128];
                        // snprintf -> sprintf (MSVC link fix)
                        sprintf(msg, "[menu id=%d] Press any key...", ret);
                        tui_puts_attr(msg, TUI_ATTR_BRIGHT);
                        (void)tui_getkey();
                        // képernyő frissítése
                        tui_draw_box(" Highly Portable Menu System ", (TuiRect){0,0,W,H-1}, TUI_ATTR_NONE, TUI_ATTR_BRIGHT|TUI_ATTR_REVERSE);
                        draw_status_bar(headers, hno, idx);
                    }
                }
            }
            break;
            default:
                if (key>=0 && key<128) {
                    int K = toupper(key);
                    // kezdőbetű választás
                    for (int j=0;j<hno;++j) {
                        if (headers[j] && toupper(headers[j][0]) == K) { 
                            idx = j; 
                            draw_status_bar(headers,hno,idx); 
                            break; 
                        }
                    }
                }
        }
    }
}
