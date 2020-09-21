#include <ToppersASP.h>

#define USE_SERIALUSB

void setup() {
#ifdef USE_SERIALUSB
  SerialUSB.begin(115200);
  ToppersASPDelayMs(1000);
  while (!SerialUSB);
#endif /* USE_SERIALUSB */

  pinMode(LED_BUILTIN, OUTPUT);

  StartToppersASP();
}

int loop_count;

void loop() {
#ifdef USE_SERIALUSB
  SerialUSB.print("loop: ");
  SerialUSB.println(loop_count++);
#else /* USE_SERIALUSB */
	syslog(LOG_NOTICE, "loop is running. %d", loop_count++);
#endif /* USE_SERIALUSB */
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}

/*
 *  サービスコールのエラーのログ出力
 */
void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
  if (ercd < 0) {
    t_perror(LOG_ERROR, file, line, expr, ercd);
  }
}

#define SVC_PERROR(expr)  svc_perror(__FILE__, __LINE__, #expr, (expr))

/*
 *  各タスクの優先度の定義
 */

#define MAIN_PRIORITY  5   /* メインタスクの優先度 */                
#define HIGH_PRIORITY 9   /* 並行実行されるタスクの優先度 */
#define MID_PRIORITY  10
#define LOW_PRIORITY  11

/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef TASK_PORTID
#define TASK_PORTID   1     /* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define STACK_SIZE    4096    /* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF    ULONG_C(1000000)  /* 速度計測用のループ回数 */
#endif /* LOOP_REF */

extern void task(intptr_t exinf);
extern void main_task(intptr_t exinf);
extern void tex_routine(TEXPTN texptn, intptr_t exinf);
#ifdef CPUEXC1
extern void cpuexc_handler(void *p_excinf);
#endif /* CPUEXC1 */
extern void cyclic_handler(intptr_t exinf);
extern void alarm_handler(intptr_t exinf);

#define MAIN_TASK TASK10

