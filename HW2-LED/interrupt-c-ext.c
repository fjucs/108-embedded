#include <nds32_intrinsic.h>
#include <nds32_isr.h>
#include "WT58F2C9.h"

void nmi_handler(int *reg_ptr)
{
}

inline void GIE_ENABLE()
{
	__nds32__setgie_en();
}

inline void GIE_DISABLE()
{
	__nds32__setgie_dis();
	__nds32__dsb();
}

 /* this function generates a s/w interrupt */
void gen_swi()
{
	unsigned int int_pend;

	int_pend = __nds32__mfsr(NDS32_SR_INT_PEND);
	int_pend |= 0x10000;
	__nds32__mtsr(int_pend, NDS32_SR_INT_PEND);
	__nds32__dsb();
}

void initIntr_standby()
{
	/* enable HW1 */
	__nds32__mtsr(0x0002, NDS32_SR_INT_MASK);
	//intc_reset();
	/* Init GPIO */

}

void clear_swi()
{
	unsigned int int_pend;

	int_pend = 0x10;
	__nds32__mtsr(int_pend, NDS32_SR_INT_PEND);
	__nds32__dsb();
}

void __cpu_init();
void __c_init();
void __soc_init();
int main();

/* 
 * Interrupt vectors
 * vectors: 7
 * HW0 ~ HW5 and SW0 
 * */
void __attribute__ ((reset("vectors=7;nmi_func=nmi_handler;warm_func=nmi_handler"))) reset_handler(void)
{
	__cpu_init();
	__c_init();
	__soc_init();
	main();
}

/*
 * system call handler
 * ready_nested for user to enable GIE when they need.
 * */
void __attribute__ ((exception("save_all_regs;ready_nested;id=8"))) syscall_handler(int vid, NDS32_CONTEXT * ptr)
{
	//static int cnt = 0;

	//if (++cnt < 5)
	//	asm("syscall 0x5000\n\t");

	//puts("Syscall handler\n");
	//ptr->ipc = ptr->ipc + 4;
}
/*
 * HW0 interrupt handler
 * not_nested for top priority interrupt 
 * */
void __attribute__ ((interrupt("not_nested;id=0"))) HW0_ISR(void)
{

}

/*
 * HW1 interrupt handler
 * ready_nested for user to enable GIE when they need.
 * */
void __attribute__ ((interrupt("not_nested;id=1"))) HW1_ISR(int vid, NDS32_CONTEXT * ptr)
{

}
/*
 * SW0 interrupt handler
 * not_nested for software interrupt
 * */
void __attribute__ ((interrupt("not_nested;id=6"))) SW0_ISR(int vid)
{
	//puts("A software interrupt occurs ...\n");
	clear_swi();
	return;
}
