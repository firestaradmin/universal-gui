//#include "ug_label.h"
//#include "ug_log.h"
//#include "ug_obj.h"

//#if UG_USE_LABEL != 0
///*********************
// *      DEFINES
// *********************/
//#define UG_OBJX_NAME "ug_label"




///**********************
// *  STATIC VARIABLES
// **********************/


///**********************
// *   GLOBAL FUNCTIONS
// **********************/

///**
// * Create a label objects
// * @param par pointer to an object, it will be the parent of the new label
// * @param copy pointer to a label object, if not NULL then the new object will be copied from it
// * @return pointer to the created button
// */
//ug_obj_t * ug_label_create(ug_obj_t * par, const ug_obj_t * copy)
//{
//    /*Create a basic object*/
//    ug_obj_t * new_label = ug_obj_create(par, copy);

//    if(new_label == NULL) return NULL;

//    /*Extend the basic object to a label object*/
//    ug_obj_allocate_ext_attr(new_label, sizeof(ug_label_ext_t));

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(new_label);

//    if(ext == NULL) {
//        ug_obj_del(new_label);
//        return NULL;
//    }

//    ext->text       = NULL;
//    ext->static_txt = 0;
//    ext->recolor    = 0;
//    ext->align      = UG_LABEL_ALIGN_AUTO;
//    ext->long_mode  = UG_LABEL_LONG_EXPAND;

//    ext->offset.x = 0;
//    ext->offset.y = 0;

//    ug_obj_set_design_cb(new_label, ug_label_design);
//    ug_obj_set_signal_cb(new_label, ug_label_signal);

//    /*Init the new label*/
//    if(copy == NULL) {
//        ug_label_set_text(new_label, "Text");
//    }
//    /*Copy 'copy' if not NULL*/
//    else {
//        ug_label_ext_t * copy_ext = ug_obj_get_ext_attr(copy);
//        ug_label_set_long_mode(new_label, ug_label_get_long_mode(copy));
//        ug_label_set_recolor(new_label, ug_label_get_recolor(copy));
//        ug_label_set_align(new_label, ug_label_get_align(copy));
//        if(copy_ext->static_txt == 0)
//            ug_label_set_text(new_label, ug_label_get_text(copy));
//        else
//            ug_label_set_text_static(new_label, ug_label_get_text(copy));

//        /*In DOT mode save the text byte-to-byte because a '\0' can be in the middle*/
//        if(copy_ext->long_mode == UG_LABEL_LONG_DOT) {
//            ext->text = ug_mem_realloc(ext->text, _ug_mem_get_size(copy_ext->text));
//            UG_ASSERT_MEM(ext->text);
//            if(ext->text == NULL) return NULL;
//            _ug_memcpy(ext->text, copy_ext->text, _ug_mem_get_size(copy_ext->text));
//        }

//        if(copy_ext->dot_tmp_alloc && copy_ext->dot.tmp_ptr) {
//            uint32_t len = (uint32_t)strlen(copy_ext->dot.tmp_ptr);
//            ug_label_set_dot_tmp(new_label, ext->dot.tmp_ptr, len);
//        }
//        else {
//            _ug_memcpy(ext->dot.tmp, copy_ext->dot.tmp, sizeof(ext->dot.tmp));
//        }
//        ext->dot_tmp_alloc = copy_ext->dot_tmp_alloc;
//        ext->dot_end       = copy_ext->dot_end;

//        /*Refresh the style with new signal function*/
//        ug_obj_refresh_style(new_label, UG_STYLE_PROP_ALL);
//    }
//    return new_label;
//}


///**
// * Refresh the label with its text stored in its extended data
// * @param label pointer to a label object
// */
//static void ug_label_refr_text(ug_obj_t * label)
//{
//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);

//    if(ext->text == NULL) return;

//    ug_area_t txt_coords;
//    get_txt_coords(label, &txt_coords);
//    ug_coord_t max_w         = ug_area_get_width(&txt_coords);
//    const ug_font_t * font   = ug_obj_get_style_text_font(label, UG_LABEL_PART_MAIN);
//    ug_style_int_t line_space = ug_obj_get_style_text_line_space(label, UG_LABEL_PART_MAIN);
//    ug_style_int_t letter_space = ug_obj_get_style_text_letter_space(label, UG_LABEL_PART_MAIN);

