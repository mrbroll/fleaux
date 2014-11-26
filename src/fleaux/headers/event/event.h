#ifndef _FL_EVENT_H
#define _FL_EVENT_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum fl_context_type_e
{
    FL_CONTEXT_EDITOR,
    FL_CONTEXT_CMD
} fl_contextType_t;

typedef struct fl_context_s
{
    fl_contextType_t type;    
    union
    {
        char *filePath;
        char *cwd;
    } data;
} fl_context_t;

typedef enum fl_event_type_s
{
    FL_EVENT_CHARINSERT,
    FL_EVENT_CHARREMOVE,
    FL_EVENT_STRINGINSERT,
    FL_EVENT_STRINGREMOVE,
    FL_EVENT_CMD
} fl_event_type_t;

typedef struct fl_event_editorData_s
{
    char *text;
    int line;
    int col;
    int buffer_pos;
} fl_event_editorData_t;

typedef struct fl_event_cmdData_s
{
    int argc;
    char *argv[];
} fl_event_cmdData_t;

typedef struct fl_event_s
{
    fl_event_type_t type;
    fl_context_t context;
    union
    {
        fl_event_editorData_t;
        fl_event_cmdData_t;
    } data;
} fl_event_t;



#ifdef __cplusplus
}
#endif

#endif
