#include <stdio.h>

unsigned short x = 1; /* 0x0001 */

int main(void)
{
    printf("%s\n", *((unsigned char *)&x) == 0 ? "big-endian" : "little-endian");
    return 0;
}
