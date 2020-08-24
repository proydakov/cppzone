#ifndef _MSC_VER
#include <x86intrin.h>
#else
#include <intrin.h>
#endif

int main()
{
    _mm_pause();
    return 0;
}
