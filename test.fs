: test_ok ( -- )
    ." test OK!" cr ;

: test_fail ( -- )
    ." test Fail!" cr  ;

: test ( n1 n2 -- n1==n2? )
    = 
    if 
        test_ok 
    else 
        test_fail 
    then ;

variable foo
3 foo ! foo @ 3 test cr
555 constant foo2 foo2 555 test cr
    
." Fib Test: Print the twelfth fib numbers " cr
: fib ( fib_index -- fib_number )
    dup 3 <
    if
        drop 1
    else
        1- dup 1- myself swap myself +
    then ;

: test_fib 
    11 fib dup . 89 test ;

see fib
test_fib cr

." Factorial Test: Us recursive to Print Eight's Factorial " cr

: factorial ( Num -- FactorialResult )
    dup 1 =
    if
        drop 1
    else
        dup 1- myself *
    then ;

: test_fact 
    8 factorial dup . 40320 test ;

see factorial
test_fact cr

." Matrix: Test nested statements " cr
: star 42 emit ;

: matrix 
    swap 0 
    do 
        dup >r 0 
        do 
            star space 
        loop 
        cr r> 
    loop drop cr ;
    
5 4 matrix cr

." Hanoi Tower: level 3" cr
: 4dup 4 pick 4 pick 4 pick 4 pick ;

: 4drop drop drop drop drop ;

: arrow 45 emit 62 emit ;

: move ( A B C n -- )
    dup 1 = 
    if
        4 pick emit arrow 2 pick emit cr 
    else 
        4dup 1- >r swap r> myself 4 pick emit arrow 2 pick emit cr 4dup 1- >r >r swap r> r> myself 
    then 
        4drop ;
        
: hanoi ( level -- ) 65 66 67 4 roll move cr ;

3 hanoi cr

." : test[ if 1 [ 4 5 + ] else 2 then ;" cr
: test[ if 1 [ 4 5 + ] else 2 then ;
see test[
9 test cr

: aaa 1 2 ;
: bbb 3 4 ;
see aaa
see bbb
: ccc compile aaa compile bbb ; immediate
see ccc
: testccc ccc ;
see testccc
testccc + + + 10 test cr

see literal
." : new-five+ [ 5 ] literal + ; " cr
: new-five+ [ 5 ] literal + ;
see new-five+
2 new-five+ 7 test cr