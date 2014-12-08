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
    size_t _bufLength;
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
    _private->_bufLength = 0;
    uv_rwlock_init(&(_private->_bufLock));
    uv_rwlock_init(&(_private->_cursLock));
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
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    size_t cX = _private->_cursor.x;
    size_t cY = _private->_cursor.y;
    size_t cIndex = _private->_cursor.index;
    char initChar = sdnb_gapBuffer_iterSet(_private->_buf, cIndex);
    char iterChar;
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
size_t sdnb_editor_getLength(fl_editor_t *editor)
{
    return ((editor_privates_t *)editor->_private)->_bufLength;
}

EXPORT
void sdnb_editor_init(fl_editor_t* editor, const char *path)
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
void sdnb_editor_cleanup(fl_editor_t *editor)
{
    cleanupPrivateMembers(editor->_private);
    free(editor->_private);
    editor->focus = FL_EDITOR_BLUR;
    editor->visibility = FL_EDITOR_HIDDEN;
    free(editor->filePath);
}

EXPORT
void sdnb_editor_getData(fl_editor_t *editor, char *data, size_t from, size_t to)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_rdlock(&(_private->_bufLock));
    sdnb_gapBuffer_getData(_private->_buf, data, from, to);
    uv_rwlock_rdunlock(&(_private->_bufLock));
}

EXPORT
void sdnb_editor_addChar(fl_editor_t *editor, const char ch, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    size_t saveIndex = _private->_cursor.index;
    int diff = 0;
    if (index <= saveIndex) { //move the cursor if necessary
        if (index < saveIndex) {
            diff = -(saveIndex - index);
            if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
                sdnb_gapBuffer_insertChar(_private->_buf, ch);
                sdnb_gapBuffer_moveGap(_private->_buf, -diff);
            } else {
                uv_rwlock_wrunlock(&(_private->_bufLock));
                return; //silently fail on out of bounds
            }
        } else {
            sdnb_gapBuffer_insertChar(_private->_buf, ch);
        }
        if (ch == '\n') {
            _private->_cursor.y++;
            _private->_cursor.x = 0;
        } else {
            _private->_cursor.x++;
        }
    } else {
        sdnb_gapBuffer_insertChar(_private->_buf, ch);
    }
    _private->_bufLength++;
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void sdnb_editor_addCharXY(fl_editor_t *editor, const char ch, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
    sdnb_editor_addChar(editor, ch, index);
}

EXPORT
void sdnb_editor_addStr(fl_editor_t *editor, const char *str, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    size_t length = strlen(str);
    size_t saveIndex = _private->_cursor.index;
    int diff = 0;
    if (index != saveIndex) { //move the cursor if necessary
        if (index < saveIndex) {
            diff = -(saveIndex - index);
            size_t i;
            size_t lastNewline;
            for (i = 0; i < length; i++) {
                if (str[i] == '\n') {
                    _private->_cursor.y++;
                    lastNewline = i;
                }
            }
            _private->_cursor.x = (i - lastNewline);
        } else { //index > saveIndex
            diff = index - saveIndex;
        }

        if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
            sdnb_gapBuffer_insertString(_private->_buf, str, length); 
            sdnb_gapBuffer_moveGap(_private->_buf, -diff);
        }
    } else {
        sdnb_gapBuffer_insertString(_private->_buf, str, length);
        size_t i;
        size_t lastNewline;
        for (i = 0; i < length; i++) {
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
void sdnb_editor_addStrXY(fl_editor_t *editor, const char *str, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
    sdnb_editor_addStr(editor, str, index);
}

EXPORT
void sdnb_editor_removeChar(fl_editor_t *editor, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    size_t saveIndex = _private->_cursor.index;
    if (index >= saveIndex) {
        //no need to update cursor
        if (index == saveIndex) { //delete
            sdnb_gapBuffer_remove(_private->_buf, 1);
        } else {
            int diff = index - saveIndex;
            if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
                sdnb_gapBuffer_remove(_private->_buf, 1);
                sdnb_gapBuffer_moveGap(_private->_buf, -diff);
            }
        }
    } else {
        //update the cursor
        char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, index);
        if (iterChar == '\n') {
            _private->_cursor.y--;
            if (index == saveIndex - 1) { //_cursor.x == 0
                do {
                    iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                    _private->_cursor.x++;
                } while (iterChar != '\n' && iterChar != '\0');
                _private->_cursor.x--;
            }
        }
        _private->_cursor.index--;
        //cursor updated
        if (index == saveIndex - 1) { //backspace
            sdnb_gapBuffer_remove(_private->_buf, -1);
        } else {
            int diff = -(saveIndex - index - 1);
            if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
                sdnb_gapBuffer_remove(_private->_buf, -1);
                sdnb_gapBuffer_moveGap(_private->_buf, -diff);
            }
        }
    }
    uv_rwlock_wrunlock(&(_private->_bufLock));
}

