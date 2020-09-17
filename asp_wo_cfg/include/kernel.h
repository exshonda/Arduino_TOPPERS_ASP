/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2014 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: kernel.h 70 2014-04-16 23:04:43Z ertl-hiro $
 */

/*
 *		TOPPERS/ASPカーネル 標準ヘッダファイル
 *
 *  TOPPERS/ASPカーネルがサポートするサービスコールの宣言と，必要なデー
 *  タ型，定数，マクロの定義を含むヘッダファイル．
 *
 *  アセンブリ言語のソースファイルからこのファイルをインクルードする時
 *  は，TOPPERS_MACRO_ONLYを定義しておく．これにより，マクロ定義以外を
 *  除くようになっている．
 *
 *  このファイルをインクルードする前にインクルードしておくべきファイル
 *  はない．
 */

#ifndef TOPPERS_KERNEL_H
#define TOPPERS_KERNEL_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	TOPPERS共通のデータ型・定数・マクロ
 */
#ifndef ARDUINO_PASS
#include <t_stddef.h>
#else /* ARDUINO_PASS */
#include "../asp_wo_cfg/include/t_stddef.h"
#endif /* ARDUINO_PASS */

/*
 *  ターゲット依存部
 */
#ifndef ARDUINO_PASS
#include "target_kernel.h"
#else /* ARDUINO_PASS */
#include "../asp_wo_cfg/target/samd51_gcc/target_kernel.h"
#endif /* ARDUINO_PASS */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  データ型の定義
 */

/*
 *  ビットパターンやオブジェクト番号の型定義
 */
typedef	uint_t		TEXPTN;		/* タスク例外要因のビットパターン */
typedef	uint_t		FLGPTN;		/* イベントフラグのビットパターン */
typedef	uint_t		INTNO;		/* 割込み番号 */
typedef	uint_t		INHNO;		/* 割込みハンドラ番号 */
typedef	uint_t		EXCNO;		/* CPU例外ハンドラ番号 */

/*
 *  処理単位の型定義
 */
typedef void	(*TASK)(intptr_t exinf);
typedef void	(*TEXRTN)(TEXPTN texptn, intptr_t exinf);
typedef void	(*CYCHDR)(intptr_t exinf);
typedef void	(*ALMHDR)(intptr_t exinf);
typedef void	(*ISR)(intptr_t exinf);
typedef void	(*INTHDR)(void);
typedef void	(*EXCHDR)(void *p_excinf);
typedef void	(*INIRTN)(intptr_t exinf);
typedef void	(*TERRTN)(intptr_t exinf);

/*
 *  メモリ領域確保のための型定義
 */
#ifndef TOPPERS_STK_T
#define TOPPERS_STK_T	intptr_t
#endif /* TOPPERS_STK_T */
typedef	TOPPERS_STK_T	STK_T;	/* スタック領域を確保するための型 */

#ifndef TOPPERS_MPF_T
#define TOPPERS_MPF_T	intptr_t
#endif /* TOPPERS_MPF_T */
typedef	TOPPERS_MPF_T	MPF_T;	/* 固定長メモリプール領域を確保するための型 */

/*
 *  メッセージヘッダの型定義
 */
typedef	struct t_msg {			/* メールボックスのメッセージヘッダ */
	struct t_msg	*pk_next;
} T_MSG;

typedef	struct t_msg_pri {		/* 優先度付きメッセージヘッダ */
	T_MSG	msgque;				/* メッセージヘッダ */
	PRI		msgpri;				/* メッセージ優先度 */
} T_MSG_PRI;

/*
 *  パケット形式の定義
 */
typedef struct t_ctsk {
	ATR			tskatr;		/* タスク属性 */
	intptr_t	exinf;		/* タスクの拡張情報 */
	TASK		task;		/* タスクのメインルーチンの先頭番地 */
	PRI			itskpri;	/* タスクの起動時優先度 */
	SIZE		stksz;		/* タスクのスタック領域のサイズ */
	STK_T 		*stk;		/* タスクのスタック領域の先頭番地 */
} T_CTSK;

