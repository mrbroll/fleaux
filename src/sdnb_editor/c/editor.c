#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include "../headers/editor.h"
#include "../headers/fs_module.h"
#include "../../fleaux/headers/plugin_models/editor.h"
#include "../../../deps/libsdnbc/src/sdnbc/headers/sdnbc.h"

#define EXPORT __attribute__((visibility("hidden")))
#define BUF_SIZE 4096

//private
typedef struct editor_privates_s
{
    fl_editor_cursor_t _cursor;
    sdnb_gapBuffer_t *_buf;
    size_t _bufLength;
    uv_rwlock_t _bufLock;
} editor_privates_t;

static void *createPrivateMembers(void)
{
    editor_privates_t *_private = (editor_privates_t*)malloc(sizeof(editor_privates_t));
    _private->_cursor.index = 0;
    _private->_cursor.x = 0;
    _private->_cursor.y = 0;
    _private->_buf = sdnb_gapBuffer_create(BUF_SIZE);
    _private->_bufLength = 0;
    uv_rwlock_init(&(_private->_bufLock));
    return (void *)_private;
}

static void destroyPrivateMembers(void *priv)
{
    editor_privates_t *_private = (editor_privates_t*)priv;
    uv_rwlock_destroy(&(_private->_bufLock));
    sdnb_gapBuffer_destroy(_private->_buf);
    free(priv);
}

//we're always going to count forward, because it makes for stupid simple code
//
//if there are performance issues with this, we'll probably introduce an
//indexing mechanism for newlines
static fl_editor_cursor_t xyToCursor(fl_editor_t *editor, fl_editor_cursor_t from, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    fl_editor_cursor_t cursor;
    if (y < from.y || (y == from.y && x < from.x)) { //behind cursor
        cursor = (fl_editor_cursor_t) { .index = 0, \
                                        .x = 0, \
                                        .y = 0 };
    } else { //at or ahead of cursor
        cursor = from;
    }
    char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, cursor.index);
    while (cursor.y <= y && cursor.index < _private->_bufLength) {
        while (iterChar != '\n' && iterChar != '\0' && (cursor.y < y || (cursor.y == y && cursor.x < x))) {
            iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
            cursor.index++;
            cursor.x++;
        }

        if (iterChar == '\n' && cursor.y < y) {
            iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
            cursor.index++;
            cursor.y++;
            cursor.x = 0;
        } else {
            break;
        }
    }
    return cursor;
}

static fl_editor_cursor_t indexToCursor(fl_editor_t *editor, fl_editor_cursor_t from, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    fl_editor_cursor_t cursor;
    if (index > _private->_bufLength) {
        index = _private->_bufLength;
    }

    if (index < from.index) {
        cursor = (fl_editor_cursor_t) {  .index = 0, \
                                            .x = 0, \
                                            .y = 0 };
    } else { //index >= _private->_cursor.index
        cursor = from;
    }
    char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, cursor.index);
    while (cursor.index < index) {
        while (iterChar != '\n' && iterChar != '\0' && cursor.index < index) {
            iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
            cursor.index++;
            cursor.x++;
        }

        if (iterChar == '\n' && cursor.index < index) {
            iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
            cursor.index++;
            cursor.y++;
            cursor.x = 0;
        } else {
            break;
        }
    }
    return cursor;
}

static fl_editor_cursor_t getStrDiff(const char *str)
{
    fl_editor_cursor_t cursor = (fl_editor_cursor_t) {  .index = 0, \
                                                        .x = 0, \
                                                        .y = 0 };
    int lastCharWasNewline = 0;
    while (str[cursor.index] != '\0') {
        if (str[cursor.index] == '\n') {
            cursor.y++;
            cursor.x++;
            lastCharWasNewline = 1;
        } else if (lastCharWasNewline) {
            cursor.x = 0;
        } else {
            cursor.x++;
        }
        cursor.index++;
    }
    return cursor;
}

