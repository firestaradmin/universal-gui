/************************************************************************************
*  Copyright (c), 2019, LXG.
*
* FileName		:UVGUI_UserConfig.h
* Author		:firestaradmin
* Version		:1.0
* Date			:2020.7.25
* Description	:a universal GUI Lib, usually used in Screen display of Embedded System.
* History		:
*
*
*************************************************************************************/

#ifndef _UVGUI_USERCONFIG_H__
#define _UVGUI_USERCONFIG_H__

#include <stdint.h>

#define __USE_ARCH_BIT 32
#ifndef __USE_ARCH_BIT
	#error "Please define __USE_ARCH_BIT, 32 or 64 !!"
#endif


/*********************\
 * UVGUI V1.10
 **********************/
/* Maximal horizontal and vertical resolution to support by the library.*/
#define UG_HOR_RES_MAX          (320)
#define UG_VER_RES_MAX          (240)

/* Color depth:
 * - 1:  1 byte per pixel
 * - 8:  RGB332
 * - 16: RGB565
 * - 32: ARGB8888
 */
#define UG_COLOR_DEPTH     16

/* Dot Per Inch: used to initialize default sizes.
 * E.g. a button with width = UG_DPI / 2 -> half inch wide
 * (Not so important, you can adjust it to modify default sizes and spaces)*/
#define UG_DPI              130     /*[px]*/

/* Type of coordinates. Should be `int16_t` (or `int32_t` for extreme cases) */
typedef int16_t ug_coord_t;

#define UG_OBJ_DEFAULT_BG_COLOR 0xFFFF;
//#define UG_OBJ_DEFAULT_FR_COLOR 0x0000;
/* Default display refresh period.
 * Can be changed in the display driver (`ug_disp_drv_t`).*/
#define UG_DISP_DEF_REFR_PERIOD      30      /*[ms]*/


#define UG_MEM_CUSTOM      0
#if UG_MEM_CUSTOM == 0
/* Size of the memory used by `ug_mem_alloc` in bytes (>= 2kB)*/
#  define UG_MEM_SIZE    (20U * 1024U)

/* Complier prefix for a big array declaration */
//#  define UG_MEM_ATTR	DRAM_ATTR
#  define UG_MEM_ATTR	


/* Set an address for the memory pool instead of allocating it as an array.
 * Can be in external SRAM too. */
#  define UG_MEM_ADR          0

/* Automatically defrag. on free. Defrag. means joining the adjacent free cells. */
#  define UG_MEM_AUTO_DEFRAG  1
#else       /*UG_MEM_CUSTOM*/
#  define UG_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#  define UG_MEM_CUSTOM_ALLOC   malloc       /*Wrapper to malloc*/
#  define UG_MEM_CUSTOM_FREE    free         /*Wrapper to free*/
#endif     /*UG_MEM_CUSTOM*/

/* Garbage Collector settings
 * Used if uvgui is binded to higher level language and the memory is managed by that language */
#define UG_ENABLE_GC 0
#if UG_ENABLE_GC != 0
#  define UG_GC_INCLUDE "ug_gc.h"                           /*Include Garbage Collector related things*/
#  define UG_MEM_CUSTOM_REALLOC   your_realloc           /*Wrapper to realloc*/
#  define UG_MEM_CUSTOM_GET_SIZE  your_mem_get_size      /*Wrapper to ug_mem_get_size*/
#endif /* UG_ENABLE_GC */


/* 1: use a custom tick source.
 * It removes the need to manually update the tick with `ug_tick_inc`) */
#define UG_TICK_CUSTOM     0
#if UG_TICK_CUSTOM == 1
#define UG_TICK_CUSTOM_INCLUDE  "Arduino.h"         /*Header for the system time function*/
#define UG_TICK_CUSTOM_SYS_TIME_EXPR (millis())     /*Expression evaluating to current system time in ms*/
#endif   /*UG_TICK_CUSTOM*/

#define UG_SPRINTF_CUSTOM 0

/* Export integer constant to binding.
 * This macro is used with constants in the form of UG_<CONST> that
 * should also appear on lvgl binding API such as Micropython
 *
 * The default value just prevents a GCC warning.
 */
#define UG_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning


/*=====================
 *  Compiler settings
 *====================*/

/* Prefix performance critical functions to place them into a faster memory (e.g RAM)
 * Uses 15-20 kB extra memory */
#define UG_ATTRIBUTE_FAST_MEM

/*===================
 *  UG_OBJ SETTINGS
 *==================*/

