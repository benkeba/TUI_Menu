
# Egyszerű, modern TUI keret (Windows Console API backend)

Ez a projekt a korábbi, PDF-ből kinyert menükód helyett egy **kompakt, modern TUI**-t ad:
- `tui.h` — backend-agnosztikus API (rajzolás, bemenet, doboz, státusz)
- `tui_win32.c` — Windows Console API-alapú backend
- `tui_menu.h/.c` — menüsáv + legördülő menük (ncurses-szerű viselkedés)
- `app.c` — demo alkalmazás (File/Data/Plot menü)

## Build (GitHub Actions / lokális Windows)

**MSVC (Developer Command Prompt):**
```bat
cl /std:c11 /W4 /EHsc app.c tui_menu.c tui_win32.c /Fe:menu_demo.exe
```

**MinGW-w64 (GCC):**
```bat
gcc -std=c99 -Wall -Wextra -Wpedantic -o menu_demo.exe app.c tui_menu.c tui_win32.c -luser32 -lkernel32
```

**CMake:**
```bat
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Használat
- Nyilak: balra/jobbra — menüsáv választás, fel/le — legördülőben mozgatás
- Enter — kiválasztás
- ESC — kilépés a menüből/főképernyőről

## Testreszabás
- Új menü: `TuiMenu`/`TuiMenuItem` tömbök
- Színek/attribútumok: `TUI_ATTR_*` bitek a kiíró függvényekben
- Rajzolás: `tui_draw_box`, `tui_fill`, `tui_goto`, `tui_puts_attr`

## Megjegyzés
A keret Windows-konfigurációhoz készült; ha cross-platformra szeretnéd (pl. ncurses Linuxon), készíthető egy `tui_ncurses.c` backend ugyanazzal az API-val.
