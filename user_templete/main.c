
#include <stdio.h>
#include "main.h"
#include "lcd.h"
/* UGUI */
#include "ug_main.h"
#include "ug_port.h"
#include "ug_task.h"


void BSP_Init(void);
void change_size_task(ug_task_t *p);

ug_task_t * task1;
ug_obj_t * obj1;
ug_obj_t * obj2;
ug_obj_t * obj3;
int main()
{
	BSP_Init();
	printf("NestlingV2.1_Demo\r\n");
	ug_init();
	ug_port_disp_init();
	
	ug_area_t area ;
	area.x1 = 0;
	area.x2 = 50;
	area.y1 = 0;
	area.y2 = 100;

	ug_obj_set_color(ug_disp_get_actdisp_actscr(), UG_COLOR_BLACK);
	obj1 = ug_obj_create(ug_disp_get_actdisp_actscr(), NULL, "obj1_blue");
	ug_obj_set_color(obj1, UG_COLOR_BLUE);
	ug_obj_set_coords(obj1, &area);
	

	obj2 = ug_obj_create(ug_disp_get_actdisp_actscr(), obj1, "obj2_red");
	ug_obj_set_color(obj2, UG_COLOR_RED);
	ug_obj_set_coords(obj1, &area);
	ug_obj_align(obj2, ug_disp_get_actdisp_actscr(),UG_ALIGN_IN_BOTTOM_LEFT, 0,0);




    obj3 = ug_obj_create(obj1, NULL, "obj3");
	ug_obj_set_color(obj3, UG_COLOR_WHITE);
	area.x1 = 0;
	area.x2 = 20;
	area.y1 = 0;
	area.y2 = 20;
    ug_obj_set_coords(obj3, &area);
    ug_obj_align(obj3, obj1,UG_ALIGN_CENTER, 0,0);



    // create move task
	task1 = ug_task_create(change_size_task, 30, 2, NULL);
    ug_task_ready(task1); 

	while(1)
	{
        // must call the handler
		ug_task_handler();
		
	}
}

void change_size_task(ug_task_t *p)
{
	ug_obj_move(obj1,2,1);
	ug_obj_move(obj2,2,-1);
}


void BSP_Init(void)
{
	SystemInit();	//晶振时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//4位抢占优先级
	DelayInit();	//延时函数初始化
	LED_init();
	usart1_init(115200);
	TIM2_Int_Init(10 - 1, 7200 - 1); //1MS
}


void TIM2_handle()
{
    ug_tick_inc(1);
}





