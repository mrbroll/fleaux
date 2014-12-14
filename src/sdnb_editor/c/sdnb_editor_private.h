#ifndef _SDNB_EDITOR_PRIVATE_H_
#define _SDNB_EDITOR_PRIVATE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define SDNB_EDITOR_PRIVATE_FIELDS  \
    fl_editor_cursor_t _cursor;     \
    sdnb_gapBuffer_t *_buf;         \
    size_t _bufLength;              \
    uv_rwlock_t _bufLock;           \
    uv_rwlock_t _cursLock;

#ifdef __cplusplus
}
#endif

#endif
