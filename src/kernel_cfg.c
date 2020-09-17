/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2012 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: kernel_cfg.c 49 2012-09-06 04:41:53Z ertl-hiro $
 */

#define ARDUINO_PASS

#ifndef ARDUINO_PASS
/*
 *  カーネル標準ヘッダファイル
 */
#include "kernel_impl.h"

/*
 *  カーネルの各ヘッダファイル
 */
#include "task.h"
#include "semaphore.h"
#include "eventflag.h"
#include "dataqueue.h"
#include "pridataq.h"
#include "mailbox.h"
#include "mempfix.h"
#include "cyclic.h"
#include "alarm.h"
#include "interrupt.h"
#include "exception.h"
#include "time_event.h"

/*
 *  カーネルコンフィギュレーション記述ヘッダファイル
 */
#include "kernel_cfg.h"
#else /* ARDUINO_PASS */
/*
 *  カーネル標準ヘッダファイル
 */
#include "../asp_wo_cfg/kernel/kernel_impl.h"

/*
 *  カーネルの各ヘッダファイル
 */
#include "../asp_wo_cfg/kernel/task.h"
#include "../asp_wo_cfg/kernel/semaphore.h"
#include "../asp_wo_cfg/kernel/eventflag.h"
#include "../asp_wo_cfg/kernel/dataqueue.h"
#include "../asp_wo_cfg/kernel/pridataq.h"
#include "../asp_wo_cfg/kernel/mailbox.h"
#include "../asp_wo_cfg/kernel/mempfix.h"
#include "../asp_wo_cfg/kernel/cyclic.h"
#include "../asp_wo_cfg/kernel/alarm.h"
#include "../asp_wo_cfg/kernel/interrupt.h"
#include "../asp_wo_cfg/kernel/exception.h"
#include "../asp_wo_cfg/kernel/time_event.h"

/*
 *  カーネルコンフィギュレーション記述ヘッダファイル
 */
#include "kernel_cfg.h"
#endif /* ARDUINO_PASS */

/*
 *  内容が空の配列を生成するためのマクロ
 */
#ifndef TOPPERS_EMPTY_LABEL
#define TOPPERS_EMPTY_LABEL(type, var)		type var[0]
#endif

/*
 *  タスク管理機能
 */

const ID tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

#if TNUM_TSKID > 0

TINIB tinib_table[TNUM_TSKID];
TCB tcb_table[TNUM_TSKID];

#else /* TNUM_TSKID > 0 */

#error TNUM_TSKID must not be 0.

#endif /* TNUM_TSKID > 0 */

/*
 *  セマフォ機能
 */

const ID tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

#if TNUM_SEMID > 0

SEMINIB seminib_table[TNUM_SEMID];
SEMCB semcb_table[TNUM_SEMID];

#else /* TNUM_SEMID > 0 */

TOPPERS_EMPTY_LABEL(SEMINIB, seminib_table);
TOPPERS_EMPTY_LABEL(SEMCB, semcb_table);

#endif /* TNUM_SEMID > 0 */

/*
 *  イベントフラグ機能
 */

const ID tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

#if TNUM_FLGID > 0

FLGINIB flginib_table[TNUM_FLGID];
FLGCB flgcb_table[TNUM_FLGID];

#else /* TNUM_FLGID > 0 */

TOPPERS_EMPTY_LABEL(FLGINIB, flginib_table);
TOPPERS_EMPTY_LABEL(FLGCB, flgcb_table);

#endif /* TNUM_FLGID > 0 */

/*
 *  データキュー機能
 */

const ID tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

#if TNUM_DTQID > 0

DTQINIB dtqinib_table[TNUM_DTQID];
DTQCB dtqcb_table[TNUM_DTQID];

#else /* TNUM_DTQID > 0 */

TOPPERS_EMPTY_LABEL(DTQINIB, dtqinib_table);
TOPPERS_EMPTY_LABEL(DTQCB, dtqcb_table);

#endif /* TNUM_DTQID > 0 */

/*
 *  優先度データキュー機能
 */

const ID tmax_pdqid = (TMIN_PDQID + TNUM_PDQID - 1);

#if TNUM_PDQID > 0

PDQINIB pdqinib_table[TNUM_PDQID];
PDQCB pdqcb_table[TNUM_PDQID];

#else /* TNUM_PDQID > 0 */

TOPPERS_EMPTY_LABEL(PDQINIB, pdqinib_table);
TOPPERS_EMPTY_LABEL(PDQCB, pdqcb_table);

#endif /* TNUM_PDQID > 0 */

/*
 *  メールボックス機能
 */

const ID tmax_mbxid = (TMIN_MBXID + TNUM_MBXID - 1);

#if TNUM_MBXID > 0

MBXINIB mbxinib_table[TNUM_MBXID];
MBXCB mbxcb_table[TNUM_MBXID];

#else /* TNUM_MBXID > 0 */

TOPPERS_EMPTY_LABEL(MBXINIB, mbxinib_table);
TOPPERS_EMPTY_LABEL(MBXCB, mbxcb_table);

#endif /* TNUM_MBXID > 0 */

/*
 *  固定長メモリプール機能
 */

const ID tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

#if TNUM_MPFID > 0

MPFINIB mpfinib_table[TNUM_MPFID];
MPFCB mpfcb_table[TNUM_MPFID];

