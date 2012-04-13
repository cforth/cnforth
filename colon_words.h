/********************** cforth... ************************/


void drop2(){ drop(); drop(); } 


void over(){ tor(); dup(); rto(); swap(); } 


void dup2(){ over(); over(); } 

void showtopDS(){ showTop(); drop(); } 


void div() { dup2(); mod(); tor(); ddiv();rto(); ifFlag0(); }

