# KeyPierce

This repository provides the firmware and related data for #KeyPierce project.

Most descriptions are for `program` subdirectory.
Check the contents with the following guide to customize your own KeyPierce.

## How to compile programs

```
make
```

is enough.

Prerequisites are:

- avr-gcc and its toolchain
- ruby

If any compilation error happens, I am happy to have feedback in issues.

## Quick customization

- Morse mode:
  Try to change `loop_nyan` in `main()` to `loop_morse()`.
  The key pierce becomes full (what do you mean by "full" ?) alphanumeric keyboard that accepts morse code.
  Basic morse code chart is in [Wikipedia](https://en.wikipedia.org/wiki/Morse_code).=
- Meow mode:
  Are your an English speaker? Then cats will say not nyan, but meow.
  Rewrite keys sent in `send_nyan()` procedure to the keys "M, E, O, W".
  The USB HID Keycode list is in `helper/makelut.rb`.

## How to write programs

KeyPierce uses https://github.com/micronucleus/micronucleus as USB bootloader.
`commandline` is used to flush your new `main.hex` firmware file.

Run the following command **BEFORE** plugging in your KeyPierce, or pull off and plug in after running.

```
micronucleus/commandline/micronucleus --run main.hex
```

If your bootloader is broken due to some hack (I know, it happens), you can use 3 pads on the side of IC.

These are "MOSI" "MISO" "SCK" lines, now Arduino users figure out what it means.
No? Then, google or ask me in Twitter https://twitter.com/tomykaira_2 .

## Troubles, questions, advice, etc

Twitter https://twitter.com/tomykaira_2

## Licenses

Different license are picked for each file, check the file header for details.

Brand new files are under MIT license and files that depends on GPL / LPGL are GPLv3.

# JA version


このリポジトリは #KeyPierce プロジェクトの関連データを提供しています。

ほとんどの説明は `program` ディレクトリのコンテンツに関してです。

## コンパイル方法

`program` ディレクトリ内で

```
make
```

を実行してください。

avr-gcc
ruby

が必要です。何か問題がおきたら Issues から連絡してください。

## カスタマイズ例

`main()` 内の `loop_nyan()` の呼び出しを `loop_morse()` にすると、モールスコードキーボードとして使えます。
`send_nyan()` の実装を変更して、好きな文字列を送信するプログラムヲ作ってみましょう

## プログラムの書き込み方法

KeyPierce は https://github.com/micronucleus/micronucleus を利用しています。
ダウンロードしてコンパイルし、 `commandline` 以下のバイナリを作成することで main.hex を書き込むことができます。

以下のコマンドを KeyPierce を差し込む前に実行してください。

```
micronucleus/commandline/micronucleus --run main.hex
```

## なにかあったら

https://twitter.com/tomykaira_2 Twitter でおしえてください。
## ビルドガイド

同梱の部品を確認します。

表面：

![omote](https://image.ibb.co/dbz6Qq/DSC-0008.jpg)

若干R2などにおいてパッドとシルクの位置がずれていますが、全て横向きで上から順に配置します。

- D1, D2: ツェナーダイオード。黒いやつ。極性があります。USBポート側が正極側。ツェナーなので電流の通る向きと利用する向きが逆になることに注意してください。テスターで向きを確認した場合は、 GND から正極に電流が流れる向きに配置します。
- R1, R2: 68Ω抵抗。抵抗は２種類あり、裏側の欄外に数値を記載しています。わからなかったらテスターで測ってください。
- R3, R4: 1.5KΩ抵抗
- U$3: RGB LED 画面左下（USBポート側でR4とつながっているやつ）がコモンアノードです。裏側に緑のポッチがついています。
- キースイッチ: 一応添付していますが、お好みのものをつけるのがオススメ。

裏面：

![ura](https://preview.ibb.co/g61xWV/DSC-0009.jpg)

- Attiny85: ポッチがキースイッチ側。 **ブートローダのみ**書き込み済みです。完成後、 "How to write programs" の手順を参考にファームウェアを書きこんでください。

上述のとおりはんだ付けをします。

うまくついていれば、動きます。

質問など:

Twitter @tomykaira_2 まで

## Licenses

英語表記をご確認ください。
