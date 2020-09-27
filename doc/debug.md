# JTAGICEを用いたデバッグ方法について
- Last Modified:2020 Sep 27 11:36:38

## このドキュメントについて
TOPPERS/ASP Arduino ライブラリを用いたアプリケーションをJTAG-ICEを用い
てデバッグする方法についてまとめたものである．

## SWD端子について
3種類の方法がある．
- PCBの背面のテストポイント
- Groveコネクタ
- 基板左側のFPCコネクタ（v1.2以降）

基板左側のFPCコネクタを使う場合は，以下の機材でデバッガとの接続を確認．

- [aitendo FFCケーブル★0.5-10P★ FFC0510P](https://www.aitendo.com/product/7614)
- [aitendo 0.5/10P★FFC変換基板 FCB05-10PC](https://www.aitendo.com/product/18491)

FCB05-10PCのそれぞれのピンには以下の信号が接続される．

| 変換基板端子番号 | 機能 |
----|---- 
| 1 | SWDCLCK   | 
| 2 | SWDIO        | 
| 3 | SWD          | 
| 4 | MCU_RESET    | 
| 6 | GND          | 
| 8 | RTL8720D_TXD   | 
| 9 | RTL8720D_RXD   | 
| 10 | RTL_PU   | 

## Atmel Studio + Atmel ICE によるデバッグ

### 機材

Atmel ICE はBASICだと端子の変換が必要となるため，ケーブルの先が個別の端子となっているケーブルが付属している以下のものを推奨．

- [ATMEL-ICE](https://www.digikey.jp/product-detail/ja/microchip-technology/ATATMEL-ICE/ATATMEL-ICE-ND/4753379)

Atmel Studio は，次のバージョンで動作を確認

- 7.0

### 接続

ATMEL-ICEのVrefはないとエラーとなるため，とりあえずRTL8720D_TXDに接続．

| 変換基板端子番号 | ATMEL-ICE | 機能 |
----|----|----  
| 1 | 4  | SWDCLCK   | 
| 2 | 2  | SWDIO        | 
| 3 | 6  | SWD          | 
| 4 | 10 | MCU_RESET    | 
| 6 |  9 | GND          | 
| 8 |  1 | RTL8720D_TXD   | 
| 9 |    | RTL8720D_RXD   | 
| 10 |   | RTL_PU   | 

### デバッグ

- Arduinoの ファイル → 環境設定 で，"より詳細な情報を表示する" のチェックボックスをセットする．

- スケッチを記述して"検証"のボタンを押して，実行ログを確認する．


```
"C:\\Users\\honda\\AppData\\Local\\Arduino15\\packages\\Seeeduino\\tools\\arm-none-eabi-gcc\\7-2017q4/bin/arm-none-eabi-size" -A "C:\\Users\\honda\\AppData\\Local\\Temp\\arduino_build_482246/Blink3.ino.elf"  
最大507904バイトのフラッシュメモリのうち、スケッチが44916バイト（8%）を使っています。
```

- xxx.elfがデバッグ対象のファイルである．
 - Arduino IDE を起動し直すとフォルダが変更されるため注意すること．

- Atmel Studioを起動する．

- メニューの File -> Open Object File For Debugging を選択

- Selet the Object File To Debug に上記のelfファイルを指定する．

- 他の項目を設定して Next を押す．

- Device Family に SAMD51 を選択

- Device のリストから ATSAMD51P19A を選択

- Start Debugging を選択すると Tool の指定が促されるので，ATMEL-ICEをPCに接続して，Selct debugger/programmer を選択して，Atmel-ICEを指定する．

- デバッグを開始．