typedef struct t_rtsk {
	STAT	tskstat;	/* タスク状態 */
	PRI		tskpri;		/* タスクの現在優先度 */
	PRI		tskbpri;	/* タスクのベース優先度 */
	STAT	tskwait;	/* 待ち要因 */
	ID		wobjid;		/* 待ち対象のオブジェクトのID */
	TMO		lefttmo;	/* タイムアウトするまでの時間 */
	uint_t	actcnt;		/* 起動要求キューイング数 */
	uint_t	wupcnt;		/* 起床要求キューイング数 */
} T_RTSK;

typedef struct t_dtex {
	ATR		texatr;		/* タスク例外処理ルーチン属性 */
	TEXRTN	texrtn;		/* タスク例外処理ルーチンの先頭番地 */
} T_DTEX;

typedef struct t_rtex {
	STAT	texstat;	/* タスク例外処理の状態 */
	TEXPTN	pndptn;		/* 保留例外要因 */
} T_RTEX;

typedef struct t_csem {
	ATR		sematr;		/* セマフォ属性 */
	uint_t	isemcnt;	/* セマフォの初期資源数 */
	uint_t	maxsem;		/* セマフォの最大資源数 */
} T_CSEM;

typedef struct t_rsem {
	ID		wtskid;		/* セマフォの待ち行列の先頭のタスクのID番号 */
	uint_t	semcnt;		/* セマフォの現在の資源数 */
} T_RSEM;

typedef struct t_cflg {
	ATR		flgatr;		/* イベントフラグ属性 */
	FLGPTN	iflgptn;	/* イベントフラグの初期ビットパターン */
} T_CFLG;

typedef struct t_rflg {
	ID		wtskid;		/* イベントフラグの待ち行列の先頭のタスクのID番号 */
	FLGPTN	flgptn;		/* イベントフラグの現在のビットパターン */
} T_RFLG;

typedef struct t_cdtq {
	ATR		dtqatr;		/* データキュー属性 */
	uint_t	dtqcnt;		/* データキュー管理領域に格納できるデータ数 */
	void 	*dtqmb;		/* データキュー管理領域の先頭番地 */
} T_CDTQ;

typedef struct t_rdtq {
	ID		stskid;		/* データキューの送信待ち行列の先頭のタスクのID番号 */
	ID		rtskid;		/* データキューの受信待ち行列の先頭のタスクのID番号 */
	uint_t	sdtqcnt;	/* データキュー管理領域に格納されているデータの数 */
} T_RDTQ;

typedef struct t_cpdq {
	ATR		pdqatr;		/* 優先度データキュー属性 */
	uint_t	pdqcnt;		/* 優先度データキュー管理領域に格納できるデータ数 */
	PRI		maxdpri;	/* 優先度データキューに送信できるデータ優先度の最
						   大値 */
	void 	*pdqmb;		/* 優先度データキュー管理領域の先頭番地 */
} T_CPDQ;

typedef struct t_rpdq {
	ID		stskid;		/* 優先度データキューの送信待ち行列の先頭のタスク
						   のID番号 */
	ID		rtskid;		/* 優先度データキューの受信待ち行列の先頭のタスク
						   のID番号 */
	uint_t	spdqcnt;	/* 優先度データキュー管理領域に格納されているデー
						   タの数 */
} T_RPDQ;

typedef struct t_cmbx {
	ATR		mbxatr;		/* メールボックス属性 */
	PRI		maxmpri;	/* 優先度メールボックスに送信できるメッセージ優先
						   度の最大値 */
	void 	*mprihd;	/* 優先度別のメッセージキューヘッダ領域の先頭番地 */
} T_CMBX;

typedef struct t_rmbx {
	ID		wtskid;		/* メールボックスの待ち行列の先頭のタスクのID番号 */
	T_MSG	*pk_msg;	/* メッセージキューの先頭につながれたメッセージ
						   の先頭番地 */
} T_RMBX;

