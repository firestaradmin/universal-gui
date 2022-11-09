#ifndef __UG_OBJ_H__
#define __UG_OBJ_H__
#include "ug_area.h"
#include "ug_ll.h"
#include "ug_color.h"
#include "ug_disp.h"





struct _ug_obj_t;



/**
 * The design callback is used to draw the object on the screen.
 * It accepts the object, a mask area, and the mode in which to draw the object.
 */
typedef ug_design_res_t (*ug_design_cb_t)(struct _ug_obj_t * obj, const ug_area_t * clip_area);

enum {
    UG_EVENT_PRESSED,             /**< The object has been pressed*/
    UG_EVENT_CLICKED,             /**< Called on release if not dragged (regardless to long press)*/
    UG_EVENT_RELEASED,            /**< Called in every cases when the object has been released*/
    UG_EVENT_KEY,
    UG_EVENT_FOCUSED,
    UG_EVENT_DEFOCUSED,
    UG_EVENT_DELETE, /**< Object is being deleted */
};
typedef uint8_t ug_event_t; /**< Type of event being sent to the object. */

/**
 * @brief Event callback.
 * Events are used to notify the user of some action being taken on the object.
 * For details, see ::ug_event_t.
 */
typedef void (*ug_event_cb_t)(struct _ug_obj_t * obj, ug_event_t event);

/** Signals are for use by the object itself or to extend the object's functionality.
  * Applications should use ::ug_obj_set_event_cb to be notified of events that occur
  * on the object. */
enum {
    UG_SIGNAL_CHILD_CHG, /**< Child was removed/added */
    UG_SIGNAL_COORD_CHG, /**< Object coordinates/size have changed */
    UG_SIGNAL_PARENT_SIZE_CHG, /**< Parent's size has changed */
    /*General signals*/
    UG_SIGNAL_CHILD_CHG, /**< Child was removed/added */
    UG_SIGNAL_COORD_CHG, /**< Object coordinates/size have changed */

    UG_SIGNAL_PRESSED,           /**< The object has been pressed*/
    UG_SIGNAL_RELEASED,          /**< User pressed object for a short period of time, then released it. Not called if dragged. */
    /*Group related*/
    UG_SIGNAL_FOCUS,
    UG_SIGNAL_DEFOCUS,
};
typedef uint8_t ug_signal_t;

typedef ug_res_t (*ug_signal_cb_t)(struct _ug_obj_t * obj, ug_signal_t sign, void * param);




typedef struct _ug_obj_t {
	char *name;
    struct _ug_obj_t * parent; /**< Pointer to the parent object*/

    ug_ll_t child_ll;       /**< Linked list to store the children objects*/

    ug_area_t coords; /**< Coordinates of the object (x1, y1, x2, y2)*/
    ug_area_t coords_needClear; 

    ug_event_cb_t event_cb; /**< Event callback function */
    ug_signal_cb_t signal_cb; /**< Object type specific signal function*/
    ug_design_cb_t design_cb; /**< Object type specific design function*/

    /*Attributes and states*/
    void * ext_attr;            /**< Object type specific extended data*/
    uint8_t hidden : 1; 
    uint8_t invalid : 1;    // 1: need redrawed,0: already drawed


    ug_color_t bg_color;

} ug_obj_t;

/*---------------------
 *  obj function
 *--------------------*/
/**
 * Create a basic object
 * @param parent pointer to a parent object.
 *                  If NULL then a screen will be created
 * @param copy pointer to a base object, if not NULL then the new object will be copied from it
 * @return pointer to the new object
 */
ug_obj_t * ug_obj_create(ug_obj_t * parent, const ug_obj_t * copy, char *name);
/**
 * Delete 'obj' and all of its children
 * @param obj pointer to an object to delete
 * @return UG_RES_INV because the object is deleted
 */
ug_res_t ug_obj_del(ug_obj_t * obj);


/**
 * Call an event function with an object, event, and data.
 * @param event_xcb an event callback function. If `NULL` `UG_RES_OK` will return without any actions.
 *        (the 'x' in the argument name indicates that its not a fully generic function because it not follows
 *         the `func_name(object, callback, ...)` convention)
 * @param obj pointer to an object to associate with the event (can be `NULL` to simply call the `event_cb`)
 * @param event an event
 * @param data pointer to a custom data
 * @return UG_RES_OK: `obj` was not deleted in the event; UG_RES_INV: `obj` was deleted in the event
 */
