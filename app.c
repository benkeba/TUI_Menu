
// app.c — Demo az egyszerű TUI keretrendszerhez (Windows Console API backend)
// Futtatás: menu_demo.exe (GitHub Actions artefakt)
#include "tui_menu.h"
#include <stdio.h>

// Dummy menüpont ID-k
enum { ID_DIR=101, ID_SHELL, ID_FILE_SUBMENU, ID_EXIT,
       ID_DEF=201, ID_READ, ID_LIST, ID_STAT,
       ID_REG=301, ID_PLOT, ID_BAR,
       ID_SAVE=401, ID_LOAD };

int main(void)
{
    tui_init();

    // Menüsáv fejlécek
    const char *headers[] = { "File", "Data", "Plot" };

    // File menü (a PDF-ben Directory/OS shell/Files/Exit)
    TuiMenuItem file_items[] = {
        {"Directory", 'D', ID_DIR},
        {"OS shell",  'O', ID_SHELL},
        {"Files",     'F', ID_FILE_SUBMENU},
        {"Exit",      'X', ID_EXIT}
    };
    TuiMenu m_file = { "File", file_items, (int)(sizeof file_items/sizeof file_items[0]) };

    // Data menü
    TuiMenuItem data_items[] = {
        {"Default",   'D', ID_DEF},
        {"Read data", 'R', ID_READ},
        {"List data", 'L', ID_LIST},
        {"Statistics",'S', ID_STAT},
    };
    TuiMenu m_data = { "Data", data_items, (int)(sizeof data_items/sizeof data_items[0]) };

    // Plot menü
    TuiMenuItem plot_items[] = {
        {"Regression", 'R', ID_REG},
        {"Plot",       'P', ID_PLOT},
        {"Bar",        'B', ID_BAR},
    };
    TuiMenu m_plot = { "Plot", plot_items, (int)(sizeof plot_items/sizeof plot_items[0]) };

    TuiMenu menus[] = { m_file, m_data, m_plot };
    int idx = 0; // kezdeti kijelölés a menüsávban

    // Fő ciklus: ESC-ig
    while (tui_menubar_loop(headers, 3, &idx, menus, 3) != -1) {
        // itt nem jövünk ki; a loop ESC-re -1-et ad
    }

    tui_end();
    return 0;
}
