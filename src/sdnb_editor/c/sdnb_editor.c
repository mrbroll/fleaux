#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include "../headers/sdnb_editor.h"
#include "../../fleaux/headers/plugin_models/editor.h"
#include "../../../libsdnbc/src/sdnbc/headers/sdnbc.h"

#define EXPORT __attribute__((visibility("hidden")))
#define BUF_SIZE 1024

typedef struct editor_cursor_s
{
    size_t index;
    size_t x;
    size_t y;
} editor_cursor_t;

typedef struct editor_privates_s
{
    editor_cursor_t _cursor;
    sdnb_gapBuffer_t *_buf;
    uv_rwlock_t _bufLock;
} editor_privates_t;

void initPrivateMembers(void *priv)
{
    editor_privates_t *_private = (editor_privates_t*)priv;
    _private->_cursor.index = 0;
    _private->_cursor.x = 0;
    _private->_cursor.y = 0;
    _private->_buf = sdnb_gapBuffer_create(BUF_SIZE);
    uv_rwlock_init(&(_private->_bufLock));
}

void cleanupPrivateMembers(void *priv)
{
    editor_privates_t *_private = (editor_privates_t*)priv;
    sdnb_gapBuffer_destroy(_private->_buf);
    uv_rwlock_destroy(&(_private->_bufLock));
}

EXPORT
void init(fl_editor_t* editor, const char *path)
{
    size_t pathLength = strlen(path);
    editor->filePath = (char *)malloc(pathLength);
    strcpy(editor->filePath, path);
    editor->visibility = FL_EDITOR_VISIBLE;
    editor->focus = FL_EDITOR_FOCUS;
    editor->_private = malloc(sizeof(editor_privates_t));
    initPrivateMembers(editor->_private);
}

EXPORT
void cleanup(fl_editor_t *editor)
{
    cleanupPrivateMembers(editor->_private);
    free(editor->_private);
    editor->focus = FL_EDITOR_BLUR;
    editor->visibility = FL_EDITOR_HIDDEN;
    free(editor->filePath);
}

EXPORT
void getData(fl_editor_t *editor, char *data, size_t from, size_t to)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;

    uv_rwlock_rdlock(&(_private->_bufLock));
    sdnb_gapBuffer_getData(_private->_buf, data, from, to);
    uv_rwlock_rdunlock(&(_private->_bufLock));
}

EXPORT
void addChar(fl_editor_t *editor, const char ch, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    // move the cursor if necessary
    int diff = (int)(index - _private->_cursor.index);
    uv_rwlock_wrlock(&(_private->_bufLock));
    if (index != _private->_cursor.index) {
        // save the current cursor
        
        //restore the cursor
    }
    //add the char
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void addCharXY(fl_editor_t *editor, const char ch, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    //translate x and y to index
    size_t index; 
    //call index-based version
    addChar(editor, ch, index);
}

EXPORT
void addStr(fl_editor_t *editor, const char *str, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    //update the cursor if necessary
    //add the string
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void addStrXY(fl_editor_t *editor, const char *str, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    //translate x and y to index
    size_t index;
    //call index-based version
    addStr(editor, str, index);
}

EXPORT
void removeChar(fl_editor_t *editor, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    //upate the cursor if necessary
    
    //remove the char
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void removeCharXY(fl_editor_t *editor, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    //translate x and y to index
    size_t index;
    //call index-based version
    removeCharXY(editor, index);
}

EXPORT
void removeStr(fl_editor_t *editor, size_t length, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    //update the cursor if necessary

    //remove the string
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void removeStrXY(fl_editor_t *editor, size_t length, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    //translate x and y to index
    size_t index;
    //call index-based version
    removeStrXY(editor, length, index);
}

EXPORT
void moveCursor(fl_editor_t *editor, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    //move the cursor
    uv_rwunlock_wrlock(&(_private->_bufLock));
}

EXPORT
void moveCursorXY(fl_editor_t *editor, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    //translate x and y to index
    size_t index;
    //call index-based version
    moveCursorXY(editor, index);
}

