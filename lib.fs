: test_ok   ." test OK!" cr ;
: test_fail ." test Fail!" cr  ;
: test = if test_ok else test_fail then ;

." Fib List Test: Print 8 Fib list numbers " cr
: dup2 over over ;
: fib_ for dup2 + next ;
: fib  0 1 >r swap r> swap fib_ .s ;
: test_fib 6 fib + + + + + + + 33 test ;
test_fib .s

variable n
: n@ n @ ;
: n-- n@ 1 - n ! ;
: factorial n@ 0 > if n@ n-- myself * else 1 then .s ; 
see factorial
." Factorial Test: Us recursive to Print Eight's Factorial " cr
: test_fact 8 n ! factorial 40320 test ;
test_fact .s
