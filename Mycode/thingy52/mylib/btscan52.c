/*
 *  ┌───┐   ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┬───┐ ┌───┬───┬───┐
 *  │Esc│   │ F1│ F2│ F3│ F4│ │ F5│ F6│ F7│ F8│ │ F9│F10│F11│F12│ │P/S│S L│P/B│  ┌┐    ┌┐    ┌┐
 *  └───┘   └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┴───┘ └───┴───┴───┘  └┘    └┘    └┘
 *  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐ ┌───┬───┬───┐ ┌───┬───┬───┬───┐
 *  │~ `│! 1│@ 2│# 3│$ 4│% 5│^ 6│& 7│* 8│( 9│) 0│_ -│+ =│ BacSp │ │Ins│Hom│PUp│ │N L│ / │ * │ - │
 *  ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤ ├───┼───┼───┤ ├───┼───┼───┼───┤
 *  │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{ [│} ]│ | \ │ │Del│End│PDn│ │ 7 │ 8 │ 9 │   │
 *  ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤ └───┴───┴───┘ ├───┼───┼───┤ + │
 *  │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  │               │ 4 │ 5 │ 6 │   │
 *  ├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────────┤     ┌───┐     ├───┼───┼───┼───┤
 *  │ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│  Shift   │     │ ↑ │     │ 1 │ 2 │ 3 │   │
 *  ├─────┬──┴─┬─┴──┬┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬────┤ ┌───┼───┼───┐ ├───┴───┼───┤ E││
 *  │ Ctrl│    │Alt │         Space         │ Alt│    │    │Ctrl│ │ ← │ ↓ │ → │ │   0   │ . │←─┘│
 *  └─────┴────┴────┴───────────────────────┴────┴────┴────┴────┘ └───┴───┴───┘ └───────┴───┴───┘
 *
 *
 * @Author: s4627347 s4627347@student.uq.edu.au
 * @Date: 2024-03-19 10:59:35
 * @LastEditors: s4627347 s4627347@student.uq.edu.au
 * @LastEditTime: 2024-05-18 23:43:44
 * @FilePath: /csse4011/prac3/mycode/apps/final/thingy52/prac2/mylib/btscan52.c
 * @Description: Bluetooth scanning functionality
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "btscan52.h"

/**
 * @brief Callback function called when a Bluetooth LE device is found during scanning.
 *
 * This function extracts relevant information from the advertisement data of the found device
 * and performs actions based on the received data.
 *
 * @param addr Pointer to the Bluetooth LE address structure of the found device.
 * @param rssi Received Signal Strength Indicator (RSSI) of the received advertisement.
 * @param type Type of the found device.
 * @param ad Pointer to the net buffer containing the advertisement data of the found device.
 */
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
                         struct net_buf_simple *ad)
{

    char addr_str[BT_ADDR_LE_STR_LEN];

    // Convert Bluetooth LE address to string format
    bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));

    uint8_t *data = ad->data;   // Advertisement data
    uint8_t data_len = ad->len; // Length of advertisement data
    uint8_t idx = 0;
    while (idx < data_len)
    {
        if (data[idx] == 0xFF)
        {
            if (idx != 0)
            {
                break;
            }
        }
        idx++;
    }
    if (data_len >= 9 &&
        data[idx + 1] == 0x4c &&
        data[idx + 2] == 0x00 &&
        data[idx + 3] == 0x40 &&
        data[idx + 4] == 0x11 &&
        data[idx + 7] == 0x05 &&
        data[idx + 8] == 0xc2)
    {
        uint8_t barcode_data[20];
        for (int i = 0; i < 20; i++)
        {
            barcode_data[i] = data[idx + 9 + i];
        }
        send_ble(barcode_data);
    }
}

/**
 * @brief Start Bluetooth LE scanning with specified scan parameters.
 *
 * This function initializes Bluetooth LE scanning with the given parameters,
 * including scan type, scan options, scan interval, and scan window. If a filter
 * address is provided, it adds the address to the accept list for filtering.
 * Upon successful initialization, it starts scanning for Bluetooth LE devices.
 *
 * @return int Returns 0 on success, or an error code if scanning initialization fails.
 */
void observer_start(void)
{
    int err;
    // Initialize scan parameters
    struct bt_le_scan_param scan_param = {
        .type = BT_LE_SCAN_TYPE_PASSIVE,       // Passive scan type
        .options = BT_LE_SCAN_OPT_NONE,        // No scan options
        .interval = BT_GAP_SCAN_FAST_INTERVAL, // Scan interval
        .window = BT_GAP_SCAN_FAST_WINDOW,     // Scan window
    };

    // Start Bluetooth LE scanning with the specified parameters
    err = bt_le_scan_start(&scan_param, device_found);
    if (err)
    {
        printk("Start scanning failed (err %d)\n", err);
        return;
    }
    // Print message indicating scanning has started successfully
    printk("Started scanning...\n");

    return;
}
