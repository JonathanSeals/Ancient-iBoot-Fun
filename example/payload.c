#include <stdio.h>
#include "target.h"

typedef void (*printf_t)(const char *fmt, ...);
typedef void (*clear_insn_cache_t)(void);

int main() {
    
    clear_insn_cache_t clear_insn_cache = (clear_insn_cache_t)(BASE_ADDR+CLEAR_INSN_CACHE);
    
    printf_t printf = (printf_t)(BASE_ADDR+PRINTF);
    
    clear_insn_cache();
    
    printf("Hacked\n");
    
    while(1);
    return 0;
}
