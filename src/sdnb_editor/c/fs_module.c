#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uv.h>
#include "../headers/fs_module.h"
#include "../headers/editor.h"
#include "../../fleaux/headers/plugin_models/editor.h"

#define EXPORT __attribute__((visibility("hidden")))
#define SDNB_EDITOR_FS_BUF_SIZE (1024)

/* Request Handles */
static uv_fs_t sdnb_editor_fsReadOpenReq;
static uv_fs_t sdnb_editor_fsReadCloseReq;
static uv_fs_t sdnb_editor_fsWriteOpenReq;
static uv_fs_t sdnb_editor_fsWriteCloseReq;
static uv_fs_t sdnb_editor_fsReadReq;
static uv_fs_t sdnb_editor_fsWriteReq;


/* Data Members */
static char sdnb_editor_fsReadBuf[SDNB_EDITOR_FS_BUF_SIZE];
static char sdnb_editor_fsReadStrBuf[SDNB_EDITOR_FS_BUF_SIZE + 1];
static char sdnb_editor_fsWriteBuf[SDNB_EDITOR_FS_BUF_SIZE];
static char sdnb_editor_fsWriteStrBuf[SDNB_EDITOR_FS_BUF_SIZE + 1];
static uv_buf_t sdnb_editor_uvReadBuf;
static uv_buf_t sdnb_editor_uvWriteBuf;

/* Callbacks and Helpers */
static void sdnb_editor_fsOnReadCheck(uv_check_t *handle)
{
    /* This is just for synchronization */
}

static void sdnb_editor_fsOnWriteCheck(uv_check_t *handle)
{
    /* This is just for synchronization */
}

static void sdnb_editor_fsOnWrite(uv_fs_t *req);

static void sdnb_editor_fsWriteData(fl_editor_t *editor)
{
    if (sdnb_editor_getLength(editor) == 0) {
        uv_check_stop(&sdnb_editor_fsWriteCheck);
        uv_fs_close(uv_default_loop(), &sdnb_editor_fsWriteCloseReq, sdnb_editor_fsWriteOpenReq.result, NULL);
    } else {
        memset(sdnb_editor_fsWriteStrBuf, 0, sizeof(sdnb_editor_fsWriteStrBuf));
        memset(sdnb_editor_fsWriteBuf, 0, sizeof(sdnb_editor_fsWriteBuf));
        sdnb_editor_getData(editor, sdnb_editor_fsWriteStrBuf, 0, SDNB_EDITOR_FS_BUF_SIZE);
        sdnb_editor_removeAtIndex(editor, 0, SDNB_EDITOR_FS_BUF_SIZE);
        memcpy(sdnb_editor_fsWriteBuf, sdnb_editor_fsWriteStrBuf, sizeof(sdnb_editor_fsWriteBuf));
        uv_fs_write(uv_default_loop(), \
                    &sdnb_editor_fsWriteReq, \
                    sdnb_editor_fsWriteOpenReq.result, \
                    &sdnb_editor_uvWriteBuf, \
                    1,
                    -1,
                    sdnb_editor_fsOnWrite);
    }
}

static void sdnb_editor_fsOnWrite(uv_fs_t *req)
{
    uv_fs_req_cleanup(req);
    if (req->result < 0) {
        fprintf(stderr, "error writing to file: %zd\n", req->result);
    } else {
        sdnb_editor_fsWriteData((fl_editor_t *)sdnb_editor_fsWriteOpenReq.data);
    }
}

static void sdnb_editor_fsWriteOnOpen(uv_fs_t *req)
{
    if (req->result < 0) {
        fprintf(stderr, "error opening file: %zd\n", req->result);
    } else {
        sdnb_editor_uvWriteBuf = uv_buf_init(sdnb_editor_fsWriteBuf, sizeof(sdnb_editor_fsWriteBuf));
        sdnb_editor_fsWriteData((fl_editor_t *)req->data);
    }
    uv_fs_req_cleanup(req);
}

