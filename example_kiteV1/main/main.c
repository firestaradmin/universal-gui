/* 
 * Kite_V1 core board demo
 */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


#include "key.h"
#include "led.h"
#include "lcd_st7735s.h"

/* UVGUI */
#include "ug_port.h"
#include "ug_disp.h"
#include "ug_refr.h"
// #include "ug_math.h"
// #include "ug_ll.h"
// #include "ug_draw_elements.h"
// #include "ug_task.h"
// #include "ug_obj.h"
// #include "ug_area.h"
#include "ug_tick.h"
#include "ug_main.h"



void vOtherFunction( void );
void vTask_ug_tick( void * pvParameters );

ug_obj_t *obj3;



// void app_main(void)
// {
// 	printf("Kite_V1 core board demo!\n");

//     led_init();
// 	key_init();

//     lcd_init();

//     lcd_fill(0, 0,80, 80, 0xffff);
//     lcd_fill(0, 80, 80, 160, 0x0000);
//     lcd_fill(40, 100, 60, 140, 0x7ffa);
//     while(1)
// 	{
// 		vTaskDelay(100 / portTICK_RATE_MS);
// 	}

// }

#if 1
void app_main(void)
{
	printf("Kite_V1 core board demo!\n");

    led_init();
	key_init();
    
	ugui_init();
    ug_port_disp_init();

	vOtherFunction();

	// ug_area_t area ;
	// area.x1 = 0;
	// area.x2 = 100;
	// area.y1 = 0;
	// area.y2 = 50;
	// obj3 = ug_obj_create(ug_disp_get_actdisp_actscr(), NULL, "obj3_red");
	// ug_obj_set_color(obj3, UG_COLOR_RED);
	// ug_obj_set_coords(obj3, &area);
    // lcd_drawPoint(5, 5, RED);
    // lcd_drawPoint(5, 6, RED);
    // lcd_drawPoint(5, 7, RED);
    // lcd_drawPoint(5, 8, RED);
    // lcd_fill(0, 0, 10, 20, BRED);
    printf("FUCK!FUCK!FUCK!FUCK!FUCK!FUCK!\r\n");
    while(1)
	{
		ug_task_handler();
		// lcd_test();
	}

}



// Function that creates a task.
void vOtherFunction( void )
{

TaskHandle_t xHandle = NULL;

 // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
 // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
 // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
 // the new task attempts to access it.
	xTaskCreate( vTask_ug_tick, "vTask_ug_tick", 1024, NULL, 9, &xHandle );
	configASSERT( xHandle );

	// Use the handle to delete the task.
	if( xHandle != NULL )
	{
		vTaskDelete( xHandle );
	}
}



// Task to be created.
void vTask_ug_tick( void * pvParameters )
{
	for( ;; )
	{
		// Task code goes here.
		ug_tick_inc(5);
		vTaskDelay(5 / portTICK_PERIOD_MS);
	}
}

#endif