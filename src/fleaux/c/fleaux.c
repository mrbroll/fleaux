#include "../headers/fleaux.h"
#include <uv.h>
#include "plugin_manager/plugin_manager.h"

int fl_initFleaux(void)
{
    // initalize error logging daemon

    // initialize the plugin manager(s) in background
    
    // open file(s)
    
    // create plugin objects

    // create watchers & callbacks
    
    // ... do work, son ...

    // stop watchers

    // destroy plugin objects

    // close file(s)

    // shut down plugin manager(s)

    // stop error logging daemon
    fl_loadAllPlugins("/");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    return 0;
}