#else /* TNUM_MPFID > 0 */

TOPPERS_EMPTY_LABEL(MPFINIB, mpfinib_table);
TOPPERS_EMPTY_LABEL(MPFCB, mpfcb_table);

#endif /* TNUM_MPFID > 0 */

/*
 *  周期ハンドラ機能
 */

const ID tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

#if TNUM_CYCID > 0

CYCINIB cycinib_table[TNUM_CYCID];
CYCCB cyccb_table[TNUM_CYCID];

#else /* TNUM_CYCID > 0 */

TOPPERS_EMPTY_LABEL(CYCINIB, cycinib_table);
TOPPERS_EMPTY_LABEL(CYCCB, cyccb_table);

#endif /* TNUM_CYCID > 0 */

/*
 *  アラームハンドラ機能
 */

const ID tmax_almid = (TMIN_ALMID + TNUM_ALMID - 1);

#if TNUM_ALMID > 0

ALMINIB alminib_table[TNUM_ALMID];
ALMCB almcb_table[TNUM_ALMID];

#else /* TNUM_ALMID > 0 */

TOPPERS_EMPTY_LABEL(ALMINIB, alminib_table);
TOPPERS_EMPTY_LABEL(ALMCB, almcb_table);

#endif /* TNUM_ALMID > 0 */

/*
 *  割込み管理機能
 */

const ID tmax_isrid = (TMIN_ISRID + TNUM_ISRID - 1);

#if TNUM_ISRID > 0

ISRINIB isrinib_table[TNUM_ISRID];
ISRCB isrcb_table[TNUM_ISRID];

#else /* TNUM_ISRID > 0 */

TOPPERS_EMPTY_LABEL(ISRINIB, isrinib_table);
TOPPERS_EMPTY_LABEL(ISRCB, isrcb_table);

#endif /* TNUM_ISRID > 0 */

/*
 *  非タスクコンテキスト用スタック領域
 */

#ifdef ISTK

#define TOPPERS_ISTKSZ	ISTKSZ
#define TOPPERS_ISTK	ISTK

#else /* ISTK */
#ifdef ISTKSZ

static STK_T			istack[COUNT_STK_T(ISTKSZ)];
#define TOPPERS_ISTKSZ	ROUND_STK_T(ISTKSZ)
#define TOPPERS_ISTK	istack

#else /* ISTKSZ */
#ifdef DEFAULT_ISTK

#define TOPPERS_ISTKSZ	DEFAULT_ISTKSZ
#define TOPPERS_ISTK	DEFAULT_ISTK

#else /* DEAULT_ISTK */

static STK_T			istack[COUNT_STK_T(DEFAULT_ISTKSZ)];
#define TOPPERS_ISTKSZ	ROUND_STK_T(DEFAULT_ISTKSZ)
#define TOPPERS_ISTK	istack

#endif /* DEAULT_ISTK */
#endif /* ISTKSZ */
#endif /* ISTK */

const SIZE		istksz = TOPPERS_ISTKSZ;
STK_T *const	istk = TOPPERS_ISTK;

#ifdef TOPPERS_ISTKPT
STK_T *const	istkpt = TOPPERS_ISTKPT(TOPPERS_ISTK, TOPPERS_ISTKSZ);
#endif /* TOPPERS_ISTKPT */

/*
 *  Memory Area Allocated by Kernel
 */

#ifdef KMEM

#define TOPPERS_KMEMSZ	KMEMSZ
#define TOPPERS_KMEM	KMEM

#else /* KMEM */

static MB_T				memory[TOPPERS_COUNT_SZ(KMM_SIZE, sizeof(MB_T))];
#define TOPPERS_KMMSZ	TOPPERS_ROUND_SZ(KMM_SIZE, sizeof(MB_T))
#define TOPPERS_KMM		memory

#endif /* KMEM */

const SIZE		kmmsz = TOPPERS_KMMSZ;
MB_T *const		kmm = TOPPERS_KMM;

/*
 *  タイムイベント管理
 */

TMEVTN   tmevt_heap[TNUM_TSKID + TNUM_CYCID + TNUM_ALMID];

/*
 *  各モジュールの初期化処理
 */

void
initialize_object(void)
{
	initialize_task();
#if TNUM_SEMID > 0
	initialize_semaphore();
#endif /* TNUM_SEMID > 0 */
#if TNUM_FLGID > 0
	initialize_eventflag();
#endif /* TNUM_FLGID > 0 */
#if TNUM_DTQID > 0
	initialize_dataqueue();
#endif /* TNUM_DTQID > 0 */
#if TNUM_PDQID > 0
	initialize_pridataq();
#endif /* TNUM_PDQID > 0 */
#if TNUM_MBXID > 0
	initialize_mailbox();
#endif /* TNUM_MBXID > 0 */
#if TNUM_MPFID > 0
	initialize_mempfix();
#endif /* TNUM_MPFID > 0 */
#if TNUM_CYCID > 0
	initialize_cyclic();
#endif /* TNUM_CYCID > 0 */
#if TNUM_ALMID > 0
	initialize_alarm();
#endif /* TNUM_ALMID > 0 */
	initialize_interrupt();
#if TNUM_ISRID > 0
	initialize_isr();
#endif /* TNUM_ISRID > 0 */
	initialize_exception();
}
