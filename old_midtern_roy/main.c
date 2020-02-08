#include <nds32_intrinsic.h>
#include "WT58F2C9.h"
#include "gpio.h"

void delay1(unsigned int nCount)
{
   volatile unsigned int i;
   for (i=0;i<nCount;i++);
}

/* Intrrupt memory address */
#define INT_MEM_ADDR_BASE     (0x00200D00)
#define rINT0_IE0_ENABLE      (INT_MEM_ADDR_BASE+0x00)
#define rINT0_IE1_ENABLE      (INT_MEM_ADDR_BASE+0x04)
#define rINT0_IE2_ENABLE      (INT_MEM_ADDR_BASE+0x08)
#define rINT1_IE0_ENABLE      (INT_MEM_ADDR_BASE+0x10)
#define rINT1_IE1_ENABLE      (INT_MEM_ADDR_BASE+0x14)
#define rINT1_IE2_ENABLE      (INT_MEM_ADDR_BASE+0x18)

#define rINT0_IE0_FLAG        (INT_MEM_ADDR_BASE+0x20)
#define rINT0_IE1_FLAG        (INT_MEM_ADDR_BASE+0x24)
#define rINT0_IE2_FLAG        (INT_MEM_ADDR_BASE+0x28)
#define rINT1_IE0_FLAG        (INT_MEM_ADDR_BASE+0x30)
#define rINT1_IE1_FLAG        (INT_MEM_ADDR_BASE+0x34)
#define rINT1_IE2_FLAG        (INT_MEM_ADDR_BASE+0x38)

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
  DRV_EnableHWInt();        //Enable global Hardware interrupt, Assembly command

  // Enable default Block interrupt
  //DRV_BlockIntEnable();     //Enable each block device interrupt

}

#define rSYS_OPTION1  (0x00200004)

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
#define UART3_ADDR_BASE   (0x0020B400)
#define UART3_CR1     (UART3_ADDR_BASE+0x00)
#define UART3_CR2     (UART3_ADDR_BASE+0x04)

//0:1 Start bit/8 Data bits/1 Stop bit,
#define UART_FORMAT_N81 0
//1:1 Start bit/9 Data bits/1 Stop bit
#define UART_FORMAT_N91 1

#define UART_EN       (1<<17) //0:Disable, 1:Enable
#define UART_TX_EN      (1<<15)
#define UART_RX_EN      (1<<14)
#define UART_OVER8      (1<<13)
#define UART_OVER16     (0<<13)
#define UART_WORD_LENGTH  (UART_FORMAT_N81<<12)// //0:1 Start bit/8 Data bits/1 Stop bit, //1:1 Start bit/9 Data bits/1 Stop bit
#define UART_TX_DMA_EN    (1<<11) //0:Disable, 1:Enable
#define UART_RX_DMA_EN    (1<<10) // 0:Disable, 1:Enable
#define UART_RX_WAKEUP    (0<<9)  // 0:In active mode, 1:In mute mode
#define UART_WAKEUP_METHOD  (0<<8)  // 0:Idle mode, 1:Address mark
#define UART_UART_ADDR_NODE (0x5<<4)  //Data 0x0=xxxx0000b ~ 0xF=xxxx1111b,
#define UART_PARITY_EN    (0<<2)//SET_BIT1//(n<<1) //0:Disable, 1:Enable
#define UART_PARITY_SEL   (1<<1)// O:Even 1:Odd
#define UART_STOP_BIT 0// O:1-bit 1:2-bit

#define UART_SET_CTL_PARA UART_EN|UART_TX_EN|UART_RX_EN|UART_WORD_LENGTH\
              |UART_RX_WAKEUP|UART_WAKEUP_METHOD|UART_UART_ADDR_NODE\
              |UART_PARITY_EN|UART_PARITY_SEL|UART_STOP_BIT

