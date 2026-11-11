#ifndef BANKING_SYSTEM_UI_H
#define BANKING_SYSTEM_UI_H

#include <stdio.h>
#include <stdlib.h>

/* Small, dependency-free ANSI terminal styling helpers. */
#define UI_RESET "\033[0m"
#define UI_BOLD "\033[1m"
#define UI_CYAN "\033[36m"
#define UI_GREEN "\033[32m"
#define UI_RED "\033[31m"
#define UI_YELLOW "\033[33m"
#define UI_BLUE "\033[34m"

static inline const char *uiColor(const char *color) {
    return getenv("NO_COLOR") != NULL ? "" : color;
}

static inline void uiPrintHeader(const char *title) {
    printf("\n%s%s============================================================%s\n",
           uiColor(UI_BOLD), uiColor(UI_CYAN), uiColor(UI_RESET));
    printf("%s%s  %s%s\n", uiColor(UI_BOLD), uiColor(UI_CYAN),
           title, uiColor(UI_RESET));
    printf("%s%s============================================================%s\n",
           uiColor(UI_BOLD), uiColor(UI_CYAN), uiColor(UI_RESET));
}

static inline void uiPrintSuccess(const char *message) {
    printf("%s%s[OK]%s %s\n", uiColor(UI_BOLD), uiColor(UI_GREEN),
           uiColor(UI_RESET), message);
}

static inline void uiPrintError(const char *message) {
    printf("%s%s[ERROR]%s %s\n", uiColor(UI_BOLD), uiColor(UI_RED),
           uiColor(UI_RESET), message);
}

static inline void uiPrintDivider(void) {
    printf("%s------------------------------------------------------------%s\n",
           uiColor(UI_BLUE), uiColor(UI_RESET));
}

#endif