//    /*Calc. the height and longest line*/
//    ug_point_t size;
//    ug_txt_flag_t flag = UG_TXT_FLAG_NONE;
//    if(ext->recolor != 0) flag |= UG_TXT_FLAG_RECOLOR;
//    if(ext->expand != 0) flag |= UG_TXT_FLAG_EXPAND;
//    if(ext->long_mode == UG_LABEL_LONG_EXPAND) flag |= UG_TXT_FLAG_FIT;
//    _ug_txt_get_size(&size, ext->text, font, letter_space, line_space, max_w, flag);

//    /*Set the full size in expand mode*/
//    if(ext->long_mode == UG_LABEL_LONG_EXPAND) {
//        size.x += ug_obj_get_style_pad_left(label, UG_LABEL_PART_MAIN) + ug_obj_get_style_pad_right(label, UG_LABEL_PART_MAIN);
//        size.y += ug_obj_get_style_pad_top(label, UG_LABEL_PART_MAIN) + ug_obj_get_style_pad_bottom(label, UG_LABEL_PART_MAIN);
//        ug_obj_set_size(label, size.x, size.y);
//    }
//    /*In roll mode keep the size but start offset animations*/
//    else if(ext->long_mode == UG_LABEL_LONG_SROLL) {
//#if UG_USE_ANIMATION
//        ug_anim_t a;
//        ug_anim_init(&a);
//        ug_anim_set_var(&a, label);
//        ug_anim_set_repeat_count(&a, UG_ANIM_REPEAT_INFINITE);
//        ug_anim_set_playback_delay(&a, (((ug_font_get_glyph_width(font, ' ', ' ') + letter_space) * 1000) /
//                                        ext->anim_speed) *
//                                   UG_LABEL_WAIT_CHAR_COUNT);
//        ug_anim_set_repeat_delay(&a, a.playback_delay);

//        bool hor_anim = false;
//        if(size.x > ug_area_get_width(&txt_coords)) {
//            ug_anim_set_values(&a, 0, ug_area_get_width(&txt_coords) - size.x);
//            ug_anim_set_exec_cb(&a, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//            ug_anim_set_time(&a, ug_anim_speed_to_time(ext->anim_speed, a.start, a.end));
//            ug_anim_set_playback_time(&a, a.time);

//            ug_anim_t * anim_cur = ug_anim_get(label, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//            int32_t act_time = 0;
//            bool playback_now = false;
//            if(anim_cur) {
//                act_time = anim_cur->act_time;
//                playback_now = anim_cur->playback_now;
//            }
//            if(act_time < a.time) {
//                a.act_time = act_time;      /*To keep the old position*/
//                a.early_apply = 0;
//                if(playback_now) {
//                    a.playback_now = 1;
//                    /*Swap the start and end values*/
//                    int32_t tmp;
//                    tmp      = a.start;
//                    a.start = a.end;
//                    a.end   = tmp;
//                }
//            }

//            ug_anim_start(&a);
//            hor_anim = true;
//        }
//        else {
//            /*Delete the offset animation if not required*/
//            ug_anim_del(label, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//            ext->offset.x = 0;
//        }

//        if(size.y > ug_area_get_height(&txt_coords) && hor_anim == false) {
//            ug_anim_set_values(&a, 0, ug_area_get_height(&txt_coords) - size.y - (ug_font_get_line_height(font)));
//            ug_anim_set_exec_cb(&a, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//            ug_anim_set_time(&a, ug_anim_speed_to_time(ext->anim_speed, a.start, a.end));
//            ug_anim_set_playback_time(&a, a.time);

//            ug_anim_t * anim_cur = ug_anim_get(label, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//            int32_t act_time = 0;
//            bool playback_now = false;
//            if(anim_cur) {
//                act_time = anim_cur->act_time;
//                playback_now = anim_cur->playback_now;
//            }
//            if(act_time < a.time) {
//                a.act_time = act_time;      /*To keep the old position*/
//                a.early_apply = 0;
//                if(playback_now) {
//                    a.playback_now = 1;
//                    /*Swap the start and end values*/
//                    int32_t tmp;
//                    tmp      = a.start;
//                    a.start = a.end;
//                    a.end   = tmp;
//                }
//            }

