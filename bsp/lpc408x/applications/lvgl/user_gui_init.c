#include <lvgl.h>
#define LOG_TAG              __FILE__
#define LOG_LVL              LOG_LVL_DBG
#include "ulog.h"

void lv_demo_keypad_encoder(void);
void lv_demo_music();
#if defined(PKG_USING_LVGL_SQUARELINE)
void ui_init()
#else
void lv_user_gui_init()
#endif
{
   lv_obj_t *label = lv_label_create(lv_scr_act());
   lv_label_set_text(label, "Hello lvgl!");
   lv_obj_set_pos(label, 100, 10);
   lv_demo_keypad_encoder();
}

#ifdef RT_USING_FINSH
#include <finsh.h>

void lvgl_cmd(int argc, char *argv[])
{
   if (argc < 1)
   {
     LOG_RAW("eg: lvgl 0");
      return;
   }
   if (0 == rt_strcmp("0", argv[1]))
   {
      lv_demo_music();
   }
   else if (0 == rt_strcmp("1", argv[1]))
   {
      lv_demo_keypad_encoder();
   }
}
MSH_CMD_EXPORT_ALIAS(lvgl_cmd, lvgl, lvgl_demo)
#endif
