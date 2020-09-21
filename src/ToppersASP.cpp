#ifdef ARDUINO
#include "Arduino.h"
#endif /* ARDUINO */

#include "ToppersASP.h"

/*
 *  ユーザー記述の初期化関数
 */
extern void user_inirtn(void);

/*
 *  ユーザー記述の終了関数
 */
extern void user_terrtn(void);

#ifdef ARDUINO
/*
 *  カーネルのスタート関数
 */
void
StartToppersASP(void)
{
	/* SEMCOM2初期化 */
	Serial1.begin(115200);
	Serial1.println("TOPPERS/ASP for Arduino");
	Serial1.flush();

	/* 割込み禁止 */
	noInterrupts();

	/* カーネルスタート */
	sta_ker();
}

#define CAL_FACTOR (F_CPU/6000)

void
ToppersASPDelayMs(uint32_t ms)
{
	uint32_t i;
	for ( i = 0; i < ms * CAL_FACTOR; i++) {
		Asm("nop");
	}
}
#endif /* ARDUINO */

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ASPカーネルのシステムライブラリの関数
 */
extern void target_timer_initialize(intptr_t exinf);
extern void	print_banner(intptr_t exinf) throw();
extern void	logtask_initialize(intptr_t exinf) throw();

extern void	logtask_terminate(intptr_t exinf) throw();
extern void	target_timer_terminate(intptr_t exinf) throw();

static void init_tinyusb(void);
static void init_idleloop(void);

void
inirtn(void)
{
	/*
	 *  各モジュールの初期化
	 */
	target_timer_initialize(0);
	syslog_initialize(0);
	print_banner(0);
	serial_initialize(0);
	logtask_initialize(0);

	/*
	 *  ユーザー記述の初期化関数の呼び出し
	 */
	user_inirtn();

#ifdef USE_TINYUSB
	init_tinyusb();
#endif /* USE_TINYUSB */

#ifdef ENABLE_IDLELOOP
	init_idleloop();
#endif /* ENABLE_IDLELOOP */
}

/*
 *  終了処理
 */
void
terrtn(void)
{
	/*
	 *  終了処理ルーチン
	 */
	logtask_terminate(0);
	serial_terminate(0);
	target_timer_terminate(0);

	/*
	 *  ユーザー記述の終了関数
	 */
	user_terrtn();
}

#ifdef ENABLE_IDLELOOP
extern void		loop(void);

void
loop_task(intptr_t exinf)
{
	while(1) {
		loop();
	}
}

/*
 *  Arduinoで用意しているスタックの初期値
 */
extern STK_T		*__StackTop;

static void
init_idleloop(void) {
	T_CTSK	ctsk;
	ER		ercd;

	ctsk.tskatr = TA_ACT;
	ctsk.exinf = 1;
	ctsk.task = loop_task;
	ctsk.itskpri = 15;
	ctsk.stksz = IDLELOOP_STACKSIZE;
	ctsk.stk = NULL;
	ercd = cre_tsk(IDLE_TASK, &ctsk);
	assert(ercd == E_OK);
}

#endif /* ENABLE_IDLELOOP */

/*
 *  Arduinoライブラリ関数の置き換え
 */
void
delay(unsigned long ms)
{
	dly_tsk(ms);
}

void
yield(void)
{
	dly_tsk(0);
}

#ifdef USE_TINYUSB

void
tinyusb_task_backgroud(void *arg)
{
	while (1) {
		tud_task();
		tud_cdc_write_flush();
		delay(10);
	}
}

static void
init_tinyusb(void) {
	T_CTSK	ctsk;
	ER		ercd;

	ctsk.tskatr = TA_NULL;
	ctsk.exinf = 1;
	ctsk.task = task;
	ctsk.itskpri = 1;
	ctsk.stksz = 512;
	ctsk.stk = NULL;
	ercd = cre_tsk(TINYUSB_TASK, &ctsk);
	assert(ercd == E_OK);
}
#endif /* USE_TINYUSB */

/*
 *  ベクターテーブルのリセットエントリ用のダミー
 */
__attribute__((weak))
void
_kernel__start(void)
{
}

#ifdef __cplusplus
}
#endif
