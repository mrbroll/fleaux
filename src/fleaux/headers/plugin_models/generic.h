#include "../plugin.h"

#ifndef _FL_GENERIC_H_
#define _FL_GENERIC_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*fl_onInsert_f)(const char *, size_t, size_t);
typedef int (*fl_onRemove_f)(const char *, size_t, size_t);

typedef struct fl_generic_s
{
    const char *name;
    fl_pluginLang_t programmingLang;
    fl_objInit_f init
    fl_onInsert_f onBufInsert;
    fl_onRemove_f onBufRemove;
    fl_objDestroy_f destroy;
} fl_generic_t;

#ifdef __cplusplus
}
#endif

#endif
