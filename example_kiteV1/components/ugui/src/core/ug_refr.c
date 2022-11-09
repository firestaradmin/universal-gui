#include "ug_refr.h"
#include "ug_mem.h"
#include "ug_math.h"
#include "ug_tick.h"
#include "ug_draw_rect.h"
// #include "ug_log.h"
#include "ug_obj.h"
#include "ug_area.h"
/**********************
 *  STATIC VARIABLES
 **********************/
//static uint32_t px_num;       /* monitor for invalid areas */
static ug_disp_t * disp_refr;   /* disp will being refreshed */



/**********************
 *  STATIC PROTOTYPES
 **********************/

static void ug_refr_vdb_flush(void);

static void ug_refr_obj_and_children(ug_obj_t *obj, const ug_area_t *mask_p);

static void _ug_refr_obj(ug_obj_t *obj, const ug_area_t *mask);

/**
 * Called periodically to handle the refreshing
 * @param task pointer to the task itself
 */
void _ug_disp_refr_task(ug_task_t * task)
{

    uint32_t start = ug_tick_get();
    uint32_t elaps = 0;     /* elapsed time */

    disp_refr = task->user_data;

#if UG_USE_PERF_MONITOR == 0
    /* Ensure the task does not run again automatically.
     * This is done before refreshing in case refreshing invalidates something else.
     */
    ug_task_set_prio(task, UG_TASK_PRIO_OFF);
#endif



    if(disp_refr->needRefreashScreen == true){
        ug_refr_screen(disp_refr);
        disp_refr->needRefreashScreen = false;
    }
    else{
        // 将需要重绘的区域合并，如果合并后区域更小的话
        // ug_refr_join_area();
        ug_area_t child_area;
        ug_obj_t * child_p;
        _UG_LL_READ_BACK(disp_refr->act_scr->child_ll, child_p) {
            if(child_p->invalid != 1){
                continue;
            }
            ug_obj_get_coords(child_p, &child_area);

            ug_refr_obj_and_children(child_p, &child_area);
        }

    }

    ug_refr_vdb_flush();
    // if(ug_disp_is_true_double_buf(disp_refr) == false) {
    //     ug_refr_vdb_flush();
    // }

    elaps = ug_tick_elaps(start);   /* get elapsed time */


}


void ug_refr_screen(ug_disp_t *disp)
{

    ug_obj_t *act_scr = disp->act_scr;
    if(act_scr == NULL) return ;

    ug_refr_obj_and_children(act_scr, &disp->area);
}


/**
 * Make the refreshing from an object. Draw all its children and the youngers too.
 * @param top_p pointer to an objects. Start the drawing from it.
 * @param mask_p pointer to an area, the objects will be drawn only here
 */
static void ug_refr_obj_and_children(ug_obj_t * obj, const ug_area_t * mask_p)
{
    if(obj == NULL) obj = ug_disp_get_actscr(disp_refr);
    if(obj == NULL) return;  /*Shouldn't happen*/

    /*Refresh the top object and its children*/
    _ug_refr_obj(obj, mask_p);

    // draw son
    ug_area_t child_area;
    ug_area_t mask_child;
    ug_obj_t * child_p;
    bool union_ok;
    _UG_LL_READ_BACK(obj->child_ll, child_p) {
        ug_obj_get_coords(child_p, &child_area);
        union_ok = _ug_area_intersect(&mask_child, &mask_p, &child_area);

        if(union_ok) {
            /*Refresh the next children*/
            ug_refr_obj_and_children(child_p, &mask_child);
        }
    }

}

/**
 * Refresh an object and all of its children. (Called recursively)
 * @param obj pointer to an object to refresh
 * @param mask pointer to an area, the objects will be drawn only here
 */
static void _ug_refr_obj(ug_obj_t * obj, const ug_area_t * mask)
{
    /*Do not refresh hidden objects*/
    if(obj->hidden != 0) return;

    bool union_ok; /* Store the return value of area_union */
    /* Truncate the original mask to the coordinates of the parent
     * because the parent and its children are visible only here */
    ug_area_t obj_ext_mask;

    union_ok = _ug_area_intersect(&obj_ext_mask, mask, &obj->coords);

    /*Draw the parent and its children only if they ore on 'mask_parent'*/
    if(union_ok != false) {
        /* Redraw the object */
        if(obj->design_cb) obj->design_cb(obj, &obj_ext_mask);
    }
}

