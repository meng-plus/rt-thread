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
#include <stdlib.h>
varWork_t g_var_work;
product_param_t g_prod_param;
sensor_param_t g_sensor_param;
const product_param_t l_prod_param_init =
    {
        .header = {
            .ver = FLASH_PRO_PARAMETER_VER,
        },
};

const var_init_t g_var_init = {
    .product_name = "矿用本安型激光火情监控主机",
    .dts_delayms_max = 10000,
};

screen_param_t g_screen_param = {
    .header = {
        .ver = FLASH_SCREEN_PARAMETER_VER,
    },
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
enum FLASH_PARAM_ID
{
    PROD_PARAM_ID,
    SCREEN_PARAM_ID,
    SENSOR_PARAM_ID,
};
static const flash_parameter_t param_table[] = {
    {"kj428-Z_B", &g_prod_param, sizeof(g_prod_param)},
    {"screen", &g_screen_param, sizeof(g_screen_param)},
    {"device", &g_sensor_param, sizeof(g_sensor_param)},
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
        currver = g_prod_param.header.ver;
        if (FLASH_PRO_PARAMETER_VER != g_prod_param.header.ver)
        {
            if (currver < 1)
            {
                g_prod_param = l_prod_param_init;
            }

            g_prod_param.header.ver = FLASH_PRO_PARAMETER_VER;
        }
    }
    if (param == &g_screen_param)
    {
        currver = g_screen_param.header.ver;
        if (FLASH_SCREEN_PARAMETER_VER != g_screen_param.header.ver)
        {
            if (currver < 1)
            {
                g_screen_param.touch = 0;
            }
            g_screen_param.header.ver = FLASH_PRO_PARAMETER_VER;
        }
    }
    if (param == &g_sensor_param)
    {
        currver = g_sensor_param.header.ver;
        struct fdb_blob blob;
        /*!< read sensor config */
        size_t len = fdb_kv_get_blob(&kj428_kvdb, "sensor",
                              fdb_blob_make(&blob, NULL, NULL));
        if (blob.saved.len)
        {
            g_sensor_param.psen_config = malloc(blob.saved.len);
            len = fdb_kv_get_blob(&kj428_kvdb, "sensor",
                                  fdb_blob_make(&blob, g_sensor_param.psen_config, blob.saved.len));
        }
        else
        {
            g_sensor_param.psen_config = 0;
        }
        if (FLASH_SENSOR_PARAMETER_VER != g_sensor_param.header.ver)
        {
            if (currver < 1)
            {
                device_config_t config_init = {
                    .type = 1,
                    .en = 1,
                    .baud = 115200,
                };
                g_sensor_param.dev_config[0] = config_init;
                g_sensor_param.dev_config[1] = config_init;
            }

            g_sensor_param.header.ver = FLASH_SENSOR_PARAMETER_VER;
        }
    }
}

uint8_t var_save(void *param)
{
    fdb_err_t err;
    struct fdb_blob blob;

    if (param == &g_prod_param)
    {
        g_prod_param.header.ver = FLASH_PRO_PARAMETER_VER;
        fdb_blob_make(&blob, &g_prod_param, sizeof(g_prod_param));
        err = fdb_kv_set_blob(&kj428_kvdb, param_table[PROD_PARAM_ID].name, &blob);
    }
    else if (param == &g_screen_param)
    {
        g_screen_param.header.ver = FLASH_PRO_PARAMETER_VER;
        fdb_blob_make(&blob, &g_screen_param, sizeof(g_prod_param));
        err = fdb_kv_set_blob(&kj428_kvdb, param_table[SCREEN_PARAM_ID].name, &blob);
    }
    else if (param == &g_sensor_param)
    {

        fdb_blob_make(&blob, g_sensor_param.psen_config, g_sensor_param.sensor_num * sizeof(sensor_config_t));
        err = fdb_kv_set_blob(&kj428_kvdb, "sensor", &blob);
        if (g_sensor_param.psen_config && err != FDB_NO_ERR)
            return err;

        g_sensor_param.header.ver = FLASH_SENSOR_PARAMETER_VER;
        fdb_blob_make(&blob, &g_sensor_param, sizeof(sensor_param_t));

        err = fdb_kv_set_blob(&kj428_kvdb, param_table[SENSOR_PARAM_ID].name, &blob);
    }

    return err;
}

uint8_t var_reload(void *param)
{
    struct fdb_blob blob;
    for (size_t i = 0; i < sizeof(param_table) / sizeof(param_table[0]); i++)
    {
        const flash_parameter_t *p_param = &param_table[i];
        if (param == p_param->param)
        {
            size_t len = fdb_kv_get_blob(&kj428_kvdb, p_param->name,
                                         fdb_blob_make(&blob, p_param->param, p_param->len));
            SysArgUpdata(p_param->param);
            return 0;
        }
    }
    return 0;
}

uint8_t var_resetting(void *param)
{
    struct fdb_blob blob;
    for (size_t i = 0; i < sizeof(param_table) / sizeof(param_table[0]); i++)
    {
        const flash_parameter_t *p_param = &param_table[i];
        if (param == p_param->param)
        {
            size_t len = fdb_kv_set_blob(&kj428_kvdb, p_param->name,
                                         fdb_blob_make(&blob, p_param->param, p_param->len));
            memset(p_param->param, 0, p_param->len);
            SysArgUpdata(p_param->param);
            return 0;
        }
    }
    return 0;
}
