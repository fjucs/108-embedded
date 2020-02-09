/*
   This file includes weak (i.e. optional) functions to perform SoC related
   initialization. They are:
	1). _nds32_init_mem():
		Executed before C language initialization to make memory
		ready so that program data can be initialized. An example
		is to initialize DRAM.
		Since this is called before C initialization, please
		use provided macros to avoid problems.
	2). __soc_init():
		Further SoC intialization. Called after C language
		initialization, so it is a typical C function.
*/

#include <nds32_intrinsic.h>
#include "WT58F2C9.h"

void _nds32_init_mem(void) __attribute__((no_prologue));
void _nds32_init_mem(void) 	//The function is weak (optional)
{
	//enable DLM
	__nds32__mtsr_dsb(0x00100003, NDS32_SR_DLMB);
}


void __soc_init()		//The function is weak (optional)
{
}
