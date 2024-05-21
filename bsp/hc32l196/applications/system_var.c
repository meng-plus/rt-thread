
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
#include "fal.h"
#include "flashdb.h"
#include "system_upload.h"
#include <stdlib.h>
#include <string.h>
#include "board.h"

#define BSP_USRING_FAL_NAME "kvdb"
#define KV_PART_NAME        BSP_USRING_FAL_NAME //"eeprom1"
#define KV_NAME             BSP_USRING_FAL_NAME

extern struct system_var g_systemVar;
struct fdb_kvdb fdb_kvdb1;

static struct fdb_default_kv_node default_kv_set[] = {
    {   "iap_need_copy_app", "0"},
    {"iap_need_crc32_check", "0"},
    {   "iap_copy_app_size", "0"},
    {  "stop_in_bootloader", "0"},
};

static int32_t systeminfo_init();
static int kj428_fdb_kvdb_init(void);
static int system_var_init(void)
{
    fal_init();
    kj428_fdb_kvdb_init();
    systeminfo_init();
    return 0;
}
INIT_ENV_EXPORT(system_var_init);


static int kj428_fdb_kvdb_init(void)
{
    struct fdb_default_kv default_kv;
#ifndef FDB_USING_FAL_MODE
    uint32_t sec_size = 1024, db_size = sec_size * 32;
    rt_bool_t file_mode = true;
#endif

    default_kv.kvs = default_kv_set;
    default_kv.num = sizeof(default_kv_set) / sizeof(default_kv_set[0]);

#ifndef FDB_USING_FAL_MODE
    fdb_kvdb_control(&(fdb_kvdb1), FDB_KVDB_CTRL_SET_SEC_SIZE, &sec_size);
    fdb_kvdb_control(&(fdb_kvdb1), FDB_KVDB_CTRL_SET_FILE_MODE, &file_mode);
    fdb_kvdb_control(&(fdb_kvdb1), FDB_KVDB_CTRL_SET_MAX_SIZE, &db_size);
#endif
    if (FDB_NO_ERR == fdb_kvdb_init(&fdb_kvdb1, KV_NAME, KV_PART_NAME, &default_kv, NULL))
    {
        fdb_kv_print(&fdb_kvdb1);
    }
    else
    {
        rt_kprintf("kvdb init failed %s\r\n", KV_NAME);
    }


    return 0;
}


varWork_t g_var_work;

product_param_t g_prod_param;

const product_param_t l_prod_param_init =
    {
        .header = {
                   .ver = FLASH_PRO_PARAMETER_VER,
                   },
        .ad5161_value = 0, /*!< 配置数据 */
        .dac_value    = 0, /*!< 配置数据 */
};
const var_init_t g_var_init = {
    .product_name = "laser_mode",

};

typedef struct _FLASH_PARAMETER
{
    const char *name;
    void *param;
    size_t len;
} flash_parameter_t;
enum FLASH_PARAM_ID
{
    PROD_PARAM_ID,
};
static const flash_parameter_t param_table[] = {
    {"ir", &g_prod_param, sizeof(g_prod_param)},
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
        if (fdb_kvdb1.parent.init_ok)
        {
            size_t len = fdb_kv_get_blob(&fdb_kvdb1, p_param->name,
                                         fdb_blob_make(&blob, p_param->param, p_param->len));
        }

        SysArgUpdata(p_param->param);
    }
    system_upload(SYSTEM_UPLOAD_ALL);
    return 0;
}

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
}

uint8_t var_save(void *param)
{
    fdb_err_t err;
    struct fdb_blob blob;

    if (param == &g_prod_param)
    {
        g_prod_param.header.ver = FLASH_PRO_PARAMETER_VER;
        fdb_blob_make(&blob, &g_prod_param, sizeof(g_prod_param));
        err = fdb_kv_set_blob(&fdb_kvdb1, param_table[PROD_PARAM_ID].name, &blob);
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
            size_t len = fdb_kv_get_blob(&fdb_kvdb1, p_param->name,
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
            size_t len = fdb_kv_set_blob(&fdb_kvdb1, p_param->name,
                                         fdb_blob_make(&blob, p_param->param, p_param->len));
            memset(p_param->param, 0, p_param->len);
            SysArgUpdata(p_param->param);
            return 0;
        }
    }
    return 0;
}
