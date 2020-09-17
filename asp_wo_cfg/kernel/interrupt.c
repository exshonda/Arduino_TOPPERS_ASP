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
 *  @(#) $Id: interrupt.c 52 2012-09-07 06:34:34Z ertl-hiro $
 */

/*
 *		割込み管理機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "interrupt.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_ISR_ENTER
#define LOG_ISR_ENTER(intno)
#endif /* LOG_ISR_ENTER */

#ifndef LOG_ISR_LEAVE
#define LOG_ISR_LEAVE(intno)
#endif /* LOG_ISR_LEAVE */

#ifndef LOG_CFG_INT_ENTER
#define LOG_CFG_INT_ENTER(intno, pk_cint)
#endif /* LOG_CFG_INT_ENTER */

#ifndef LOG_CFG_INT_LEAVE
#define LOG_CFG_INT_LEAVE(ercd)
#endif /* LOG_CFG_INT_LEAVE */

#ifndef LOG_ATT_ISR_ENTER
#define LOG_ATT_ISR_ENTER(pk_cisr)
#endif /* LOG_ATT_ISR_ENTER */

#ifndef LOG_ATT_ISR_LEAVE
#define LOG_ATT_ISR_LEAVE(ercd)
#endif /* LOG_ATT_ISR_LEAVE */

#ifndef LOG_DEF_INH_ENTER
#define LOG_DEF_INH_ENTER(inhno, pk_dinh)
#endif /* LOG_DEF_INH_ENTER */

#ifndef LOG_DEF_INH_LEAVE
#define LOG_DEF_INH_LEAVE(ercd)
#endif /* LOG_DEF_INH_LEAVE */

#ifndef LOG_DIS_INT_ENTER
#define LOG_DIS_INT_ENTER(intno)
#endif /* LOG_DIS_INT_ENTER */

#ifndef LOG_DIS_INT_LEAVE
#define LOG_DIS_INT_LEAVE(ercd)
#endif /* LOG_DIS_INT_LEAVE */

#ifndef LOG_ENA_INT_ENTER
#define LOG_ENA_INT_ENTER(intno)
#endif /* LOG_ENA_INT_ENTER */

#ifndef LOG_ENA_INT_LEAVE
#define LOG_ENA_INT_LEAVE(ercd)
#endif /* LOG_ENA_INT_LEAVE */

#ifndef LOG_CHG_IPM_ENTER
#define LOG_CHG_IPM_ENTER(intpri)
#endif /* LOG_CHG_IPM_ENTER */

#ifndef LOG_CHG_IPM_LEAVE
#define LOG_CHG_IPM_LEAVE(ercd)
#endif /* LOG_CHG_IPM_LEAVE */

#ifndef LOG_GET_IPM_ENTER
#define LOG_GET_IPM_ENTER(p_intpri)
#endif /* LOG_GET_IPM_ENTER */

#ifndef LOG_GET_IPM_LEAVE
#define LOG_GET_IPM_LEAVE(ercd, intpri)
#endif /* LOG_GET_IPM_LEAVE */

/*
 *  割込みサービスルーチンの数
 */
#define tnum_isr	((uint_t)(tmax_isrid - TMIN_SEMID + 1))

/*
 *  割込みサービスルーチンキューへの登録
 */
void
enqueue_isr(QUEUE *p_isr_queue, ISRCB *p_isrcb)
{
	QUEUE	*p_entry;
	PRI		isrpri = p_isrcb->p_isrinib->isrpri;

	for (p_entry = p_isr_queue->p_next; p_entry != p_isr_queue;
											p_entry = p_entry->p_next) {
		if (isrpri < ((ISRCB *) p_entry)->p_isrinib->isrpri) {
			break;
		}
	}
	queue_insert_prev(p_entry, &(p_isrcb->isr_queue));
}

#ifdef TOPPERS_isrini

/*
 *  割込みサービスルーチンキューのエリア
 */
QUEUE	isr_queue_table[TNUM_INTNO];

/*
 *  使用していない割込みサービスルーチン管理ブロックのリスト
 */
QUEUE	free_isrcb;

/* 
 *  割込みサービスルーチン機能の初期化
 */
void
initialize_isr(void)
{
	uint_t	i;
	ISRCB	*p_isrcb;
	ISRINIB	*p_isrinib;

	for (i = 0; i < TNUM_INTNO; i++) {
		queue_initialize(&(isr_queue_table[i]));
	}
	queue_initialize(&free_isrcb);
	for (i = 0; i < tnum_isr; i++) {
		p_isrcb = &(isrcb_table[i]);
		p_isrinib = &(isrinib_table[i]);
		p_isrinib->isratr = TA_NOEXS;
		p_isrcb->p_isrinib = ((const ISRINIB *) p_isrinib);
		queue_insert_prev(&free_isrcb, &(p_isrcb->isr_queue));
	}
}

#endif /* TOPPERS_isrini */

/*
 *  割込みサービスルーチンの呼出し
 */
#ifdef TOPPERS_isrcal

void
inthdr_call_isr(INTNO intno)
{
	QUEUE	*p_isr_queue, *p_queue;
	ISRINIB	*p_isrinib;
	PRI		saved_ipm;

	i_begin_int(intno);
	saved_ipm = i_get_ipm();
	p_isr_queue = &isr_queue_table[INDEX_INTNO(intno)];
	for (p_queue = p_isr_queue->p_next; p_queue != p_isr_queue;
											p_queue = p_queue->p_next) {
		p_isrinib = (ISRINIB *)(((ISRCB *) p_queue)->p_isrinib);
		LOG_ISR_ENTER(p_isrinib->intno);
		(*(p_isrinib->isr))(p_isrinib->exinf);
		LOG_ISR_LEAVE(p_isrinib->intno);

		if (p_queue->p_next != p_isr_queue) {
			/* ISRの呼出し前の状態に戻す */
			if (i_sense_lock()) {
				i_unlock_cpu();
			}
			i_set_ipm(saved_ipm);
		}
	}
	i_end_int(intno);
}

#endif /* TOPPERS_isrcal */

/*
 *  割込みサービスルーチンの生成
 */
#ifdef TOPPERS_att_isr

ER
att_isr(const T_CISR *pk_cisr)
{
	ISRCB	*p_isrcb;
	ISRINIB	*p_isrinib;
	QUEUE	*p_isr_queue;
	INHINIB	*p_inhinib;
	ER		ercd;

	LOG_ATT_ISR_ENTER(pk_cisr);
	CHECK_INIRTN();
	CHECK_RSATR(pk_cisr->isratr, TARGET_ISRATR);
	CHECK_INTNO_CREISR(pk_cisr->intno);
	CHECK_ALIGN_FUNC(pk_cisr->isr);
	CHECK_NONNULL_FUNC(pk_cisr->isr);
	CHECK_ISRPRI(pk_cisr->isrpri);

	p_inhinib = get_inhinib(INHNO_INTNO(pk_cisr->intno));
	if (get_intinib(pk_cisr->intno)->intatr == TA_NOEXS
				|| (p_inhinib->inhatr != TA_NOEXS
						&& p_inhinib->inthdr != inthdr_call_isr)) {
		ercd = E_OBJ;
	}
	else if (tnum_isr == 0 || queue_empty(&free_isrcb)) {
		ercd = E_NOID;
	}
	if ((ercd = target_check_aisr(pk_cisr)) == E_OK) {
		if (p_inhinib->inhatr == TA_NOEXS) {
			p_inhinib->inhatr = TA_NULL;
			p_inhinib->inthdr = inthdr_call_isr;
		}
		p_isr_queue = &isr_queue_table[INDEX_INTNO(pk_cisr->intno)];
		p_isrcb = ((ISRCB *) queue_delete_next(&free_isrcb));
		p_isrinib = (ISRINIB *)(p_isrcb->p_isrinib);
		p_isrinib->isratr = pk_cisr->isratr;
		p_isrinib->exinf = pk_cisr->exinf;
		p_isrinib->intno = pk_cisr->intno;
		p_isrinib->p_isr_queue = p_isr_queue;
		p_isrinib->isr = pk_cisr->isr;
		p_isrinib->isrpri = pk_cisr->isrpri;
		enqueue_isr(p_isr_queue, p_isrcb);
	}

  error_exit:
	LOG_ATT_ISR_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_att_isr */

#ifdef TOPPERS_intini

/*
 *  割込みハンドラ初期化ブロックのエリア
 */
INHINIB	inhinib_table[TNUM_INHNO];

/*
 *  割込み要求ライン初期化ブロックのエリア
 */
INTINIB	intinib_table[TNUM_INTNO];

/* 
 *  割込み管理機能の初期化
 */
void
initialize_interrupt(void)
{
	uint_t	i;

	for (i = 0; i < TNUM_INHNO; i++) {
		inhinib_table[i].inhatr = TA_NOEXS;
	}
	for (i = 0; i < TNUM_INTNO; i++) {
		intinib_table[i].intatr = TA_NOEXS;
	}
}

#endif /* TOPPERS_intini */

/*
 *  割込み要求ラインの属性の設定
 */
#ifdef TOPPERS_cfg_int

ER
cfg_int(INTNO intno, const T_CINT *pk_cint)
{
	INTINIB	*p_intinib;
	ER		ercd;

	LOG_CFG_INT_ENTER(intno, pk_cint);
	CHECK_INIRTN();
	CHECK_PAR(VALID_INTNO_CFGINT(intno));
	CHECK_RSATR(pk_cint->intatr, TA_ENAINT|TA_EDGE|TARGET_INTATR);
	CHECK_PAR(VALID_INTPRI_CFGINT(pk_cint->intpri));
	p_intinib = get_intinib(intno);

	if ((ercd = target_check_cint(intno, pk_cint)) == E_OK) {
		p_intinib->intatr = pk_cint->intatr;
		p_intinib->intpri = pk_cint->intpri;
	}

  error_exit:
	LOG_CFG_INT_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_cfg_int */

/*
 *  割込みハンドラの定義
 */
#ifdef TOPPERS_def_inh

ER
def_inh(INHNO inhno, const T_DINH *pk_dinh)
{
	INHINIB	*p_inhinib;
	ER		ercd;

	LOG_DEF_INH_ENTER(inhno, pk_dinh);
	CHECK_INIRTN();
	CHECK_PAR(VALID_INHNO_DEFINH(inhno));
	CHECK_RSATR(pk_dinh->inhatr, TARGET_INHATR);
	CHECK_NOSPT(pk_dinh->inthdr != NULL);
	CHECK_ALIGN_FUNC(pk_dinh->inthdr);
	CHECK_NONNULL_FUNC(pk_dinh->inthdr);
	p_inhinib = get_inhinib(inhno);

	if (get_intinib(INTNO_INHNO(inhno))->intatr == TA_NOEXS
							|| p_inhinib->inhatr != TA_NOEXS) {
		ercd = E_OBJ;
	}
	else if ((ercd = target_check_dinh(inhno, pk_dinh)) == E_OK) {
		p_inhinib->inhatr = pk_dinh->inhatr;
		p_inhinib->inthdr = (IINTHDR)(pk_dinh->inthdr);
	}

  error_exit:
	LOG_DEF_INH_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_def_inh */

/*
 *  割込みの禁止
 */
#ifdef TOPPERS_dis_int
#ifdef TOPPERS_SUPPORT_DIS_INT

ER
dis_int(INTNO intno)
{
	bool_t	locked;
	ER		ercd;

	LOG_DIS_INT_ENTER(intno);
	CHECK_TSKCTX();
	CHECK_INTNO_DISINT(intno);

	locked = t_sense_lock();
	if (!locked) {
		t_lock_cpu();
	}
	if (t_disable_int(intno)) {
		ercd = E_OK;
	}
	else {
		ercd = E_OBJ;
	}
	if (!locked) {
		t_unlock_cpu();
	}

  error_exit:
	LOG_DIS_INT_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_SUPPORT_DIS_INT */
#endif /* TOPPERS_dis_int */

/*
 *  割込みの許可
 */
#ifdef TOPPERS_ena_int
#ifdef TOPPERS_SUPPORT_ENA_INT

ER
ena_int(INTNO intno)
{
	bool_t	locked;
	ER		ercd;

	LOG_ENA_INT_ENTER(intno);
	CHECK_TSKCTX();
	CHECK_INTNO_DISINT(intno);

	locked = t_sense_lock();
	if (!locked) {
		t_lock_cpu();
	}
	if (t_enable_int(intno)) {
		ercd = E_OK;
	}
	else {
		ercd = E_OBJ;
	}
	if (!locked) {
		t_unlock_cpu();
	}

  error_exit:
	LOG_ENA_INT_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_SUPPORT_ENA_INT */
#endif /* TOPPERS_ena_int */

/*
 *  割込み優先度マスクの変更
 */
#ifdef TOPPERS_chg_ipm

ER
chg_ipm(PRI intpri)
{
	ER		ercd;

	LOG_CHG_IPM_ENTER(intpri);
	CHECK_TSKCTX_UNL();
	CHECK_INTPRI_CHGIPM(intpri);

	t_lock_cpu();
	t_set_ipm(intpri);
	if (intpri == TIPM_ENAALL) {
		ipmflg = true;
		if (!disdsp) {
			dspflg = true;
			if (p_runtsk != p_schedtsk) {
				dispatch();
			}
		}
		if (p_runtsk->enatex && p_runtsk->texptn != 0U) {
			call_texrtn();
		}
	}
	else {
		ipmflg = false;
		dspflg = false;
	}
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_CHG_IPM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_chg_ipm */

/*
 *  割込み優先度マスクの参照
 */
#ifdef TOPPERS_get_ipm

ER
get_ipm(PRI *p_intpri)
{
	ER		ercd;

	LOG_GET_IPM_ENTER(p_intpri);
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_intpri = t_get_ipm();
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_GET_IPM_LEAVE(ercd, *p_intpri);
	return(ercd);
}

#endif /* TOPPERS_get_ipm */
