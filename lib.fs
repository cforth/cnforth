: 2dup over over ;
: 2drop drop drop ;

: test_ok   ." test OK!" cr ;
: test_fail ." test Fail!" cr  ;
: test = if test_ok else test_fail then ;

." Fib List Test: Print 8 Fib list numbers " cr
: fib_ do  .s 2dup + loop ;
: fib  0 1 4 roll 4 roll fib_ .s ;
: test_fib 6 0 fib + + + + + + + 33 test ;
test_fib .s

variable n
: n@ n @ ;
: n-- n@ 1 - n ! ;
: factorial n@ 0 > if n@ n-- myself * else 1 then .s ; 
see factorial
." Factorial Test: Us recursive to Print Eight's Factorial " cr
: test_fact 8 n ! factorial 40320 test ;
test_fact .s
