#include "../../fleaux/headers/plugin_models/editor.h"

#ifndef _SDNB_EDITOR_H_
#define _SDNB_EDITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

void init(fl_editor_t *editor, const char *path);

void cleanup(fl_editor_t *editor);

void getData(fl_editor_t *editor, char *data, size_t from, size_t to);
void addChar(fl_editor_t *editor, char ch, size_t index);
void addCharXY(fl_editor_t *editor, char ch, size_t x, size_t y);
void addStr(fl_editor_t *editor, char *str, size_t index);
void addStrXY(fl_editor_t *editor, char *str, size_t x, size_t y);
void removeChar(fl_editor_t *editor, size_t index);
void removeCharXY(fl_editor_t *editor, size_t x, size_t y);
void removeStr(fl_editor_t *editor, size_t length, size_t index);
void removeStrXY(fl_editor_t *editor, size_t length, size_t x, size_t y);
void moveCursor(fl_editor_t *editor, size_t index);
void moveCursorXY(fl_editor_t *editor, size_t x, size_t y);

#ifdef __cplusplus
}
#endif

#endif
