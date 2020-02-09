#include <nds32_intrinsic.h>
#include <string.h>
#include "WT58F2C9.h"
#include "gpio.h"


void delay1(unsigned int nCount)
{
   volatile unsigned int i;
   for (i=0;i<nCount;i++);
}


/* Intrrupt memory address */
#define INT_MEM_ADDR_BASE			(0x00200D00)
#define rINT0_IE0_ENABLE			(INT_MEM_ADDR_BASE+0x00)
#define rINT0_IE1_ENABLE			(INT_MEM_ADDR_BASE+0x04)
#define rINT0_IE2_ENABLE			(INT_MEM_ADDR_BASE+0x08)
#define rINT1_IE0_ENABLE			(INT_MEM_ADDR_BASE+0x10)
#define rINT1_IE1_ENABLE			(INT_MEM_ADDR_BASE+0x14)
#define rINT1_IE2_ENABLE			(INT_MEM_ADDR_BASE+0x18)

#define rINT0_IE0_FLAG				(INT_MEM_ADDR_BASE+0x20)
#define rINT0_IE1_FLAG				(INT_MEM_ADDR_BASE+0x24)
#define rINT0_IE2_FLAG				(INT_MEM_ADDR_BASE+0x28)
#define rINT1_IE0_FLAG				(INT_MEM_ADDR_BASE+0x30)
#define rINT1_IE1_FLAG				(INT_MEM_ADDR_BASE+0x34)
#define rINT1_IE2_FLAG				(INT_MEM_ADDR_BASE+0x38)


inline void GIE_ENABLE();

void DRV_EnableHWInt (void)
{
	/* enable SW0, HW0 and HW1 */
	__nds32__mtsr(0x10003, NDS32_SR_INT_MASK);
	/* Enable SW0 */
	//__nds32__mtsr(0x10000, NDS32_SR_INT_MASK);
	/* Enable global interrupt */
	GIE_ENABLE();
}

void DRV_BlockIntDisable(void)
{
	// Disable all interrupt
	OUTW(rINT0_IE0_ENABLE, 0x0000);
	OUTW(rINT0_IE1_ENABLE, 0x0000);
	OUTW(rINT0_IE2_ENABLE, 0x0000);
	OUTW(rINT1_IE0_ENABLE, 0x0000);
	OUTW(rINT1_IE1_ENABLE, 0x0000);
	OUTW(rINT1_IE2_ENABLE, 0x0000);
}

void DRV_IntInitial(void)
{
	// Disable all interrupt
	DRV_BlockIntDisable();

	// Enable all HW interrupt
	DRV_EnableHWInt();				//Enable global Hardware interrupt, Assembly command

	// Enable default Block interrupt
	//DRV_BlockIntEnable();			//Enable each block device interrupt

}

#define rSYS_OPTION1	(0x00200004)

void DRV_SysXtal(U8 u8XtalMode)
{

	#if(EXTERNAL_XTAL == XTAL_MODE)
		//-----External Crystal
		//-----24MHz
		OUTW(rSYS_OPTION1,((INW(rSYS_OPTION1)&0xFFFFFF00) | 0x0012)); //Use HXTAL and divide 2
		//-----Crystal 12MHz
		//OUTW(rSYS_OPTION1, (INW(rSYS_OPTION1) | 0x000A));

		//OUTW(rSYS_OPTION1,(INW(rSYS_OPTION1) | (XTAL<<2) | (HSE_OSC_ON <<1)));
		//OUTW(rSYS_CLOCK_SELECT,(INW(rSYS_CLOCK_SELECT) | 0x0001));
	#else
		//-----Internal RC
		//-----24MHz
		//OUTW(rSYS_OPTION1,(INW(rSYS_OPTION1) | 0x0012));
		//-----Crystal 12MHz
		//OUTW(rSYS_OPTION1, (INW(rSYS_OPTION1) | 0x000A));

		//OUTW(rSYS_OPTION1,(INW(rSYS_OPTION1) | (XTAL<<2)));
		//OUTW(rSYS_CLOCK_SELECT,(INW(rSYS_CLOCK_SELECT));
	#endif
	//-----MCU Clock Output Test
	//OUTW(rSYS_OPTION3,0x0090);
}

