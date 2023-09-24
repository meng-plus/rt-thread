/**
 * @file CLedTime.cpp
 * @author §è§Ú§è§Úplus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "LedTime.hpp"

#include "product_def.h"
#include "CylinderTime.hpp"
#define LOG_TAG "dev.led"
#define LOG_LVL LOG_LVL_DBG
#include "ulog.h"

CLedTime::CLedTime(/* args */)
    : osTime("LedTime", RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER, NULL, 500),
      m_run(GET_PIN(A, 3)),
      m_warning(GET_PIN(C, 13)),
      m_err(GET_PIN(B, 9))
{
    start();
}

CLedTime::~CLedTime()
{
}
void CLedTime::Tick()
{
    m_run.Toggle();
}
void CLedTime::Update(const OHOS::Observable *o, const OHOS::ObserverArg *arg)
{
    if (o == 0 || arg == 0)
    {
        return;
    }
    if (o == &CylinderTime::GetInstance())
    {
        if (arg != NULL)
        {
            const ObsertverCyVal *cyVal_ptr = (const ObsertverCyVal *)arg;
            LOG_E("%s:%d st:%d timeOut error  ", cyVal_ptr->Cylinder->m_name, cyVal_ptr->timeout, cyVal_ptr->status);
        }
        else
        {
            LOG_E("updata NULL");
        }
    }
}
