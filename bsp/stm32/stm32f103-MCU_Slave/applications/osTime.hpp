/**
 * @file osTime.hpp
 * @author √…√…plus (chengmeng_2@outlook.com)
 * @brief ∂® ±∆˜¿‡
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include <stdint.h>
class osTime
{
private:
    void *handle;
    void *parameter;
    static void time_cb(osTime *param);

protected:
    void Tick();

public:
    osTime(const char *name, uint8_t flag, void *parameter, uint32_t time);
    int32_t control(int cmd, void *arg);
    void setTime(uint32_t time);
    void start();
    void stop();
    virtual ~osTime();
};
