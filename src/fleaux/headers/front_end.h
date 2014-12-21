#include "plugin_models/editor.h"

#ifndef _FL_FRONT_END_H_
#define _FL_FRONT_END_H_

#ifdef __cplusplus
extern "C"
{
#endif

void fl_frontEnd_insert(fl_editor_t *editor, fl_editor_cursor_t pos, const char *str, fl_editor_cursor_t curs);
void fl_frontEnd_remove(fl_editor_t *editor, fl_editor_cursor_t pos, size_t length, fl_editor_cursor_t curs);
void fl_frontEnd_moveCursor(fl_editor_t *editor, fl_editor_cursor_t pos);

#ifdef s
}
#endif

#endif
