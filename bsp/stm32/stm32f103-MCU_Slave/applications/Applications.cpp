/**
 * @file Applications.cpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Applications.hpp"
#ifdef __cplusplus
extern "C"
{
#endif
    int Applications_GetInstance()
    {
        CApplications::GetInstance();
        return 0;
    }
#ifdef __cplusplus
}
#endif