typedef struct t_cmpf {
	ATR		mpfatr;		/* 固定長メモリプール属性 */
	uint_t	blkcnt;		/* 獲得できる固定長メモリブロックの数 */
	uint_t	blksz;		/* 固定長メモリブロックのサイズ */
	MPF_T 	*mpf;		/* 固定長メモリプール領域の先頭番地 */
	void 	*mpfmb;		/* 固定長メモリプール管理領域の先頭番地 */
} T_CMPF;

typedef struct t_rmpf {
	ID		wtskid;		/* 固定長メモリプールの待ち行列の先頭のタスクの
						   ID番号 */
	uint_t	fblkcnt;	/* 固定長メモリプール領域の空きメモリ領域に割り
						   付けることができる固定長メモリブロックの数 */
} T_RMPF;

typedef struct t_ccyc {
	ATR			cycatr;		/* 周期ハンドラ属性 */
	intptr_t	exinf;		/* 周期ハンドラの拡張情報 */
	CYCHDR		cychdr;		/* 周期ハンドラの先頭番地 */
	RELTIM		cyctim;		/* 周期ハンドラの起動周期 */
	RELTIM		cycphs;		/* 周期ハンドラの起動位相 */
} T_CCYC;

typedef struct t_rcyc {
	STAT	cycstat;	/* 周期ハンドラの動作状態 */
	RELTIM	lefttim;	/* 次に周期ハンドラを起動する時刻までの相対時間 */
} T_RCYC;

typedef struct t_calm {
	ATR			almatr;		/* アラームハンドラ属性 */
	intptr_t	exinf;		/* アラームハンドラの拡張情報 */
	ALMHDR		almhdr;		/* アラームハンドラの先頭番地 */
} T_CALM;

typedef struct t_ralm {
	STAT	almstat;	/* アラームハンドラの動作状態 */
	RELTIM	lefttim;	/* アラームハンドラを起動する時刻までの相対時間 */
} T_RALM;

typedef struct t_cint {
	ATR			intatr;		/* 割込み要求ライン属性 */
	PRI			intpri;		/* 割込み優先度 */
} T_CINT;

typedef struct t_cisr {
	ATR			isratr;		/* 割込みサービスルーチン属性 */
	intptr_t	exinf;		/* 割込みサービスルーチンの拡張情報 */
	INTNO		intno;		/* 割込みサービスルーチンを登録する割込み番号 */
	ISR			isr;		/* 割込みサービスルーチンの先頭番地 */
	PRI			isrpri;		/* 割込みサービスルーチン優先度 */
} T_CISR;

typedef struct t_dinh {
	ATR			inhatr;		/* 割込みハンドラ属性 */
	INTHDR		inthdr;		/* 割込みハンドラの先頭番地 */
} T_DINH;

typedef struct t_dexc {
	ATR			excatr;		/* CPU例外ハンドラ属性 */
	EXCHDR		exchdr;		/* CPU例外ハンドラの先頭番地 */
} T_DEXC;

/*
 *  サービスコールの宣言
 */

/*
 *  タスク管理機能
 */
extern ER		cre_tsk(ID tskid, const T_CTSK *pk_ctsk) throw();
extern ER		act_tsk(ID tskid) throw();
extern ER		iact_tsk(ID tskid) throw();
extern ER_UINT	can_act(ID tskid) throw();
extern ER		ext_tsk(void) throw();
extern ER		ter_tsk(ID tskid) throw();
extern ER		chg_pri(ID tskid, PRI tskpri) throw();
extern ER		get_pri(ID tskid, PRI *p_tskpri) throw();
extern ER		get_inf(intptr_t *p_exinf) throw();
extern ER		ref_tsk(ID tskid, T_RTSK *pk_rtsk) throw();

/*
 *  タスク付属同期機能
 */
extern ER		slp_tsk(void) throw();
extern ER		tslp_tsk(TMO tmout) throw();
extern ER		wup_tsk(ID tskid) throw();
extern ER		iwup_tsk(ID tskid) throw();
extern ER_UINT	can_wup(ID tskid) throw();
extern ER		rel_wai(ID tskid) throw();
extern ER		irel_wai(ID tskid) throw();
extern ER		sus_tsk(ID tskid) throw();
extern ER		rsm_tsk(ID tskid) throw();
extern ER		dly_tsk(RELTIM dlytim) throw();

