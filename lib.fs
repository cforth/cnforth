( Forth System Words Begin )
: cr 10 emit ;
: dup 1 pick ;
: swap 2 roll ;
: over 2 pick ;
: rot 3 roll ;
: -rot 3 roll 3 roll ;
: 2dup over over ;
: 2drop drop drop ;
: 2swap 4 roll 4 roll ;
: 2over 4 pick 4 pick ;
: 2rot 6 roll 6 roll ;
: nip swap drop ;
: tuck swap over ;
: 3dup 3 pick 3 pick 3 pick ;

: mod ( n1 n2 -- mod ) 2dup / * - ;
: /mod ( n1 n2 -- mod div ) 2dup / >r mod r> ;
: negate ( n -- -n ) -1 * ;
: abs dup 0 < if negate then ;
: 1+ 1 + ;
: 1- 1 - ;
: 2+ 2 + ;
: 2- 2 - ;
: 2* 2 * ;
: 2/ 2 / ;
: max 2dup < if swap then drop ;
: min 2dup > if swap then drop ;
: max2 ( n1 n2 n3 -- max1 max2 ) 2dup max >r min max r> ;

: 0= 0 = ;
: 0<> 0 <> ;
: 0> 0 > ;
: 0< 0 < ;
: true -1 ;
: false 0 ;
: ?dup dup 0<> if dup then ;

: space 32 emit ;
( Forth System Words End ) words cr
