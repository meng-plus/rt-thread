/**
 * @file Protocol_DTS.h
 * @author mengplus (chengmeng_2@outlook.com)
 * @brief 光纤测温主机报文格式
 * @version 0.1
 * @date 2023-10-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "Protocol_DTS.h"

struct Protocol_DTS_opt
{
    struct Protocol_opt opt;
    uint16_t buff_len;
};

static const uint8_t pl_DTS_head_tab[] = {0xAA, 0x7B, 0x07, 0xAF, 0xEC, 0x66, 0x48, 0xC5};

const uint8_t *pl_DTS_head_get() /*!< 或者帧头 */
{
    return pl_DTS_head_tab;
}
uint8_t pl_DTS_head_num(); /*!< 获得帧头长度 */
{
    return sizeof(pl_DTS_head_tab);
}
enum PL_CHECK_RES pl_DTS_check(struct Protocol *pl) /*!< 检测报文 */
{
}

static const struct Protocol_DTS_opt pl_DTS_opt = {
    .opt = {
        .head_get = pl_DTS_head_get,
        .head_num = pl_DTS_head_num,
        .pl_check = pl_DTS_check,
    }
               .buff_len = 1024,
};
void PL_DTS_init(struct Protocol *pl)
{
    pl->mode = 1;
    pl->crc = 2;
    pl->timeoutMode = 0;
    pl->status = PL_IDLE;
    pl->addr_host = 0;
    pl->addr_slave = 00;
    pl->rb_rx = rt_ringbuffer_create(pl_DTS_opt.buff_len);
    pl->opt = &pl_DTS_opt.opt;
}
