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
 *  @(#) $Id: semaphore.c 49 2012-09-06 04:41:53Z ertl-hiro $
 */

/*
 *		セマフォ機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "semaphore.h"

/*
 *  トレースログマクロのデフォルト定義
 */
#ifndef LOG_CRE_SEM_ENTER
#define LOG_CRE_SEM_ENTER(semid, pk_csem)
#endif /* LOG_CRE_SEM_ENTER */

#ifndef LOG_CRE_SEM_LEAVE
#define LOG_CRE_SEM_LEAVE(ercd)
#endif /* LOG_CRE_SEM_LEAVE */

#ifndef LOG_SIG_SEM_ENTER
#define LOG_SIG_SEM_ENTER(semid)
#endif /* LOG_SIG_SEM_ENTER */

#ifndef LOG_SIG_SEM_LEAVE
#define LOG_SIG_SEM_LEAVE(ercd)
#endif /* LOG_SIG_SEM_LEAVE */

#ifndef LOG_ISIG_SEM_ENTER
#define LOG_ISIG_SEM_ENTER(semid)
#endif /* LOG_ISIG_SEM_ENTER */

#ifndef LOG_ISIG_SEM_LEAVE
#define LOG_ISIG_SEM_LEAVE(ercd)
#endif /* LOG_ISIG_SEM_LEAVE */

#ifndef LOG_WAI_SEM_ENTER
#define LOG_WAI_SEM_ENTER(semid)
#endif /* LOG_WAI_SEM_ENTER */

#ifndef LOG_WAI_SEM_LEAVE
#define LOG_WAI_SEM_LEAVE(ercd)
#endif /* LOG_WAI_SEM_LEAVE */

#ifndef LOG_POL_SEM_ENTER
#define LOG_POL_SEM_ENTER(semid)
#endif /* LOG_POL_SEM_ENTER */

#ifndef LOG_POL_SEM_LEAVE
#define LOG_POL_SEM_LEAVE(ercd)
#endif /* LOG_POL_SEM_LEAVE */

#ifndef LOG_TWAI_SEM_ENTER
#define LOG_TWAI_SEM_ENTER(semid, tmout)
#endif /* LOG_TWAI_SEM_ENTER */

#ifndef LOG_TWAI_SEM_LEAVE
#define LOG_TWAI_SEM_LEAVE(ercd)
#endif /* LOG_TWAI_SEM_LEAVE */

#ifndef LOG_INI_SEM_ENTER
#define LOG_INI_SEM_ENTER(semid)
#endif /* LOG_INI_SEM_ENTER */

#ifndef LOG_INI_SEM_LEAVE
#define LOG_INI_SEM_LEAVE(ercd)
#endif /* LOG_INI_SEM_LEAVE */

#ifndef LOG_REF_SEM_ENTER
#define LOG_REF_SEM_ENTER(semid, pk_rsem)
#endif /* LOG_REF_SEM_ENTER */

#ifndef LOG_REF_SEM_LEAVE
#define LOG_REF_SEM_LEAVE(ercd, pk_rsem)
#endif /* LOG_REF_SEM_LEAVE */

/*
 *  セマフォの数
 */
#define tnum_sem	((uint_t)(tmax_semid - TMIN_SEMID + 1))

/*
 *  セマフォIDからセマフォ管理ブロックを取り出すためのマクロ
 */
#define INDEX_SEM(semid)	((uint_t)((semid) - TMIN_SEMID))
#define get_semcb(semid)	(&(semcb_table[INDEX_SEM(semid)]))

/* 
 *  セマフォ機能の初期化
 */
#ifdef TOPPERS_semini

void
initialize_semaphore(void)
{
	uint_t	i;
	SEMCB	*p_semcb;
	SEMINIB	*p_seminib;

	for (i = 0; i < tnum_sem; i++) {
		p_semcb = &(semcb_table[i]);
		p_seminib = &(seminib_table[i]);
		p_seminib->sematr = TA_NOEXS;
		p_semcb->p_seminib = ((const SEMINIB *) p_seminib);
	}
}

#endif /* TOPPERS_semini */

/*
 *  セマフォの生成
 */
#ifdef TOPPERS_cre_sem

ER
cre_sem(ID semid, const T_CSEM *pk_csem)
{
	SEMCB	*p_semcb;
	SEMINIB	*p_seminib;
	ER		ercd;

	LOG_CRE_SEM_ENTER(semid, pk_csem);
	CHECK_INIRTN();
	CHECK_SEMID(semid);
	CHECK_RSATR(pk_csem->sematr, TA_TPRI);
	CHECK_PAR(0 <= pk_csem->isemcnt && pk_csem->isemcnt <= pk_csem->maxsem);
	CHECK_PAR(1 <= pk_csem->maxsem && pk_csem->maxsem <= TMAX_MAXSEM);
	p_semcb = get_semcb(semid);

	if (p_semcb->p_seminib->sematr != TA_NOEXS) {
		ercd = E_OBJ;
	}
	else {
		p_seminib = (SEMINIB *)(p_semcb->p_seminib);
		p_seminib->sematr = pk_csem->sematr;
		p_seminib->isemcnt = pk_csem->isemcnt;
		p_seminib->maxsem = pk_csem->maxsem;

		queue_initialize(&(p_semcb->wait_queue));
		p_semcb->semcnt = p_semcb->p_seminib->isemcnt;
		ercd = E_OK;
	}

  error_exit:
	LOG_CRE_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_cre_sem */

/*
 *  セマフォ資源の返却
 */
#ifdef TOPPERS_sig_sem

ER
sig_sem(ID semid)
{
	SEMCB	*p_semcb;
	TCB		*p_tcb;
	ER		ercd;
    
	LOG_SIG_SEM_ENTER(semid);
	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	t_lock_cpu();
	if (!queue_empty(&(p_semcb->wait_queue))) {
		p_tcb = (TCB *) queue_delete_next(&(p_semcb->wait_queue));
		if (wait_complete(p_tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (p_semcb->semcnt < p_semcb->p_seminib->maxsem) {
		p_semcb->semcnt += 1;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	t_unlock_cpu();

  error_exit:
	LOG_SIG_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_sig_sem */

/*
 *  セマフォ資源の返却（非タスクコンテキスト用）
 */
#ifdef TOPPERS_isig_sem

ER
isig_sem(ID semid)
{
	SEMCB	*p_semcb;
	TCB		*p_tcb;
	ER		ercd;
    
	LOG_ISIG_SEM_ENTER(semid);
	CHECK_INTCTX_UNL();
	CHECK_SEMID(semid);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	i_lock_cpu();
	if (!queue_empty(&(p_semcb->wait_queue))) {
		p_tcb = (TCB *) queue_delete_next(&(p_semcb->wait_queue));
		if (wait_complete(p_tcb)) {
			reqflg = true;
		}
		ercd = E_OK;
	}
	else if (p_semcb->semcnt < p_semcb->p_seminib->maxsem) {
		p_semcb->semcnt += 1;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	i_unlock_cpu();

  error_exit:
	LOG_ISIG_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_isig_sem */

/*
 *  セマフォ資源の獲得
 */
#ifdef TOPPERS_wai_sem

ER
wai_sem(ID semid)
{
	SEMCB	*p_semcb;
	WINFO_SEM winfo_sem;
	ER		ercd;

	LOG_WAI_SEM_ENTER(semid);
	CHECK_DISPATCH();
	CHECK_SEMID(semid);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	t_lock_cpu();
	if (p_semcb->semcnt >= 1) {
		p_semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else {
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_SEM);
		wobj_make_wait((WOBJCB *) p_semcb, (WINFO_WOBJ *) &winfo_sem);
		dispatch();
		ercd = winfo_sem.winfo.wercd;
	}
	t_unlock_cpu();

  error_exit:
	LOG_WAI_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_wai_sem */

/*
 *  セマフォ資源の獲得（ポーリング）
 */
#ifdef TOPPERS_pol_sem

ER
pol_sem(ID semid)
{
	SEMCB	*p_semcb;
	ER		ercd;

	LOG_POL_SEM_ENTER(semid);
	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	t_lock_cpu();
	if (p_semcb->semcnt >= 1) {
		p_semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_POL_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_pol_sem */

/*
 *  セマフォ資源の獲得（タイムアウトあり）
 */
#ifdef TOPPERS_twai_sem

ER
twai_sem(ID semid, TMO tmout)
{
	SEMCB	*p_semcb;
	WINFO_SEM winfo_sem;
	TMEVTB	tmevtb;
	ER		ercd;

	LOG_TWAI_SEM_ENTER(semid, tmout);
	CHECK_DISPATCH();
	CHECK_SEMID(semid);
	CHECK_TMOUT(tmout);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	t_lock_cpu();
	if (p_semcb->semcnt >= 1) {
		p_semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_SEM);
		wobj_make_wait_tmout((WOBJCB *) p_semcb, (WINFO_WOBJ *) &winfo_sem,
														&tmevtb, tmout);
		dispatch();
		ercd = winfo_sem.winfo.wercd;
	}
	t_unlock_cpu();

  error_exit:
	LOG_TWAI_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_twai_sem */

/*
 *  セマフォの再初期化
 */
#ifdef TOPPERS_ini_sem

ER
ini_sem(ID semid)
{
	SEMCB	*p_semcb;
	bool_t	dspreq;
	ER		ercd;
    
	LOG_INI_SEM_ENTER(semid);
	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	t_lock_cpu();
	dspreq = init_wait_queue(&(p_semcb->wait_queue));
	p_semcb->semcnt = p_semcb->p_seminib->isemcnt;
	if (dspreq) {
		dispatch();
	}
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_INI_SEM_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ini_sem */

/*
 *  セマフォの状態参照
 */
#ifdef TOPPERS_ref_sem

ER
ref_sem(ID semid, T_RSEM *pk_rsem)
{
	SEMCB	*p_semcb;
	ER		ercd;
    
	LOG_REF_SEM_ENTER(semid, pk_rsem);
	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	p_semcb = get_semcb(semid);
	CHECK_NOEXS(p_semcb->p_seminib->sematr);

	t_lock_cpu();
	pk_rsem->wtskid = wait_tskid(&(p_semcb->wait_queue));
	pk_rsem->semcnt = p_semcb->semcnt;
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_REF_SEM_LEAVE(ercd, pk_rsem);
	return(ercd);
}

#endif /* TOPPERS_ref_sem */
