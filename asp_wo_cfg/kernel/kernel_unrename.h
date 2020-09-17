/* This file is generated from kernel_rename.def by genrename. */

/* This file is included only when kernel_rename.h has been included. */
#ifdef TOPPERS_KERNEL_RENAME_H
#undef TOPPERS_KERNEL_RENAME_H

/*
 *  startup.c
 */
#undef kerflg
#undef exit_kernel
#undef initialize_kmm
#undef kernel_malloc
#undef kernel_free

/*
 *  task.c
 */
#undef p_runtsk
#undef p_schedtsk
#undef reqflg
#undef ipmflg
#undef disdsp
#undef dspflg
#undef ready_queue
#undef ready_primap
#undef free_tcb
#undef initialize_task
#undef search_schedtsk
#undef make_runnable
#undef make_non_runnable
#undef make_dormant
#undef make_active
#undef change_priority
#undef rotate_ready_queue
#undef call_texrtn
#undef calltex

/*
 *  wait.c
 */
#undef make_wait_tmout
#undef wait_complete
#undef wait_tmout
#undef wait_tmout_ok
#undef wait_release
#undef wobj_make_wait
#undef wobj_make_wait_tmout
#undef init_wait_queue

/*
 *  time_event.c
 */
#undef current_time
#undef min_time
#undef next_time
#undef next_subtime
#undef last_index
#undef initialize_tmevt
#undef tmevt_up
#undef tmevt_down
#undef tmevtb_insert
#undef tmevtb_delete
#undef tmevt_lefttim
#undef signal_time

/*
 *  semaphore.c
 */
#undef free_semcb
#undef initialize_semaphore

/*
 *  eventflag.c
 */
#undef free_flgcb
#undef initialize_eventflag
#undef check_flg_cond

/*
 *  dataqueue.c
 */
#undef free_dtqcb
#undef initialize_dataqueue
#undef enqueue_data
#undef force_enqueue_data
#undef dequeue_data
#undef send_data
#undef force_send_data
#undef receive_data

/*
 *  pridataq.c
 */
#undef free_pdqcb
#undef initialize_pridataq
#undef enqueue_pridata
#undef dequeue_pridata
#undef send_pridata
#undef receive_pridata

/*
 *  mailbox.c
 */
#undef free_mbxcb
#undef initialize_mailbox

/*
 *  mempfix.c
 */
#undef free_mpfcb
#undef initialize_mempfix
#undef get_mpf_block

/*
 *  cyclic.c
 */
#undef free_cyccb
#undef initialize_cyclic
#undef call_cychdr

/*
 *  alarm.c
 */
#undef free_almcb
#undef initialize_alarm
#undef call_almhdr

/*
 *  interrupt.c
 */
#undef inhinib_table
#undef intinib_table
#undef free_isrcb
#undef initialize_isr
#undef inthdr_call_isr
#undef initialize_interrupt

/*
 *  exception.c
 */
#undef excinib_table
#undef initialize_exception

/*
 *  kernel_cfg.c
 */
#undef initialize_object
#undef tmax_tskid
#undef tinib_table
#undef tcb_table
#undef tmax_semid
#undef seminib_table
#undef semcb_table
#undef tmax_flgid
#undef flginib_table
#undef flgcb_table
#undef tmax_dtqid
#undef dtqinib_table
#undef dtqcb_table
#undef tmax_pdqid
#undef pdqinib_table
#undef pdqcb_table
#undef tmax_mbxid
#undef mbxinib_table
#undef mbxcb_table
#undef tmax_mpfid
#undef mpfinib_table
#undef mpfcb_table
#undef tmax_cycid
#undef cycinib_table
#undef cyccb_table
#undef tmax_almid
#undef alminib_table
#undef almcb_table
#undef isr_queue_table
#undef tmax_isrid
#undef isrinib_table
#undef isrcb_table
#undef tmevt_heap
#undef istksz
#undef istk
#undef istkpt
#undef kmmsz
#undef kmm


#ifdef TOPPERS_LABEL_ASM

/*
 *  startup.c
 */
