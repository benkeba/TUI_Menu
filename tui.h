
// tui.h — Egyszerű, modern C99 TUI keretrendszer (backend-agnosztikus API)
// Jelen implementációhoz a tui_win32.c ad Windows Console API hátteret.
#ifndef TUI_H
#define TUI_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Színek és attribútumok (bitmask) ---
// A backend (Win32) ezeket mappolja a konkrét konzol attribútumokra.

enum {
    TUI_ATTR_NONE    = 0u,
    TUI_ATTR_BRIGHT  = 1u << 0,
    TUI_ATTR_REVERSE = 1u << 1,
    TUI_ATTR_UNDERLN = 1u << 2
};

// Egyszerű rajzolási téglalap
typedef struct { int x, y, w, h; } TuiRect;

// Billentyűkódok (ASCII-n kívül)
typedef enum TuiKey {
    TUI_KEY_NONE = 0,
    TUI_KEY_ESC  = 27,
    TUI_KEY_CR   = 13,

    TUI_KEY_UP = 1001,
    TUI_KEY_DOWN,
    TUI_KEY_LEFT,
    TUI_KEY_RIGHT,
    TUI_KEY_HOME,
    TUI_KEY_END,
    TUI_KEY_F1,
    TUI_KEY_ENTER = 1010
} TuiKey;

// --- Alap API ---
// Inicializálás / befejezés
void tui_init(void);
void tui_end(void);

// Képernyőműveletek
void tui_clear(void);
void tui_goto(int x, int y);
void tui_puts(const char *s);
void tui_puts_attr(const char *s, unsigned attr);
void tui_draw_box(const char *title, TuiRect r, unsigned attr_border, unsigned attr_title);
void tui_fill(TuiRect r, char ch);
void tui_size(int *out_w, int *out_h);

// Bemenet
int  tui_getkey(void);            // ASCII vagy TuiKey érték

// Kényelmi
void tui_sleep_ms(unsigned ms);

#ifdef __cplusplus
}
#endif

#endif // TUI_H
