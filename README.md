# TOPPERS/ASP Arduino ライブラリ（TA2LIB）

## 本ライブラリについて

本ライブラリは，[TOPPERS/ASPカーネル](https://www.toppers.jp/asp-kernel.html)（以下，ASPカーネル）を Wio Terminal の Arduino IDE 用のライブラリである．

[Seeed_Arduino_FreeRTOS](https://github.com/Seeed-Studio/Seeed_Arduino_FreeRTOS)にインスパイアされて開発した．

## 特徴

- Arduino 環境でマルチタスクプログラミングが可能．
- Arduino IDEでビルド可能
- Arduino の各種ライブラリを使用可能．

## オリジナルのASPカーネルとの違い

- 静的APIをサポートしていない
 - オブジェクトは初期化関数で実行時に生成する．
  -  [asp_wo_cfg](https://dev.toppers.jp/trac_user/contrib/browser/asp_wo_cfg/)をベースとしている．

## R2CAとの違い

- [R2CA](https://dev.toppers.jp/trac_user/contrib/wiki/rtos_arduino)は，ArduinoライブラリをASPカーネルでビルドすることを目的としている．
  一方，本ライブラリは Arduino IDE でASPカーネルの機能を使用することを目的としています．

## 動作確認バージョン

- Arduino IDE : 1.8.13 
	- プリコンパイル済のライブラリを使用するため，1.8.6以上が必要．
- Seeed SAMD Boards : 1.8.0


## 使用方法

### 機材の準備

SERCOM2にシリアルコンソールを使用する場合は，背面の以下のピンに USB UART を接続する．

| 端子番号 | 機能 |
----|---- 
|  6 | GND | 
|  8 | TXD | 
| 10 | RXD | 

### Wio Terminal用の環境の準備

[このページ](https://wiki.seeedstudio.com/jp/Wio-Terminal-Getting-Started/)を参考にWio Terminal用の環境を用意する．

### ライブラリのダウンロード

[リリース]()のページからリリースパッケージ(zipファイル)をダウンロードして，ArduinoIDEの

```
 スケッチ -> ライブラリをインクルード -> .ZIP形式のライブラリをインストール
```

を選択してリリースパッケージをインストールする．

Arduino IDE を再起動する．


### サンプルのビルド

- サンプルのプロジェクトを選択する．

```
 ファイル -> スケッチの例 -> ToppersASP -> ToppersASP_Blink 
```

- Wio TerminalのUSB端子とPCを接続する．

- マイコンボードに書き込むを選択して書き込む．

- ターミナルエミュレータを接続
	- Wio TerminalのUSB端子によるCOMポート
	- USB UARTによるCOMポート(オプション)

- 実行
	- Wio TerminalのUSB端子によるCOMポート にターミナルエミュレータを接続すると実行が開始される．


## 使用方法

### 初期化(setup())

setup()では，以下の内容を記載する．

- USBUartの初期化(オプション)
	- 必ず以下の順序で記載する．
- その他の初期化(オプション)
	- ポートの初期化等を記載する．
- カーネルの起動
	- StartToppersASP()を呼び出すことでカーネルが起動する．

```
void setup() {
  //USBUartの初期化(オプション)
  SerialUSB.begin(115200);
  ToppersASPDelayMs(1000);
  while (!SerialUSB);

  //その他の初期化(オプション)

  //カーネルの起動
  StartToppersASP();
}
```

### カーネルオブジェクトの初期化(user_inirtn())
カーネル起動後に呼び出される．カーネルオブジェクトの初期化処理を記載する．
これらのサービスコールは，この関数内で飲み呼び出し可能．

```
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
```

各オブジェクトのIDは次の値を使用する．nは1～10を指定可能．10個以上のオブジェクトを使用したい場合は，./doc/customize.txt を参考のこと．

| オブジェクト | マクロ |
----|---- 
|  タスク              | TASKn | 
|  セマフォ            | SEMn  | 
|  イベントフラグ      | FLGn  | 
|  データキュー        | DTQn  | 
|  優先度データキュー  | PDQn  | 
|  メールボックス      | MBXn  | 
|  メッセージバッファ  | MBFn  | 
|  周期ハンドラ        | CYCHDRn  | 
|  アラームハンドラ    | ALMHDRn  | 

#### タスク生成の例
タスクを生成する例は次のようになる．

```
void
user_inirtn() {

  T_CTSK  ctsk;
  ER    ercd;

  ctsk.tskatr = TA_ACT;
  ctsk.exinf = 1;
  ctsk.task = task1;
  ctsk.itskpri = 10;
  ctsk.stksz = 1024;
  ctsk.stk = NULL;
  ercd = cre_tsk(TASK1, &ctsk);

  ....
}
```

タスク本体の記述は通常のASPカーネルと同じである．タスク本体では，ASPカーネルのAPIとArduinoの両方を呼び出し可能である．

```
void
task1(intptr_t exinf)
{
  int count = 0;
  while(1){
    syslog(LOG_NOTICE, "task1 is running. %d", count++);
    dly_tsk(1000);
  }
}
```

### loop()

loop()関数はASPカーネルの最低優先度のタスクとして実行される．タスクIDは，IDLE_TASK である．

### カーネルの終了処理(user_terrtn()) オプション

カーネル終了時に呼ばれる関数．必要なら定義して終了処理等を記載する．

## 使用しているリソース

本ライブラリは以下のリソースを使用する．

- ROM/RAM
	- コードとデータを配置する．
	- 使用量はオブジェクト数に依存する．
  
- SysTick
	- カーネル内部のティックの生成に用いる．
     
- SERCOM2
	- コンソールの出力に使用．

## Tips

Arduino IDEから書き込めない場合は，Wio Terminalの電源スイッチを下に素早く2回スライドすることによりブートローダが起動して書き込み可能となる．

## フォルダ構成

- src 
	- ライブラリ本体
- examples
	- サンプル．
- asp_wo_cfg
	- ASPカーネル本体
- doc
	- 各種ドキュメント．
- standalone
	- スタンドアローン実行のためのフォルダ．

## 既知の問題

- 多重割込みが発生するとベクタ番号7ないし8のアドレスのハンドラが実行される場合がある．
