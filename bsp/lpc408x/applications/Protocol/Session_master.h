/**
 * @file Session_master.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 主机管理层
 * @version 0.1
 * @date 2023-10-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __SESSION_MASTER_H_
#define __SESSION_MASTER_H_

#include <stdint.h>
#include <stdbool.h>
#include "Transport.h"
#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct __SESSION_MASTER session_master_t;
    typedef struct SESSION_MSG
    {
        uint32_t flag;                                                                 /*!< 状态标识 */
        uint32_t tick;                                                                 /*!< 记录请求时间戳 */
        int32_t (*request)(session_master_t *se_handle, uint8_t *buff, uint16_t len);  /*!< 请求报文 */
        int32_t (*response)(session_master_t *se_handle, uint8_t *buff, uint16_t len); /*!< 响应报文 */
    } session_msg_t;

   struct __SESSION_MASTER
    {
        transport_t transport;
        uint32_t msg_id;

    } ;

#ifdef __cplusplus
}
#endif

#endif
