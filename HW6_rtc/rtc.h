/*
 * rtc.h
 *
 *  Created on: Dec 8, 2016
 *      Author: KHWANG-HOME
 */

#ifndef RTC_H_
#define RTC_H_

#define RTC_CONTROL *((unsigned int *)0x00200400)
#define RTC_SEC 	*((unsigned int *)0x00200404)
#define RTC_MIN 	*((unsigned int *)0x00200408)
#define RTC_HOUR 	*((unsigned int *)0x0020040C)
#define RTC_DAY 	*((unsigned int *)0x00200410)
#define RTC_WEEK 	*((unsigned int *)0x00200414)
#define RTC_MONTH  	*((unsigned int *)0x00200418)
#define RTC_YEAR  	*((unsigned int *)0x0020041C)

//#define RTC_CAL *((unsigned int *)0x00201034)
//#define RTC_PARAMETER *((unsigned int *)0x00201038)

#define To_ASCII 0x30

#endif /* RTC_H_ */
