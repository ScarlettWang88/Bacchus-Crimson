/*
 *                   ___====-_  _-====___
 *             _--^^^#####//      \\#####^^^--_
 *          _-^##########// (    ) \\##########^-_
 *         -############//  |\^^/|  \\############-
 *       _/############//   (@::@)   \############\_
 *      /#############((     \\//     ))#############\
 *     -###############\\    (oo)    //###############-
 *    -#################\\  / VV \  //#################-
 *   -###################\\/      \//###################-
 *  _#/|##########/\######(   /\   )######/\##########|\#_
 *  |/ |#/\#/\#/\/  \#/\##\  |  |  /##/\#/  \/\#/\#/\#| \|
 *  `  |/  V  V  `   V  \#\| |  | |/#/  V   '  V  V  \|  '
 *     `   `  `      `   / | |  | | \   '      '  '   '
 *                      (  | |  | |  )
 *                     __\ | |  | | /__
 *                    (vvv(VVV)(VVV)vvv)
 *
 *      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *                神兽保佑            永无BUG
 *
 * @Author: s4627347 s4627347@student.uq.edu.au
 * @Date: 2024-03-19 11:00:12
 * @LastEditors: s4627347 s4627347@student.uq.edu.au
 * @LastEditTime: 2024-04-12 11:06:10
 * @FilePath: /csse4011/prac3/mycode/apps/thingy52/prac2/include/btscan52.h
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include <zephyr/sys/printk.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/addr.h>
#include "bt52.h"

#define NAME_LEN 30
#define UUID_LENGTH 16
#define UUID_OFFSET 6

#define BEACON_A "F5:75:FE:85:34:67 (random)"
#define BEACON_B "E5:73:87:06:1E:86 (random)"
#define BEACON_C "CA:99:9E:FD:98:B1 (random)"
#define BEACON_D "CB:1B:89:82:FF:FE (random)"
#define BEACON_E "D4:D2:A0:A4:5C:AC (random)"
#define BEACON_F "C1:13:27:E9:B7:7C (random)"
#define BEACON_G "F1:04:48:06:39:A0 (random)"
#define BEACON_H "CA:0C:E0:DB:CE:60 (random)"
#define BEACON_I "D4:7F:D4:7C:20:13 (random)"
#define BEACON_J "F7:0B:21:F1:C8:E1 (random)"
#define BEACON_K "FD:E0:8D:FA:3E:4A (random)"
#define BEACON_L "EE:32:F7:28:FA:AC (random)"

/**
 * @brief Start Bluetooth scanning.
 *
 * This function starts Bluetooth scanning to discover nearby devices.
 *
 * @return int Returns 0 on success, or an error code otherwise.
 */
int btscan(void);

/**
 * @brief Get the temperature value.
 *
 * This function returns the temperature value obtained from Bluetooth scanning.
 *
 * @return uint8_t The temperature value.
 */
int8_t get_rssi_beacon(uint8_t beacon_id);
void observer_start(void);