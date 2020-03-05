#include <stdio.h>
#include "target.h"

typedef void (*printf_t)(const char *fmt, ...);
typedef int (*bdev_read_t)(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4, uint32_t a5);

int bdev_read(uint32_t a1, uint32_t buffer, uint32_t offset, uint32_t a4, uint32_t size) {
    
    printf_t printf = (printf_t)(BASE_ADDR+PRINTF);
    bdev_read_t real_bdev_read = (bdev_read_t)(BASE_ADDR+BDEV_READ);
    
    //Print arguments being passed to the hooked function
    printf("bdev_read buffer 0x%x offset 0x%x size 0x%x\n", buffer, offset, size);
    
    //Call the function with provided arguments and return the status
    int ret = real_bdev_read(a1, buffer, offset, a4, size);
    return ret;
}
