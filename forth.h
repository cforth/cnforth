#define    BUFF_LEN 1024

#define DEBUG 1
#if DEBUG
    #define PRINT(fmt, args...) printf(fmt,##args)
#else
    #define PRINT(fmt, args...)
#endif


