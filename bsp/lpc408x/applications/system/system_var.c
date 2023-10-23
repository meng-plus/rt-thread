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

product_param_t g_prod_param;
extern const product_param_t l_prod_param_init;
const var_init_t g_var_init;
varWork_t g_var_work;

extern struct fdb_kvdb kj428_kvdb;
#define DB_NAME_KJ428Z_B "kj428-Z_B"

/**
 * @brief 升级参数配置
 *
 * @param id 参数id
 * @param currver 当前存储的参数ID号
 */
static void SysArgUpdata(uint16_t id, uint16_t currver);

static int32_t systeminfo_init()
{

    struct fdb_blob blob;
    size_t len = fdb_kv_get_blob(&kj428_kvdb, DB_NAME_KJ428Z_B,
                                 fdb_blob_make(&blob, &g_prod_param, sizeof(g_prod_param)));
    if (!len)
    {
        g_prod_param = l_prod_param_init;
        return -1;
    }

    if (FLASH_PARAMETER_VER != g_prod_param.ver)
    {
        SysArgUpdata(0, g_prod_param.ver);
    }
    return 0;
}
INIT_ENV_EXPORT(systeminfo_init);

void SysArgUpdata(uint16_t id, uint16_t currver)
{
    if (id == 0)
    {
        if (currver < 1)
        {
            g_prod_param = l_prod_param_init;
        }
    }
}
const product_param_t l_prod_param_init =
    {
        .ver = FLASH_PARAMETER_VER,
};
