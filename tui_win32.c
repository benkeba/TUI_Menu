
// tui_win32.c — Windows Console API backend a TUI-hoz
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "tui.h"

static HANDLE hOut = NULL;
static WORD   g_default_attr = 0;
static CONSOLE_SCREEN_BUFFER_INFO g_csbi0;

static WORD map_attr(unsigned attr)
{
    WORD base = g_default_attr;
    WORD fg = base & 0x0F;
    WORD bg = (base >> 4) & 0x0F;
    if (attr & TUI_ATTR_BRIGHT) fg |= FOREGROUND_INTENSITY;
    if (attr & TUI_ATTR_REVERSE) { WORD t = fg; fg = bg; bg = t; }
    return (WORD)((bg << 4) | (fg & 0x0F));
}

void tui_init(void)
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(hOut, &g_csbi0)) {
        g_csbi0.wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        g_csbi0.dwCursorPosition.X = 0; g_csbi0.dwCursorPosition.Y = 0;
    }
    g_default_attr = g_csbi0.wAttributes;
}

void tui_end(void)
{
    if (!hOut) return;
    SetConsoleTextAttribute(hOut, g_default_attr);
    SetConsoleCursorPosition(hOut, g_csbi0.dwCursorPosition);
}

void tui_clear(void)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD home = {0,0}; DWORD count;
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        DWORD cells = (DWORD)csbi.dwSize.X * (DWORD)csbi.dwSize.Y;
        FillConsoleOutputCharacter(hOut, ' ', cells, home, &count);
        FillConsoleOutputAttribute(hOut, g_default_attr, cells, home, &count);
        SetConsoleCursorPosition(hOut, home);
    }
}

void tui_goto(int x, int y)
{
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, c);
}

void tui_puts(const char *s)
{
    fputs(s, stdout);
}

void tui_puts_attr(const char *s, unsigned attr)
{
    WORD a = map_attr(attr);
    SetConsoleTextAttribute(hOut, a);
    fputs(s, stdout);
    SetConsoleTextAttribute(hOut, g_default_attr);
}

void tui_fill(TuiRect r, char ch)
{
    for (int row = 0; row < r.h; ++row) {
        tui_goto(r.x, r.y + row);
        for (int col = 0; col < r.w; ++col) putchar(ch);
    }
}

static void line_h(int x, int y, int w, char ch)
{
    tui_goto(x, y); for (int i=0;i<w;++i) putchar(ch);
}

static void line_v(int x, int y, int h, char ch)
{
    for (int i=0;i<h;++i){ tui_goto(x, y+i); putchar(ch);}    
}

void tui_draw_box(const char *title, TuiRect r, unsigned attr_border, unsigned attr_title)
{
    // ASCII box: + - |
    const char UL = '+'; const char UR = '+'; const char DL = '+'; const char DR = '+';
    const char H  = '-'; const char V  = '|';

    // töröljünk alatta, majd keret
    tui_fill(r, ' ');
    // keret
    tui_puts_attr("", attr_border); // csak attribútum beállítás
    tui_goto(r.x, r.y); putchar(UL);
    line_h(r.x+1, r.y, r.w-2, H);
    tui_goto(r.x + r.w -1, r.y); putchar(UR);
    line_v(r.x, r.y+1, r.h-2, V);
    line_v(r.x + r.w -1, r.y+1, r.h-2, V);
    tui_goto(r.x, r.y + r.h -1); putchar(DL);
    line_h(r.x+1, r.y + r.h -1, r.w-2, H);
    tui_goto(r.x + r.w -1, r.y + r.h -1); putchar(DR);

    // title középre
    if (title && *title) {
        int len = (int)strlen(title);
        int inner = r.w - 2;
        int left = (inner - len)/2;
        tui_goto(r.x+1+left, r.y);
        tui_puts_attr(title, attr_title);
    }
    // vissza alap attribútumra
    SetConsoleTextAttribute(hOut, g_default_attr);
}

void tui_size(int *out_w, int *out_h)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
        if (out_w) *out_w = csbi.dwSize.X; if (out_h) *out_h = csbi.dwSize.Y;
    } else { if (out_w) *out_w = 80; if (out_h) *out_h = 25; }
}

int tui_getkey(void)
{
    int c = _getch();
    if (c == 0 || c == 224) {
        int c2 = _getch();
        switch (c2) {
            case 71: return TUI_KEY_HOME;  // Home
            case 79: return TUI_KEY_END;   // End
            case 72: return TUI_KEY_UP;    // Up
            case 80: return TUI_KEY_DOWN;  // Down
            case 75: return TUI_KEY_LEFT;  // Left
            case 77: return TUI_KEY_RIGHT; // Right
            case 59: return TUI_KEY_F1;    // F1
            default: return 10000 + c2;    // egyéb
        }
    }
    if (c == 13) return TUI_KEY_CR;
    if (c == 27) return TUI_KEY_ESC;
    return c;
}

void tui_sleep_ms(unsigned ms)
{
    Sleep(ms);
}