//HXTAL = HXTAL_24M (OVER8=0)
//9600 = 156.25
#define BUARRATE_9600_MANTISSA_24MHZ  156
#define BUARRATE_9600_FRACTION_24MHZ  4
//19200 = 78.125
#define BUARRATE_19200_MANTISSA_24MHZ 78
#define BUARRATE_19200_FRACTION_24MHZ 2
//38400 = 39.0625
#define BUARRATE_38400_MANTISSA_24MHZ 39
#define BUARRATE_38400_FRACTION_24MHZ 1
//57600 = 26.0625
#define BUARRATE_57600_MANTISSA_24MHZ 26
#define BUARRATE_57600_FRACTION_24MHZ 1
//115200 = 13.0
#define BUARRATE_115200_MANTISSA_24MHZ  13
#define BUARRATE_115200_FRACTION_24MHZ  0
//230400 = 6.5
#define BUARRATE_230400_MANTISSA_24MHZ  6
#define BUARRATE_230400_FRACTION_24MHZ  8
//921600 = 1.625
#define BUARRATE_921600_MANTISSA_24MHZ  1
#define BUARRATE_921600_FRACTION_24MHZ  10


#define FALSE 0
#define TRUE  1
#define UART_TXD_BUFFER_SIZE  24
#define _EOS_ '\0' //End of string

#include <stdarg.h>
char printf_buf[1024];
void DRV_PutChar(char u8Char){
  U16 u16Count;
  OUTW(UART3_ADDR_BASE+0x0C, u8Char);
  u16Count = 0;
  //Wait transmission complete then clear by SW write to 0
  while((INW(UART3_ADDR_BASE+0x08)&0x00000020) == 0) {
  #if 1 //Don't delete.
    //-----Time out
    u16Count++;
    if(u16Count >= 1000) break;
  #endif
  }
  OUTW(UART3_ADDR_BASE+0x08, INW(UART3_ADDR_BASE+0x08) & 0xFFFFFFDF);
}
void DRV_PutStr(const char *s) {
  char c;
  while((c = *s++)) {
    if(c == '\n') DRV_PutChar('\r');
    DRV_PutChar(c);
  }
}
int DRV_Printf(const char *fmt, ...)
{
  //-----Pin configuration for UART3
  GPIO_PTC_FS = 0x0300;
  GPIO_PTC_PADINSEL = 0x0000;
  GPIO_PTC_DIR = 0xFEFF;
  GPIO_PTC_CFG = 0x0000;
  //UART Parameter
  OUTW(UART3_ADDR_BASE+0x00, UART_SET_CTL_PARA);
  //Set Baud rate with default sysclk
  OUTW(UART3_ADDR_BASE+0x14, ((BUARRATE_38400_MANTISSA_24MHZ<<4)|BUARRATE_38400_FRACTION_24MHZ)/2); //38400 for 12MHz MCUCLK
  va_list args;
  va_start (args, fmt);
  int rt = vsprintf (printf_buf, fmt, args);
  DRV_PutStr(printf_buf);
  va_end (args);
  return rt;
}
#define printf(fmt, ...) DRV_Printf(fmt, ##__VA_ARGS__)

unsigned int num[18] = {Number_0, Number_1, Number_2, Number_3, Number_4, Number_5, Number_6, Number_7,
      Number_8, Number_9, Number_A, Number_b, Number_C, Number_d, Number_E, Number_F, Number_Dot, 0x0};
unsigned int dig[8] = {Digit_1, Digit_2, Digit_3, Digit_4, Digit_5, Digit_6, Digit_7, Digit_8};
#define BIT(x) (1<<(x))
void led(int state) {
    GPIO_PTB_GPIO = ~state;
    // delay1(10);
}
typedef unsigned char uint8;
uint8 dip_read() {
  return (GPIO_PTC_PADIN>>2) & (BIT(8)-1);
}
uint8 dip_check(int idx) {
  return (dip_read() & BIT(idx)) >> idx;
}
void init()
{
  // led
  GPIO_PTB_DIR = 0x0000;  //for GPIO
  GPIO_PTB_CFG = 0xFFFF;  //for push pull
  GPIO_PTB_PADIN = 0x00;  //for Output
  GPIO_PTB_GPIO = 0xffff;
  // dip
  GPIO_PTC_FS       = 0x0000;
  GPIO_PTC_PADINSEL = 0x0000;
  GPIO_PTC_DIR      = 0xFFFF;
  GPIO_PTC_CFG      = 0x0000;
  // 7seg
  // Setting for 7LED select
  GPIO_PTA_DIR = 0x0000;
  GPIO_PTA_CFG = 0x0000;
  GPIO_PTA_GPIO = 0x0;
  // Setting for 7LED number
  GPIO_PTD_DIR = 0x0000;
  GPIO_PTD_CFG = 0x0000;
  GPIO_PTD_GPIO = 0x0;
}
#include <stdio.h>

