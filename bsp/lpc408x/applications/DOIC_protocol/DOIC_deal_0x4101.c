#include "DOIC_map.h"
#include "ipc/ringbuffer.h"
#include "Session_master.h"
#include "thread_DOIC.h"
struct rt_ringbuffer *rb_sdc = NULL;
extern thread_doic_t *pthread_doic;
int8_t DOIC_deal_0x4101(doic_data_t *pdata, doic_data_t *pdst)
{
    if (NULL == rb_sdc)
        rb_sdc = rt_ringbuffer_create(512);

    if (pdata->data[0])
    {
        rt_ringbuffer_put(rb_sdc, pdata->data + 1, pdata->len - 1);
    }

    /*!< 透传数据的处理过程 */
    { /*!< SDC 数据处理 */
        session_master_t *pSDCSession = &pthread_doic->m_DSCsession;
        transport_t *pSDCTransport = &pSDCSession->transport;
        sdc_master_t *phander_sdc = &pthread_doic->m_SDC_master;
        TR_CHECK_RES_E res;
        if (pSDCTransport && pSDCTransport->waiting_response)
        {
            res = pSDCTransport->waiting_response(pSDCTransport);
            if (TR_CHECK_FRAME == res)
            {
                SDC_deal(phander_sdc, (sdc_data_t *)pSDCTransport->rxBuff);
            }
        }
    }
    return 0;
}