//            ug_anim_start(&a);
//        }
//        else {
//            /*Delete the offset animation if not required*/
//            ug_anim_del(label, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//            ext->offset.y = 0;
//        }
//#endif
//    }
//    /*In roll inf. mode keep the size but start offset animations*/
//    else if(ext->long_mode == UG_LABEL_LONG_SROLL_CIRC) {
//#if UG_USE_ANIMATION
//        ug_anim_t a;
//        ug_anim_init(&a);
//        ug_anim_set_var(&a, label);
//        ug_anim_set_repeat_count(&a, UG_ANIM_REPEAT_INFINITE);

//        bool hor_anim = false;
//        if(size.x > ug_area_get_width(&txt_coords)) {
//            ug_anim_set_values(&a, 0, -size.x - ug_font_get_glyph_width(font, ' ', ' ') * UG_LABEL_WAIT_CHAR_COUNT);
//            ug_anim_set_exec_cb(&a, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//            ug_anim_set_time(&a, ug_anim_speed_to_time(ext->anim_speed, a.start, a.end));

//            ug_anim_t * anim_cur = ug_anim_get(label, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//            int32_t act_time = anim_cur ? anim_cur->act_time : 0;
//            if(act_time < a.time) {
//                a.act_time = act_time;      /*To keep the old position*/
//                a.early_apply = 0;
//            }

//            ug_anim_start(&a);
//            hor_anim = true;
//        }
//        else {
//            /*Delete the offset animation if not required*/
//            ug_anim_del(label, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//            ext->offset.x = 0;
//        }

//        if(size.y > ug_area_get_height(&txt_coords) && hor_anim == false) {
//            ug_anim_set_values(&a, 0, -size.y - (ug_font_get_line_height(font)));
//            ug_anim_set_exec_cb(&a, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//            ug_anim_set_time(&a, ug_anim_speed_to_time(ext->anim_speed, a.start, a.end));

//            ug_anim_t * anim_cur = ug_anim_get(label, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//            int32_t act_time = anim_cur ? anim_cur->act_time : 0;
//            if(act_time < a.time) {
//                a.act_time = act_time;      /*To keep the old position*/
//                a.early_apply = 0;
//            }

//            ug_anim_start(&a);
//        }
//        else {
//            /*Delete the offset animation if not required*/
//            ug_anim_del(label, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//            ext->offset.y = 0;
//        }
//#endif
//    }
//    else if(ext->long_mode == UG_LABEL_LONG_DOT) {
//        if(size.y <= ug_area_get_height(&txt_coords)) { /*No dots are required, the text is short enough*/
//            ext->dot_end = UG_LABEL_DOT_END_INV;
//        }
//        else if(_ug_txt_get_encoded_length(ext->text) <= UG_LABEL_DOT_NUM) {   /*Don't turn to dots all the characters*/
//            ext->dot_end = UG_LABEL_DOT_END_INV;
//        }
//        else {
//            ug_point_t p;
//            p.x = ug_area_get_width(&txt_coords) -
//                  (ug_font_get_glyph_width(font, '.', '.') + letter_space) *
//                  UG_LABEL_DOT_NUM; /*Shrink with dots*/
//            p.y = ug_area_get_height(&txt_coords);
//            p.y -= p.y %
//                   (ug_font_get_line_height(font) + line_space); /*Round down to the last line*/
//            p.y -= line_space;                                               /*Trim the last line space*/
//            uint32_t letter_id = ug_label_get_letter_on(label, &p);


//            /*Be sure there is space for the dots*/
//            size_t txt_len = strlen(ext->text);
//            uint32_t byte_id     = _ug_txt_encoded_get_byte_id(ext->text, letter_id);
//            while(byte_id + UG_LABEL_DOT_NUM > txt_len) {
//                byte_id -= _ug_txt_encoded_size(&ext->text[byte_id]);
//                letter_id--;
//            }

//            /*Save letters under the dots and replace them with dots*/
//            uint32_t byte_id_ori = byte_id;
//            uint32_t i;
//            uint8_t len          = 0;
//            for(i = 0; i <= UG_LABEL_DOT_NUM; i++) {
//                len += _ug_txt_encoded_size(&ext->text[byte_id]);
//                _ug_txt_encoded_next(ext->text, &byte_id);
//            }

