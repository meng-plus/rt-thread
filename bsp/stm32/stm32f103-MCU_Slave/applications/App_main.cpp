#include "Applications.hpp"
#include <rtthread.h>
#ifdef __cplusplus
extern "C"
{
#endif
    int Applications_GetInstance()
    {
        CApplications::GetInstance();
        return 0;
    }
    INIT_APP_EXPORT(Applications_GetInstance);
#ifdef __cplusplus
}
#endif