ug_res_t ug_event_send_func(ug_event_cb_t event_xcb, ug_obj_t * obj, ug_event_t event, const void * data);

/**
 * Send an event to the object
 * @param obj pointer to an object
 * @param event the type of the event from `ug_event_t`.
 * @return UG_RES_OK or UG_RES_INV
 */
ug_res_t ug_signal_send(ug_obj_t * obj, ug_signal_t signal, void * param);

/**
 * Mark an area of an object as invalid.
 * This area will be redrawn by 'ug_refr_task'
 * @param obj pointer to an object
 * @param area the area to redraw
 */
void ug_obj_markRedraw_area(const ug_obj_t * obj, const ug_area_t * area);

/**
 * Mark the object as invalid therefore its current position will be redrawn by 'ug_refr_task'
 * @param obj pointer to an object
 */
void ug_obj_markRedraw(const ug_obj_t * obj);


/*====================
 *  get function
 *====================*/

/**
 * Copy the coordinates of an object to an area
 * @param obj pointer to an object
 * @param cords_p pointer to an area to store the coordinates
 */
void ug_obj_get_coords(const ug_obj_t * obj, ug_area_t * cords_p);

/**
 * Get the x coordinate of object
 * @param obj pointer to an object
 * @return distance of 'obj' from the left side of its parent
 */
ug_coord_t ug_obj_get_x(const ug_obj_t * obj);

/**
 * Get the y coordinate of object
 * @param obj pointer to an object
 * @return distance of 'obj' from the top of its parent
 */
ug_coord_t ug_obj_get_y(const ug_obj_t * obj);


/**
 * Get the width of an object
 * @param obj pointer to an object
 * @return the width
 */
ug_coord_t ug_obj_get_width(const ug_obj_t * obj);


/**
 * Get the height of an object
 * @param obj pointer to an object
 * @return the height
 */
ug_coord_t ug_obj_get_height(const ug_obj_t * obj);
/**
 * Get the hidden attribute of an object
 * @param obj pointer to an object
 * @return true: the object is hidden
 */
bool ug_obj_get_hidden(const ug_obj_t * obj);

/**
 * Return with the screen of an object
 * @param obj pointer to an object
 * @return pointer to a screen
 */
ug_obj_t * ug_obj_get_screen(const ug_obj_t * obj);

/**
 * Get the display of an object
 * @param scr pointer to an object
 * @return pointer the object's display
 */
struct _disp_t * ug_obj_get_disp(const ug_obj_t * obj);

/**
 * Returns with the parent of an object
 * @param obj pointer to an object
 * @return pointer to the parent of  'obj'
 */
ug_obj_t * ug_obj_get_parent(const ug_obj_t * obj);

/**
 * Iterate through the children of an object (start from the "youngest")
 * @param obj pointer to an object
 * @param child NULL at first call to get the next children
 *                  and the previous return value later
 * @return the child after 'act_child' or NULL if no more child
 */
ug_obj_t * ug_obj_get_child(const ug_obj_t * obj, const ug_obj_t * child);

/**
 * Get the really focused object by taking `focus_parent` into account.
 * @param obj the start object
 * @return the object to really focus
 */
ug_obj_t * ug_obj_get_focused_obj(const ug_obj_t * obj);


/**
 * Get the protect field of an object
 * @param obj pointer to an object
 * @return protect field ('OR'ed values of `ug_protect_t`)
 */
uint8_t ug_obj_get_protect(const ug_obj_t * obj);

/**
 * Check at least one bit of a given protect bitfield is set
 * @param obj pointer to an object
 * @param prot protect bits to test ('OR'ed values of `ug_protect_t`)
 * @return false: none of the given bits are set, true: at least one bit is set
 */
bool ug_obj_is_protected(const ug_obj_t * obj, uint8_t prot);


ug_state_t ug_obj_get_state(const ug_obj_t * obj, uint8_t part);


/**
 * Get the signal function of an object
 * @param obj pointer to an object
 * @return the signal function
 */
ug_signal_cb_t ug_obj_get_signal_cb(const ug_obj_t * obj);

/**
 * Get the design function of an object
 * @param obj pointer to an object
 * @return the design function
 */
ug_design_cb_t ug_obj_get_design_cb(const ug_obj_t * obj);

