c_forth 0.1版
====================

######作者:Chai Fei
######E-mail:cforth@cfxyz.com
--------------------

## 简介

[c_forth](https://github.com/cforth/c_forth)项目是一个用C语言实现的玩具版Forth编程语言。受到[耳朵](https://github.com/earforth)的[ear_cforth](https://github.com/earforth/ear-cforth)的启发，以及他的指点。再次感谢[耳朵](https://github.com/earforth)和[CNFIG社区](https://github.com/CNFIG)！！!

## 特性支持

1. 核心词;

2. 定义和使用扩展词;

3. `if else then` 控制语句（包含`if then`）;

4. `do loop` 循环语句;

5. `variable`定义变量，`!`存入，`@`取出;

6. 从外部读取forth代码（`./forth lib.fs`）;

7. 递归定义扩展词（`myself`）。

## 编译环境

1. Windows(32位/64位) + Cygwin + gcc 4.9.3

2. Linux(32位) + gcc 4.9.2

## 安装

```bash
make forth
```

## 使用

求8的阶乘，使用递归实现：

```bash
./forth

>>> variable n
>>> : n@ n @ ;
>>> : n-- n@ 1 - n ! ;
>>> : factorial n@ 0 > if n@ n-- myself * else 1 then .s ; 
>>> 8 n ! factorial

40320
```

## 2015-12-9

c_forth 0.1版的开发至此告一段落，在更加深入学习Forth编程语言的实现后，我将继续开发0.2版。
