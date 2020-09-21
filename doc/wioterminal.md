# Wio Terminalに関する情報

## ブートローダ

[ここから](https://github.com/Seeed-Studio/uf2-samdx1)ダウロードしてビルドする．

## Arduinoのライブラリ仕様
[オフィシャルドキュメント](https://arduino.github.io/arduino-cli/library-specification/)


### フォルダ
- ./src
- ./examples

### ビルド対象のコード
- src以下の全てのファイル

### インクルードルール
- library.propertiesにユーザーインクルードするファイルを指定可能．
- src内はインクルード対象．それ以外は対象ではないので相対パスで指定
  - srcより上のフォルダでも問題なさそう．

### スタティックライブラリの使い方．
[サンプル](https://wiki.seeedstudio.com/Software-Static-Library/)
- ライブラリがあってもソースはコンパイルしてくれる．
- -lはつけなくても勝手にしてくれる．
- インクルードファイルの挙動
  - ユーザー側のファイルはインクルードできない．

### Serialについて
- sercom1 : RTL8720D に接続 : Serial2
- sercom2 : 裏のラズパイ互換コネクタに接続 : Serial1
- sercom3 : grove 左
- sercom4 : grove 右

[sercom3/sercom4 の使い方例](https://gist.github.com/ciniml/bd2bb4dea22a55c067ca6790ad0c3eae)
