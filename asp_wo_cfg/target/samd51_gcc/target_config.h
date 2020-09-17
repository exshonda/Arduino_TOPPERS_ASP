/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2015 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: target_config.h 2693 2015-11-04 03:55:57Z ertl-honda $
 */

/*
 *  ターゲット依存モジュール（SAMD51用）
 *
 *  カーネルのチップ依存部のインクルードファイル．kernel_impl.hのター
 *  ゲット依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_CONFIG_H
#define TOPPERS_TARGET_CONFIG_H

/*
 *  ターゲットのハードウェア資源の定義
 */
#include "samd51.h"

/*
 *  トレースログに関する設定
 */
#ifdef TOPPERS_ENABLE_TRACE
#include "logtrace/trace_config.h"
#endif /* TOPPERS_ENABLE_TRACE */

/*
 *  デフォルトの非タスクコンテキスト用のスタック領域の定義
 */
#define DEFAULT_ISTKSZ			(0x1000)		/* 4KByte */

/*
 * スタートアップルーチンでスタックを初期化する
 */
#define INIT_MSP

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 *  (ToDo) 調整する
 */
#define SIL_DLY_TIM1    162
#define SIL_DLY_TIM2    100

/*
 *  使用するシリアルポートID
 */
#define SIO_PORTID		(1)

/*
 *  割込み要求ライン属性，割込みハンドラ，CPU例外ハンドラの登録可能数
 */
#define TNUM_INTNO			(16 + 138)
#define TNUM_INHNO			(16 + 138)
#define TNUM_EXCNO			14

#define INDEX_INTNO(intno)	((uint_t)((intno) - 1))
#define INDEX_INHNO(inhno)	((uint_t)((inhno) - 1))
#define INDEX_EXCNO(excno)	((uint_t)((excno) - 1))

/*
 *  割込み番号と割込みハンドラ番号を変換するためのマクロ
 */
#define INTNO_INHNO(inhno)	((INTNO)(inhno))
#define INHNO_INTNO(intno)	((INHNO)(intno))

/*
 *  割込み番号の範囲の判定
 */
#define VALID_INTNO_CFGINT(intno)	VALID_INTNO(intno)
#define	VALID_INTNO_CREISR(intno)	VALID_INTNO(intno)
#define	VALID_INTNO_DISINT(intno)	VALID_INTNO(intno)

/*
 *  割込み優先度の範囲の判定
 */
#define VALID_INTPRI_CFGINT(intpri)	(-1 >= (intpri) && (intpri) >= -7)

/*
 *  割込みハンドラ番号とCPU例外ハンドラ番号の範囲の判定
 */
#define VALID_INHNO_DEFINH(inhno)		VALID_INTNO((INTNO)(inhno))


#ifndef TOPPERS_MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void	target_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void	target_exit(void) NoReturn;

/*
 *  LEDの操作
 */
extern void led_on(void);
extern void led_off(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  コア依存モジュール（ARM-M用）
 */
#ifndef ARDUINO_PASS
#include "core_config.h"
#else /* ARDUINO_PASS */
#include "../asp_wo_cfg/arch/arm_m_gcc/common/core_config.h"
#endif /* ARDUINO_PASS */
#endif /* TOPPERS_TARGET_CONFIG_H */