void
user_inirtn(void)
{
	T_CTSK	ctsk;
	T_DTEX	dtex;
	T_CCYC	ccyc;
	T_CALM	calm;
	T_DEXC	dexc;
	ER		ercd;

	/*
	 *  サンプルプログラムの初期化処理（sample1.cfg相当の処理）
	 *
	 *  CRE_TSK(TASK1, { TA_NULL, 1, task, MID_PRIORITY,
	 *											STACK_SIZE, NULL });
	 *  CRE_TSK(TASK2, { TA_NULL, 2, task, MID_PRIORITY,
	 *											STACK_SIZE, NULL });
	 *  CRE_TSK(TASK3, { TA_NULL, 3, task, MID_PRIORITY,
	 *											STACK_SIZE, NULL });
	 *  CRE_TSK(MAIN_TASK, { TA_ACT, 0, main_task, MAIN_PRIORITY,
	 *											STACK_SIZE, NULL });
	 */
	ctsk.tskatr = TA_NULL;
	ctsk.exinf = 1;
	ctsk.task = task;
	ctsk.itskpri = MID_PRIORITY;
	ctsk.stksz = STACK_SIZE;
	ctsk.stk = NULL;
	ercd = cre_tsk(TASK1, &ctsk);
	assert(ercd == E_OK);

	ctsk.tskatr = TA_NULL;
	ctsk.exinf = 2;
	ctsk.task = task;
	ctsk.itskpri = MID_PRIORITY;
	ctsk.stksz = STACK_SIZE;
	ctsk.stk = NULL;
	ercd = cre_tsk(TASK2, &ctsk);
	assert(ercd == E_OK);

	ctsk.tskatr = TA_NULL;
	ctsk.exinf = 3;
	ctsk.task = task;
	ctsk.itskpri = MID_PRIORITY;
	ctsk.stksz = STACK_SIZE;
	ctsk.stk = NULL;
	ercd = cre_tsk(TASK3, &ctsk);
	assert(ercd == E_OK);

	ctsk.tskatr = TA_ACT;
	ctsk.exinf = 0;
	ctsk.task = main_task;
	ctsk.itskpri = MAIN_PRIORITY;
	ctsk.stksz = STACK_SIZE;
	ctsk.stk = NULL;
	ercd = cre_tsk(MAIN_TASK, &ctsk);
	assert(ercd == E_OK);

	/*
	 *  DEF_TEX(TASK1, { TA_NULL, tex_routine })
	 *  DEF_TEX(TASK2, { TA_NULL, tex_routine })
	 *  DEF_TEX(TASK3, { TA_NULL, tex_routine })
	 */
	dtex.texatr = TA_NULL;
	dtex.texrtn = tex_routine;
	ercd = def_tex(TASK1, &dtex);
	assert(ercd == E_OK);
		
	dtex.texatr = TA_NULL;
	dtex.texrtn = tex_routine;
	ercd = def_tex(TASK2, &dtex);
	assert(ercd == E_OK);

	dtex.texatr = TA_NULL;
	dtex.texrtn = tex_routine;
	ercd = def_tex(TASK3, &dtex);
	assert(ercd == E_OK);

	/*
	 *  CRE_CYC(CYCHDR1, { TA_NULL, 0, cyclic_handler, 2000, 0 });
	 *  CRE_ALM(ALMHDR1, { TA_NULL, 0, alarm_handler });
	 *  DEF_EXC(CPUEXC1, { TA_NULL, cpuexc_handler });
	 */
	ccyc.cycatr = TA_NULL;
	ccyc.exinf = 0;
	ccyc.cychdr = cyclic_handler;
	ccyc.cyctim = 2000;
	ccyc.cycphs = 0;
	ercd = cre_cyc(CYCHDR1, &ccyc);
	assert(ercd == E_OK);

	calm.almatr = TA_NULL;
	calm.exinf = 0;
	calm.almhdr = alarm_handler;
	ercd = cre_alm(ALMHDR1, &calm);
	assert(ercd == E_OK);

#ifdef CPUEXC1
	dexc.excatr = TA_NULL;
	dexc.exchdr = cpuexc_handler;
	ercd = def_exc(CPUEXC1, &dexc);
	assert(ercd == E_OK);
#endif /* CPUEXC1 */
}

void
user_terrtn(void) {
  
}

/*
 *  並行実行されるタスクへのメッセージ領域
 */
char	message[3];

/*
 *  ループ回数
 */
ulong_t	task_loop;		/* タスク内でのループ回数 */
ulong_t	tex_loop;		/* 例外処理ルーチン内でのループ回数 */

/*
 *  並行実行されるタスク
 */
void task(intptr_t exinf)
{
	volatile ulong_t	i;
	int_t		n = 0;
	int_t		tskno = (int_t) exinf;
	const char	*graph[] = { "|", "  +", "    *" };
	char		c;

	SVC_PERROR(ena_tex());  
	while (true) {
		syslog(LOG_NOTICE, "task%d is running (%03d).   %s",
										tskno, ++n, graph[tskno-1]);
		for (i = 0; i < task_loop; i++);
		c = message[tskno-1];
		message[tskno-1] = 0;
		switch (c) {
		case 'e':
			syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
			SVC_PERROR(ext_tsk());
			assert(0);
		case 's':
			syslog(LOG_INFO, "#%d#slp_tsk()", tskno);
			SVC_PERROR(slp_tsk());
			break;
		case 'S':
			syslog(LOG_INFO, "#%d#tslp_tsk(10000)", tskno);
			SVC_PERROR(tslp_tsk(10000));
			break;
		case 'd':
			syslog(LOG_INFO, "#%d#dly_tsk(10000)", tskno);
			SVC_PERROR(dly_tsk(10000));
			break;
		case 'y':
			syslog(LOG_INFO, "#%d#dis_tex()", tskno);
			SVC_PERROR(dis_tex());
			break;
		case 'Y':
			syslog(LOG_INFO, "#%d#ena_tex()", tskno);
			SVC_PERROR(ena_tex());
			break;
#ifdef CPUEXC1
		case 'z':
			syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
			RAISE_CPU_EXCEPTION;
			break;
		case 'Z':
			SVC_PERROR(loc_cpu());
			syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
			RAISE_CPU_EXCEPTION;
			SVC_PERROR(unl_cpu());
			break;
#endif /* CPUEXC1 */
		default:
			break;
		}
	}
}

