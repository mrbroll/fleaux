#include "../headers/fleaux.h"
#include <uv.h>

int fl_init(int argc, char **argv)
{
    // initialize pluginmanager in it's own thread
    
    // open file(s) in main thread
    
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    return 0;
}

void fl_shutdown(void)
{
    uv_stop(uv_default_loop());
    return;
}
