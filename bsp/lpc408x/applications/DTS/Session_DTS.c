/**
 * @file Session_DTS.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-10-20
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Session_DTS.h"

#define LOG_TAG __FILE__
#define LOG_LVL LOG_LVL_DBG
#include "rtdbg.h"

int32_t dts_sys_req(session_master_t *se_handle, uint8_t *buff, uint16_t len)
{
    rt_thread_mdelay(1000);
    agile_modbus_t *ctx = &((thread_dts_t *)se_handle)->ctx_rtu._ctx;
    int32_t send_len = agile_modbus_serialize_read_registers(ctx, 1, 2);
    rt_device_write(pthread_dts->device, 0, ctx->send_buf, send_len);
    return 0;
}
int32_t dts_sys_res(session_master_t *se_handle, uint8_t *buff, uint16_t len)
{    
    thread_dts_t *pDts = (thread_dts_t *)se_handle;
    agile_modbus_t *ctx = &((thread_dts_t *)se_handle)->ctx_rtu._ctx;
    int32_t rc = agile_modbus_deserialize_read_registers(ctx, len, (uint16_t *)&pthread_dts->data.system);
    if (rc < 0)
    {
        LOG_W("Receive failed.");
        if (rc != -1)
            LOG_W("Error code:%d", -128 - rc);
    }
    if (pDts->data.system.status)
    {
        se_handle->msg_id = DTS_CHN;
    }
    return 0;
}
int32_t dts_chn_req(session_master_t *se_handle, uint8_t *buff, uint16_t len)
{    
    thread_dts_t *pdts = (thread_dts_t *)se_handle;
    agile_modbus_t *ctx = &pdts->ctx_rtu._ctx;
    if (pdts->sel_chn < pdts->data.system.chn_num && pdts->sel_chn < DTS_CHANNEL_NUM)
    {
        int32_t send_len = agile_modbus_serialize_read_registers(ctx, 21 + (pdts->sel_chn * 8), sizeof(dts_channel_t) / 2);
        rt_device_write(pthread_dts->device, 0, ctx->send_buf, send_len);
    }
    return 0;
}
int32_t dts_chn_res(session_master_t *se_handle, uint8_t *buff, uint16_t len)
{
    thread_dts_t *pdts = (thread_dts_t *)se_handle;
    agile_modbus_t *ctx = &pdts->ctx_rtu._ctx;
    if (pdts->sel_chn < pdts->data.system.chn_num && pdts->sel_chn < DTS_CHANNEL_NUM)
    {
        int32_t rc = agile_modbus_deserialize_read_registers(ctx, len, (uint16_t *)&pthread_dts->data.channel[pdts->sel_chn]);
        if (rc < 0)
        {
            LOG_W("Receive failed.");
            if (rc != -1)
                LOG_W("Error code:%d", -128 - rc);
            se_handle->msg_id = DTS_SYS;
        }
    }

    se_handle->msg_id = DTS_PART;
    return 0;
}
int32_t dts_part_req(session_master_t *se_handle, uint8_t *buff, uint16_t len)
{
    thread_dts_t *pdts = (thread_dts_t *)se_handle;
    agile_modbus_t *ctx = &pdts->ctx_rtu._ctx;
    if (pdts->sel_chn < pdts->data.system.chn_num && pdts->sel_chn < DTS_CHANNEL_NUM)
    {
        int32_t send_len = agile_modbus_serialize_read_registers(ctx, 200 + (pdts->sel_chn * 1000), sizeof(dts_chn_Data_t) / 2);
        rt_device_write(pdts->device, 0, ctx->send_buf, send_len);
    }
    return 0;
}
int32_t dts_part_res(session_master_t *se_handle, uint8_t *buff, uint16_t len)
{
    thread_dts_t *pdts = (thread_dts_t *)se_handle;
    agile_modbus_t *ctx = &pdts->ctx_rtu._ctx;
    if (pdts->sel_chn < pdts->data.system.chn_num && pdts->sel_chn < DTS_CHANNEL_NUM)
    {
        int32_t rc = agile_modbus_deserialize_read_registers(ctx, len, (uint16_t *)&pdts->data.partition[pdts->sel_chn]);
        if (rc < 0)
        {
            LOG_W("Receive failed.");
            if (rc != -1)
                LOG_W("Error code:%d", -128 - rc);
            se_handle->msg_id = DTS_SYS;
        }
    }
    return 0;
}
const session_msg_t dts_msg_array[DTS_MSG_NUM] =
    {
        [DTS_SYS] = {.request = dts_sys_req, .response = dts_sys_res},    /*!< 获取设备信息 */
        [DTS_CHN] = {.request = dts_chn_req, .response = dts_chn_res},    /*!< 读取通道信息 */
        [DTS_PART] = {.request = dts_part_req, .response = dts_part_res}, /*!< 读取分区数据 */
};

extern TR_CHECK_RES_E waiting_response(transport_t *pTr); /*!< 等待帧数据 */
void session_dts_init(session_master_t *se_handle)
{
    if (NULL == se_handle)
    {
        return;
    }
    memset(se_handle, 0, sizeof(session_master_t));
    agile_modbus_t *ctx = &((thread_dts_t *)se_handle)->ctx_rtu._ctx;
    tr_init(&se_handle->transport, ctx->read_buf, ctx->read_bufsz);
    tr_control(&se_handle->transport, TR_SET_WAITING_RESPONSE, (void *)waiting_response);
}
int32_t session_dts_tick(session_master_t *se_handle)
{
    session_dts_request(se_handle, se_handle->msg_id);
    TR_CHECK_RES_E res = se_handle->transport.waiting_response(&se_handle->transport);
    if (res == TR_CHECK_FRAME)
    {
        session_dts_response(se_handle, se_handle->msg_id);
    }
    return res;
}
int32_t session_dts_request(session_master_t *se_handle, DTS_MSG_ID_E id)
{
    dts_msg_array[id].request(se_handle, NULL, NULL);
    return 0;
}
int32_t session_dts_response(session_master_t *se_handle, DTS_MSG_ID_E id)
{
    dts_msg_array[id].response(se_handle, NULL, se_handle->transport.read_len);
    return 0;
}
