#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include "../headers/sdnb_editor.h"
#include "../../fleaux/headers/plugin_models/editor.h"
#include "../../../libsdnbc/src/sdnbc/headers/sdnbc.h"

#define EXPORT __attribute__((visibility("hidden")))
#define BUF_SIZE 1024

//private
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
    uv_rwlock_t _cursLock;
} editor_privates_t;

//private
static void initPrivateMembers(void *priv)
{
    editor_privates_t *_private = (editor_privates_t*)priv;
    _private->_cursor.index = 0;
    _private->_cursor.x = 0;
    _private->_cursor.y = 0;
    _private->_buf = sdnb_gapBuffer_create(BUF_SIZE);
    uv_rwlock_init(&(_private->_bufLock));
    uv_rwlock_init(&(_private->cursLock));
}

//private
static void cleanupPrivateMembers(void *priv)
{
    editor_privates_t *_private = (editor_privates_t*)priv;
    uv_rwlock_destroy(&(_private->_cursLock));
    uv_rwlock_destroy(&(_private->_bufLock));
    sdnb_gapBuffer_destroy(_private->_buf);
}

//private
static size_t xyToIndex(fl_editor_t *editor, size_t x, size_t y)
{
    uv_rwlock_wrlock(&(_private->_bufLock));
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    size_t seekX = _private->_cursor.x;
    size_t seekY = _private->_cursor.y;
    size_t seekIndex = _private->_cursor.index;
    char initChar = sdnb_gapBuffer_iterSet(_private->_buf, cIndex);
    char iterChar;
    //move the cursor if necessary
    if (y != cY) { //get to the right line
        if (y < cY) {
            while (cY > y) {
                while (sdnb_gapBuffer_iterPrev(_private->_buf) != '\n') {
                    cIndex--;
                }
                cY--;
            }
            //now that we're on the right line,
            //go to the beginning of the line
            if (cIndex > 0) {
                do {
                    iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                    cIndex--;
                } while (iterChar != '\n' || iterChar != '\0');
                cIndex++;
            }
        } else if (y > cY) {
            if (initChar == '\n') {
                cY++;
            }
            while (cY < y) {
                do {
                    iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
                    cIndex++;
                } while (iterChar != '\n');
                cY++;
            }
            sdnb_gapBuffer_iterNext(_private->_buf);
            cIndex++;
        }
        cX = 0;
    }

    if (x != cX) { // get to the right column
        if (x < cX) {
            int diffX = cX - x;
            cIndex -= diffX;
        } else if (x > cX) {
            do {
                iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
                cIndex++;
                cX++;
            } while (cX < x || iterChar != '\n' || iterChar != '\0');
            cIndex--;
        }
    }
    uv_rwlock_wrunlock(&(_private->_bufLock));
    return cIndex;
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
    uv_rwlock_rdlock(&(_private->_bufLock));
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    sdnb_gapBuffer_getData(_private->_buf, data, from, to);
    uv_rwlock_rdunlock(&(_private->_bufLock));
}

EXPORT
void addChar(fl_editor_t *editor, char ch, size_t index)
{
    uv_rwlock_wrlock(&(_private->_bufLock));
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    size_t saveIndex = _private->_cursor.index;
    int diff = 0;
    if (index != saveIndex) { //move the cursor if necessary
        if (index < saveIndex) {
            diff = -(saveIndex - index);
            _private->_cursor.index++;
            if (ch == '\n') {
                _private->_cursor.y++;
                _private->cursor.x = 0;
            } else {
                _private->_cursor.x++;
            }
        } else { //index > saveIndex
            diff = index - saveIndex;
        }
        sdnb_gapBuffer_moveGap(editor->_private->_buf, diff);
        sdnb_gapBuffer_insertChar(editor->_private->_buf, ch);
        sdnb_gapBuffer_moveGap(editor->_private->_buf, -diff);
    } else {
        sdnb_gapBuffer_insertChar(editor->_private->_buf, ch);
        _private->_cursor.index++;
        if (ch == '\n') {
            _private->_cursor.y++;
            _private->cursor.x = 0;
        } else {
            _private->_cursor.x++;
        }
    }
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void addCharXY(fl_editor_t *editor, char ch, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
    addChar(editor, ch, index);
}

EXPORT
void addStr(fl_editor_t *editor, char *str, size_t index)
{
    uv_rwlock_wrlock(&(_private->_bufLock));
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    size_t length = strlen(str);
    size_t saveIndex = _private->_cursor.index;
    int diff = 0;
    if (index != saveIndex) { //move the cursor if necessary
        if (index < saveIndex) {
            diff = -(saveIndex - index);
            size_t i;
            size_t lastNewline;
            for (i = 0; i < length, i++) {
                if (str[i] == '\n') {
                    _private->_cursor.y++;
                    lastNewline = i;
                }
            }
            _private->_cursor.x = (i - lastNewline);
        } else { //index > saveIndex
            diff = index - saveIndex;
        }
        sdnb_gapBuffer_moveGap(_private->_buf, diff);
        sdnb_gapBuffer_insertString(_private->_buf, str, length); 
        sdnb_gapBuffer_moveGap(_private->_buf, -diff);
    } else {
        sdnb_gapBuffer_insertString(_private->_buf, str, length);
        size_t i;
        size_t lastNewline;
        for (i = 0; i < length, i++) {
            if (str[i] == '\n') {
                _private->_cursor.y++;
                lastNewline = i;
            }
        }
        _private->_cursor.x = (i - lastNewline);
    }
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void addStrXY(fl_editor_t *editor, char *str, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
    addStr(editor, str, index);
}

EXPORT
void removeChar(fl_editor_t *editor, size_t index)
{
    uv_rwlock_wrlock(&(_private->_bufLock));
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    size_t saveIndex = _private->_cursor.index;
    if (index > saveIndex) {
        int diff = index - saveIndex - 1;
        sdnb_gapBuffer_moveGap(_private->_buf, diff);
        sdnb_gapBuffer_remove(_private->buf, 1);
        sdnb_gapBuffer_moveGap(_private->_buf, -diff);
    } else if (index < saveIndex - 1) {
        int diff = -(saveIndex - index - 1);
        sdnb_gapBuffer_moveGap(_private->_buf, diff);
        sdnb_gapBuffer_remove(_private->_buf, -1);
        sdnb_gapBuffer_moveGap(_private->_buf, -diff);
        if (saveIndex > 0) {
            if (_private->_cursor.x == 0) {
                _private->_cursor.y--;
                _private->_cursor.index--;
                char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, saveIndex);
                while (iterChar != '\n' && iterChar != '\0') {
                    _private->_cursor.x++;
                }
            }
        }
    } else {

    }
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void removeCharXY(fl_editor_t *editor, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
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
    size_t index = xyToIndex(editor, x, y);
    removeStrXY(editor, length, index);
}

EXPORT
void moveCursor(fl_editor_t *editor, size_t index)
{
    uv_rwlock_wrlock(&(_private->_bufLock));
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
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