EXPORT
void sdnb_editor_removeCharXY(fl_editor_t *editor, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
    sdnb_editor_removeChar(editor, index);
}

EXPORT
void sdnb_editor_removeStr(fl_editor_t *editor, size_t length, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    size_t saveIndex = _private->_cursor.index;
    if (index >= saveIndex) {
        //no need to update cursor
        if (index == saveIndex) { //delete
            sdnb_gapBuffer_remove(_private->_buf, length);
        } else {
            int diff = index - saveIndex;
            if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
                sdnb_gapBuffer_remove(_private->_buf, length);
                sdnb_gapBuffer_moveGap(_private->_buf, -diff);
            }
        }
    } else {
        //update cursor
        char initChar;
        char iterChar;
        size_t iterLength;
        if (saveIndex - index + length >= saveIndex) {
            initChar = sdnb_gapBuffer_iterSet(_private->_buf, saveIndex - 1);
            iterLength = saveIndex - index;
        } else {
            initChar = sdnb_gapBuffer_iterSet(_private->_buf, saveIndex - index + length - 1);
            iterLength = length;
        }
        size_t i;
        for (i = 0; i < iterLength; i++) {
            iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
            if (iterChar == '\n') {
                _private->_cursor.y--;
            }
        }
        if (initChar == '\n') { //_cursor.x == 0
            do {
                iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                _private->_cursor.x++;
            } while (iterChar != '\n' && iterChar != '\0');
            _private->_cursor.x--;
        }
        _private->_cursor.index -= iterLength;
        //cursor updated
        if (index + length == saveIndex - 1) { //backspace
            sdnb_gapBuffer_remove(_private->_buf, -(int)length);
        } else if (index + length < saveIndex - 1) {
            int diff = -(saveIndex - index - length);
            if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
                sdnb_gapBuffer_remove(_private->_buf, -(int)length);
                sdnb_gapBuffer_moveGap(_private->_buf, -diff);
            }
        } else { //string overlaps cursor
            int diffLo = -(saveIndex - index);
            int diffHi = saveIndex - index + length;
            if (abs(diffLo) < diffHi) {
                if (sdnb_gapBuffer_moveGap(_private->_buf, diffLo) == 0) {
                    sdnb_gapBuffer_remove(_private->_buf, length);
                    sdnb_gapBuffer_moveGap(_private->_buf, -diffLo);
                }
            } else {
                if (sdnb_gapBuffer_moveGap(_private->_buf, diffHi) == 0) {
                    sdnb_gapBuffer_remove(_private->_buf, -(int)length);
                    sdnb_gapBuffer_moveGap(_private->_buf, -diffHi);
                }
            }
        }
    }
    uv_rwlock_wrunlock(&(_private->_bufLock));
    return;
}

EXPORT
void sdnb_editor_removeStrXY(fl_editor_t *editor, size_t length, size_t x, size_t y)
{
    size_t index = xyToIndex(editor, x, y);
    sdnb_editor_removeStr(editor, length, index);
    return;
}

