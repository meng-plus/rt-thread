/**
 * @file Session.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-20
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Session_master.h"
#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

const session_msg_t msg_array[1];

extern TR_CHECK_RES_E waiting_response(transport_t *pTr); /*!< 等待帧数据 */
void session_change(session_master_t *se_handle, uint8_t id);
int32_t session_response(session_master_t *se_handle, uint8_t id);
int32_t session_request(session_master_t *se_handle, uint8_t id);
int32_t session_tick(session_master_t *se_handle)
{
    session_request(se_handle, se_handle->msg_id);
    TR_CHECK_RES_E res = se_handle->transport.waiting_response(&se_handle->transport);
    if (res == TR_CHECK_FRAME)
    {
        if (0 == session_response(se_handle, se_handle->msg_id))
        {
            //session_change(se_handle, SYS);
        }
    }
    else
    {
        //session_change(se_handle, SYS);
    }
    return res;
}
void session_change(session_master_t *se_handle, uint8_t id)
{
    se_handle->msg_id = id;
}

int32_t session_request(session_master_t *se_handle, uint8_t id)
{
    msg_array[id].request(se_handle, NULL, NULL);
    return 0;
}
int32_t session_response(session_master_t *se_handle, uint8_t id)
{
    return msg_array[id].response(se_handle, NULL, se_handle->transport.read_len);
}
void session_master_init(session_master_t *se_handle)
{
    if (NULL == se_handle)
    {
        return;
    }
    memset(se_handle, 0, sizeof(session_master_t));
    tr_init(&se_handle->transport, NULL, 0);
    // tr_control(&se_handle->transport, TR_SET_WAITING_RESPONSE, (void *)waiting_response);
}
