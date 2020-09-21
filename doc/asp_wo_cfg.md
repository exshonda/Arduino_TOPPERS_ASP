# TOPPERS/ASP Kernel without Configurator の変更点について
-  Last Modified:2020 Sep 21 18:37:20

## このドキュメントについて

TOPPERS/ASP Arduino ライブラリ向けに，オリジナルの TOPPERS/ASP Kernel 
without Configurator からの変更点についてまとめたものである．

## オリジナルのSVN

オリジナルはTOPPERS Contributed Softwareにコミットされた以下のコードである．

  [TOPPERS Contributed Software SVN](https://dev.toppers.jp/trac_user/contrib/browser/asp_wo_cfg/)
    
## 変更点について
Wio TerminalのArduinoライブラリとして動作させるために次の変更を実施．

### SAMD51依存部の追加
- クロックやポートの初期化は行わない．

### 全体    
- ARDUINO_PASS を定義すると各種インクルードファイルを相対パスでインクルードするように切り替える．        
- コンパイルオプションの-Dなしでビルド出来るように変更．
 - ターゲット依存部のヘッダファイルで定義．
- 外部ツールなしでビルド出来るように変更．
 - offset.hは事前に生成しておく．
 - makedep は使用しない．
- ARM_M依存部
 - bitpat_cfgint[]を使用しないよう変更．
 - int_iipm_tbl[]を使用しないよう変更．  
 - 未定義の割込みが発生したら(default_int_handler())，Arduino側のベクタテーブルを読み込んで，定義されているハンドラを呼び出す． 
 - タイマ割込みハンドラで，Arduino側のタイマ割込みハンドラを呼び出す．