EXPORT
void sdnb_editor_moveCursor(fl_editor_t *editor, size_t index)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    //calc diff
    int diff;
    if (index == _private->_cursor.index) {
        uv_rwlock_wrunlock(&(_private->_bufLock));
        return;
    } else if (index > _private->_cursor.index) {
        diff = index - _private->_cursor.index;
    } else { //index < _private->_cursor.index
        diff = -(_private->_cursor.index - index);
    }
    if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
        size_t x = 0;
        size_t y = 0;
        size_t i;
        if (diff < 0) {
            if (index < _private->_cursor.index - index) { //start from beginning
                char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, 0);
                for (i = 0; i < index; i++) {
                    if (iterChar == '\n') {
                        y++;
                        x = 0;
                    } else {
                        x++;
                    }
                    sdnb_gapBuffer_iterNext(_private->_buf);
                } 
            } else { //start from cursor
                char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, _private->_cursor.index);
                y = _private->_cursor.y;
                for (i = _private->_cursor.index; i > index; i--) {
                    iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                    if (iterChar == '\n') {
                        y--;
                    }
                }
                //find x coordinate
                iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                while (iterChar != '\n' && iterChar != '\0') {
                    x++;
                    iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                }
            }
        } else { //start from cursor
            char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, _private->_cursor.index);
            x = _private->_cursor.x;
            y = _private->_cursor.y;
            for (i = _private->_cursor.index; i < index; i++) {
                if (iterChar == '\n') {
                    y++;
                    x = 0;   
                } else {
                    x++;
                }
                sdnb_gapBuffer_iterNext(_private->_buf);
            }
        }
        _private->_cursor.x = x;
        _private->_cursor.y = y;
        _private->_cursor.index = index;
    }
    uv_rwlock_wrlock(&(_private->_bufLock));
    return;
}

EXPORT
void sdnb_editor_moveCursorXY(fl_editor_t *editor, size_t x, size_t y)
{
    editor_privates_t *_private = (editor_privates_t *)editor->_private;
    uv_rwlock_wrlock(&(_private->_bufLock));
    size_t index;
    size_t iX;
    size_t iY;
    if (x == _private->_cursor.x && y == _private->_cursor.y) {
        uv_rwlock_wrunlock(&(_private->_bufLock));
        return;
    } else if (y < _private->_cursor.y || (x < _private->_cursor.x && y <= _private->_cursor.y)) { //move backward
        if (_private->_cursor.y >> 1 > y) { //start from beginning
            char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, 0);
            index = 0;
            iY = 0;
            while(iY < y) { //get to the line
                if (iterChar == '\n') {
                    iY++;
                }
                iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
                index++;
            }
            iX = 0;
            while (iX < x) { //get to the column or EOL if out of bounds
                if (iterChar == '\n') {
                    break;
                }
                iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
                iX++;
                index++;
            }
        } else { //start from cursor
            index = _private->_cursor.index;
            iY = _private->_cursor.y;
            char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, index);
            do {
                iterChar = sdnb_gapBuffer_iterPrev(_private->_buf);
                index--;
                if (iterChar == '\n') {
                    iY--;
                }
            } while (iY >= y);
            iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
            iX = 0;
            do {
                iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
                index++;
                iX++;
            } while (iX < x);
        }
    } else { //move forward
        index = _private->_cursor.index;
        iY = _private->_cursor.y;
        iX = _private->_cursor.x;
        char iterChar = sdnb_gapBuffer_iterSet(_private->_buf, index);
        while (iY <= y && iterChar != '\0') {
            while (((iY < y && iterChar != '\n') || (iY == y && iX < x)) \
                    && iterChar != '\0') {
                iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
                iX++;
                index++;
            }

            if (iterChar != '\0' && iY < y) {
                iX = 0;
                iY++;
                iterChar = sdnb_gapBuffer_iterNext(_private->_buf);
            }
        }
    }
    //move the gapBuffer's gap
    int diff;
    if (index < _private->_cursor.index) {
        diff = -(_private->_cursor.index - index);
    } else {
        diff = index - _private->_cursor.index;
    }

    if (sdnb_gapBuffer_moveGap(_private->_buf, diff) == 0) {
        _private->_cursor.x = iX;
        _private->_cursor.y = iY;
        _private->_cursor.index = index;
    }

    uv_rwlock_wrunlock(&(_private->_bufLock));
}