#undef _kerflg
#undef _exit_kernel
#undef _initialize_kmm
#undef _kernel_malloc
#undef _kernel_free

/*
 *  task.c
 */
#undef _p_runtsk
#undef _p_schedtsk
#undef _reqflg
#undef _ipmflg
#undef _disdsp
#undef _dspflg
#undef _ready_queue
#undef _ready_primap
#undef _free_tcb
#undef _initialize_task
#undef _search_schedtsk
#undef _make_runnable
#undef _make_non_runnable
#undef _make_dormant
#undef _make_active
#undef _change_priority
#undef _rotate_ready_queue
#undef _call_texrtn
#undef _calltex

/*
 *  wait.c
 */
#undef _make_wait_tmout
#undef _wait_complete
#undef _wait_tmout
#undef _wait_tmout_ok
#undef _wait_release
#undef _wobj_make_wait
#undef _wobj_make_wait_tmout
#undef _init_wait_queue

/*
 *  time_event.c
 */
#undef _current_time
#undef _min_time
#undef _next_time
#undef _next_subtime
#undef _last_index
#undef _initialize_tmevt
#undef _tmevt_up
#undef _tmevt_down
#undef _tmevtb_insert
#undef _tmevtb_delete
#undef _tmevt_lefttim
#undef _signal_time

/*
 *  semaphore.c
 */
#undef _free_semcb
#undef _initialize_semaphore

/*
 *  eventflag.c
 */
#undef _free_flgcb
#undef _initialize_eventflag
#undef _check_flg_cond

/*
 *  dataqueue.c
 */
#undef _free_dtqcb
#undef _initialize_dataqueue
#undef _enqueue_data
#undef _force_enqueue_data
#undef _dequeue_data
#undef _send_data
#undef _force_send_data
#undef _receive_data

/*
 *  pridataq.c
 */
#undef _free_pdqcb
#undef _initialize_pridataq
#undef _enqueue_pridata
#undef _dequeue_pridata
#undef _send_pridata
#undef _receive_pridata

/*
 *  mailbox.c
 */
#undef _free_mbxcb
#undef _initialize_mailbox

/*
 *  mempfix.c
 */
#undef _free_mpfcb
#undef _initialize_mempfix
#undef _get_mpf_block

/*
 *  cyclic.c
 */
#undef _free_cyccb
#undef _initialize_cyclic
#undef _call_cychdr

/*
 *  alarm.c
 */
#undef _free_almcb
#undef _initialize_alarm
#undef _call_almhdr

/*
 *  interrupt.c
 */
#undef _inhinib_table
#undef _intinib_table
#undef _free_isrcb
#undef _initialize_isr
#undef _inthdr_call_isr
#undef _initialize_interrupt

/*
 *  exception.c
 */
#undef _excinib_table
#undef _initialize_exception

/*
 *  kernel_cfg.c
 */
#undef _initialize_object
#undef _tmax_tskid
#undef _tinib_table
#undef _tcb_table
#undef _tmax_semid
#undef _seminib_table
#undef _semcb_table
#undef _tmax_flgid
#undef _flginib_table
#undef _flgcb_table
#undef _tmax_dtqid
#undef _dtqinib_table
#undef _dtqcb_table
#undef _tmax_pdqid
#undef _pdqinib_table
#undef _pdqcb_table
#undef _tmax_mbxid
#undef _mbxinib_table
#undef _mbxcb_table
#undef _tmax_mpfid
#undef _mpfinib_table
#undef _mpfcb_table
#undef _tmax_cycid
#undef _cycinib_table
#undef _cyccb_table
#undef _tmax_almid
#undef _alminib_table
#undef _almcb_table
#undef _isr_queue_table
#undef _tmax_isrid
#undef _isrinib_table
#undef _isrcb_table
#undef _tmevt_heap
#undef _istksz
#undef _istk
#undef _istkpt
#undef _kmmsz
#undef _kmm


#endif /* TOPPERS_LABEL_ASM */

#include "target_unrename.h"

#endif /* TOPPERS_KERNEL_RENAME_H */
