// oled.h
// OLED header file
// rev 1 - January 2014 - Shabaz

// OLED function      WireColor			BBB
// ------------       -----------   -----------------
// SDA		            Yellow        P8_44  
// SCL        				Orange        P8_46
// HV-ENABLE          Black         P8_45
// RS									Blue					P8_11
// CS									Green					P8_12
// RESET							Green					P8_15

#ifndef _OLED_H_
#define _OLED_H_

#include "BBBiolib.h"

// definitions (these are all GPIO2)
#define SCL_LOW pin_low(8, 46)
#define SCL_HIGH pin_high(8, 46)
#define SDA_LOW pin_low(8, 44)
#define SDA_HIGH pin_high(8, 44)
#define HV_DIS pin_low(8, 45)
#define HV_EN pin_high(8, 45)
#define RS_LOW pin_low(8, 11)
#define RS_HIGH pin_high(8, 11)
#define CS_LOW pin_low(8, 12)
#define CS_HIGH pin_high(8, 12)
#define RESET_LOW pin_low(8, 15)
#define RESET_HIGH pin_high(8, 15)



// Display registers
#define INDEX 0
#define OSC_CTL 2
#define IREF 0x80
#define CLOCK_DIV 3
#define REDUCE_CURRENT 4
#define SOFT_RST 5
#define DISP_ON_OFF 6
#define PRECHARGE_TIME_R 8
#define PRECHARGE_TIME_G 0x09
#define PRECHARGE_TIME_B 0x0A
#define PRECHARGE_CURRENT_R 0x0B
#define PRECHARGE_CURRENT_G 0x0C
#define PRECHARGE_CURRENT_B 0x0D
#define DRIVING_CURRENT_R 0x10
#define DRIVING_CURRENT_G 0x11
#define DRIVING_CURRENT_B 0x12
#define DISPLAY_MODE_SET 0x13
#define RGB_IF 0x14
#define RGB_POL 0x15
#define MEMORY_WRITE_MODE 0x16
#define MX1_ADDR 0x17
#define MX2_ADDR 0x18
#define MY1_ADDR 0x19
#define MY2_ADDR 0x1A
#define MEM_PTR_X 0x20
#define MEM_PTR_Y 0x21
#define DDRAM_DATA_ACCESS_PORT 0x22
#define GRAY_SCALE_TABLE_INDEX 0x50
#define GRAY_SCALE_TABLE_DATA 0x51
#define DUTY 0x28
#define DSL 0x29
#define D1_DDRAM_FAC 0x2E
#define D1_DDRAM_FAR 0x2F
#define D2_DDRAM_SAC 0x31
#define D2_DDRAM_SAR 0x32
#define SCR1_FX1 0x33
#define SCR1_FX2 0x34
#define SCR1_FY1 0x35
#define SCR1_FY2 0x36
#define SCR2_SX1 0x37
#define SCR2_SX2 0x38
#define SCR2_SY1 0x39
#define SCR2_SY2 0x3A
#define SCREEN_SAVER_CONTROL 0x3B
#define SS_SLEEP_TIMER 0x3C
#define SCREEN_SAVER_MODE 0x3D
#define SS_SCR1_FU 0x3E
#define SS_SCR1_MXY 0x3F
#define SS_SCR2_FU 0x40
#define SS_SCR2_MXY 0x41
#define MOVING_DIRECTION 0x42
#define SS_SCR2_SX1 0x47
#define SS_SCR2_SX2 0x48
#define SS_SCR2_SY1 0x49
#define SS_SCR2_SY2 0x4A
#define FIN 0xff



typedef struct av_s
{
	unsigned char addr;
	unsigned char val;
} av_t;



#endif // _OLED_H_
