/**
 * @file spinbox_ex.h
 *
 */

#ifndef SPINBOX_EX_H
#define SPINBOX_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../../lvgl.h"


#define LV_USE_SPINBOX_EX 1
#if LV_USE_SPINBOX_EX

/*Testing of dependencies*/
#if LV_USE_TEXTAREA == 0
#error "spinbox_ex: lv_ta is required. Enable it in lv_conf.h (LV_USE_TEXTAREA  1) "
#endif

/*********************
 *      DEFINES
 *********************/
#define SPINBOX_EX_MAX_DIGIT_COUNT 10

/**********************
 *      TYPEDEFS
 **********************/

/*Data of passwd*/
typedef struct {
    lv_textarea_t ta;   /*Ext. of ancestor*/
    /*New data for this type*/
    int32_t value;
    int32_t range_max;
    int32_t range_min;
    int32_t step;
    uint16_t digit_count : 4;
    uint16_t dec_point_pos : 4; /*if 0, there is no separator and the number is an integer*/
    uint16_t rollover : 1;   // Set to true for rollover functionality
    uint16_t digit_step_dir : 2; // the direction the digit will step on encoder button press when editing
} spinbox_ex_t;

extern const lv_obj_class_t spinbox_ex_class;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**
 * Create a Passwd object
 * @param parent pointer to an object, it will be the parent of the new passwd
 * @return pointer to the created passwd
 */
lv_obj_t * spinbox_ex_create(lv_obj_t * parent);

/*=====================
 * Setter functions
 *====================*/

/**
 * Set passwd value
 * @param obj pointer to passwd
 * @param i value to be set
 */
void spinbox_ex_set_value(lv_obj_t * obj, int32_t i);

/**
 * Set passwd rollover function
 * @param obj pointer to passwd
 * @param b true or false to enable or disable (default)
 */
void spinbox_ex_set_rollover(lv_obj_t * obj, bool b);

/**
 * Set passwd digit format (digit count and decimal format)
 * @param obj pointer to passwd
 * @param digit_count number of digit excluding the decimal separator and the sign
 * @param separator_position number of digit before the decimal point. If 0, decimal point is not
 * shown
 */
void spinbox_ex_set_digit_format(lv_obj_t * obj, uint8_t digit_count, uint8_t separator_position);

/**
 * Set passwd step
 * @param obj pointer to passwd
 * @param step steps on increment/decrement. Can be 1, 10, 100, 1000, etc the digit that will change.
 */
void spinbox_ex_set_step(lv_obj_t * obj, uint32_t step);

/**
 * Set passwd value range
 * @param obj pointer to passwd
 * @param range_min maximum value, inclusive
 * @param range_max minimum value, inclusive
 */
void spinbox_ex_set_range(lv_obj_t * obj, int32_t range_min, int32_t range_max);

/**
 * Set cursor position to a specific digit for edition
 * @param obj pointer to passwd
 * @param pos selected position in passwd
 */
void spinbox_ex_set_cursor_pos(lv_obj_t * obj, uint8_t pos);

/**
 * Set direction of digit step when clicking an encoder button while in editing mode
 * @param obj pointer to passwd
 * @param direction the direction (LV_DIR_RIGHT or LV_DIR_LEFT)
 */
void spinbox_ex_set_digit_step_direction(lv_obj_t * obj, lv_dir_t direction);

/*=====================
 * Getter functions
 *====================*/

/**
 * Get passwd rollover function status
 * @param obj pointer to passwd
 */
bool spinbox_ex_get_rollover(lv_obj_t * obj);

/**
 * Get the passwd numeral value (user has to convert to float according to its digit format)
 * @param obj pointer to passwd
 * @return value integer value of the passwd
 */
int32_t spinbox_ex_get_value(lv_obj_t * obj);

/**
 * Get the passwd step value (user has to convert to float according to its digit format)
 * @param obj pointer to passwd
 * @return value integer step value of the passwd
 */
int32_t spinbox_ex_get_step(lv_obj_t * obj);

/*=====================
 * Other functions
 *====================*/

/**
 * Select next lower digit for edition by dividing the step by 10
 * @param obj pointer to passwd
 */
void spinbox_ex_step_next(lv_obj_t * obj);

/**
 * Select next higher digit for edition by multiplying the step by 10
 * @param obj pointer to passwd
 */
void spinbox_ex_step_prev(lv_obj_t * obj);

/**
 * Increment passwd value by one step
 * @param obj pointer to passwd
 */
void spinbox_ex_increment(lv_obj_t * obj);

/**
 * Decrement passwd value by one step
 * @param obj pointer to passwd
 */
void spinbox_ex_decrement(lv_obj_t * obj);

/**********************
 *      MACROS
 **********************/

/* It was ambiguous in MicroPython. See https://github.com/lvgl/lvgl/issues/3301
 * TODO remove in v9*/
#define spinbox_ex_set_pos spinbox_ex_set_cursor_pos

#endif /*LV_USE_SPINBOX_EX*/

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif /*SPINBOX_EX_H*/