/**
 * Flush the content of the VDB
 */
static void ug_refr_vdb_flush(void)
{
    ug_disp_buf_t * vdb = ug_disp_get_buf(disp_refr);

    /*In double buffered mode wait until the other buffer is flushed before flushing the current
     * one*/
    if(ug_disp_is_double_buf(disp_refr)) {
        while(vdb->flushing) {
            if(disp_refr->driver.wait_cb) disp_refr->driver.wait_cb(&disp_refr->driver);
        }
    }

    vdb->flushing = 1;

    if(disp_refr->driver.buffer->last_area && disp_refr->driver.buffer->last_part)
        vdb->flushing_last = 1;
    else 
        vdb->flushing_last = 0;

    /*Flush the rendered content to the display*/
    ug_disp_t * disp = disp_refr;
    if(disp->driver.flush_screen_cb) disp->driver.flush_screen_cb(&disp->driver, &disp->area, vdb->buf_act);

    if(vdb->buf1 && vdb->buf2) {
        if(vdb->buf_act == vdb->buf1)
            vdb->buf_act = vdb->buf2;
        else
            vdb->buf_act = vdb->buf1;
    }
}





ug_disp_t * _ug_refr_get_refrdisp(void)
{
    return disp_refr;
}

























#if 0
/* old way */

static ug_area_t * redrawn_area;

/* 刷新区域是否被更改标志 */
static uint8_t ug_redrawn_area_changed_flag;


/* screen refresh task, need to run in while */
void _ug_refr_task(void * task)
{
	
    ug_disp_t * disp = ((ug_task_t *)task)->user_data;
    uint32_t tick_now = ug_tick_get();
	_ug_refr_screen(disp);
	disp->last_activity_time = tick_now;

}



void _ug_refr_draw_obj(ug_obj_t * obj)
{
    
    if(obj->haschanged || _ug_refr_check_children_changed(obj)){

        _ug_refr_draw_obj_on_scr(obj);

        /* _ug_refr_updata_redrawn_area */
        if(ug_redrawn_area_changed_flag == 0){
            _ug_memcpy(redrawn_area, &obj->coords, sizeof(ug_area_t));
            ug_redrawn_area_changed_flag = 1;
        }
        else {
            redrawn_area->x1 = UG_MATH_MIN(redrawn_area->x1, obj->coords.x1);
            redrawn_area->x2 = UG_MATH_MAX(redrawn_area->x2, obj->coords.x2);
            redrawn_area->y1 = UG_MATH_MIN(redrawn_area->y1, obj->coords.y1);
            redrawn_area->x1 = UG_MATH_MAX(redrawn_area->y2, obj->coords.y2);
        }

        obj->haschanged = false;
    }

    ug_ll_node_t *obj_child_next = _ug_ll_get_head(&((ug_obj_t *)obj)->child_ll);
    if(obj_child_next == NULL) return;

    while(obj_child_next){
        _ug_refr_draw_obj((ug_obj_t *)obj_child_next);
        obj_child_next = _ug_ll_get_next(&((ug_obj_t *)obj)->child_ll, obj_child_next);
    }

}


uint8_t _ug_refr_check_children_changed(ug_obj_t * obj)
{
    ug_ll_node_t *nextchild = _ug_ll_get_head(&obj->child_ll);
    if(nextchild == NULL) return UG_OBJ_NO_CHANGED;

    while(nextchild){
        if(((ug_obj_t *)nextchild)->haschanged)
            return UG_OBJ_HAS_CHANGED;

        nextchild = _ug_ll_get_next(&obj->child_ll, nextchild);
    }

    return UG_OBJ_NO_CHANGED;
}




void _ug_refr_draw_obj_on_scr(ug_obj_t *obj)
{
    const ug_area_t *coords = &obj->coords;
    //ug_disp_t *disp = ug_disp_get_actdisp();

    ug_fillRectangle(coords->x1, coords->y1, coords->x2, coords->y2, obj->bg_color);

}



#endif


