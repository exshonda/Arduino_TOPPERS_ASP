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

 
#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ASPカーネルのシステムライブラリの関数
 */
void inirtn(void);

extern void target_prepare(void);

extern void target_timer_initialize(intptr_t exinf);
extern void	print_banner(intptr_t exinf) throw();
extern void	logtask_initialize(intptr_t exinf) throw();

extern void	logtask_terminate(intptr_t exinf) throw();
extern void	target_timer_terminate(intptr_t exinf) throw();

#define kerflg _kernel_kerflg
extern bool_t kerflg;

#define target_initialize _kernel_target_initialize
extern void target_initialize(void);

#define initialize_kmm _kernel_initialize_kmm
extern void initialize_kmm(void);
#define initialize_tmevt _kernel_initialize_tmevt
extern void initialize_tmevt(void);
#define initialize_object _kernel_initialize_object
extern void initialize_object(void);

#define start_dispatch				_kernel_start_dispatch
extern void start_dispatch(void) NoReturn;

#define vector_table _kernel_vector_table
extern const FP vector_table[];
#define NVIC_VECTTBL        0xE000ED08

#ifdef __cplusplus
}
#endif

#ifdef ARDUINO

/*
 *  カーネルの初期化
 */
void
preini_ker(void)
{
	/*
	 *  ターゲット依存の初期化
	 */
	target_initialize();

	/*
	 *  各モジュールの初期化
	 *
	 *  タイムイベント管理モジュールは他のモジュールより先に初期化
	 *  する必要がある．
	 */
	initialize_kmm();
	initialize_tmevt();
	initialize_object();
}

void *kernel_init __attribute__((section(".preinit_array"))) = (void *)&preini_ker;

void
initVariant(void)
{
	/* SEMCOM2初期化 */
	Serial1.begin(115200);
	Serial1.println("TOPPERS/ASP for Arduino");
	Serial1.flush();

	/*
	 *  初期化ルーチンの実行
	 */ 
	inirtn();
}

/*
 *  カーネルのスタート関数
 */
void
StartToppersASP(void)
{
	/* 割込みロック状態へ */
	Asm("cpsid f");

	/*
	 *  割込み/CPU例外の初期化処理（ターゲット依存）
	 */
	target_prepare();

	/*
	 *  カーネル動作の開始
	 */
	kerflg = true;
	start_dispatch();
	assert(0);
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

__attribute__((weak))
void
user_terrtn() {
}

#endif /* ARDUINO */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_TINYUSB
static void init_tinyusb(void);
#endif /* USE_TINYUSB */

#ifdef ENABLE_IDLELOOP
void init_idleloop(void);
#endif /* ENABLE_IDLELOOP */

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

__attribute__((weak))
void
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
	ER ercd;

	if (!kerflg){
		ToppersASPDelayMs(ms);
		return;
	}

	ercd = dly_tsk(ms);
	if (ercd != E_OK) {
		ToppersASPDelayMs(ms);
	}
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
	ctsk.task = tinyusb_task_backgroud;
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
