#include <rtthread.h>
#include <rtdevice.h>
#include "CommLora.h"
#include "drv_pin.h"
void CommLoraInit(void);
static rt_timer_t timer_lora = NULL;
void lora_DIO1_isr(void *args)
{
    void LORA1_DIO1_update_flag(uint8_t idx, uint8_t flag);
    LORA1_DIO1_update_flag(0, 1);
}
void task_lora(void *param)
{
    CommLoraHandler();
}
int drv_lora_init()
{
    CommLoraInit(); /*!<  lora irq P0.6 */
    rt_pin_attach_irq(GET_PIN(0, 6), PIN_IRQ_MODE_RISING,
                      lora_DIO1_isr, 0);

    rt_pin_irq_enable(GET_PIN(0, 6), 1);
    timer_lora = rt_timer_create("task.lora",
                                 task_lora,
                                 NULL,
                                 500,
                                 RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    return 0;
}
INIT_APP_EXPORT(drv_lora_init);
