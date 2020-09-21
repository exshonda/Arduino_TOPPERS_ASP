# 各種情報
- Last Modified:2020 Sep 21 20:14:16

## このドキュメントについて
TOPPERS/ASP Arduino ライブラリ及びWio Terminalについて他のドキュメントに記載していない情報を記載する．

## ライブラリの再ビル

makeが使えてARM用のGCCのパスが通った環境で，以下のフォルダでmakeを実行する．

```
 ./src/cortex-m4
```

## スタンドアローン実行


makeが使えてARM用のGCCのパスが通った環境で，以下のフォルダでmakeを実行する．

```
 ./standalone
```

## Arduino関数の置き換え

以下の関数はArduinoのオリジナルの関数をToppersASP.cpp内で置き換えている．

- yield(void)
 - dly_tsk(0)に置き換え
- delay(unsigned long ms)
 - dly_tsk(ms)に置き換え