//            if(ug_label_set_dot_tmp(label, &ext->text[byte_id_ori], len)) {
//                for(i = 0; i < UG_LABEL_DOT_NUM; i++) {
//                    ext->text[byte_id_ori + i] = '.';
//                }
//                ext->text[byte_id_ori + UG_LABEL_DOT_NUM] = '\0';
//                ext->dot_end                              = letter_id + UG_LABEL_DOT_NUM;
//            }
//        }
//    }
//    /*In break mode only the height can change*/
//    else if(ext->long_mode == UG_LABEL_LONG_BREAK) {
//        size.y += ug_obj_get_style_pad_top(label, UG_LABEL_PART_MAIN) + ug_obj_get_style_pad_bottom(label, UG_LABEL_PART_MAIN);
//        ug_obj_set_height(label, size.y);
//    }
//    /*Do not set the size in Clip mode*/
//    else if(ext->long_mode == UG_LABEL_LONG_CROP) {
//        /*Do nothing*/
//    }

//    ug_obj_invalidate(label);
//}

///*=====================
// * Setter functions
// *====================*/

///**
// * Set a new text for a label. Memory will be allocated to store the text by the label.
// * @param label pointer to a label object
// * @param text '\0' terminated character string. NULL to refresh with the current text.
// */
//void ug_label_set_text(ug_obj_t * label, const char * text)
//{
//    ug_obj_invalidate(label);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);

//    /*If text is NULL then refresh */
//    if(text == NULL) {
//        ug_label_refr_text(label);
//        return;
//    }

//    if(ext->text == text && ext->static_txt == 0) {
//        /*If set its own text then reallocate it (maybe its size changed)*/
//        ext->text = ug_mem_realloc(ext->text, strlen(ext->text) + 1);
//        if(ext->text == NULL) return;
//    }
//    else {
//        /*Free the old text*/
//        if(ext->text != NULL && ext->static_txt == 0) {
//            ug_mem_free(ext->text);
//            ext->text = NULL;
//        }

//        /*Get the size of the text*/
//        size_t len = strlen(text) + 1;

//        /*Allocate space for the new text*/
//        ext->text = ug_mem_alloc(len);
//        if(ext->text == NULL) return;

//        strcpy(ext->text, text);

//        /*Now the text is dynamically allocated*/
//        ext->static_txt = 0;
//    }

//    ug_label_refr_text(label);
//}

///**
// * Set a new formatted text for a label. Memory will be allocated to store the text by the label.
// * @param label pointer to a label object
// * @param fmt `printf`-like format
// */
//void ug_label_set_text_fmt(ug_obj_t * label, const char * fmt, ...)
//{
//    ug_obj_invalidate(label);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);

//    /*If text is NULL then refresh */
//    if(fmt == NULL) {
//        ug_label_refr_text(label);
//        return;
//    }

//    if(ext->text != NULL && ext->static_txt == 0) {
//        ug_mem_free(ext->text);
//        ext->text = NULL;
//    }

//    va_list ap, ap2;
//    va_start(ap, fmt);
//    va_copy(ap2, ap);

//    /*Allocate space for the new text by using trick from C99 standard section 7.19.6.12 */
//    uint32_t len = ug_vsnprintf(NULL, 0, fmt, ap);
//    va_end(ap);


//    ext->text = ug_mem_alloc(len + 1);

//    if(ext->text == NULL) {
//        va_end(ap2);
//        return;
//    }
//    ext->text[len - 1] = 0; /* Ensure NULL termination */

//    ug_vsnprintf(ext->text, len + 1, fmt, ap2);


//    va_end(ap2);
//    ext->static_txt = 0; /*Now the text is dynamically allocated*/

//    ug_label_refr_text(label);
//}

///**
// * Set a static text. It will not be saved by the label so the 'text' variable
// * has to be 'alive' while the label exist.
// * @param label pointer to a label object
// * @param text pointer to a text. NULL to refresh with the current text.
// */
//void ug_label_set_text_static(ug_obj_t * label, const char * text)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);
//    if(ext->static_txt == 0 && ext->text != NULL) {
//        ug_mem_free(ext->text);
//        ext->text = NULL;
//    }

//    if(text != NULL) {
//        ext->static_txt = 1;
//        ext->text       = (char *)text;
//    }

//    ug_label_refr_text(label);
//}