EXPORT
fl_editor_t *sdnb_editor_create(const char *path)
{
    fl_editor_t *editor = (fl_editor_t *)malloc(sizeof(fl_editor_t));
    if (editor != NULL) {
        editor->_private = createPrivateMembers();
        if (editor->_private != NULL) {
            size_t pathLength = strlen(path);
            editor->filePath = (char *)malloc(pathLength + 1);
            strcpy(editor->filePath, path);
            editor->visibility = FL_EDITOR_VISIBLE;
            editor->focus = FL_EDITOR_FOCUS;
            //open and read file
            sdnb_editor_readFile(editor, path);
            while (uv_is_active((uv_handle_t *)(&sdnb_editor_fsReadCheck))) {}
        }
    }
    return editor;
}

EXPORT
void sdnb_editor_destroy(fl_editor_t *editor)
{
    while (uv_is_active((uv_handle_t *)(&sdnb_editor_fsReadCheck))) {}
    destroyPrivateMembers(editor->_private);
    editor->focus = FL_EDITOR_BLUR;
    editor->visibility = FL_EDITOR_HIDDEN;
    free(editor->filePath);
    free(editor);
}

EXPORT
size_t sdnb_editor_getLength(fl_editor_t *editor)
{
    return ((editor_privates_t *)editor->_private)->_bufLength;
}

EXPORT
void sdnb_editor_getData(fl_editor_t *editor, char *data, size_t from, size_t length)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_rdlock(&(_private->_bufLock));
    sdnb_gapBuffer_getData(_private->_buf, data, from, length);
    uv_rwlock_rdunlock(&(_private->_bufLock));
}

EXPORT
fl_editor_cursor_t sdnb_editor_getCursor(fl_editor_t *editor)
{
    return ((editor_privates_t *)editor->_private)->_cursor;
}

EXPORT
void sdnb_editor_insertAtCursor(fl_editor_t *editor, const char *str, fl_editor_cursor_t cursor)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    size_t strLength = strlen(str);
    int diff;
    if (cursor.index < _private->_cursor.index) {
        diff = -(_private->_cursor.index - cursor.index);
    } else {
        diff = cursor.index - _private->_cursor.index;
    }

    if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
        sdnb_gapBuffer_insertString(_private->_buf, str, strLength);
        sdnb_gapBuffer_moveGap(_private->_buf, -diff);
        _private->_bufLength += strLength;
        if (diff <= 0) {
            fl_editor_cursor_t cursorDiff = getStrDiff(str);
            _private->_cursor.index += cursorDiff.index;
            _private->_cursor.y += cursorDiff.y;
            if (cursor.y == _private->_cursor.y) {
                _private->_cursor.x += cursorDiff.x;
            }
        }
    }
}

EXPORT
void sdnb_editor_insertAtIndex(fl_editor_t *editor, const char *str, size_t index)
{
    uv_rwlock_wrlock(&(((editor_privates_t *)editor->_private)->_bufLock));
    fl_editor_cursor_t cursor = indexToCursor(editor, ((editor_privates_t *)editor->_private)->_cursor, index);
    sdnb_editor_insertAtCursor(editor, str, cursor);
    uv_rwlock_wrunlock(&(((editor_privates_t *)editor->_private)->_bufLock));
}

EXPORT
void sdnb_editor_insertAtXY(fl_editor_t *editor, const char *str, size_t x, size_t y)
{
    uv_rwlock_wrlock(&(((editor_privates_t *)editor->_private)->_bufLock));
    fl_editor_cursor_t cursor = xyToCursor(editor, ((editor_privates_t *)editor->_private)->_cursor, x, y);
    sdnb_editor_insertAtCursor(editor, str, cursor);
    uv_rwlock_wrunlock(&(((editor_privates_t *)editor->_private)->_bufLock));
}

