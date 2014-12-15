#include "../../fleaux/headers/plugin_models/editor.h"

#ifndef _SDNB_EDITOR_FS_MODULE_H_
#define _SDNB_EDITOR_FS_MODULE_H_

#ifdef __cplusplus
extern "C"
{
#endif

void sdnb_editor_readFile(fl_editor_t *editor, const char *path);
void sdnb_editor_writeFile(fl_editor_t *editor, const char *path);

#ifdef __cplusplus
}
#endif

#endif
