#ifndef TOPPERS_ASP_H
#define TOPPERS_ASP_H

#define ARDUINO_PASS

#include <../asp_wo_cfg/include/kernel.h>
#include <../asp_wo_cfg/include/t_syslog.h>
#include <../asp_wo_cfg/include/t_stdlib.h>
#include "../asp_wo_cfg/syssvc/syslog.h"
#include "../asp_wo_cfg/syssvc/serial.h"
#include "kernel_id.h"

/*
 *  カーネルのスタート関数
 */
extern void StartToppersASP(void);

#ifdef __cplusplus
extern "C" {
#endif

extern void sta_ker(void);
extern void CLOCK_Initialize(void);
  
#ifdef __cplusplus
}
#endif

#endif /* TOPPERS_ASP_H */
