#ifndef TOPPERS_ASP_H
#define TOPPERS_ASP_H

#define ENABLE_IDLELOOP

#define IDLELOOP_STACKSIZE   (1024U*10U)

#define ARDUINO_PASS

#include <../asp_wo_cfg/include/kernel.h>
#include <../asp_wo_cfg/include/t_syslog.h>
#include <../asp_wo_cfg/include/t_stdlib.h>
#include "../asp_wo_cfg/syssvc/syslog.h"
#include "../asp_wo_cfg/syssvc/serial.h"
#include "kernel_id.h"
#undef true
#undef false

/*
 *  カーネルのスタート関数
 */
extern void StartToppersASP(void);

/*
 *  NOPによる一定時間待ち
 */
extern void ToppersASPDelayMs(uint32_t ms);

#ifdef __cplusplus
extern "C" {
#endif

extern void CLOCK_Initialize(void);
  
#ifdef __cplusplus
}
#endif

#endif /* TOPPERS_ASP_H */
