#ifndef __PIGA_PROCFS_H__
#define __PIGA_PROCFS_H__

#include "piga.h"

#define PROC_PIGA_DIR "piga"
#define PROC_PIGA_STATUS "status"
#define PROC_PIGA_MODE "mode"
#define PROC_PIGA_GETSID "getsid"
#define PROC_PIGA_LOAD_SEQ "load_sequence"
#define PROC_PIGA_LOAD_LINK "load_link"
#define PROC_PIGA_POLICY "policy"


int init_piga_procfs(void);


#endif /* __PIGA_PROCFS_H__ */