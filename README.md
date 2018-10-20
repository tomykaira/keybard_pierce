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

## Licenses

英語表記をご確認ください。
