#ifndef COLOR_H
#define COLOR_H

typedef struct {
  const char *reset;
  const char *bold;
  const char *primary;
  const char *secondary;
  const char *accent;
  const char *header;
  const char *error;
  const char *warn;
  const char *ok;
  const char *muted;
} Palette;

const Palette *paletteGet(void);

#endif
