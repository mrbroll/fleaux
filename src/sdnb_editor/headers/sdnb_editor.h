#include "../../fleaux/headers/plugin_models/editor.h"

#ifndef _SDNB_EDITOR_H_
#define _SDNB_EDITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

void sdnb_editor_init(fl_editor_t *editor, const char *path);

void sdnb_editor_cleanup(fl_editor_t *editor);

size_t sdnb_editor_getLength(fl_editor_t *editor);
void sdnb_editor_getData(fl_editor_t *editor, char *data, size_t from, size_t to);
void sdnb_editor_addChar(fl_editor_t *editor, const char ch, size_t index);
void sdnb_editor_addCharXY(fl_editor_t *editor, const char ch, size_t x, size_t y);
void sdnb_editor_addStr(fl_editor_t *editor, const char *str, size_t index);
void sdnb_editor_addStrXY(fl_editor_t *editor, const char *str, size_t x, size_t y);
void sdnb_editor_removeChar(fl_editor_t *editor, size_t index);
void sdnb_editor_removeCharXY(fl_editor_t *editor, size_t x, size_t y);
void sdnb_editor_removeStr(fl_editor_t *editor, size_t length, size_t index);
void sdnb_editor_removeStrXY(fl_editor_t *editor, size_t length, size_t x, size_t y);
void sdnb_editor_moveCursor(fl_editor_t *editor, size_t index);
void sdnb_editor_moveCursorXY(fl_editor_t *editor, size_t x, size_t y);

#ifdef __cplusplus
}
#endif

#endif
