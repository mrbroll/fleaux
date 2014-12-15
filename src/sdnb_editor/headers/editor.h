#include "../../fleaux/headers/plugin_models/editor.h"

#ifndef _SDNB_EDITOR_H_
#define _SDNB_EDITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

fl_editor_t *sdnb_editor_create(const char *path);

void sdnb_editor_destroy(fl_editor_t *editor);

size_t sdnb_editor_getLength(fl_editor_t *editor);
void sdnb_editor_getData(fl_editor_t *editor, char *data, size_t from, size_t length);
fl_editor_cursor_t sdnb_editor_getCursor(fl_editor_t *editor);
void sdnb_editor_insertAtCursor(fl_editor_t *editor, const char *str, fl_editor_cursor_t cursor);
void sdnb_editor_insertAtIndex(fl_editor_t *editor, const char *str, size_t index);
void sdnb_editor_insertAtXY(fl_editor_t *editor, const char *str, size_t x, size_t y);
void sdnb_editor_removeAtCursor(fl_editor_t *editor, fl_editor_cursor_t cursor, size_t length);
void sdnb_editor_removeAtIndex(fl_editor_t *editor, size_t index, size_t length);
void sdnb_editor_removeAtXY(fl_editor_t *editor, size_t x, size_t y, size_t length);
void sdnb_editor_moveCursorToIndex(fl_editor_t *editor, size_t index);
void sdnb_editor_moveCursorToXY(fl_editor_t *editor, size_t x, size_t y);

#ifdef __cplusplus
}
#endif

#endif
