#include "../plugin.h"

#ifndef _GENERIC_H_
#define _GENERIC_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef int (*fl_pro_onInsert_f)(const char *, size_t, size_t);
typedef int (*fl_pro_onRemove_f)(const char *, size_t, size_t);

typedef struct fl_pro_generic_s
{
    const char *name;
    fl_pro_pluginLang_t programmingLang;
    fl_pro_objInit_f init
    fl_pro_onInsert_f onBufInsert;
    fl_pro_onRemove_f onBufRemove;
    fl_pro_objDestroy_f destroy;
} fl_pro_generic_t;

#ifdef __cplusplus
}
#endif

#endif
