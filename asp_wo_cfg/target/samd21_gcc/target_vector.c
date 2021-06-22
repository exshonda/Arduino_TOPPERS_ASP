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
 *  @(#) $Id: target_config.c 2697 2015-11-06 04:00:42Z ertl-honda $
 */

/*
 * チップ依存ベクターテーブル（SAMD21用）
 */
#include "kernel_impl.h"

 /*
  *  ベクタテーブル
  */
__attribute__ ((aligned(0x400U)))
 __attribute__((section(".rodata.vector")))
const  FP _kernel_vector_table[] =      
{                                    
	(FP)(NULL), /* 0 The initial stack pointer */
	(FP)_start,           /* 1 The reset handler */
	(FP)(core_exc_entry), /* 2 */
	(FP)(core_exc_entry), /* 3 */
	(FP)(core_exc_entry), /* 4 */
	(FP)(core_exc_entry), /* 5 */
	(FP)(core_exc_entry), /* 6 */
	(FP)(core_exc_entry), /* 7 */
	(FP)(core_exc_entry), /* 8 */
	(FP)(core_exc_entry), /* 9 */
	(FP)(core_exc_entry), /* 10 */
	(FP)(core_exc_entry), /* 11 */
	(FP)(core_exc_entry), /* 12 */
	(FP)(core_exc_entry), /* 13 */
	(FP)(pendsvc_handler),/* 14 */
	(FP)(core_int_entry), /* 15 */
	(FP)(core_int_entry), /* 16 */
	(FP)(core_int_entry), /* 17 */
	(FP)(core_int_entry), /* 18 */
	(FP)(core_int_entry), /* 19 */
	(FP)(core_int_entry), /* 20 */
	(FP)(core_int_entry), /* 21 */
	(FP)(core_int_entry), /* 22 */
	(FP)(core_int_entry), /* 23 */
	(FP)(core_int_entry), /* 24 */
	(FP)(core_int_entry), /* 25 */
	(FP)(core_int_entry), /* 26 */
	(FP)(core_int_entry), /* 27 */
	(FP)(core_int_entry), /* 28 */
	(FP)(core_int_entry), /* 29 */
	(FP)(core_int_entry), /* 30 */
	(FP)(core_int_entry), /* 31 */
	(FP)(core_int_entry), /* 32 */
	(FP)(core_int_entry), /* 33 */
	(FP)(core_int_entry), /* 34 */
	(FP)(core_int_entry), /* 35 */
	(FP)(core_int_entry), /* 36 */
	(FP)(core_int_entry), /* 37 */
	(FP)(core_int_entry), /* 38 */
	(FP)(core_int_entry), /* 39 */
	(FP)(core_int_entry), /* 40 */
	(FP)(core_int_entry), /* 41 */
	(FP)(core_int_entry), /* 42 */
	(FP)(core_int_entry), /* 43 */
	(FP)(core_int_entry), /* 44 */
	(FP)(core_int_entry), /* 45 */
};
