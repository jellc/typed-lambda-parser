# typed-lambda-parser

[![C++](https://img.shields.io/badge/C++-11-green)](https://github.com/jellc/typed-lambda-parser/blob/master/src.cpp)
[![License](https://img.shields.io/badge/licence-MIT-blue)](https://github.com/jellc/typed-lambda-parser/blob/master/LICENSE)

2020 年度 S セメスター総合科目「計算の理論」のレポート課題で作成したもの。\
単純型付き λ 計算の型検査を行う。

## Instance

次のような BNF 記法で定まる syntax に従う単純型付き λ 式を扱う。

- \<expression> ::= \<alphabets> | '%' + \<alphabets> + ':' + \<type> + '.' + \<expression> | '(' + \<expression> \<expression> + ')'

- \<type> ::= \<alphabets> | \<type> + '->' + \<alphabets> | '(' + \<type> + ')' + '->' + \<alphabets>

英字、あるいは上のsyntaxで特別な意味をもたない記号のみからなる任意の空でない文字列を primitive 型、あるいは変数名とみなすことに注意する。\
ここでは実装の都合上、関数定義で通例用いられる 'λ' は 1 byte 文字である '%' で代用しており、また定数項は型環境に含まれるものとしている。

## Requirement

- C++11 以降に対応したコンパイラ

## Usage

まず [src.cpp](https://github.com/jellc/typed-lambda-parser/blob/master/src.cpp) をコンパイルし、実行ファイル(以下では 'type')を生成する。

```bash
g++ src.cpp -o type
```

文字列 expr を評価したいラムダ式、文字列 env を型環境として

```bash
$ ./type env expr
```

とコマンドラインで実行する。env は空文字列でも構わない。

### example 1

```bash
$ ./type "(%x:int->int.%y:int.(x y) %z:int.z)"
int->int
```

### example 2

```bash
$ ./type "+:int->int->int 1:int" "%x:int.((+ x) 1)"
int->int
```

### example 3

```bash
$ ./type "%x:int.(x x)"
parse failed.
problem: applying type doesn't match.
```

## Note

- 空白を含む引数は引用符が必要。
- 引数の空白は適宜無視されるが、区切り文字としては機能する。
- 入力サイズに対し線形時間で動作することが保証される。
- syntax に沿わない引数を指定された場合のエラー報告については改善の余地があるはず。

## Author

[Hirota Kinoshita](https://github.com/jellc)
