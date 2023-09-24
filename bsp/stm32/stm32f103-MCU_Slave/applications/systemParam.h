/**
 * @file systemParam.h
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 系统运行参数
 * @version 0.1
 * @date 2023-09-17
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SYSTEM_PARAM_H_
#define __SYSTEM_PARAM_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum __TASK_STATE
    {
        TASK_WAIT_START, /*!< 等待启动 */
        TASK_STEP_1,
        TASK_STEP_2,
        TASK_STEP_3,
        TASK_STEP_4,
        TASK_STEP_COMP, /*!< 任务完成 */
        TASK_STEP_ERR,  /*!< 报错 */
    } task_state_e;

    typedef struct __RAM_DATA
    {
        uint16_t input_data; /*!< 输入信号 */
        uint8_t out_step;    /*!< 输出步骤[0,3] 4完成 */
        uint8_t motor_ctrl;  /*!<  0:idle 1:checkself 2:auto 3:error  */
    } systemParam_t;

    extern systemParam_t g_sysRam;

#ifdef __cplusplus
}
#endif
#endif //__SYSTEM_PARAM_H_