static void sdnb_editor_fsOnRead(uv_fs_t *req);

static void sdnb_editor_fsReadData(fl_editor_t *editor)
{
    memset(sdnb_editor_fsReadStrBuf, 0, sizeof(sdnb_editor_fsReadStrBuf));
    memcpy(sdnb_editor_fsReadStrBuf, sdnb_editor_fsReadBuf, sizeof(sdnb_editor_fsReadBuf));
    sdnb_editor_insertAtCursor(editor, sdnb_editor_fsReadStrBuf, sdnb_editor_getCursor(editor));
    memset(sdnb_editor_fsReadBuf, 0, sizeof(sdnb_editor_fsReadBuf));
    uv_fs_read( uv_default_loop(), \
                &sdnb_editor_fsReadReq, \
                sdnb_editor_fsReadOpenReq.result, \
                &sdnb_editor_uvReadBuf, \
                1, \
                -1, \
                sdnb_editor_fsOnRead);
}

static void sdnb_editor_fsOnRead(uv_fs_t *req)
{
    uv_fs_req_cleanup(req);
    if (req->result < 0) {
        const char *errString = uv_strerror(req->result);
        fprintf(stderr, "ERROR UV: %s\n", errString);
        uv_check_stop(&sdnb_editor_fsReadCheck);
    } else if (req->result == 0) {
        uv_fs_close(uv_default_loop(), &sdnb_editor_fsReadCloseReq, sdnb_editor_fsReadOpenReq.result, NULL);
        uv_check_stop(&sdnb_editor_fsReadCheck);
    } else {
        sdnb_editor_fsReadData((fl_editor_t *)sdnb_editor_fsReadOpenReq.data);
    }
}

static void sdnb_editor_fsReadOnOpen(uv_fs_t *req)
{
    if (req->result < 0) {
        const char *errString = uv_strerror(req->result);
        fprintf(stderr, "error opening file: %s\n", errString);
        uv_check_stop(&sdnb_editor_fsReadCheck);
    } else {
        memset(sdnb_editor_fsReadBuf, 0, sizeof(sdnb_editor_fsReadBuf));
        sdnb_editor_uvReadBuf = uv_buf_init(sdnb_editor_fsReadBuf, sizeof(sdnb_editor_fsReadBuf));
        uv_fs_read( uv_default_loop(), \
                    &sdnb_editor_fsReadReq, \
                    req->result, \
                    &sdnb_editor_uvReadBuf, \
                    1, \
                    -1, \
                    sdnb_editor_fsOnRead);
    }
    uv_fs_req_cleanup(req);
}

EXPORT
void sdnb_editor_readFile(fl_editor_t *editor, const char *path)
{
    sdnb_editor_fsReadOpenReq.data = (void *)editor;
    uv_check_init(uv_default_loop(), &sdnb_editor_fsReadCheck);
    uv_check_start(&sdnb_editor_fsReadCheck, sdnb_editor_fsOnReadCheck);
    uv_fs_open( uv_default_loop(), \
                &sdnb_editor_fsReadOpenReq, \
                path, \
                O_RDONLY | O_CREAT, \
                S_IRWXU | S_IRWXG, \
                sdnb_editor_fsReadOnOpen);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

EXPORT
void sdnb_editor_writeFile(fl_editor_t *editor, const char *path)
{
    sdnb_editor_fsWriteOpenReq.data = (void *)editor;
    uv_check_init(uv_default_loop(), &sdnb_editor_fsWriteCheck);
    uv_check_start(&sdnb_editor_fsWriteCheck, sdnb_editor_fsOnWriteCheck);
    uv_fs_open( uv_default_loop(), \
                &sdnb_editor_fsWriteOpenReq, \
                path, \
                O_WRONLY | O_CREAT, \
                S_IRWXU | S_IRWXG,
                sdnb_editor_fsWriteOnOpen);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