void OS_PowerOnDriverInitial(void)
{
	//========================= << Typedef Initial  >>
	//SYS_TypeDefInitial();
	//========================= << System Clock Initial >>
	//-----External Crystal
	DRV_SysXtal(EXTERNAL_XTAL);
	//========================= << Interrupt Initial >>
	DRV_IntInitial();
	//========================= << GPIO Initial >>
	//DRV_GpioInitial();
	//========================= << UART Initial >>
	//DRV_UartInitial();
	//========================= << Timer Initial >>
	//DRV_TimerInitial(TIMER_0, SIMPLE_TIMER);
	//DRV_TimerInitial(TIMER_1, SIMPLE_TIMER);
	//========================= << PWM Initial >>
	//DRV_PwmInitial();
	//========================= << SPI Initial >>
	//DRV_SpiInitial(SPI_CH2);
	//========================= << Watchdog Initial >>
}




//-----  UART Functions  -----
#define UART3_ADDR_BASE		(0x0020B400)
#define UART3_CR1			(UART3_ADDR_BASE+0x00)
#define UART3_CR2			(UART3_ADDR_BASE+0x04)

//0:1 Start bit/8 Data bits/1 Stop bit,
#define	UART_FORMAT_N81	0
//1:1 Start bit/9 Data bits/1 Stop bit
#define	UART_FORMAT_N91	1

#define UART_EN				(1<<17)	//0:Disable, 1:Enable
#define UART_TX_EN			(1<<15)
#define UART_RX_EN			(1<<14)
#define UART_OVER8			(1<<13)
#define UART_OVER16			(0<<13)
#define UART_WORD_LENGTH	(UART_FORMAT_N81<<12)//	//0:1 Start bit/8 Data bits/1 Stop bit, //1:1 Start bit/9 Data bits/1 Stop bit
#define UART_TX_DMA_EN		(1<<11) //0:Disable, 1:Enable
#define UART_RX_DMA_EN		(1<<10)	// 0:Disable, 1:Enable
#define UART_RX_WAKEUP		(0<<9)	// 0:In active mode, 1:In mute mode
#define UART_WAKEUP_METHOD	(0<<8)	// 0:Idle mode, 1:Address mark
#define UART_UART_ADDR_NODE	(0x5<<4)	//Data 0x0=xxxx0000b ~ 0xF=xxxx1111b,
#define UART_PARITY_EN		(0<<2)//SET_BIT1//(n<<1) //0:Disable, 1:Enable
#define UART_PARITY_SEL		(1<<1)// O:Even 1:Odd
#define UART_STOP_BIT	0// O:1-bit 1:2-bit

#define UART_SET_CTL_PARA	UART_EN|UART_TX_EN|UART_RX_EN|UART_WORD_LENGTH\
							|UART_RX_WAKEUP|UART_WAKEUP_METHOD|UART_UART_ADDR_NODE\
							|UART_PARITY_EN|UART_PARITY_SEL|UART_STOP_BIT

//HXTAL = HXTAL_24M (OVER8=0)
//9600 = 156.25
#define BUARRATE_9600_MANTISSA_24MHZ	156
#define BUARRATE_9600_FRACTION_24MHZ	4
//19200 = 78.125
#define BUARRATE_19200_MANTISSA_24MHZ	78
#define BUARRATE_19200_FRACTION_24MHZ	2
//38400 = 39.0625
#define BUARRATE_38400_MANTISSA_24MHZ	39
#define BUARRATE_38400_FRACTION_24MHZ	1
//57600 = 26.0625
#define BUARRATE_57600_MANTISSA_24MHZ	26
#define BUARRATE_57600_FRACTION_24MHZ	1
//115200 = 13.0
#define BUARRATE_115200_MANTISSA_24MHZ	13
#define BUARRATE_115200_FRACTION_24MHZ	0
//230400 = 6.5
#define BUARRATE_230400_MANTISSA_24MHZ	6
#define BUARRATE_230400_FRACTION_24MHZ	8
//921600 = 1.625
#define BUARRATE_921600_MANTISSA_24MHZ	1
#define BUARRATE_921600_FRACTION_24MHZ	10


