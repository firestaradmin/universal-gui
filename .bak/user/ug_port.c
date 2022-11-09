#include "lcd.h"
#include "ug_disp.h"
#include "ug_area.h"

static void disp_init(void);
static void disp_flush(ug_disp_drv_t * disp_drv, const ug_area_t * area, ug_color_t * color_p);


void ug_port_disp_init(void)
{

    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();


    /* Register disp buf. */
    static ug_disp_buf_t disp_buf;
    static ug_color_t buf1[UG_HOR_RES_MAX * 50];

    ug_disp_buf_init(&disp_buf, buf1, NULL, UG_HOR_RES_MAX * 50);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in UVGUI
     *----------------------------------*/

    ug_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    ug_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = UG_HOR_RES_MAX;
    disp_drv.ver_res = UG_VER_RES_MAX;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_screen = disp_flush;

    /*Set a display buffer*/
    disp_drv.buffer = &disp_buf;

    /*Set DPI. */
	disp_drv.dpi = UG_DPI;
    
    /*Finally register the driver*/
    ug_disp_drv_register(&disp_drv);
	
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/* Initialize your display and the required peripherals. */
static void disp_init(void)
{
    /*You code here*/
	LCD_Init();

}

/* Flush the content of the internal buffer the specific area on the display
 * You can use DMA or any hardware acceleration to do this operation in the background but
 * 'lv_disp_flush_ready()' has to be called when finished. */
static void disp_flush(ug_disp_drv_t * disp_drv, const ug_area_t * area, ug_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

	int32_t x;
	int32_t y;
	LCD_Set_Window(area->x1, area->y1, area->x2-area->x1+1, area->y2-area->y1+1);
	LCD_WriteRAM_Prepare();
	for(y = area->y1; y <= area->y2; y++) {
		for(x = area->x1; x <= area->x2; x++) {
			/* Put a pixel to the display. For example: */
			/* put_px(x, y, *color_p)*/
			LCD_WriteRAM(color_p->full);
			color_p++;
		}
	}

    /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
    ug_disp_flush_ready(disp_drv);
}


