#ifndef _EDITOR_H_
#define _EDITOR_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct fl_pro_editor_s fl_pro_editor_t;

typedef int (*fl_pro_editorInit_f)(fl_pro_editor_t *);
typedef const char *(*fl_pro_editorGetData_f)(fl_pro_editor_t *, size_t);
typedef void (*fl_pro_editorAddChar)(fl_pro_editor_t *, const char, size_t);
typedef void (*fl_pro_editorAddStr)(fl_pro_editor_t *, const char *, size_t, size_t);
typedef const char (*fl_pro_editorRemoveChar)(fl_pro_editor_t *, size_t);
typedef const char *(*fl_pro_editorRemoveStr)(fl_pro_editor_t *, size_t, size_t);
typedef void (*fl_pro_editorDestroy)(fl_pro_editor_t *, void);


struct fl_pro_editor_s
{
    fl_pro_editorInit_f init;
    fl_pro_editorGetData_f getData;
    fl_pro_editorAddChar_f addChar;
    fl_pro_editorAddStr_f addStr;
    fl_pro_editorRemoveChar removeChar;
    fl_pro_editorRemoveStr removeStr;
    fl_pro_editorDestroy destroy;
};

#ifdef __cplusplus
}
#endif

#endif
