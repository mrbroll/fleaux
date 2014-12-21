#include "../../fleaux/headers/plugin_models/editor.h"
#include <uv.h>

#ifndef _SDNB_EDITOR_FS_MODULE_H_
#define _SDNB_EDITOR_FS_MODULE_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* check handle to make sure we don't destroy
 * the editor before we finish writing
 */
static uv_check_t sdnb_editor_fsReadCheck;
static uv_check_t sdnb_editor_fsWriteCheck;

void sdnb_editor_readFile(fl_editor_t *editor, const char *path);
void sdnb_editor_writeFile(fl_editor_t *editor, const char *path);

#ifdef __cplusplus
}
#endif

#endif
