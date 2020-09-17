/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: interrupt.h 49 2012-09-06 04:41:53Z ertl-hiro $
 */

/*
 *		割込み管理機能
 */

#ifndef TOPPERS_INTERRUPT_H
#define TOPPERS_INTERRUPT_H

/*
 *  割込み番号を引数に取る割込みハンドラの型の定義
 */
typedef void	(*IINTHDR)(INTNO intno);

/*
 *  割込みサービスルーチン初期化ブロック
 */
typedef struct isr_initialization_block {
	ATR			isratr;			/* 割込みサービスルーチン属性 */
	intptr_t	exinf;			/* 割込みサービスルーチンの拡張情報 */
	INTNO		intno;			/* 割込みサービスルーチンを登録する割込み番号 */
	QUEUE		*p_isr_queue;	/* 登録先割込みサービスルーチンキューの番地 */
	ISR			isr;			/* 割込みサービスルーチンの先頭番地 */
	PRI			isrpri;			/* 割込みサービスルーチン優先度 */
} ISRINIB;

/*
 *  割込みサービスルーチン管理ブロック
 */
typedef struct isr_control_block {
	QUEUE		isr_queue;		/* 割込みサービスルーチン呼出しキュー */
	const ISRINIB *p_isrinib;	/* 初期化ブロックへのポインタ */
} ISRCB;

/*
 *  割込みサービスルーチンキューのエリア
 */
extern QUEUE	isr_queue_table[];

/*
 *  使用していない割込みサービスルーチン管理ブロックのリスト
 */
extern QUEUE	free_isrcb;

/*
 *  割込みサービスルーチンIDの最大値（kernel_cfg.c）
 */
extern const ID		tmax_isrid;

/*
 *  割込みサービスルーチン初期化ブロックのエリア（kernel_cfg.c）
 */
extern ISRINIB	isrinib_table[];

/*
 *  割込みサービスルーチン管理ブロックのエリア（kernel_cfg.c）
 */
extern ISRCB	isrcb_table[];

/*
 *  割込みサービスルーチン機能の初期化
 */
extern void initialize_isr(void);

/*
 *  割込みサービスルーチンの呼出し
 */
extern void inthdr_call_isr(INTNO intno);

/*
 *  割込みハンドラ初期化ブロック
 */
typedef struct interrupt_handler_initialization_block {
	ATR			inhatr;			/* 割込みハンドラ属性 */
	IINTHDR		inthdr;			/* 割込みハンドラの先頭番地 */
} INHINIB;

/*
 *  割込み要求ライン初期化ブロック
 */
typedef struct interrupt_request_initialization_block {
	ATR			intatr;			/* 割込み属性 */
	PRI			intpri;			/* 割込み優先度 */
} INTINIB;

/*
 *  割込みハンドラ初期化ブロックのエリア（kernel_cfg.c）
 */
extern INHINIB	inhinib_table[];

/*
 *  割込み要求ライン初期化ブロックのエリア（kernel_cfg.c）
 */
extern INTINIB	intinib_table[];

/*
 *  割込み番号から割込み要求ライン初期化ブロックを取り出すためのマクロ
 */
#define get_intinib(intno)	(&(intinib_table[INDEX_INTNO(intno)]))

/*
 *  割込みハンドラ番号から割込みハンドラ初期化ブロックを取り出すための
 *  マクロ
 */
#define get_inhinib(inhno)	(&(inhinib_table[INDEX_INHNO(inhno)]))

/*
 *  割込み管理機能の初期化
 */
extern void	initialize_interrupt(void);

#endif /* TOPPERS_INTERRUPT_H */
