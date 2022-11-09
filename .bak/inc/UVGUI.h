/************************************************************************************
 * Copyright (c), 2021, LXG.
 * A universal easy GUI Lib, usually used in screen display of Embedded System.
 * @file UVGUI.h
 * @author firestaradmin
 * @version	2.1.1
 * @date 2021/9/15
 * @history:
 * 2020/7/25 V1.00 简易的版本
 * 2020/9/25 V1.10 仿照LVGL，使用链表和对象的思维方式。
 * 2021/9/15 V2.1.1 更改目录结构，放弃复杂化GUI
 *
 *************************************************************************************/

#ifndef _UVGUI_H__
#define _UVGUI_H__

// #include "UVGUI_Animation.h"
// #include "UVGUI_Ascii_Code_Tab.h"
// #include "ug_user_config.h"
// #include "ug_type.h"
//#include "ug_port.h"
//#include "ug_disp.h"
#include "ug_refr.h"
// #include "ug_math.h"
// #include "ug_ll.h"
// #include "ug_draw_elements.h"
// #include "ug_task.h"
#include "ug_obj.h"
// #include "ug_area.h"
//#include "ug_tick.h"


/* Init UVGUI Lib */
void ug_init(void);



extern ug_ll_t _ug_disp_ll;

extern ug_ll_t _ug_task_ll;

extern ug_task_t * _ug_task_act;





#endif