///**
// * Set the behavior of the label with longer text then the object size
// * @param label pointer to a label object
// * @param long_mode the new mode from 'ug_label_long_mode' enum.
// *                  In ug_LONG_BREAK/LONG/ROLL the size of the label should be set AFTER this
// * function
// */
//void ug_label_set_long_mode(ug_obj_t * label, ug_label_long_mode_t long_mode)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);

//#if ug_USE_ANIMATION
//    /*Delete the old animation (if exists)*/
//    ug_anim_del(label, (ug_anim_exec_xcb_t)ug_obj_set_x);
//    ug_anim_del(label, (ug_anim_exec_xcb_t)ug_obj_set_y);
//    ug_anim_del(label, (ug_anim_exec_xcb_t)ug_label_set_offset_x);
//    ug_anim_del(label, (ug_anim_exec_xcb_t)ug_label_set_offset_y);
//#endif
//    ext->offset.x = 0;
//    ext->offset.y = 0;

//    if(long_mode == ug_LABEL_LONG_SROLL || long_mode == ug_LABEL_LONG_SROLL_CIRC || long_mode == ug_LABEL_LONG_CROP)
//        ext->expand = 1;
//    else
//        ext->expand = 0;

//    /*Restore the character under the dots*/
//    if(ext->long_mode == ug_LABEL_LONG_DOT && ext->dot_end != ug_LABEL_DOT_END_INV) {
//        ug_label_revert_dots(label);
//    }

//    ext->long_mode = long_mode;
//    ug_label_refr_text(label);
//}

///**
// * Set the align of the label (left or center)
// * @param label pointer to a label object
// * @param align 'ug_LABEL_ALIGN_LEFT' or 'ug_LABEL_ALIGN_LEFT'
// */
//void ug_label_set_align(ug_obj_t * label, ug_label_align_t align)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);
//    if(ext->align == align) return;

//    ext->align = align;

//    ug_obj_invalidate(label); /*Enough to invalidate because alignment is only drawing related
//                                 (ug_refr_label_text() not required)*/
//}

///**
// * Enable the recoloring by in-line commands
// * @param label pointer to a label object
// * @param en true: enable recoloring, false: disable
// */
//void ug_label_set_recolor(ug_obj_t * label, bool en)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);
//    if(ext->recolor == en) return;

//    ext->recolor = en == false ? 0 : 1;

//    ug_label_refr_text(label); /*Refresh the text because the potential color codes in text needs to
//                                  be hided or revealed*/
//}



///*=====================
// * Getter functions
// *====================*/

///**
// * Get the text of a label
// * @param label pointer to a label object
// * @return the text of the label
// */
//char * ug_label_get_text(const ug_obj_t * label)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);

//    return ext->text;
//}

///**
// * Get the long mode of a label
// * @param label pointer to a label object
// * @return the long mode
// */
//ug_label_long_mode_t ug_label_get_long_mode(const ug_obj_t * label)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);
//    return ext->long_mode;
//}

///**
// * Get the align attribute
// * @param label pointer to a label object
// * @return ug_LABEL_ALIGN_LEFT or ug_LABEL_ALIGN_CENTER
// */
//ug_label_align_t ug_label_get_align(const ug_obj_t * label)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);

//    ug_label_align_t align = ext->align;

//    if(align == ug_LABEL_ALIGN_AUTO) {
//#if ug_USE_BIDI
//        ug_bidi_dir_t base_dir = ug_obj_get_base_dir(label);
//        if(base_dir == ug_BIDI_DIR_AUTO) base_dir = _ug_bidi_detect_base_dir(ext->text);

//        if(base_dir == ug_BIDI_DIR_LTR) align = ug_LABEL_ALIGN_LEFT;
//        else if(base_dir == ug_BIDI_DIR_RTL) align = ug_LABEL_ALIGN_RIGHT;
//#else
//        align = ug_LABEL_ALIGN_LEFT;
//#endif
//    }

//    return align;
//}

///**
// * Get the recoloring attribute
// * @param label pointer to a label object
// * @return true: recoloring is enabled, false: disable
// */
//bool ug_label_get_recolor(const ug_obj_t * label)
//{
//    ug_ASSERT_OBJ(label, ug_OBJX_NAME);

//    ug_label_ext_t * ext = ug_obj_get_ext_attr(label);
//    return ext->recolor == 0 ? false : true;
//}




















//#endif