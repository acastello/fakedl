#include <wtypes.h>
#include <stdio.h>

void __attribute__((constructor)) wov_init(void)
{
    puts("wov_init");
}

BOOL WINAPI GetCursorPos(POINT *p)
{
    puts("getpos");
    p->x = 0;
    p->y = 0;
    return 1;
}
