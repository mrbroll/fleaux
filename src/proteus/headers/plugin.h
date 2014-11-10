#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum fl_pro_pluginLang_e
{
    FL_PRO_LANG_C,
    FL_PRO_LANG_CPP,
    FL_PRO_LANG_JAVA
} fl_pro_pluginLang_t;

typedef void (*fl_pro_objDestroy_f)(void *);
typedef fl_pro_objDestroy_f (*fl_pro_objInit_f)(void *, void *);

int fl_pro_objRegister(const char *pluginName, const char *objName, fl_pro_objInit_f objInit);

#ifdef __cplusplus
}
#endif

#endif
