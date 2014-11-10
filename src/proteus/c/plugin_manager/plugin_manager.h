#include "../../headers/plugin.h"

#ifndef _PLUGIN_MANAGER_H_
#define _PLUGIN_MANAGER_H_

#ifdef __cplusplus
extern "C"
{
#endif

int fl_pro_loadPlugin(const char *pluginName);
int fl_pro_loadAllPlugins(const char *dirName);
int fl_pro_unloadPlugin(const char *pluginName);
int fl_pro_unloadAllPlugins(void);

#ifdef __cplusplus
}
#endif

#endif
