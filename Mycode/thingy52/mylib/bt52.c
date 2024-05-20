
/*
 *                                                     __----~~~~~~~~~~~------___
 *                                    .  .   ~~//====......          __--~ ~~
 *                    -.            \_|//     |||\\  ~~~~~~::::... /~
 *                 ___-==_       _-~o~  \/    |||  \\            _/~~-
 *         __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\        _/~
 *     _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /
 *   .~       .~       |   \\ -_    /  /-   /   ||      \   /
 *  /  ____  /         |     \\ ~-_/  /|- _/   .||       \ /
 *  |~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\
 *           '         ~-|      /|    |-~\~~       __--~~
 *                       |-~~-_/ |    |   ~\_   _-~            /\
 *                            /  \     \__   \/~                \__
 *                        _--~ _/ | .-~~____--~-/                  ~~==.
 *                       ((->/~   '.|||' -_|    ~~-/ ,              . _||
 *                                  -_     ~\      ~~---l__i__i__i--~~_/
 *                                  _-~-__   ~)  \--______________--~~
 *                                //.-~~~-~_--~- |-------~~~~~~~~
 *                                       //.-~~~--\
 *                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *                               神兽保佑            永无BUG
 *
 *
 * Bluetooth Module
 *
 * This module handles Bluetooth functionalities such as initialization,
 * advertisement data setup, and data transmission.
 *
 * Author: s4627347 <s4627347@student.uq.edu.au>
 * Date: 2024-03-19 09:42:14
 * Last Updated By: s4627347 <s4627347@student.uq.edu.au>
 * Last Updated: 2024-03-24 14:07:49
 * File Path: /csse4011/prac2/mycode/apps/project/prac2/mylib/bt.c
 * Description: This module initializes Bluetooth, sets up iBeacon advertisement
 * data, and handles data transmission.
 *
 * [License information]
 */

#include <zephyr/bluetooth/bluetooth.h>
#include "bt52.h" // Including custom Bluetooth header file
#include "btscan52.h"

// Bluetooth initialization function
void bt_init(void);
void send_ble(uint8_t *basic);

// Thread stack definition
K_THREAD_STACK_DEFINE(thread_stack, STACKSIZE);

// iBeacon advertisement data
static uint8_t data[] = {
    0x4c,
    0x00, // Beacons
    0x40,
    0x11, // project code
    0x01,
    0x52, //  BASE UUID
    0x05,
    0xc2, // Targic UUID
    0x01, // USER data (9th)
    0x00, // Barcode data (19 byte)
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    IBEACON_RSSI, // Calibrated RSSI @ 1m
};

uint8_t barcode_data[20];

//
/**
 * @description: Advertisement data structure
 */
static struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, data, 29),
};

/**
 * @brief Initializes Bluetooth and starts advertising.
 *
 * This function enables Bluetooth and starts advertising with the specified advertisement data.
 *
 * @note The advertisement data must be configured before calling this function.
 */
void bt_init(void)
{

    int err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
                              NULL, 0);
    if (err)
    {
        printk("Advertising failed to start (err %d)\n", err);
        return;
    }
}

/**
 * @brief Main function for Bluetooth operation.
 *
 * This function initializes Bluetooth and creates a thread to handle data reception.
 *
 * @return int Returns 0 on success.
 */
int bt(void)
{
    int err = bt_enable(NULL);
    if (err)
    {
        printk("Bluetooth init failed (err %d)\n", err);
    }
    err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
                          NULL, 0);
    if (err)
    {
        printk("Advertising failed to start (err %d)\n", err);
    }
    observer_start();
    while (true)
    {

        // Stop advertisement and disable Bluetooth

        err = bt_le_adv_stop();
        if (err)
        {
            printk("Failed to stop advertising (err %d)\n", err);
            // 根据具体的错误码进行错误处理
            switch (err)
            {
            case -EALREADY:
                printk("Advertising is already stopped\n");
                break;
            default:
                printk("An unknown error occurred\n");
            }
        }
        k_sleep(K_MSEC(500)); // Delay to ensure proper initialization
        for (size_t i = 0; i < 20; i++)
        {
            data[8 + i] = barcode_data[i];
        }

        err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad),
                              NULL, 0);
        if (err)
        {
            printk("Advertising failed to start (err %d)\n", err);
        }
        k_sleep(K_MSEC(500)); // Delay to ensure proper initialization
    }

    return 0;
}

void send_ble(uint8_t *basic)
{
    for (size_t i = 0; i < 20; i++)
    {
        barcode_data[i] = basic[i];
    }
}