#define FALSE 0
#define TRUE  1
#define UART_TXD_BUFFER_SIZE	24
#define _EOS_	'\0' //End of string

char u8TxdBuf[UART_TXD_BUFFER_SIZE];

void DRV_PutChar(char u8Char)
{
	U16 u16Count;

	OUTW(UART3_ADDR_BASE+0x0C, u8Char);

	u16Count = 0;
	//Wait transmission complete then clear by SW write to 0
	while((INW(UART3_ADDR_BASE+0x08)&0x00000020) == 0)
	{
	#if 1 //Don't delete.
		//-----Time out
		u16Count++;
		if(u16Count >= 1000)
		{
			break;
		}
	#endif
	}
	OUTW(UART3_ADDR_BASE+0x08, INW(UART3_ADDR_BASE+0x08) & 0xFFFFFFDF);
}

void DRV_PutStr(const char *pFmt)
{
	U8 u8Buffer;	//Character buffer

	while (1)
	{
		u8Buffer = *pFmt; //Get a character
		if(u8Buffer == _EOS_) //Check end of string
			break;

		DRV_PutChar(u8Buffer); //Put a character
		pFmt++;
	}
}

void DRV_IntToStr(U16 u16Val, U8 u8Base, char *pBuf, U8 u8Length)
{
	U8 bShowZero = FALSE;
	U16 u16Divider;
	U8 u8Disp;
	U16 u16Temp;

	u8Length -= 1;
	if(u8Base == 16) //Hex
	{
		u16Temp = 0x01 << u8Length;
	}
	else //Dec
	{
		u16Temp = 1;
		while(u8Length--)
		{
			u16Temp *= 10;
		}
	}

	if( 0 == u16Val )
	{
		if( 16 == u8Base )
		{
			pBuf[0] = '0';
			pBuf[1] = '0';
			pBuf[2] = '0';
			pBuf[3] = '0';
			pBuf[4] = '\0';
		}
		else
		{
			pBuf[0] = '0';
			pBuf[1] = '0';
			pBuf[2] = '0';
			pBuf[3] = '0';
			pBuf[4] = '0';
			pBuf[5] = '\0';
		}
		return;
	}

	if( 16 == u8Base )
	{
		u16Divider = 0x1000;
	}
	else
	{
		u16Divider = 10000;
	}

	while( u16Divider )
	{
		u8Disp = u16Val / u16Divider;
		u16Val = u16Val % u16Divider;

		if(u16Temp == u16Divider)
		{
			bShowZero = TRUE;
		}
		if( u8Disp || bShowZero || (u16Divider>0))
		{
			if( u8Disp < 10 )
				*pBuf = '0' + u8Disp;
			else
				*pBuf = 'A' + u8Disp - 10;
			pBuf ++;
		}

		if( 16 == u8Base )
		{
			u16Divider /= 0x10;
			if(bShowZero)
				u16Temp /= 0x10;
		}
		else
		{
			u16Divider /= 10;
			if(bShowZero)
				u16Temp /= 10;
		}
	}
	*pBuf = '\0';
}