EXPORT
void sdnb_editor_removeAtCursor(fl_editor_t *editor, fl_editor_cursor_t from, size_t length)
{
    if (length > 0) {
        editor_privates_t *_private = (editor_privates_t *)editor->_private;
        size_t toIndex;
        if (from.index + length >= _private->_bufLength) {
            toIndex = _private->_bufLength - 1;
        } else {
            toIndex = from.index + length - 1;
        }
        fl_editor_cursor_t to = indexToCursor(editor, from, toIndex);
        fl_editor_cursor_t newCursor;
        int diff;
        int removeLength;
        int backDiff;
        if (to.index < _private->_cursor.index) {
            diff = -(_private->_cursor.index - to.index) + 1;
            removeLength = -length;
            backDiff = -diff;
            if (to.x == _private->_cursor.x) {
                newCursor.x = _private->_cursor.x + from.x;
            }
            newCursor.y = _private->_cursor.y - (to.y - from.y);
            newCursor.index = _private->_cursor.index - length;
        } else if (from.index < _private->_cursor.index && to.index >= _private->_cursor.index) {
            diff = -(_private->_cursor.index - from.index);
            removeLength = length;
            backDiff = 0;
            newCursor = from;
        } else {
            diff = from.index - _private->_cursor.index;
            removeLength = length;
            backDiff = -diff;
            newCursor = _private->_cursor;
        }

        if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
            sdnb_gapBuffer_remove(_private->_buf, removeLength);
            sdnb_gapBuffer_moveGap(_private->_buf, backDiff);
            _private->_bufLength -= (to.index - from.index + 1);
            _private->_cursor = newCursor;
        }
    }
}


EXPORT
void sdnb_editor_removeAtIndex(fl_editor_t *editor, size_t index, size_t length)
{
    uv_rwlock_wrlock(&(((editor_privates_t *)editor->_private)->_bufLock));
    fl_editor_cursor_t cursor = indexToCursor(editor, ((editor_privates_t *)editor->_private)->_cursor, index);
    sdnb_editor_removeAtCursor(editor, cursor, length);
    uv_rwlock_wrunlock(&(((editor_privates_t *)editor->_private)->_bufLock));
}

EXPORT
void sdnb_editor_removeAtXY(fl_editor_t *editor, size_t x, size_t y, size_t length)
{
    uv_rwlock_wrlock(&(((editor_privates_t *)editor->_private)->_bufLock));
    fl_editor_cursor_t cursor = xyToCursor(editor, ((editor_privates_t *)editor->_private)->_cursor, x, y);
    sdnb_editor_removeAtCursor(editor, cursor, length);
    uv_rwlock_wrunlock(&(((editor_privates_t *)editor->_private)->_bufLock));
}

void sdnb_editor_moveCursorToCursor(fl_editor_t *editor, fl_editor_cursor_t cursor)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    int diff;
    if (cursor.index < _private->_cursor.index) {
        diff = -(_private->_cursor.index - cursor.index);
    } else {
        diff = cursor.index - _private->_cursor.index;
    }

    if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
        _private->_cursor = cursor;
    }
}

EXPORT
void sdnb_editor_moveCursorToIndex(fl_editor_t *editor, size_t index)
{
    uv_rwlock_wrlock(&((editor_privates_t *)editor->_private)->_bufLock);
    fl_editor_cursor_t cursor = indexToCursor(editor, ((editor_privates_t *)editor->_private)->_cursor, index);
    sdnb_editor_moveCursorToCursor(editor, cursor);
    uv_rwlock_wrunlock(&((editor_privates_t *)editor->_private)->_bufLock);
}

EXPORT
void sdnb_editor_moveCursorToXY(fl_editor_t *editor, size_t x, size_t y)
{
    uv_rwlock_wrlock(&((editor_privates_t *)editor->_private)->_bufLock);
    fl_editor_cursor_t cursor = xyToCursor(editor, ((editor_privates_t *)editor->_private)->_cursor, x, y);
    sdnb_editor_moveCursorToCursor(editor, cursor);
    uv_rwlock_wrunlock(&((editor_privates_t *)editor->_private)->_bufLock);
}
