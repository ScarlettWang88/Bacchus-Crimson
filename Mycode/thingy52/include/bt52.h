/*
 * @Author: s4627347 s4627347@student.uq.edu.au
 * @Date: 2024-03-19 09:42:44
 * @LastEditors: s4627347 s4627347@student.uq.edu.au
 * @LastEditTime: 2024-05-18 20:01:16
 * @FilePath: /csse4011/prac3/mycode/apps/final/thingy52/prac2/include/bt52.h
 * @Description: Header file containing function declarations for Bluetooth operation.
 *
 * This header file declares the main function for Bluetooth operation, which initializes
 * Bluetooth and creates a thread to handle data reception.
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#ifndef BT52_H
#define BT52_H

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#ifndef IBEACON_RSSI
#define IBEACON_RSSI 0xc8 /**< Default Calibrated RSSI @ 1m for iBeacon */
#define STACKSIZE 2048    /**< Default stack size for the Bluetooth thread */
#define PRIORITY 7        /**< Default priority for the Bluetooth thread */
#endif

/**
 * @brief Main function for Bluetooth operation.
 *
 * This function initializes Bluetooth and creates a thread to handle data reception.
 *
 * @return int Returns 0 on success.
 */
int bt(void);
void send_ble(uint8_t *basic);

#endif /* BT52_H */