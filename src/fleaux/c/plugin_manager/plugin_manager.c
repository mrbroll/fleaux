#include "plugin_manager.h"
#include <string.h>
#include <stdlib.h>
#include <uv.h>
#include <stdio.h>

uv_lib_t **libs;

int fileIsLib(const char *fileName)
{
    const char *it = fileName;
    int dot = 0;
    int end = 0;
    while (*it != '\0')
    {
        it++;
        end++;
    }
    dot = end;
    while (*it != '.')
    {
        it--;
        dot--;
    }
    printf("extension: %s\n", it);

    return (!strcmp(it, ".dylib") || 
            !strcmp(it, ".so") || 
            !strcmp(it, ".dll"));
}

void on_pluginDirScan(uv_fs_t *req)
{
    if (req->result > 0)
    {
        int i;
        for (int i = 0; i < req->result; i++)
        {
            uv_dirent_t *dent = (uv_dirent_t *)malloc(sizeof(uv_dirent_t));
            uv_fs_scandir_next(req, dent);
            if (dent->type == UV_DIRENT_FILE)
            {
                // TODO: check for extension type
                char *mutablePath = strcpy(mutablePath, req->path);
                char *fullPath = strcat(mutablePath, dent->name);
                if (fileIsLib(fullPath)) {

                }
            }
            free(dent);
        }
    }

    uv_fs_req_cleanup(req);
}




int fl_loadAllPlugins(const char *path)
{
    uv_fs_t pluginReq;
    uv_fs_scandir(uv_default_loop(), &pluginReq, path, O_RDONLY, on_pluginDirScan);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    return 0;
}


