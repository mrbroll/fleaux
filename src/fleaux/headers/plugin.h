#ifndef _FL_PLUGIN_H_
#define _FL_PLUGIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum fl_pluginLang_e
{
    FL_LANG_C,
    FL_LANG_CPP,
    FL_LANG_JAVA
} fl_pluginLang_t;

typedef void (*fl_objDestroy_f)(void *);
typedef fl_objDestroy_f (*fl_objInit_f)(void *, void *);

int fl_objRegister(const char *pluginName, const char *objName, fl_objInit_f objInit);

#ifdef __cplusplus
}
#endif

#endif
