#include <stdlib.h>

#ifndef _FL_EDITOR_H_
#define _FL_EDITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fl_editor_s fl_editor_t;

typedef enum fl_editor_visibility_e
{
    FL_EDITOR_VISIBLE,
    FL_EDITOR_HIDDEN
} fl_editor_visibility_t;

typedef enum fl_editor_focus_e
{
    FL_EDITOR_FOCUS,
    FL_EDITOR_BLUR
} fl_editor_focus_t;

//caller must allocate memory
typedef void (*fl_editor_init_f)(fl_editor_t *, const char *path);

//caller must free memory
typedef void (*fl_editor_cleanup_f)(fl_editor_t *);

typedef void (*fl_editor_getData_f)(fl_editor_t *, char *data, size_t from, size_t to);
typedef void (*fl_editor_addChar_f)(fl_editor_t *, const char, size_t index);
typedef void (*fl_editor_addCharXY_f)(fl_editor_t *, const char, size_t x, size_t y);
typedef void (*fl_editor_addStr_f)(fl_editor_t *, const char *, size_t index);
typedef void (*fl_editor_addStrXY_f)(fl_editor_t *, const char *, size_t x, size_t y);
typedef void (*fl_editor_removeChar_f)(fl_editor_t *, size_t index);
typedef void (*fl_editor_removeCharXY_f)(fl_editor_t *, size_t x, size_t y);
typedef void (*fl_editor_removeStr_f)(fl_editor_t *, size_t length, size_t index);
typedef void (*fl_editor_removeStrXY_f)(fl_editor_t *, size_t length, size_t x, size_t y);
typedef void (*fl_editor_moveCursor_f)(fl_editor_t *, size_t index);
typedef void (*fl_editor_moveCursorXY_f)(fl_editor_t *, size_t x, size_t y);

struct fl_editor_s
{
    char *filePath;
    fl_editor_visibility_t visibility;
    fl_editor_focus_t focus;
    void *_private; //don't touch!
};

#ifdef __cplusplus
}
#endif

#endif
