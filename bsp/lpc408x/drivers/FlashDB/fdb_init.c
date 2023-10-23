
#include <flashdb.h>
#include <stdio.h>
#include <stdlib.h>

#define KV_PART_NAME BSP_USRING_FAL_NAME //"eeprom1"
#define KV_NAME "KJ428_kv"

#if defined(FDB_USING_KVDB)

static struct fdb_default_kv_node default_kv_set[] = {
    {"iap_need_copy_app", "0"},
    {"iap_need_crc32_check", "0"},
    {"iap_copy_app_size", "0"},
    {"stop_in_bootloader", "0"},
};

struct fdb_kvdb kj428_kvdb;

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
    fdb_kvdb_control(&(kj428_kvdb), FDB_KVDB_CTRL_SET_SEC_SIZE, &sec_size);
    fdb_kvdb_control(&(kj428_kvdb), FDB_KVDB_CTRL_SET_FILE_MODE, &file_mode);
    fdb_kvdb_control(&(kj428_kvdb), FDB_KVDB_CTRL_SET_MAX_SIZE, &db_size);
#endif
    fdb_kvdb_init(&kj428_kvdb, KV_NAME, KV_PART_NAME, &default_kv, NULL);
    fdb_kv_print(&kj428_kvdb);
    return 0;
}
INIT_COMPONENT_EXPORT(kj428_fdb_kvdb_init);
#endif /* FDB_USING_KVDB */

#ifdef FDB_USING_TSDB
{ /* TSDB Sample */
    /* set the lock and unlock function if you want */
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_LOCK, lock);
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_SET_UNLOCK, unlock);
    /* Time series database initialization
     *
     *       &tsdb: database object
     *       "log": database name
     * "fdb_tsdb1": The flash partition name base on FAL. Please make sure it's in FAL partition table.
     *              Please change to YOUR partition name.
     *    get_time: The get current timestamp function.
     *         128: maximum length of each log
     *        NULL: The user data if you need, now is empty.
     */
    result = fdb_tsdb_init(&tsdb, "log", "fdb_tsdb1", get_time, 128, NULL);
    /* read last saved time for simulated timestamp */
    fdb_tsdb_control(&tsdb, FDB_TSDB_CTRL_GET_LAST_TIME, &counts);

    if (result != FDB_NO_ERR)
    {
        return -1;
    }

    /* run TSDB sample */
    tsdb_sample(&tsdb);
}
#endif /* FDB_USING_TSDB */
