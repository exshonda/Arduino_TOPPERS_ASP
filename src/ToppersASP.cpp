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
