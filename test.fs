: test_ok   ." test OK!" cr ;
: test_fail ." test Fail!" cr  ;
: test = if test_ok else test_fail then ;

." Fib List Test: Print 8 Fib list numbers " cr
: fib_ do  .s 2dup + loop ;
: fib  0 1 2swap fib_ .s ;
: test_fib 6 0 fib + + + + + + + 33 test ;
test_fib .s

." Factorial Test: Us recursive to Print Eight's Factorial " cr
variable n
: n@ n @ ;
: n-- n@ 1 - n ! ;
: factorial n@ 0 > if n@ n-- myself * else 1 then .s ; 
see factorial
: test_fact 8 n ! factorial 40320 test ;
test_fact .s

." Matrix: Test nested statements " cr
: star 42 emit ;
: matrix swap 0 do dup >r 0 do star space loop cr r> loop drop cr ;
5 4 matrix

." Hanoi Tower: level 3" cr
: 4dup 4 pick 4 pick 4 pick 4 pick ;
: 4drop drop drop drop drop ;
: move dup 1 = if 4 pick emit space 2 pick emit cr else 4dup 1- >r swap r> myself 4 pick emit space 2 pick emit cr 4dup 1- >r >r swap r> r> myself then 4drop ;
: hanoi ( level -- ) 65 66 67 4 roll move cr ;
3 hanoi