/*
 *  タスク例外処理機能
 */
extern ER		def_tex(ID tskid, const T_DTEX *pk_dtex) throw();
extern ER		ras_tex(ID tskid, TEXPTN rasptn) throw();
extern ER		iras_tex(ID tskid, TEXPTN rasptn) throw();
extern ER		dis_tex(void) throw();
extern ER		ena_tex(void) throw();
extern bool_t	sns_tex(void) throw();
extern ER		ref_tex(ID tskid, T_RTEX *pk_rtex) throw();

/*
 *  同期・通信機能
 */
extern ER		cre_sem(ID semid, const T_CSEM *pk_csem) throw();
extern ER		sig_sem(ID semid) throw();
extern ER		isig_sem(ID semid) throw();
extern ER		wai_sem(ID semid) throw();
extern ER		pol_sem(ID semid) throw();
extern ER		twai_sem(ID semid, TMO tmout) throw();
extern ER		ini_sem(ID semid) throw();
extern ER		ref_sem(ID semid, T_RSEM *pk_rsem) throw();

extern ER		cre_flg(ID flgid, const T_CFLG *pk_cflg) throw();
extern ER		set_flg(ID flgid, FLGPTN setptn) throw();
extern ER		iset_flg(ID flgid, FLGPTN setptn) throw();
extern ER		clr_flg(ID flgid, FLGPTN clrptn) throw();
extern ER		wai_flg(ID flgid, FLGPTN waiptn,
						MODE wfmode, FLGPTN *p_flgptn) throw();
extern ER		pol_flg(ID flgid, FLGPTN waiptn,
						MODE wfmode, FLGPTN *p_flgptn) throw();
extern ER		twai_flg(ID flgid, FLGPTN waiptn,
						MODE wfmode, FLGPTN *p_flgptn, TMO tmout) throw();
extern ER		ini_flg(ID flgid) throw();
extern ER		ref_flg(ID flgid, T_RFLG *pk_rflg) throw();

extern ER		cre_dtq(ID dtqid, const T_CDTQ *pk_cdtq) throw();
extern ER		snd_dtq(ID dtqid, intptr_t data) throw();
extern ER		psnd_dtq(ID dtqid, intptr_t data) throw();
extern ER		ipsnd_dtq(ID dtqid, intptr_t data) throw();
extern ER		tsnd_dtq(ID dtqid, intptr_t data, TMO tmout) throw();
extern ER		fsnd_dtq(ID dtqid, intptr_t data) throw();
extern ER		ifsnd_dtq(ID dtqid, intptr_t data) throw();
extern ER		rcv_dtq(ID dtqid, intptr_t *p_data) throw();
extern ER		prcv_dtq(ID dtqid, intptr_t *p_data) throw();
extern ER		trcv_dtq(ID dtqid, intptr_t *p_data, TMO tmout) throw();
extern ER		ini_dtq(ID dtqid) throw();
extern ER		ref_dtq(ID dtqid, T_RDTQ *pk_rdtq) throw();

extern ER		cre_pdq(ID pdqid, const T_CPDQ *pk_cpdq) throw();
extern ER		snd_pdq(ID pdqid, intptr_t data, PRI datapri) throw();
extern ER		psnd_pdq(ID pdqid, intptr_t data, PRI datapri) throw();
extern ER		ipsnd_pdq(ID pdqid, intptr_t data, PRI datapri) throw();
extern ER		tsnd_pdq(ID pdqid, intptr_t data,
										PRI datapri, TMO tmout) throw();
extern ER		rcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri) throw();
extern ER		prcv_pdq(ID pdqid, intptr_t *p_data, PRI *p_datapri) throw();
extern ER		trcv_pdq(ID pdqid, intptr_t *p_data,
										PRI *p_datapri, TMO tmout) throw();
extern ER		ini_pdq(ID pdqid) throw();
extern ER		ref_pdq(ID pdqid, T_RPDQ *pk_rpdq) throw();

