#ifndef VLOG_H
#define VLOG_H
#include <stdio.h>


#ifndef VLOG_Output
#define VLOG_Output stdout
#endif

#define VLOG_OK    "OK:   "
#define VLOG_WAIT  "WAIT: "
#define VLOG_ERROR "ERROR:"
#define VLOG_INFO "INFO: "

#ifdef VLOG_LOG
#define iVLOG_Log(level, ...)                                                  \
  do {                                                                         \
    fprintf(VLOG_Output, "%s\t%s:%d:\t", level, __FILE__, __LINE__);           \
    fprintf(VLOG_Output, ##__VA_ARGS__);                                       \
    fprintf(VLOG_Output, "\n");                                                \
  } while (0);
#else
#define iVLOG_Log(...)
#endif

#define VLOG_Ok(...)   iVLOG_Log(VLOG_OK, ##__VA_ARGS__)
#define VLOG_Wait(...) iVLOG_Log(VLOG_WAIT, ##__VA_ARGS__)
#define VLOG_Error(...)                                                        \
  do {                                                                         \
    iVLOG_Log(VLOG_ERROR, ##__VA_ARGS__);                                      \
    abort();								\
  } while (0)



#define VLOG_Info(...) iVLOG_Log(VLOG_INFO, ##__VA_ARGS__)


#endif
