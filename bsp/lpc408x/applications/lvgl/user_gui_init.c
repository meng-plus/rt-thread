#include <lvgl.h>

#if defined(PKG_USING_LVGL_SQUARELINE)
void ui_init()
#else
void lv_user_gui_init()
#endif
{
   lv_obj_t *label = lv_label_create(lv_scr_act());
   lv_label_set_text(label, "Hello lvgl!");
   lv_obj_set_pos(label, 100, 10);
   void lv_demo_music();
   lv_demo_music();
}