extern ER		cre_mbx(ID mbxid, const T_CMBX *pk_cmbx) throw();
extern ER		snd_mbx(ID mbxid, T_MSG *pk_msg) throw();
extern ER		rcv_mbx(ID mbxid, T_MSG **ppk_msg) throw();
extern ER		prcv_mbx(ID mbxid, T_MSG **ppk_msg) throw();
extern ER		trcv_mbx(ID mbxid, T_MSG **ppk_msg, TMO tmout) throw();
extern ER		ini_mbx(ID mbxid) throw();
extern ER		ref_mbx(ID mbxid, T_RMBX *pk_rmbx) throw();

/*
 *  メモリプール管理機能
 */
extern ER		cre_mpf(ID mpfid, const T_CMPF *pk_cmpf) throw();
extern ER		get_mpf(ID mpfid, void **p_blk) throw();
extern ER		pget_mpf(ID mpfid, void **p_blk) throw();
extern ER		tget_mpf(ID mpfid, void **p_blk, TMO tmout) throw();
extern ER		rel_mpf(ID mpfid, void *blk) throw();
extern ER		ini_mpf(ID mpfid) throw();
extern ER		ref_mpf(ID mpfid, T_RMPF *pk_rmpf) throw();

/*
 *  時間管理機能
 */
extern ER		get_tim(SYSTIM *p_systim) throw();
extern ER		get_utm(SYSUTM *p_sysutm) throw();

extern ER		cre_cyc(ID cycid, const T_CCYC *pk_ccyc) throw();
extern ER		sta_cyc(ID cycid) throw();
extern ER		stp_cyc(ID cycid) throw();
extern ER		ref_cyc(ID cycid, T_RCYC *pk_rcyc) throw();

extern ER		cre_alm(ID almid, const T_CALM *pk_calm) throw();
extern ER		sta_alm(ID almid, RELTIM almtim) throw();
extern ER		ista_alm(ID almid, RELTIM almtim) throw();
extern ER		stp_alm(ID almid) throw();
extern ER		istp_alm(ID almid) throw();
extern ER		ref_alm(ID almid, T_RALM *pk_ralm) throw();

/*
 *  システム状態管理機能
 */
extern ER		rot_rdq(PRI tskpri) throw();
extern ER		irot_rdq(PRI tskpri) throw();
extern ER		get_tid(ID *p_tskid) throw();
extern ER		iget_tid(ID *p_tskid) throw();
extern ER		loc_cpu(void) throw();
extern ER		iloc_cpu(void) throw();
extern ER		unl_cpu(void) throw();
extern ER		iunl_cpu(void) throw();
extern ER		dis_dsp(void) throw();
extern ER		ena_dsp(void) throw();
extern bool_t	sns_ctx(void) throw();
extern bool_t	sns_loc(void) throw();
extern bool_t	sns_dsp(void) throw();
extern bool_t	sns_dpn(void) throw();
extern bool_t	sns_ker(void) throw();
extern ER		ext_ker(void) throw();

/*
 *  割込み管理機能
 */
extern ER		cfg_int(INTNO intno, const T_CINT *pk_cint) throw();
extern ER		att_isr(const T_CISR *pk_cisr) throw();
extern ER		def_inh(INHNO inhno, const T_DINH *pk_dinh) throw();
extern ER		dis_int(INTNO intno) throw();
extern ER		ena_int(INTNO intno) throw();
extern ER		chg_ipm(PRI intpri) throw();
extern ER		get_ipm(PRI *p_intpri) throw();

/*
 *  CPU例外管理機能
 */
extern ER		def_exc(EXCNO excno, const T_DEXC *pk_dexc) throw();
extern bool_t	xsns_dpn(void *p_excinf) throw();
extern bool_t	xsns_xpn(void *p_excinf) throw();

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  オブジェクト属性の定義
 */
#define TA_ACT			UINT_C(0x02)	/* タスクを起動された状態で生成 */

#define TA_TPRI			UINT_C(0x01)	/* タスクの待ち行列を優先度順に */
#define TA_MPRI			UINT_C(0x02)	/* メッセージキューを優先度順に */