/*
 *  並行して実行されるタスク用のタスク例外処理ルーチン
 */
void tex_routine(TEXPTN texptn, intptr_t exinf)
{
	volatile ulong_t	i;
	int_t	tskno = (int_t) exinf;

	syslog(LOG_NOTICE, "task%d receives exception 0x%04x.", tskno, texptn);
	for (i = 0; i < tex_loop; i++);

	if ((texptn & 0x8000U) != 0U) {
		syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
		SVC_PERROR(ext_tsk());
		assert(0);
	}
}

/*
 *  CPU例外ハンドラ
 */
#ifdef CPUEXC1

void
cpuexc_handler(void *p_excinf)
{
	ID		tskid;

	syslog(LOG_NOTICE, "CPU exception handler (p_excinf = %08p).", p_excinf);
	if (sns_ctx() != true) {
		syslog(LOG_WARNING,
					"sns_ctx() is not true in CPU exception handler.");
	}
	if (sns_dpn() != true) {
		syslog(LOG_WARNING,
					"sns_dpn() is not true in CPU exception handler.");
	}
	syslog(LOG_INFO, "sns_loc = %d sns_dsp = %d sns_tex = %d",
									sns_loc(), sns_dsp(), sns_tex());
	syslog(LOG_INFO, "xsns_dpn = %d xsns_xpn = %d",
									xsns_dpn(p_excinf), xsns_xpn(p_excinf));

	if (xsns_xpn(p_excinf)) {
		syslog(LOG_NOTICE, "Sample program ends with exception.");
		SVC_PERROR(ext_ker());
		assert(0);
	}

	SVC_PERROR(iget_tid(&tskid));
	SVC_PERROR(iras_tex(tskid, 0x8000U));
}

#endif /* CPUEXC1 */

/*
 *  周期ハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void cyclic_handler(intptr_t exinf)
{
	SVC_PERROR(irot_rdq(HIGH_PRIORITY));
	SVC_PERROR(irot_rdq(MID_PRIORITY));
	SVC_PERROR(irot_rdq(LOW_PRIORITY));
}

/*
 *  アラームハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void alarm_handler(intptr_t exinf)
{
	SVC_PERROR(irot_rdq(HIGH_PRIORITY));
	SVC_PERROR(irot_rdq(MID_PRIORITY));
	SVC_PERROR(irot_rdq(LOW_PRIORITY));
}

/*
 *  メインタスク
 */
