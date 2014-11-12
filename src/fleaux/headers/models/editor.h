#ifndef _FL_EDITOR_H_
#define _FL_EDITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fl_editor_s fl_editor_t;

typedef int (*fl_editorInit_f)(fl_editor_t *);
typedef const char *(*fl_editorGetData_f)(fl_editor_t *, size_t);
typedef void (*fl_editorAddChar)(fl_editor_t *, const char, size_t);
typedef void (*fl_editorAddStr)(fl_editor_t *, const char *, size_t, size_t);
typedef const char (*fl_editorRemoveChar)(fl_editor_t *, size_t);
typedef const char *(*fl_editorRemoveStr)(fl_editor_t *, size_t, size_t);
typedef void (*fl_editorDestroy)(fl_editor_t *, void);


struct fl_editor_s
{
    fl_editorInit_f init;
    fl_editorGetData_f getData;
    fl_editorAddChar_f addChar;
    fl_editorAddStr_f addStr;
    fl_editorRemoveChar removeChar;
    fl_editorRemoveStr removeStr;
    fl_editorDestroy destroy;
};

#ifdef __cplusplus
}
#endif

#endif