void DRV_Printf(char *pFmt, U16 u16Val)
{
	U8 u8Buffer;

	//-----Pin configuration for UART3
	GPIO_PTC_FS = 0x0300;
	GPIO_PTC_PADINSEL = 0x0000;
	GPIO_PTC_DIR = 0xFEFF;
	GPIO_PTC_CFG = 0x0000;

	//UART Parameter
	OUTW(UART3_ADDR_BASE+0x00, UART_SET_CTL_PARA);

	//Set Baud rate with default sysclk
	OUTW(UART3_ADDR_BASE+0x14, ((BUARRATE_38400_MANTISSA_24MHZ<<4)|BUARRATE_38400_FRACTION_24MHZ)/2); //38400 for 12MHz MCUCLK

	while((u8Buffer =(U8)*(pFmt++)))
	{
		if(u8Buffer == '%') //check special case
		{
			switch(*(pFmt++)) //check next character
			{
				case 'x': //hexadecimal number
				case 'X':
					DRV_IntToStr(u16Val, 16, u8TxdBuf, 2);
					DRV_PutStr(u8TxdBuf);
				break;
				case 'd': //decimal number
				case 'i':
					DRV_IntToStr(u16Val, 10, u8TxdBuf,5);
					DRV_PutStr(u8TxdBuf);
				break;
				case 'c':
				case 'C':
					DRV_PutChar((char)u16Val);
				break;
			} //switch
		}
		else //general
		{
			DRV_PutChar(u8Buffer); //put a character
		}
	}
}

#define BIT(x) (1<<(x))

void led(int state) {
    GPIO_PTB_GPIO = ~state;
}

typedef unsigned char uint8;

uint8 dip_read()
{
	return (GPIO_PTC_PADIN >> 2) & (BIT(8)-1);
}

uint8 dip_check(int idx)
{
	return (dip_read() & BIT(idx)) >> idx;
}

int rrot(int tmp)
{
    return tmp >> 1 | tmp << 16-1;
}


unsigned int index_7LED[8] = {
        Digit_1,
        Digit_2,
        Digit_3,
        Digit_4,
        Digit_5,
        Digit_6,
        Digit_7,
        Digit_8
};

unsigned int num[18] = {Number_0, Number_1, Number_2, Number_3, Number_4, Number_5, Number_6, Number_7,
			Number_8, Number_9, Number_A, Number_b, Number_C, Number_d, Number_E, Number_F, Number_Dot, 0x0};


int ans_tmp = 0;
int ans = 0;
int state = 0;


void one2three()
{
	int i = 0;
	ans = 0;
	for(i = 0; i < 8; i++)
	{
		if(dip_check(i))
			ans |= BIT(15-i);
		else
			ans |= BIT(7-i);
	}

	if(ans_tmp != ans && ans_tmp != 0)
	{
		int ans_2 = ans;
		for(i = 0; i < 16; i++)
		{
			ans_2 = rrot(ans_2);
			delay1(100000);
			led(ans_2);
		}
	}
	else
	{
		led(ans);
	}
	ans_tmp = ans;
}

int v1 = 0, v0 = 0;
int v12= 0, v02 = 0;

void four()
{
	v1 = 0, v0 = 0;
	int i = 0;

	//init
	for(i = 0; i < 8; i++)
	{
		GPIO_PTA_GPIO = index_7LED[i];
		GPIO_PTD_GPIO = num[17];
	}

	//v1 input
	for(i = 0; i <= 3; i++)
	{
		if(dip_check(i))
		{
			v0 |= BIT(i);
		}
	}

	//v2 input
	for(i = 4; i < 8; i++)
	{
		if(dip_check(i))
		{
			v1 |= BIT(i-4);
		}
	}

	//v1 7,8 is 1
	v1 |= BIT(2);
	v1 |= BIT(3);

	//store state
	v12 = v1;
	v02 = v0;

	int cnt2 = 0;
	while(v0 > 0)
	{
		if(cnt2 == 2)
			break;
		GPIO_PTA_GPIO = index_7LED[cnt2++];
		GPIO_PTD_GPIO = num[v0%10];
		v0 /= 10;
		delay1(100);
	}


	int cnt1 = 2;
	while(v1 > 0)
	{
		GPIO_PTA_GPIO = index_7LED[cnt1++];
		GPIO_PTD_GPIO = num[v1%10];
		v1 /= 10;
		delay1(100);
	}


}

