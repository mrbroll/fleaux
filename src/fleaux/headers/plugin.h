#ifndef _FL_PLUGIN_H_
#define _FL_PLUGIN_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*fl_modelDestroy_f)(void *);
typedef fl_modelDestroy_f (*fl_modelInit_f)(void *, void *);

typedef struct fl_model_s
{
    const char *modelType;
    fl_modelInit_f modelInit;
} fl_model_t;

#ifdef __cplusplus
}
#endif

#endif
