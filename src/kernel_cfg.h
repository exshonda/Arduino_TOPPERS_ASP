#ifndef TOPPERS_KERNEL_CFG_H
#define TOPPERS_KERNEL_CFG_H

/*
 *  各カーネルオブジェクトの最大登録数
 *
 *  シリアルドライバのTNUM_PORTが2の場合に合わせてある．
 */
#define TNUM_TSKID	5		/* タスクの数 */
#define TNUM_SEMID	4		/* セマフォの数 */
#define TNUM_FLGID	0		/* イベントフラグ */
#define TNUM_DTQID	0		/* データキュー */
#define TNUM_PDQID	0		/* 優先度データキュー */
#define TNUM_MBXID	0		/* メールボックス */
#define TNUM_MPFID	0		/* 固定長メモリプール */
#define TNUM_CYCID	1		/* 周期ハンドラ */
#define TNUM_ALMID	1		/* アラームハンドラ */
#define TNUM_ISRID	2		/* 割込みサービスルーチン */

/*
 *  カーネルが割り付けるメモリ領域のサイズ
 */
#define	KMM_SIZE	(1024 * 16)

#endif /* TOPPERS_KERNEL_CFG_H */
