c_forth 0.1版
====================

######Author:Chai Fei
######E-mail:cforth@cfxyz.com
--------------------

## 简介

[c_forth](https://github.com/cforth/c_forth)项目是一个用C语言实现的简单的Forth系统。受到[耳朵](https://github.com/earforth)的[ear_cforth](https://github.com/earforth/ear-cforth)的启发，以及他的指点。再次感谢[耳朵](https://github.com/earforth)和[CNFIG社区](https://github.com/CNFIG)！！!

## 特性支持

1. 核心词;

2. 定义和使用扩展词;

3. `if else then` 控制语句（支持嵌套）;

4. `do loop` 循环语句（支持嵌套）;

5. 变量和常量（`variable` `constant`）;

6. 从外部读取forth代码（`load lib.fs`或者命令行`./forth lib.fs`）;

7. 递归定义扩展词（`myself`）;

8. 其他特性参见[Wiki](https://github.com/cforth/c_forth/wiki/c_forth%E6%94%AF%E6%8C%81%E7%9A%84%E7%89%B9%E6%80%A7)。

## 环境

1. Windows(32位/64位) + Cygwin + gcc 4.9.3

2. Linux(32位) + gcc 4.9.2

## 安装

```bash
make forth
```

## 使用

求8的阶乘，使用递归实现：

```bash
./forth lib.fs

>>> variable n
>>> : n@ n @ ;
>>> : n-- n@ 1 - n ! ;
>>> : factorial n@ 0 > if n@ n-- myself * else 1 then .s ; 
>>> 8 n ! factorial
<9> 8 7 6 5 4 3 2 1 1
<8> 8 7 6 5 4 3 2 1
<7> 8 7 6 5 4 3 2
<6> 8 7 6 5 4 6
<5> 8 7 6 5 24
<4> 8 7 6 120
<3> 8 7 720
<2> 8 5040
<1> 40320
```