#if UG_USE_USER_DATA
/*Declare the type of the user data of object (can be e.g. `void *`, `int`, `struct`)*/
typedef void * ug_obj_user_data_t;
/*Provide a function to free user data*/
#define UG_USE_USER_DATA_FREE 0
#if UG_USE_USER_DATA_FREE
#  define UG_USER_DATA_FREE_INCLUDE  "something.h"  /*Header for user data free function*/
/* Function prototype : void user_data_free(ug_obj_t * obj); */
#  define UG_USER_DATA_FREE  (user_data_free)       /*Invoking for user data free function*/
#endif
#endif

/*1: enable `ug_obj_realign()` based on `ug_obj_align()` parameters*/
#define UG_USE_OBJ_REALIGN          1

/*==================
 *  LV OBJ X USAGE
 *================*/
/*

/*Label (dependencies: -*/
#define UG_USE_LABEL    1
#if UG_USE_LABEL != 0
/*Hor, or ver. scroll speed [px/sec] in 'UG_LABEL_LONG_ROLL/ROLL_CIRC' mode*/
#  define UG_LABEL_DEF_SCROLL_SPEED       25

/* Waiting period at beginning/end of animation cycle */
#  define UG_LABEL_WAIT_CHAR_COUNT        3

/*Enable selecting text of the label */
#  define UG_LABEL_TEXT_SEL               0

/*Store extra some info in labels (12 bytes) to speed up drawing of very long texts*/
#  define UG_LABEL_LONG_TXT_HINT          0
#endif


/*********************\
 * UVGUI V1.00
 **********************/
/* The number of using screen devices. In general, it should be set to 1.*/
#define UVGUI_DEV_NUM	2

/* Represents the size required for the color of a pixel   
   if your screen is mono, the screen_Color_bitNum need to be init as 1;*/
#define UVGUI_COLOR_DATA_BITS  16	//unit:bit



#if(UVGUI_COLOR_DATA_BITS == 1)
	#define UVGUI_COLOR_BLACK  0
	#define UVGUI_COLOR_WHITE  1
#elif (UVGUI_COLOR_DATA_BITS == 16)
	//画笔颜色
	#define UVGUI_COLOR_WHITE         	 0xFFFF
	#define UVGUI_COLOR_BLACK         	 0x0000	  
	#define UVGUI_COLOR_BLUE         	 0x001F  
	#define UVGUI_COLOR_BRED             0XF81F
	#define UVGUI_COLOR_GRED 			 0XFFE0
	#define UVGUI_COLOR_GBLUE			 0X07FF
	#define UVGUI_COLOR_RED           	 0xF800
	#define UVGUI_COLOR_MAGENTA       	 0xF81F
	#define UVGUI_COLOR_GREEN         	 0x07E0
	#define UVGUI_COLOR_CYAN          	 0x7FFF
	#define UVGUI_COLOR_YELLOW        	 0xFFE0
	#define UVGUI_COLOR_BROWN 			 0XBC40 //棕色
	#define UVGUI_COLOR_BRRED 			 0XFC07 //棕红色
	#define UVGUI_COLOR_GRAY  			 0X8430 //灰色
	//GUI颜色
	#define UVGUI_COLOR_DARKBLUE      	 0X01CF	//深蓝色
	#define UVGUI_COLOR_LIGHTBLUE      	 0X7D7C	//浅蓝色  
	#define UVGUI_COLOR_GRAYBLUE       	 0X5458 //灰蓝色
	//以上三色为PANEL的颜色 
	#define UVGUI_COLOR_LIGHTGREEN     	 0X841F //浅绿色
	#define UVGUI_COLOR_LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
	#define UVGUI_COLOR_LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
	#define UVGUI_COLOR_LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)	
#elif (UVGUI_COLOR_DATA_BITS == 18)

#elif (UVGUI_COLOR_DATA_BITS == 24)

#elif (UVGUI_COLOR_DATA_BITS == 32)
#endif

/* display default config	*/
#define UVGUI_DEFAULT_FRONT_COLOR		UVGUI_COLOR_BLACK
#define UVGUI_DEFAULT_BACK_COLOR		UVGUI_COLOR_WHITE
#define UVGUI_DEFAULT_FONTSIZE	UVGUI_FONT_Size_16







/*W25Q64
容量为8M字节,共有128个Block,2048个Sector 
4Kbytes为一个Sector
16个扇区为1个Block */

/* GB2312 Font Code start address in EX-FLASH(W25QXX) */
#define GB2312_FONT_SIZE8_ADDRESS	0
#define GB2312_FONT_SIZE12_ADDRESS	0
#define GB2312_FONT_SIZE16_ADDRESS	4*1024*16			/* address: 65536 ~ 348287 */
#define GB2312_FONT_SIZE24_ADDRESS	4*1024*16+282752 	/* address: 348288 ~ 984479 */
#define GB2312_FONT_SIZE32_ADDRESS 	0


#endif
