#ifndef DEBUG_PANIC
#define DEBUG_PANIC

#define PANIC(msg) panic(msg, __FILE__, __LINE__)

void panic(char* msg,char* file, uint32_t line);

#endif
