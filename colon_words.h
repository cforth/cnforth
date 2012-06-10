/***************************** colon_words.h ****************************
 * Used Code_words compose Colon words  				*
 ************************************************************************/

/* 2drop */
void drop2(){ drop(); drop(); } 

/* over */
void over(){ tor(); dup(); rto(); swap(); } 

/* 2dup */
void dup2(){ over(); over(); } 

/* show top of DS */
void showtopDS(){ showTop(); drop(); } 

/* 带余数的除法 */
void divv() { dup2(); mod(); tor(); ddiv();rto(); ifFlag0(); }