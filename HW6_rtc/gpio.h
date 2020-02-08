/*
 * GPIO.h
 *
 *  Created on: May 26, 2016
 *      Author: ianfeng
 */

#ifndef GPIO_H_
#define GPIO_H_

#define GPIO_PTA_GPIO		*((volatile unsigned int *)0x001F6800)
#define GPIO_PTA_PADIN		*((volatile unsigned int *)0x001F6804)
#define GPIO_PTA_DIR		*((volatile unsigned int *)0x001F6808)
#define GPIO_PTA_CFG		*((volatile unsigned int *)0x001F680C)
#define GPIO_PTA_BR			*((volatile unsigned int *)0x001F6810)
#define GPIO_PTA_BS			*((volatile unsigned int *)0x001F6814)
#define GPIO_PTA_PADINSEL	*((volatile unsigned int *)0x001F6818)
#define GPIO_PTA_FS			*((volatile unsigned int *)0x001F681C)

#define GPIO_PTB_GPIO		*((volatile unsigned int *)0x001F6820)
#define GPIO_PTB_PADIN		*((volatile unsigned int *)0x001F6824)
#define GPIO_PTB_DIR		*((volatile unsigned int *)0x001F6828)
#define GPIO_PTB_CFG		*((volatile unsigned int *)0x001F682C)
#define GPIO_PTB_BR			*((volatile unsigned int *)0x001F6830)
#define GPIO_PTB_BS			*((volatile unsigned int *)0x001F6834)
#define GPIO_PTB_PADINSEL	*((volatile unsigned int *)0x001F6838)
#define GPIO_PTB_FS			*((volatile unsigned int *)0x001F683C)

#define GPIO_PTC_GPIO		*((volatile unsigned int *)0x001F6840)
#define GPIO_PTC_PADIN		*((volatile unsigned int *)0x001F6844)
#define GPIO_PTC_DIR		*((volatile unsigned int *)0x001F6848)
#define GPIO_PTC_CFG		*((volatile unsigned int *)0x001F684C)
#define GPIO_PTC_BR			*((volatile unsigned int *)0x001F6850)
#define GPIO_PTC_BS			*((volatile unsigned int *)0x001F6854)
#define GPIO_PTC_PADINSEL	*((volatile unsigned int *)0x001F6858)
#define GPIO_PTC_FS			*((volatile unsigned int *)0x001F685C)

#define GPIO_PTD_GPIO		*((volatile unsigned int *)0x001F6860)
#define GPIO_PTD_PADIN		*((volatile unsigned int *)0x001F6864)
#define GPIO_PTD_DIR		*((volatile unsigned int *)0x001F6868)
#define GPIO_PTD_CFG		*((volatile unsigned int *)0x001F686C)
#define GPIO_PTD_BR			*((volatile unsigned int *)0x001F6870)
#define GPIO_PTD_BS			*((volatile unsigned int *)0x001F6874)
#define GPIO_PTD_PADINSEL	*((volatile unsigned int *)0x001F6878)
#define GPIO_PTD_FS			*((volatile unsigned int *)0x001F687C)

#define GPIO_PTE_GPIO		*((volatile unsigned int *)0x001F6880)
#define GPIO_PTE_PADIN		*((volatile unsigned int *)0x001F6884)
#define GPIO_PTE_DIR		*((volatile unsigned int *)0x001F6888)
#define GPIO_PTE_CFG		*((volatile unsigned int *)0x001F688C)
#define GPIO_PTE_BR			*((volatile unsigned int *)0x001F6890)
#define GPIO_PTE_BS			*((volatile unsigned int *)0x001F6894)
#define GPIO_PTE_PADINSEL	*((volatile unsigned int *)0x001F6898)
#define GPIO_PTE_FS			*((volatile unsigned int *)0x001F689C)

#define GPIO_PTF_GPIO		*((volatile unsigned int *)0x001F68A0)
#define GPIO_PTF_PADIN		*((volatile unsigned int *)0x001F68A4)
#define GPIO_PTF_DIR		*((volatile unsigned int *)0x001F68A8)
#define GPIO_PTF_CFG		*((volatile unsigned int *)0x001F68AC)
#define GPIO_PTF_BR			*((volatile unsigned int *)0x001F68B0)
#define GPIO_PTF_BS			*((volatile unsigned int *)0x001F68B4)
#define GPIO_PTF_PADINSEL	*((volatile unsigned int *)0x001F68B8)
#define GPIO_PTF_FS			*((volatile unsigned int *)0x001F68BC)

#define SELECT_7LED1_1	0x6000
#define SELECT_7LED1_2	0x4000
#define SELECT_7LED1_3	0x2000
#define SELECT_7LED1_4	0x0000
#define SELECT_7LED2_1	0xE000
#define SELECT_7LED2_2	0xC000
#define SELECT_7LED2_3	0xA000
#define SELECT_7LED2_4	0x8000

#define Digit_1	0x6000
#define Digit_2	0x4000
#define Digit_3	0x2000
#define Digit_4	0x0000
#define Digit_5	0xE000
#define Digit_6	0xC000
#define Digit_7	0xA000
#define Digit_8	0x8000

#define Number_0	0x3F3F
#define Number_1	0x0606
#define Number_2	0x5B5B
#define Number_3	0x4F4F
#define Number_4	0x6666
#define Number_5	0x6D6D
#define Number_6	0x7D7D
#define Number_7	0x2727
#define Number_8	0x7F7F
#define Number_9	0x6767
#define Number_A	0x7777
#define Number_b	0x7C7C
#define Number_C	0x3939
#define Number_d	0x5E5E
#define Number_E	0x7979
#define Number_F	0x7171
#define Number_Dot	0x8080



#endif /* GPIO_H_ */