void main_task(intptr_t exinf)
{
	char	c;
	ID		tskid = TASK1;
	int_t	tskno = 1;
	ER_UINT	ercd;
	PRI		tskpri;
#ifndef TASK_LOOP
	volatile ulong_t	i;
	SYSTIM	stime1, stime2;
#endif /* TASK_LOOP */
#ifdef TOPPERS_SUPPORT_GET_UTM
	SYSUTM	utime1, utime2;
#endif /* TOPPERS_SUPPORT_GET_UTM */

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);
  
	/*
	 *  シリアルポートの初期化
	 *
	 *  システムログタスクと同じシリアルポートを使う場合など，シリアル
	 *  ポートがオープン済みの場合にはここでE_OBJエラーになるが，支障は
	 *  ない．
	 */
	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

	/*
 	 *  ループ回数の設定
	 *
	 *  並行実行されるタスク内での空ループの回数（task_loop）は，空ルー
	 *  プの実行時間が約0.4秒になるように設定する．この設定のために，
	 *  LOOP_REF回の空ループの実行時間を，その前後でget_timを呼ぶことで
	 *  測定し，その測定結果から空ループの実行時間が0.4秒になるループ回
	 *  数を求め，task_loopに設定する．
	 *
	 *  LOOP_REFは，デフォルトでは1,000,000に設定しているが，想定したよ
	 *  り遅いプロセッサでは，サンプルプログラムの実行開始に時間がかか
	 *  りすぎるという問題を生じる．逆に想定したより速いプロセッサでは，
	 *  LOOP_REF回の空ループの実行時間が短くなり，task_loopに設定する値
	 *  の誤差が大きくなるという問題がある．
	 *
	 *  そこで，そのようなターゲットでは，target_test.hで，LOOP_REFを適
	 *  切な値に定義するのが望ましい．
	 *
	 *  また，task_loopの値を固定したい場合には，その値をTASK_LOOPにマ
	 *  クロ定義する．TASK_LOOPがマクロ定義されている場合，上記の測定を
	 *  行わずに，TASK_LOOPに定義された値を空ループの回数とする．
	 *
	 * ターゲットによっては，空ループの実行時間の1回目の測定で，本来よ
	 * りも長めになるものがある．このようなターゲットでは，MEASURE_TWICE
	 * をマクロ定義することで，1回目の測定結果を捨てて，2回目の測定結果
	 * を使う．
	 *
	 *  タスク例外処理ルーチン内での空ループの回数（tex_loop）は，
	 *  task_loopの4分の1の値（空ループの実行時間が0.1秒になるループ回
	 *  数）に設定する．
	 */
#ifdef TASK_LOOP
	task_loop = TASK_LOOP;
#else /* TASK_LOOP */

#ifdef MEASURE_TWICE
	task_loop = LOOP_REF;
	SVC_PERROR(get_tim(&stime1));
	for (i = 0; i < task_loop; i++);
	SVC_PERROR(get_tim(&stime2));
#endif /* MEASURE_TWICE */

	task_loop = LOOP_REF;
	SVC_PERROR(get_tim(&stime1));
	for (i = 0; i < task_loop; i++);
	SVC_PERROR(get_tim(&stime2));
	task_loop = LOOP_REF * 400UL / (stime2 - stime1);

