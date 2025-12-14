#include "color.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static Palette gPalette;
static int gPaletteInit = 0;

static void paletteInit(void) {
  if (gPaletteInit) {
    return;
  }
  gPaletteInit = 1;

  const char *term = getenv("TERM");
  const char *colorterm = getenv("COLORTERM");
  const char *noColor = getenv("NO_COLOR");

  int disable = 0;
  if (!isatty(STDOUT_FILENO) || (term && strcmp(term, "dumb") == 0) ||
      noColor) {
    disable = 1;
  }

  int truecolor = 0;
  int color256 = 0;
  if (colorterm && (strstr(colorterm, "truecolor") || strstr(colorterm, "24bit"))) {
    truecolor = 1;
  } else if (term && strstr(term, "256color")) {
    color256 = 1;
  }

  if (disable) {
    gPalette = (Palette){
        .reset = "",
        .bold = "",
        .primary = "",
        .secondary = "",
        .accent = "",
        .header = "",
        .error = "",
        .warn = "",
        .ok = "",
        .muted = "",
    };
  } else if (truecolor) {
    gPalette = (Palette){
        .reset = "\x1b[0m",
        .bold = "\x1b[1m",
        .primary = "\x1b[38;2;0;190;80m",
        .secondary = "\x1b[38;2;220;0;60m",
        .accent = "\x1b[38;2;240;170;30m",
        .header = "\x1b[38;2;0;220;170m",
        .error = "\x1b[38;2;255;80;80m",
        .warn = "\x1b[38;2;255;200;0m",
        .ok = "\x1b[38;2;0;200;120m",
        .muted = "\x1b[38;2;170;170;170m",
    };
  } else if (color256) {
    gPalette = (Palette){
        .reset = "\x1b[0m",
        .bold = "\x1b[1m",
        .primary = "\x1b[38;5;34m",
        .secondary = "\x1b[38;5;196m",
        .accent = "\x1b[38;5;214m",
        .header = "\x1b[38;5;44m",
        .error = "\x1b[38;5;203m",
        .warn = "\x1b[38;5;220m",
        .ok = "\x1b[38;5;40m",
        .muted = "\x1b[38;5;244m",
    };
  } else {
    gPalette = (Palette){
        .reset = "\x1b[0m",
        .bold = "\x1b[1m",
        .primary = "\x1b[32m",
        .secondary = "\x1b[31m",
        .accent = "\x1b[33m",
        .header = "\x1b[36m",
        .error = "\x1b[31m",
        .warn = "\x1b[33m",
        .ok = "\x1b[32m",
        .muted = "\x1b[37m",
    };
  }
}

const Palette *paletteGet(void) {
  paletteInit();
  return &gPalette;
}
