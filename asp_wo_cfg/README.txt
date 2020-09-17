
	TOPPERS/ASP Kernel without Configurator（Release 1.9.1）
		Toyohashi Open Platform for Embedded Real-Time Systems/
		Advanced Standard Profile Kernel

TOPPERS/ASPカーネル without Configurator（以下，ASP_WO_CFGカーネル）は，
コンフィギュレータなしでTOPPERS/ASPカーネルを使いたいという要望に対応し
て，ASPカーネルを改造して作成したものです．改造にあたっては，ASPカーネ
ルの動的機能生成拡張パッケージのコードも活用しました．

オリジナルのSVN
  https://dev.toppers.jp/trac_user/contrib/browser/asp_wo_cfg/
  
以下の変更を追加．  
・SAMD51依存部の追加
・-Dなしでビルド出来るように変更．
・外部ツールなしでビルド出来るように変更．
  ・offset.hは事前に生成しておく．
  ・makedep は使用しない．
・ARM_M依存部
  ・bitpat_cfgint[]を使用しないよう変更．
  ・int_iipm_tbl[]を使用しないよう変更．
  
【使用方法】

ASP_WO_CFGカーネルのユーザーズマニュアルを用意する予定はありません．ま
ずは，サンプルプログラム（sampleディレクトリの下のファイル）を，ASPカー
ネルのものと比較することで，使い方を把握して下さい．

カーネルオブジェクトの数を定義するkernel_cfg.hファイルと，カーネルオブ
ジェクトのIDを定義するkernel_id.hファイルは，アプリケーション側で用意し
てください．kernel_cfg.cは，カーネル側で用意しているものをそのまま使用
してください．

初期化処理と終了時処理は，アプリケーション側で以下の名称で用意して下さ
い．

	void	inirtn(void);
	void	terrtn(void);

カーネルオブジェクトの生成は，システムコンフィギュレーションファイルに
静的APIを記述する代わりに，初期化処理（inirtn）から以下のサービスコール
を呼び出すことで行います．これらのサービスコールは，初期化処理の中での
み呼び出すことができます（カーネルの起動後は呼び出せません）．

	ER	cre_tsk(ID tskid, const T_CTSK *pk_ctsk);
	ER	cre_sem(ID semid, const T_CSEM *pk_csem);
	ER	cre_flg(ID flgid, const T_CFLG *pk_cflg);
	ER	cre_dtq(ID dtqid, const T_CDTQ *pk_cdtq);
	ER	cre_pdq(ID pdqid, const T_CPDQ *pk_cpdq);
	ER	cre_mbx(ID mbxid, const T_CMBX *pk_cmbx);
	ER	cre_mpf(ID mpfid, const T_CMPF *pk_cmpf);
	ER	cre_cyc(ID cycid, const T_CCYC *pk_ccyc);
	ER	cre_alm(ID almid, const T_CALM *pk_calm);
	ER	cfg_int(INTNO intno, const T_CINT *pk_cint);
	ER	att_isr(const T_CISR *pk_cisr);
	ER	def_inh(INHNO inhno, const T_DINH *pk_dinh);
	ER	def_exc(EXCNO excno, const T_DEXC *pk_dexc);

【質問・バグレポート・意見等の送付先】

ASP_WO_CFGカーネルに対するご意見・ご質問等は，TOPPERSユーザーズメーリン
グリスト（users@toppers.jp）宛にお願いします．

TOPPERSユーザーズメーリングリストへの登録方法については，以下のページに
説明があります．

	http://www.toppers.jp/community.html

【ポーティングにあたって】

ASPカーネルのターゲット依存部に対して，以下の修正が必要です．

(1) 割込み要求ライン属性の設定方法と，割込みハンドラおよびCPU例外ハンド
ラの登録方法が，大幅に変更されています．cfg_int，def_inh，def_excの各サー
ビスコールは，カーネル内の標準的なデータ構造に登録内容を書き込みます．
それらの処理が終わった後，カーネルの起動前に，ターゲット依存部の
target_prepare が呼ばれます．

	void target_prepare(void)

target_prepareの中では，カーネル内の標準的なデータ構造を読み込んで，割
込み要求ラインの属性の設定と，割込みハンドラおよびCPU例外ハンドラの登録
処理を行って下さい．

この変更に伴って，以下のマクロと関数（または，それに変わるマクロと関数
など）は不要になります．

	void x_config_int(INTNO intno, ATR intatr, PRI intpri)

	INT_ENTRY(inhno, inthdr)
	INTHDR_ENTRY(inhno, inhno_num, inthdr)
	void x_define_inh(INHNO inhno, FP int_entry)

	EXC_ENTRY(excno, exchdr)
	EXCHDR_ENTRY(excno, excno_num, exchdr)
	void x_define_exc(EXCNO excno, FP exc_entry)

(2) cre_tsk，att_isr，cfg_int，def_inh，def_excでターゲット依存にエラー
チェックを行うための以下の関数が必要になります．ターゲット依存にチェッ
クすべきエラーがない場合には，こららの関数を，常にE_OKを返す関数として
実装してください．

	ER target_check_ctsk(ID tskid, const T_CTSK *pk_ctsk)
	ER target_check_aisr(const T_CISR *pk_cisr)
	ER target_check_cint(INTNO intno, const T_CINT *pk_cint)
	ER target_check_dinh(INHNO inhno, const T_DINH *pk_dinh)
	ER target_check_dexc(EXCNO excno, const T_DEXC *pk_dexc)

(3) ターゲット依存部にコンフィギュレーションファイル（例えば，
target_timer.cfgやtarget_serial.cfg）があった場合，その内容を，初期化処
理（例えば，target_timer_initializeやsio_initialize）に追加します．

また，コンフィギュレータを使わないとい趣旨からは，offset.hの自動生成も
使わないのが望ましいと考えます．ターゲット毎にoffset.hを準備する方法を
推奨します．

以上
