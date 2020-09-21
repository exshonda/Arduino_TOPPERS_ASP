# カスタマイズ

## このドキュメントについて
TOPPERS/ASP Arduino ライブラリ のカスタマイズ方法について説明する．

## カーネルメモリサイズの変更(kernel_cfg.h)
KMM_SIZEの値を変更することで，タスクスタック等のカーネルオブジェクトが使用するメモリのサイズを変更することが出来る．

```
/*
 *  カーネルが割り付けるメモリ領域のサイズ
 */
#define	KMM_SIZE	(1024 * 32)
```

## オブジェクト数(kernel_cfg.h/kernel_id.h)

kernel_cfg.h の以下の定義を変更すことで，カーネルオブジェクトの最大数を変更可能である．

```
/*
 *  各カーネルオブジェクトの最大登録数
 */
#define TNUM_TSKID	14		/* タスクの数 */
#define TNUM_SEMID	12		/* セマフォの数 */
#define TNUM_FLGID	10		/* イベントフラグ */
#define TNUM_DTQID	10		/* データキュー */
#define TNUM_PDQID	10		/* 優先度データキュー */
#define TNUM_MBXID	10		/* メールボックス */
#define TNUM_MPFID	10		/* 固定長メモリプール */
#define TNUM_CYCID	10		/* 周期ハンドラ */
#define TNUM_ALMID	10		/* アラームハンドラ */
#define TNUM_ISRID	10		/* 割込みサービスルーチン */
```

kernel_id.h に追加した分のオブジェクトのIDを定義する．

```
/*
 *  オブジェクトのID
 */
#define LOGTASK			1 /* 変更した場合はライブラリを作り直し */
#define TINYUSBTASK		2
#define IDLE_TASK		3
#define TASK1			4
#define TASK2			5
#define TASK3			6
#define TASK4			7
#define TASK5			8
#define TASK6			9
#define TASK7			10
#define TASK8			11
#define TASK9			12
#define TASK10			13
```

## 使用するカーネルサービスの変更(ToppersASP.cpp)

inirtn()の記述を変更することで，使用するカーネルサービスを変更可能である．

```
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

....

}
```