#define TA_WMUL			UINT_C(0x02)	/* 複数の待ちタスク */
#define TA_CLR			UINT_C(0x04)	/* イベントフラグのクリア指定 */

#define TA_STA			UINT_C(0x02)	/* 周期ハンドラを動作状態で生成 */

#define TA_NONKERNEL	UINT_C(0x02)	/* カーネル管理外の割込み */

#define TA_ENAINT		UINT_C(0x01)	/* 割込み要求禁止フラグをクリア */
#define TA_EDGE			UINT_C(0x02)	/* エッジトリガ */

/*
 *  サービスコールの動作モードの定義
 */
#define TWF_ORW			UINT_C(0x01)	/* イベントフラグのOR待ち */
#define TWF_ANDW		UINT_C(0x02)	/* イベントフラグのAND待ち */

/*
 *  オブジェクトの状態の定義
 */
#define TTS_RUN			UINT_C(0x01)	/* 実行状態 */
#define TTS_RDY			UINT_C(0x02)	/* 実行可能状態 */
#define TTS_WAI			UINT_C(0x04)	/* 待ち状態 */
#define TTS_SUS			UINT_C(0x08)	/* 強制待ち状態 */
#define TTS_WAS			UINT_C(0x0c)	/* 二重待ち状態 */
#define TTS_DMT			UINT_C(0x10)	/* 休止状態 */

#define TTW_SLP			UINT_C(0x0001)	/* 起床待ち */
#define TTW_DLY			UINT_C(0x0002)	/* 時間経過待ち */
#define TTW_SEM			UINT_C(0x0004)	/* セマフォの資源獲得待ち */
#define TTW_FLG			UINT_C(0x0008)	/* イベントフラグ待ち */
#define TTW_SDTQ		UINT_C(0x0010)	/* データキューへの送信待ち */
#define TTW_RDTQ		UINT_C(0x0020)	/* データキューからの受信待ち */
#define TTW_SPDQ		UINT_C(0x0100)	/* 優先度データキューへの送信待ち */
#define TTW_RPDQ		UINT_C(0x0200)	/* 優先度データキューからの受信待ち */
#define TTW_MBX			UINT_C(0x0040)	/* メールボックスからの受信待ち */
#define TTW_MPF			UINT_C(0x2000)	/* 固定長メモリブロックの獲得待ち */

#define TTEX_ENA		UINT_C(0x01)	/* タスク例外処理許可状態 */
#define TTEX_DIS		UINT_C(0x02)	/* タスク例外処理禁止状態 */

#define TCYC_STP		UINT_C(0x01)	/* 周期ハンドラが動作していない */
#define TCYC_STA		UINT_C(0x02)	/* 周期ハンドラが動作している */

#define TALM_STP		UINT_C(0x01)	/* アラームハンドラが動作していない */
#define TALM_STA		UINT_C(0x02)	/* アラームハンドラが動作している */

/*
 *  その他の定数の定義
 */
#define TSK_SELF		0			/* 自タスク指定 */
#define TSK_NONE		0			/* 該当するタスクがない */

#define TPRI_SELF		0			/* 自タスクのベース優先度 */
#define TPRI_INI		0			/* タスクの起動時優先度 */

#define TIPM_ENAALL		0			/* 割込み優先度マスク全解除 */

/*
 *  構成定数とマクロ
 */

/*
 *  サポートする機能
 */
#ifdef TOPPERS_TARGET_SUPPORT_DIS_INT
#define TOPPERS_SUPPORT_DIS_INT			/* dis_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_DIS_INT */

#ifdef TOPPERS_TARGET_SUPPORT_ENA_INT
#define TOPPERS_SUPPORT_ENA_INT			/* ena_intがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_ENA_INT */

#ifdef TOPPERS_TARGET_SUPPORT_GET_UTM
#define TOPPERS_SUPPORT_GET_UTM			/* get_utmがサポートされている */
#endif /* TOPPERS_TARGET_SUPPORT_GET_UTM */

/*
 *  優先度の範囲
 */
