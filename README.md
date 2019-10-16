cnforth -- C语言实现的Forth语言解释器
====================

###### Author:Chai Fei
###### E-mail:cforth@cfxyz.com
--------------------

## 简介

[cnforth](https://github.com/cforth/cnforth)项目是一个用C语言实现的简单的Forth语言解释器，不是一个传统的Forth系统。受到[耳朵](https://github.com/earforth)的[ear_cforth](https://github.com/earforth/ear-cforth)的启发，以及他的指点。再次感谢[耳朵](https://github.com/earforth)和[CNFIG社区](https://github.com/CNFIG)！！！

## 特性支持

1. 核心词;

2. 定义和使用扩展词;

3. `if else then` 控制语句（支持嵌套）;

4. `do loop` 循环语句（支持嵌套）;

5. 变量和常量（`variable` `constant`）;

6. 状态变量`state`，状态切换词`[`和`]` ; 

7. 反编译扩展词`see` ;

8. 递归定义扩展词（`myself`）;

9. 从外部读取forth代码（`load lib.fs`或者命令行`./forth lib.fs`）;

10. 其他特性参见[Wiki](https://github.com/cforth/cnforth/wiki/cnforth%E6%94%AF%E6%8C%81%E7%9A%84%E7%89%B9%E6%80%A7)。

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

>>> : factorial ( Num -- FactorialResult ) dup 1 = if drop 1 else dup 1- myself * then ;
>>> 8 factorial .
40320
```