#endif /* TASK_LOOP */
	tex_loop = task_loop / 4;

	/*
 	 *  タスクの起動
	 */
	//SVC_PERROR(act_tsk(TASK1));
	//SVC_PERROR(act_tsk(TASK2));
	//SVC_PERROR(act_tsk(TASK3));
  
	/*
 	 *  メインループ
	 */
	do {
		SVC_PERROR(serial_rea_dat(TASK_PORTID, &c, 1));
		switch (c) {
		case 'e':
		case 's':
		case 'S':
		case 'd':
		case 'y':
		case 'Y':
		case 'z':
		case 'Z':
			message[tskno-1] = c;
			break;
		case '1':
			tskno = 1;
			tskid = TASK1;
			break;
		case '2':
			tskno = 2;
			tskid = TASK2;
			break;
		case '3':
			tskno = 3;
			tskid = TASK3;
			break;
		case 'a':
			syslog(LOG_INFO, "#act_tsk(%d)", tskno);
			SVC_PERROR(act_tsk(tskid));
			break;
		case 'A':
			syslog(LOG_INFO, "#can_act(%d)", tskno);
			SVC_PERROR(ercd = can_act(tskid));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "can_act(%d) returns %d", tskno, ercd);
			}
			break;
		case 't':
			syslog(LOG_INFO, "#ter_tsk(%d)", tskno);
			SVC_PERROR(ter_tsk(tskid));
			break;
		case '>':
			syslog(LOG_INFO, "#chg_pri(%d, HIGH_PRIORITY)", tskno);
			SVC_PERROR(chg_pri(tskid, HIGH_PRIORITY));
			break;
		case '=':
			syslog(LOG_INFO, "#chg_pri(%d, MID_PRIORITY)", tskno);
			SVC_PERROR(chg_pri(tskid, MID_PRIORITY));
			break;
		case '<':
			syslog(LOG_INFO, "#chg_pri(%d, LOW_PRIORITY)", tskno);
			SVC_PERROR(chg_pri(tskid, LOW_PRIORITY));
			break;
		case 'G':
			syslog(LOG_INFO, "#get_pri(%d, &tskpri)", tskno);
			SVC_PERROR(ercd = get_pri(tskid, &tskpri));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "priority of task %d is %d", tskno, tskpri);
			}
			break;
		case 'w':
			syslog(LOG_INFO, "#wup_tsk(%d)", tskno);
			SVC_PERROR(wup_tsk(tskid));
			break;
		case 'W':
			syslog(LOG_INFO, "#can_wup(%d)", tskno);
			SVC_PERROR(ercd = can_wup(tskid));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "can_wup(%d) returns %d", tskno, ercd);
			}
			break;
		case 'l':
			syslog(LOG_INFO, "#rel_wai(%d)", tskno);
			SVC_PERROR(rel_wai(tskid));
			break;
		case 'u':
			syslog(LOG_INFO, "#sus_tsk(%d)", tskno);
			SVC_PERROR(sus_tsk(tskid));
			break;
		case 'm':
			syslog(LOG_INFO, "#rsm_tsk(%d)", tskno);
			SVC_PERROR(rsm_tsk(tskid));
			break;
		case 'x':
			syslog(LOG_INFO, "#ras_tex(%d, 0x0001U)", tskno);
			SVC_PERROR(ras_tex(tskid, 0x0001U));
			break;
		case 'X':
			syslog(LOG_INFO, "#ras_tex(%d, 0x0002U)", tskno);
			SVC_PERROR(ras_tex(tskid, 0x0002U));
			break;
		case 'r':
			syslog(LOG_INFO, "#rot_rdq(three priorities)");
			SVC_PERROR(rot_rdq(HIGH_PRIORITY));
			SVC_PERROR(rot_rdq(MID_PRIORITY));
			SVC_PERROR(rot_rdq(LOW_PRIORITY));
			break;
		case 'c':
			syslog(LOG_INFO, "#sta_cyc(1)");
			SVC_PERROR(sta_cyc(CYCHDR1));
			break;
		case 'C':
			syslog(LOG_INFO, "#stp_cyc(1)");
			SVC_PERROR(stp_cyc(CYCHDR1));
			break;
		case 'b':
			syslog(LOG_INFO, "#sta_alm(1, 5000)");
			SVC_PERROR(sta_alm(ALMHDR1, 5000));
			break;
		case 'B':
			syslog(LOG_INFO, "#stp_alm(1)");
			SVC_PERROR(stp_alm(ALMHDR1));
			break;

		case 'V':
#ifdef TOPPERS_SUPPORT_GET_UTM
			SVC_PERROR(get_utm(&utime1));
			SVC_PERROR(get_utm(&utime2));
			syslog(LOG_NOTICE, "utime1 = %ld, utime2 = %ld",
										(ulong_t) utime1, (ulong_t) utime2);
#else /* TOPPERS_SUPPORT_GET_UTM */
			syslog(LOG_NOTICE, "get_utm is not supported.");
#endif /* TOPPERS_SUPPORT_GET_UTM */
			break;

		case 'v':
			SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO),
										LOG_UPTO(LOG_EMERG)));
			break;
		case 'q':
			SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_NOTICE),
										LOG_UPTO(LOG_EMERG)));
			break;

#ifdef BIT_KERNEL
		case ' ':
			SVC_PERROR(loc_cpu());
			{
				extern ER	bit_kernel(void);

				SVC_PERROR(ercd = bit_kernel());
				if (ercd >= 0) {
					syslog(LOG_NOTICE, "bit_kernel passed.");
				}
			}
			SVC_PERROR(unl_cpu());
			break;
#endif /* BIT_KERNEL */

		default:
			break;
		}
	} while (c != '\003' && c != 'Q');

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ext_ker());
	assert(0);
}
