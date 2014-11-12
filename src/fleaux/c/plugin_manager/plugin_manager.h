#include "../../headers/plugin.h"

#ifndef _FL_PLUGIN_MANAGER_H_
#define _FL_PLUGIN_MANAGER_H_

#ifdef __cplusplus
extern "C"
{
#endif

int fl_loadPlugin(const char *pluginName);
int fl_loadAllPlugins(const char *dirName);
int fl_unloadPlugin(const char *pluginName);
int fl_unloadAllPlugins(void);

#ifdef __cplusplus
}
#endif

#endif
