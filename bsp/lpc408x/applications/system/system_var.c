/**
 * @file systeminfo.c
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 全局系统参数管理
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "system_var.h"
#include "flashdb.h"

varWork_t g_var_work;
product_param_t g_prod_param;
extern const product_param_t l_prod_param_init;
const var_init_t g_var_init = {
    .product_name = "矿用本安型激光火情监控主机",
    .dts_delayms_max = 10000,
};

screen_param_t g_screen_param = {
    /*!< touch  */
    .min_raw_x = 200,
    .min_raw_y = 600,
    .max_raw_x = 3800,
    .max_raw_y = 3500,
};
extern struct fdb_kvdb kj428_kvdb;

typedef struct _FLASH_PARAMETER
{
    const char *name;
    void *param;
    size_t len;
} flash_parameter_t;

static const flash_parameter_t param_table[] = {
    {"kj428-Z_B", &g_prod_param, sizeof(g_prod_param)},
    {"screen", &g_screen_param, sizeof(g_screen_param)},
};
/**
 * @brief 升级参数配置
 *
 * @param id 参数id
 * @param currver 当前存储的参数ID号
 */
static void SysArgUpdata(void *);

static int32_t systeminfo_init()
{

    struct fdb_blob blob;
    for (size_t i = 0; i < sizeof(param_table) / sizeof(param_table[0]); i++)
    {
        const flash_parameter_t *p_param = &param_table[i];
        size_t len = fdb_kv_get_blob(&kj428_kvdb, p_param->name,
                                     fdb_blob_make(&blob, p_param->param, p_param->len));
        if (!len)
        {
            g_prod_param = l_prod_param_init;
            return -1;
        }

        SysArgUpdata(p_param->param);
    }

    return 0;
}
INIT_ENV_EXPORT(systeminfo_init);

void SysArgUpdata(void *param)
{
    uint16_t currver;
    if (param == &g_prod_param)
    {
        currver = g_prod_param.ver;
        if (FLASH_PRO_PARAMETER_VER != g_prod_param.ver)
        {
            if (currver < 1)
            {
                g_prod_param = l_prod_param_init;
            }

            g_prod_param.ver = FLASH_PRO_PARAMETER_VER;
        }
    }
    if (param == &g_screen_param)
    {
        if (FLASH_SCREEN_PARAMETER_VER != g_screen_param.ver)
        {
            if (currver < 1)
            {
                g_screen_param.touch = 0;
            }
            g_prod_param.ver = FLASH_PRO_PARAMETER_VER;
        }
    }
}
const product_param_t l_prod_param_init =
    {
        .ver = FLASH_PRO_PARAMETER_VER,
};
