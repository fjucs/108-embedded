#include <nds32_intrinsic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "WT58F2C9.h"
#include "gpio.h"
#include "lcm.h"
#include "rtc.h"


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

void CheckBusy(void) //BF ,CHACK BUSY FLAG
{
	unsigned short int i=0x80;
	while(i&0x80)      // the 7'th bit is the position of busy flag
	{
		GPIO_PTE_DIR = 0x0000; //Initialize GPIO_E output
		GPIO_PTE_CFG = 0x0;

		GPIO_PTE_GPIO = (RW + E);  // LCD_ReadBusyAC RS=0 (Instruction Register) RW=1 (Read)
		delay1(500);

		GPIO_PTE_DIR = 0xFF;  //Initialize GPIO_E input
		GPIO_PTE_CFG = 0x0;

		delay1(100);
		i = GPIO_PTE_PADIN;       //
		delay1(100);

		GPIO_PTE_GPIO = CleanSet;
		delay1(1000);
	}
}


void WriteData(unsigned char i)
{
	GPIO_PTE_DIR = 0x0000; //Initialize GPIO_E output
	GPIO_PTE_CFG = 0x0;

	GPIO_PTE_GPIO = (i + RS + E); // RS: 1 RW:0 Write data (i) to RAM
	delay1(100);
	GPIO_PTE_GPIO = CleanSet;
	delay1(100);
	CheckBusy();
}

void WriteIns(unsigned char instruction)
{
	GPIO_PTE_DIR = 0x0000; //Initialize GPIO_E output
	GPIO_PTE_CFG = 0x0;

	GPIO_PTE_GPIO = (instruction + E ); // RS=RW=0  Instruction Write
	delay1(100);
	GPIO_PTE_GPIO = CleanSet;
	delay1(100);

	CheckBusy();
}

void SetCursor(unsigned short Line, unsigned short Position)
{
	unsigned short Address = Line * 0x0040 + Position * 0x0001;  // 2 lines mode
	WriteIns(LCD_SetDDRAM | Address);
}

void InitialLCD(void)
{
	WriteIns(0x38);  //FUNCTION SET  DL: 1 (8 bits) N: 1 (2 lines) F: 0 (5*8 font)
	WriteIns(LCD_Function | LCD_InstructionMode | LCD_NumLine); //0x38 FUNCTION SET  DL: 1 (8 bits) N: 1 (2 lines) F: 0 (5*8 font)
	delay1(300000);
	WriteIns(LCD_ClearDisplay );  // clear display (buffer): 0x01
	delay1(300000);
	WriteIns(LCD_DisplayCursor | LCD_Display | LCD_Cursor);  // display on D=C=1 entire display + cursor on  B=0 cursor position off
	delay1(300000);
	WriteIns(LCD_EntryMode | LCD_ACShiftDirection);  // set input mode   I/D=1 increment  S=0  shift off
	delay1(300000);
}

void InitialRTC(void) {
	RTC_CONTROL = (RTC_CONTROL | 0x0080); // Enable RTC
	//System_Control_20 = (System_Control_20 | 0x0005); // Switch clock source from external crystal.
	//RTC_PARAMETER = 0x07; //Clock out=32.768KHz
}


void SetupRTCTime(void)
{
	unsigned int tyear[2] = {1, 9}, tmon = 12, tday[2] = { 3, 1}, tweek = 2;
	unsigned int thour[2] = {1, 2}, tmin[2] = { 5, 9 }, tsec[2] = {5, 5};
	// unsigned int thour[2] = {2, 3}, tmin[2] = { 5, 9 }, tsec[2] = {5, 5};

	// Initialize RTC (Real Time Clock)
	RTC_YEAR = (tyear[0] << 4) | tyear[1];  	// fill in op
	RTC_MONTH =  tmon;
	RTC_DAY = (tday[0] << 4) | tday[1]; 		// fill in op
	RTC_WEEK = tweek;

	RTC_HOUR = (thour[0] << 4) | thour[1];
	RTC_MIN = (tmin[0] << 4) | tmin[1];
	RTC_SEC = (tsec[0] << 4) | tsec[1];

}

const char *halfDay[3] = {"AM", "PM"};
#define AM 0
#define PM 1

#define BCD_TO_DEC(x) (((x >> 4) & 0x000F) * 10 + (x & 0x000F))
#define DEC_TO_BCD(x) (x / 10) << 4 | (x % 10)

void DisplayTime(void) {

	char week_array[7][4] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	int i;

	unsigned int year = RTC_YEAR, mon = RTC_MONTH, day = RTC_DAY;
	unsigned int hour = RTC_HOUR, min = RTC_MIN, sec = RTC_SEC;
	unsigned int week = RTC_WEEK;

	SetCursor(0, 0x6);
	WriteData('2'); // 
	WriteData('0'); // 
	WriteData(((year >> 4) & 0x000F) + To_ASCII);
	WriteData((year & 0x000F) + To_ASCII);
	WriteData(0x3A); // :
	WriteData(((mon & 0x000F) / 10) + To_ASCII);
	WriteData(((mon & 0x000F) % 10) + To_ASCII);
	WriteData(0x3A); // :
	WriteData(((day >> 4) & 0x000F) + To_ASCII);
	WriteData((day & 0x000F) + To_ASCII);

	SetCursor(1, 0x0);
	for(i=0;i<3;i++) // weekday
		WriteData(week_array[(int)week][i]);
	WriteData('.');

	int dec_hour = BCD_TO_DEC(hour);
	if(dec_hour % 12 == 0)
		hour = 12;
	else
		hour = dec_hour % 12;
	hour = DEC_TO_BCD(hour);
	WriteData(0x20); // space
	WriteData(((hour >> 4) & 0x000F) + To_ASCII);
	WriteData((hour & 0x000F) + To_ASCII);
	WriteData(0x3A); //":"²Å¸¹
	WriteData(((min >> 4) & 0x000F) + To_ASCII);
	WriteData((min & 0x000F) + To_ASCII);
	WriteData(0x3A); //":"²Å¸¹
	WriteData(((sec >> 4) & 0x000F) + To_ASCII);
	WriteData((sec & 0x000F) + To_ASCII);

	WriteData(' ');

	// AM or PM
	if(dec_hour >= 12) // PM
		for(i = 0; i < 2; i++)
			WriteData(halfDay[PM][i]);
	else // AM
		for(i = 0; i < 2; i++)
			WriteData(halfDay[AM][i]);

}

int main()
{

	OS_PowerOnDriverInitial();

	DRV_Printf("====================================\r\n", 0);
	DRV_Printf("   ADP-WT58F2C9 RTC demo program   \r\n", 0);
	DRV_Printf("====================================\r\n", 0);

	DRV_Printf("RTC testing ...\r\n", 0);

	InitialLCD();
	//WriteIns(LCD_ClearDisplay);

	InitialRTC();
	SetupRTCTime();	 // SetupRealTime4RTC();

	while (1){
		DisplayTime();
		//delay1(500000);
	}

	DRV_Printf("====================================\r\n", 0);


	return 0;
}
