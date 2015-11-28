c_forth 开发分支
====================

######E-mail:cforth@cfxyz.com
--------------------

## 简介

[c_forth](https://github.com/cforth/c_forth)项目原先只是一个用C语言写的模拟forth系统，不是一个真正的forth系统。受到[耳朵](https://github.com/earforth)的[ear_cforth](https://github.com/earforth/ear-cforth)的启发，以及他的指点，我开始写真正的forth系统。目前这个c_forth是从ear-cforth克隆过来，并且自己加上了控制语句、循环语句与变量等。我正在努力的写自己的forth系统。再次感谢[耳朵](https://github.com/earforth)和[CNFIG社区](https://github.com/CNFIG)！！!

## 编译环境

1. Windows(32位) + Cygwin + gcc 4.9.3 ;

2. Windows(64位) + Cygwin + gcc 4.9.3。


## 特性支持

1. 核心字;

2. 定义和使用扩展字;

3. if else then 控制语句（包含if then）;

4. for next 循环语句;

5. $字定义变量，!存入，@取出;

6. 从外部读取forth代码（Hack中...）。

--------------------