int sw = 0, presw = -1;
char n[5];
int siz = 3;
void to_str()
{
  int tmp = sw;
  sprintf(n, "%d", sw);
  siz = 0;
  while(tmp)
  {
    tmp /= 10;
    siz++;
  }
  // int i;
  // for(i = 0; i < siz; i++)
  //   n[i] -= '0';
}

unsigned int clk = 0, clkLim = 100;
int wait_clock() {
  int rt = 1;
  if(clk++ > clkLim)
  {
    clk = 0;
    rt = 0;
  }
  return rt;
}
int frame[8];
void render()
{
  int i;
  for(i = 0; i < 8; i++)
  {
    GPIO_PTA_GPIO = dig[7-i];
    if(frame[i] == -1)
      GPIO_PTD_GPIO = 0x0;
    else
      GPIO_PTD_GPIO = num[frame[i]];
    delay1(800);
  }
}
void clear()
{
  int i;
  for(i = 0; i < 8; i++)
    frame[i] = -1;
}
int stage = 0;
int pos=0, cur;

int main()
{
  int tmp = 0, i, j;
  OS_PowerOnDriverInitial();
  init();
  while(1)
  {
    // process input
    int buf = 0;
    sw = 0;
    for(i = 0; i < 8; i++)
    {
      if(dip_check(i))
      {
        sw |= BIT(i);
        buf |= BIT(i+8);
      }
      else
        buf |= BIT(i);
    }
    led(buf);

    if(presw != sw)
    {
      presw = sw;
      to_str();
      cur = siz-1;
      stage = 0;
      for(i = 0; i < siz; i++)
        printf("%d ", n[i]);
    }

    // Update
    switch(stage)
    {
      case 0:
        pos = 0;
        stage = 1;
        cur = siz-1;
      break;

      case 1:
        frame[pos] = n[cur]-'0';
        pos++;
        if(pos == 8)
        {
          stage = 2;
          cur--;
          pos = 0;
        }
      break;

      case 2:
        frame[7] = n[cur+1]-'0';
        frame[pos] = n[cur]-'0';
        pos++;
        if(pos == 7)
        {
          stage = 3;
          cur--;
          pos = 0;
        }
      break;

      case 3:
        frame[7] = n[cur+2]-'0';
        frame[6] = n[cur+1]-'0';
        frame[pos] = n[cur]-'0';
        pos++;
        if(pos == 6)
        {
          stage = 4;
          pos = 0;
        }
      break;

      case 4:
        for(i = 7-siz, j=0; i <= 7 && j < siz; i++, j++)
          frame[i] = n[j]-'0';
        stage = 5;
      break;

      case 5:
      {
        for(i = 7-pos-siz-1, j=0; i <= 7-pos && j < siz; i++, j++)
          frame[i] = n[j]-'0';
        pos++;
        if(pos == 7-siz)
        {
          stage = 6; // end
        }
      }
      break;

      case 6:
        for(i = 0, j =0; i < siz && j < siz; i++, j++)
            frame[i] = n[j]-'0';
      break;
    }
    // printf("stage=%d pos=%d cur=%d\n", stage, pos, cur);
    // for(i = 0; i < 8; i++)
    //   printf("%d ", frame[i]);
    // printf("\n");

    // Render
    while(wait_clock())
      render();
    clear();
  }

  return 0;
}