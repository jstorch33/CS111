#include "schedos-app.h"
#include "x86sync.h"

/*****************************************************************************
 * schedos-1
 *
 *   This tiny application prints red "1"s to the console.
 *   It yields the CPU to the kernel after each "1" using the sys_yield()
 *   system call.  This lets the kernel (schedos-kern.c) pick another
 *   application to run, if it wants.
 *
 *   The other schedos-* processes simply #include this file after defining
 *   PRINTCHAR appropriately.
 *
 *****************************************************************************/

#ifndef PRINTCHAR
#define PRINTCHAR	('1' | 0x0C00)
#endif

#ifndef PRIORITY
#define PRIORITY 1
#endif

#ifndef SHARE
#define SHARE 4
#endif

// UNCOMMENT THE NEXT LINE TO USE EXERCISE 8 CODE INSTEAD OF EXERCISE 6
// #define __EXERCISE_8__
// Use the following structure to choose between them:
// #infdef __EXERCISE_8__
// (exercise 6 code)
// #else
// (exercise 8 code)
// #endif


void
start(void)
{
  int i;
  for (i = 0; i < RUNCOUNT; i++) {
    // Write characters to the console, yielding after each one.
    sys_print_character(PRINTCHAR);
    // while(atomic_swap(&spin_lock,1) != 0)
    //;
    //*cursor++ = PRINTCHAR;
    //atomic_swap(&spin_lock, 0);
	  
    //*cursorpos++ = PRINTCHAR;
    sys_yield();
  }
  
  // Yield forever.
  sys_exit(0);
}
