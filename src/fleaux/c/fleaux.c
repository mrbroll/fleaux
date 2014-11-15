#include "../headers/fleaux.h"
#include <uv.h>
#include "plugin_manager/plugin_manager.h"

int fl_initFleaux(void)
{
    // initialize pluginmanager in it's own thread
    
    // open file(s) in main thread
    
    // ... do work, son ...

    // stop watchers

    // close file(s)

    // shut down plugin manager

    fl_loadAllPlugins("/");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    return 0;
}
