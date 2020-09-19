#ifndef TOPPERS_KERNEL_CFG_H
#define TOPPERS_KERNEL_CFG_H

/*
 *  各カーネルオブジェクトの最大登録数
 *
 *  シリアルドライバのTNUM_PORTが2の場合に合わせてある．
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

/*
 *  カーネルが割り付けるメモリ領域のサイズ
 */
#define	KMM_SIZE	(1024 * 16)

#endif /* TOPPERS_KERNEL_CFG_H */