/**
 * Get the event function of an object
 * @param obj pointer to an object
 * @return the event function
 */
ug_event_cb_t ug_obj_get_event_cb(const ug_obj_t * obj);

/*------------------
 * Other get
 *-----------------*/

/**
 * Get the ext pointer
 * @param obj pointer to an object
 * @return the ext pointer but not the dynamic version
 *         Use it as ext->data1, and NOT da(ext)->data1
 */
void * ug_obj_get_ext_attr(const ug_obj_t * obj);

/*====================
 *  set function
 *====================*/



/**
 * Set the a signal function of an object. Used internally by the library.
 * Always call the previous signal function in the new.
 * @param obj pointer to an object
 * @param cb the new signal function
 */
void ug_obj_set_signal_cb(ug_obj_t * obj, ug_signal_cb_t signal_cb);

/**
 * Set a new design function for an object
 * @param obj pointer to an object
 * @param design_cb the new design function
 */
void ug_obj_set_design_cb(ug_obj_t * obj, ug_design_cb_t design_cb);

/**
 * Set a an event handler function for an object.
 * Used by the user to react on event which happens with the object.
 * @param obj pointer to an object
 * @param event_cb the new event function
 */
void ug_obj_set_event_cb(ug_obj_t * obj, ug_event_cb_t event_cb);


/*--------------------
 * Coordinate set
 * ------------------*/


/**
 * Set relative the position of an object (relative to the parent)
 * @param obj pointer to an object
 * @param x new distance from the left side of the parent
 * @param y new distance from the top of the parent
 */
void ug_obj_set_pos(ug_obj_t * obj, ug_coord_t x, ug_coord_t y);

/**
 * Set the x coordinate of a object
 * @param obj pointer to an object
 * @param x new distance from the left side from the parent
 */
void ug_obj_set_x(ug_obj_t * obj, ug_coord_t x);

/**
 * Set the y coordinate of a object
 * @param obj pointer to an object
 * @param y new distance from the top of the parent
 */
void ug_obj_set_y(ug_obj_t * obj, ug_coord_t y);

/**
 * Set the size of an object
 * @param obj pointer to an object
 * @param w new width
 * @param h new height
 */
void ug_obj_set_size(ug_obj_t * obj, ug_coord_t w, ug_coord_t h);

/**
 * Align an object to an other object.
 * @param obj pointer to an object to align
 * @param base pointer to an object (if NULL the parent is used). 'obj' will be aligned to it.
 * @param align type of alignment (see 'ug_align_t' enum)
 * @param x_ofs x coordinate offset after alignment
 * @param y_ofs y coordinate offset after alignment
 */
void ug_obj_align(ug_obj_t * obj, const ug_obj_t * base, ug_align_t align, ug_coord_t x_ofs, ug_coord_t y_ofs);

/**
 * Realign the object based on the last `ug_obj_align` parameters.
 * @param obj pointer to an object
 */
void ug_obj_realign(ug_obj_t * obj);

/**
 * Align an object's middle point to an other object.
 * @param obj pointer to an object to align
 * @param base pointer to an object (if NULL the parent is used). 'obj' will be aligned to it.
 * @param align type of alignment (see 'ug_align_t' enum)
 * @param x_ofs x coordinate offset after alignment
 * @param y_ofs y coordinate offset after alignment
 */
void ug_obj_align_origo(ug_obj_t * obj, const ug_obj_t * base, ug_align_t align, ug_coord_t x_ofs, ug_coord_t y_ofs);

/**
 * Enable the automatic realign of the object when its size has changed based on the last
 * `ug_obj_align` parameters.
 * @param obj pointer to an object
 * @param en true: enable auto realign; false: disable auto realign
 */
void ug_obj_set_auto_realign(ug_obj_t * obj, bool en);


void ug_obj_set_color(ug_obj_t *obj, ug_color_t color);
void ug_obj_set_coords(ug_obj_t *obj, ug_area_t *area);
void ug_obj_move(ug_obj_t *obj, int16_t dx, int16_t dy);



/**
 * Allocate a new ext. data for an object
 * @param obj pointer to an object
 * @param ext_size the size of the new ext. data
 * @return pointer to the allocated ext
 */
void * ug_obj_allocate_ext_attr(ug_obj_t * obj, uint16_t ext_size);


#endif // !__UG_OBJ_H__