#define TMIN_TPRI		1			/* タスク優先度の最小値（最高値）*/
#define TMAX_TPRI		16			/* タスク優先度の最大値（最低値）*/
#define TMIN_DPRI		1			/* データ優先度の最小値（最高値）*/
#define TMAX_DPRI		16			/* データ優先度の最大値（最低値）*/
#define TMIN_MPRI		1			/* メッセージ優先度の最小値（最高値）*/
#define TMAX_MPRI		16			/* メッセージ優先度の最大値（最低値）*/
#define TMIN_ISRPRI		1			/* 割込みサービスルーチン優先度の最小値 */
#define TMAX_ISRPRI		16			/* 割込みサービスルーチン優先度の最大値 */

/*
 *  バージョン情報
 */
#define TKERNEL_MAKER	UINT_C(0x0118)	/* カーネルのメーカーコード */
#define TKERNEL_PRID	UINT_C(0x0007)	/* カーネルの識別番号 */
#define TKERNEL_SPVER	UINT_C(0xf516)	/* カーネル仕様のバージョン番号 */
#define TKERNEL_PRVER	UINT_C(0x1091)	/* カーネルのバージョン番号 */

/*
 *  キューイング回数の最大値
 */
#define TMAX_ACTCNT		UINT_C(1)		/* 起動要求キューイング数の最大値 */
#define TMAX_WUPCNT		UINT_C(1)		/* 起床要求キューイング数の最大値 */

/*
 *  ビットパターンのビット数
 */
#ifndef TBIT_TEXPTN					/* タスク例外要因のビット数 */
#define TBIT_TEXPTN		(sizeof(TEXPTN) * CHAR_BIT)
#endif /* TBIT_TEXPTN */

#ifndef TBIT_FLGPTN					/* イベントフラグのビット数 */
#define TBIT_FLGPTN		(sizeof(FLGPTN) * CHAR_BIT)
#endif /* TBIT_FLGPTN */

/*
 *  メモリ領域確保のためのマクロ
 *
 *  以下のTOPPERS_COUNT_SZとTOPPERS_ROUND_SZの定義は，unitが2の巾乗であ
 *  ることを仮定している．
 */
#ifndef TOPPERS_COUNT_SZ
#define TOPPERS_COUNT_SZ(sz, unit)	(((sz) + (unit) - 1) / (unit))
#endif /* TOPPERS_COUNT_SZ */
#ifndef TOPPERS_ROUND_SZ
#define TOPPERS_ROUND_SZ(sz, unit)	(((sz) + (unit) - 1) & ~((unit) - 1))
#endif /* TOPPERS_ROUND_SZ */

#define COUNT_STK_T(sz)		TOPPERS_COUNT_SZ(sz, sizeof(STK_T))
#define ROUND_STK_T(sz)		TOPPERS_ROUND_SZ(sz, sizeof(STK_T))

#define COUNT_MPF_T(blksz)	TOPPERS_COUNT_SZ(blksz, sizeof(MPF_T))
#define ROUND_MPF_T(blksz)	TOPPERS_ROUND_SZ(blksz, sizeof(MPF_T))

#define TSZ_DTQMB(dtqcnt)	(sizeof(intptr_t) * (dtqcnt))
#define TCNT_DTQMB(dtqcnt)	TOPPERS_COUNT_SZ(TSZ_DTQMB(dtqcnt), sizeof(MB_T))

#ifndef TSZ_PDQMB
#define TSZ_PDQMB(pdqcnt)	(sizeof(intptr_t) * 3 * (pdqcnt))
#endif /* TSZ_PDQMB */
#define TCNT_PDQMB(pdqcnt)	TOPPERS_COUNT_SZ(TSZ_PDQMB(pdqcnt), sizeof(MB_T))

#define TSZ_MPFMB(blkcnt)	(sizeof(uint_t) * (blkcnt))
#define TCNT_MPFMB(blkcnt)	TOPPERS_COUNT_SZ(TSZ_MPFMB(blkcnt), sizeof(MB_T))

/*
 *  その他の構成定数
 */
#define TMAX_MAXSEM		UINT_MAX	/* セマフォの最大資源数の最大値 */

#ifdef __cplusplus
}
#endif

#endif /* TOPPERS_KERNEL_H */
