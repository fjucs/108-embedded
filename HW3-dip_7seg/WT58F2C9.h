
/* Check to see if this file has been included already.  */

#ifndef         WT58F2C9_REGS
#define         WT58F2C9_REGS



#define	INW(reg)		(*((volatile unsigned int *) (reg)))
#define	OUTW(reg, data) 	((*((volatile unsigned int *)(reg)))=(unsigned int)(data))

#ifndef NDS32_INTR_DEF
/* 
 * NDS32_REG_SET_BITS(addr, mask)
 * Do set bits to 1 at specified location
 * Operation: *addr = (*addr | mask)
 * */
#define NDS32_SET_BITS(addr, mask) \
__asm__ __volatile__(\
"	sethi 	$r1, hi20(%1)		\n\t"	\
"	ori   	$r1, $r1, lo12(%1)	\n\t"	\
"	sethi 	$r2, hi20(%0)		\n\t"	\
"	ori   	$r2, $r2, lo12(%0)	\n\t"	\
"	lwi   	$r0, [$r2]		\n\t"	\
"	or    	$r0, $r0, $r1		\n\t"	\
"	sw      $r0, [$r2]		\n\t"	\
	::"i"(addr),"i"(mask):"$r0", "$r1", "$r2")
/*
 * NDS32_REG_WAIT4_BIT_ON(addr, mask)
 * Read the contents at addr and use mask to 
 * mask off unused bits then wait until all of 
 * the remaining bits are on
 *
 * Operation: while(!(*addr & mask));
 * */
#define NDS32_REG_WAIT4_BIT_ON(addr, mask) \
__asm__ __volatile__(\
"	sethi  $r1, hi20(%1)		\n\t"	\
"	ori    $r1, $r1, lo12(%1)	\n\t"	\
"	sethi  $r2, hi20(%0)		\n\t"	\
"	ori    $r2, $r2, lo12(%0)	\n\t"	\
"1:	lwi    $r0, [$r2]		\n\t"	\
"	and    $r0, $r0, $r1		\n\t"	\
"	beqz   $r0, 1b			\n\t"	\
        ::"i"(addr),"i"(mask):"$r0", "$r1", "$r2")

/*
 * NDS32_REG_WAIT4_BIT_OFF(addr, mask)
 * Read the contents at addr and use mask to 
 * mask off unused bits then wait until all of 
 * the remaining bits are off
 *
 * Operation: while((*addr & mask));
 * */
#define NDS32_REG_WAIT4_BIT_OFF(addr, mask) \
__asm__ __volatile__(\
"	sethi  $r1, hi20(%1)		\n\t"	\
"	ori    $r1, $r1, lo12(%1)	\n\t"	\
"	sethi  $r2, hi20(%0)		\n\t"	\
"	ori    $r2, $r2, lo12(%0)	\n\t"	\
"1:	lwi    $r0, [$r2]		\n\t"	\
"	and    $r0, $r0, $r1		\n\t"	\
"	bnez   $r0, 1b			\n\t"	\
        ::"i"(addr),"i"(mask):"$r0", "$r1", "$r2")

/*
 * NDS32_REG_WRITE(addr, data)
 * Write data to specified location at addr
 *
 * Operation: *addr = data
 * */
#define NDS32_REG_WRITE(addr, data) \
__asm__ __volatile__(\
"	sethi 	$r1, hi20(%1)		\n\t"	\
"	ori   	$r1, $r1, lo12(%1)	\n\t"	\
"	sethi 	$r0, hi20(%0)		\n\t"	\
"	swi   	$r1, [$r0 + lo12(%0)]	\n\t"	\
	::"i"(addr),"i"(data):"$r0", "$r1")

/*
 * NDS32_FMEMCPY_BYTE(dst, src, size)
 * Do forward (low address to high address) memory copy in byte 
 * */
#define NDS32_FMEMCPY_BYTE(dst, src, size) \
__asm__ __volatile__(\
"	sethi	$r3, hi20(%2)		\n\t"	\
"	ori	$r3, $r3, lo12(%2)	\n\t"	\
"	sethi	$r1, hi20(%0)		\n\t"	\
"	ori	$r1, $r1, lo12(%0)	\n\t"	\
"	sethi	$r2, hi20(%1)		\n\t"	\
"	ori	$r2, $r2, lo12(%1)	\n\t"	\
"1:	lbi.bi	$r0, [$r2], 1		\n\t"	\
"	sbi.bi	$r0, [$r1], 1		\n\t"	\
"	addi	$r3, $r3, -1		\n\t"	\
"	bnez	$r3, 1b			\n\t"	\
	::"i"(dst),"i"(src),"i"(size):"$r0", "$r1","$r2","$r3")

#define NDS32_FMEMCPY(dst, src, size) \
__asm__ __volatile__(\
"	sethi	$r3, hi20(%2)		\n\t"	\
"	ori	$r3, $r3, lo12(%2)	\n\t"	\
"	sethi	$r1, hi20(%0)		\n\t"	\
"	ori	$r1, $r1, lo12(%0)	\n\t"	\
"	sethi	$r2, hi20(%1)		\n\t"	\
"	ori	$r2, $r2, lo12(%1)	\n\t"	\
"1:	lwi.bi	$r0, [$r2], 4 		\n\t"	\
"	swi.bi	$r0, [$r1], 4		\n\t"	\
"	addi	$r3, $r3, -4		\n\t"	\
"	bgtz	$r3, 1b			\n\t"	\
	::"i"(dst),"i"(src),"i"(size):"$r0", "$r1","$r2","$r3")

#endif

typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;
// ============================================== << XTAL >>
#define	XTAL_1M		0 							// 24.000 MHz
#define	XTAL_6M		1 							// 24.000 MHz
#define	XTAL_12M	2 							// 24.000 MHz
#define	XTAL_24M	3 							// 24.000 MHz
#define	XTAL_36M	4 							// 27.000 MHz
#define	XTAL	 XTAL_24M//XTAL_36M

#if(XTAL == XTAL_1M)
	#define	XTAL_FREQ	(double)1000000.0
#elif(XTAL == XTAL_6M)
	#define	XTAL_FREQ	(double)6000000.0
#elif(XTAL == XTAL_12M)
	#define	XTAL_FREQ	(double)12000000.0
#elif(XTAL == XTAL_24M)
	#define	XTAL_FREQ	(double)24000000.0
#elif(XTAL == XTAL_36M)
	#define	XTAL_FREQ	(double)36000000.0
#endif

#define INTERNAL_RC		0
#define EXTERNAL_XTAL	1
#define XTAL_MODE		EXTERNAL_XTAL

#endif  /* WT58F2C9 */