int frame[8];
int buffer[8];
int idxcnt = 0;
int pre = 0;
int flag = 0;

void five()
{
	int i = 0, j = 0, k = 0, idxcnt = 0, ans = v12 * v02 * abs(v12 - v02);
	memset(frame,-1,sizeof(frame));
	memset(buffer,0,sizeof(buffer));

	//check_the_swich

	if( pre != ans ){
		state = 0;
		flag = 0;
	}
	pre = ans;
	

	int cnt = 0;

	if(ans == 0)
	{
		frame[cnt] = 0;
	}
	while(ans > 0)
	{
		frame[cnt] = ans % 10;
		ans /= 10;
		cnt++;
	}


	if(state == 0)
	{
		for(i = 7; i >= 7; i--)
		{
			if(frame[idxcnt] >= 0)
				buffer[i] = num[frame[idxcnt]];
			else if(frame[idxcnt] == -1)
				buffer[i] = -1;
		}
		state = 1;
		
	}
	else if (state == 1)
	{
		idxcnt = 1;
		for(i = 7; i >= 6; i--)
		{
			if(frame[idxcnt] >= 0)
				buffer[i] = num[frame[idxcnt]];
			else if(frame[idxcnt] == -1)
				buffer[i] = -1;
			--idxcnt;
		}
		state = 2;
		
	}
	else if (state == 2)
	{
		idxcnt = 2;
		for(i = 7; i >= 5; i--)
		{
			if(frame[idxcnt] >= 0)
				buffer[i] = num[frame[idxcnt]];
			else if(frame[idxcnt] == -1)
				buffer[i] = -1;
			idxcnt--;
		}
		state = 3;
	}
	else if (state == 3)
	{
		idxcnt = 3;
		for(i = 7; i >= 4; i--)
		{
			if(frame[idxcnt] >= 0)
				buffer[i] = num[frame[idxcnt]];
			else if(frame[idxcnt] == -1)
				buffer[i] = -1;
			idxcnt--;
		}
		state = 3;
	}


	/********************** render_data *********************/

	/* ___ ___ ___ ___ ** ___ ___ ___ ___ */

	for(i = 0; i < 100; i++)
	{
		for(j = 7; j >= 4; j--)
		{
			four();
			//v0是0的時候要顯示
			if(v02 == 0)
			{
				GPIO_PTA_GPIO = index_7LED[0];
				GPIO_PTD_GPIO = num[0];
			}

			if(buffer[j] == -1)
			{
				GPIO_PTA_GPIO = index_7LED[j];
				GPIO_PTD_GPIO = num[17];
			}
			else
			{
				GPIO_PTA_GPIO = index_7LED[j];
				GPIO_PTD_GPIO = buffer[j];
			}
			delay1(500);
		}
	}
}


void init()
{
	OS_PowerOnDriverInitial();
	//7LED select
	GPIO_PTA_DIR = 0x0000;
	GPIO_PTA_CFG = 0x0000;
	GPIO_PTA_GPIO = Digit_1;

	//7LED number
	GPIO_PTD_DIR = 0x0000;
	GPIO_PTD_CFG = 0x0000;
	GPIO_PTD_GPIO = num[17];

	//dip switch
	GPIO_PTC_FS = 0x0000;
	GPIO_PTC_PADINSEL = 0x0000;
	GPIO_PTC_DIR = 0xFFFF;
	GPIO_PTC_CFG = 0x0000;

	//LED
	GPIO_PTB_DIR = 0x0000;
	GPIO_PTB_CFG = 0xFFFF;
	GPIO_PTB_PADIN = 0x00;
	GPIO_PTB_GPIO = 0xFFFF;
}


int main()
{

	init();
	while(1)
	{
		one2three();
		four();
		five();
	}
	return 0;
}
