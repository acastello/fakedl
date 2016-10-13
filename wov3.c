#include <wtypes.h>

BOOL WINAPI GetCursorPos(POINT *p)
{
    p->x = 0;
    p->y = 0;
    return 1;
}
