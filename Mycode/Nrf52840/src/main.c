/*
 * _______________#########_______________________
 * ______________############_____________________
 * ______________#############____________________
 * _____________##__###########___________________
 * ____________###__######_#####__________________
 * ____________###_#######___####_________________
 * ___________###__##########_####________________
 * __________####__###########_####_______________
 * ________#####___###########__#####_____________
 * _______######___###_########___#####___________
 * _______#####___###___########___######_________
 * ______######___###__###########___######_______
 * _____######___####_##############__######______
 * ____#######__#####################_#######_____
 * ____#######__##############################____
 * ___#######__######_#################_#######___
 * ___#######__######_######_#########___######___
 * ___#######____##__######___######_____######___
 * ___#######________######____#####_____#####____
 * ____######________#####_____#####_____####_____
 * _____#####________####______#####_____###______
 * ______#####______;###________###______#________
 * ________##_______####________####______________
 *
 * @Author: s4627347 s4627347@student.uq.edu.au
 * @Date: 2024-03-12 10:16:16
 * @LastEditors: s4627347 s4627347@student.uq.edu.au
 * @LastEditTime: 2024-05-20 12:23:10
 * @FilePath: /csse4011/prac3/mycode/apps/final/src/main.c
 * @Description:
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/device.h>
#include "cJSON.h" // Include cJSON for JSON parsing
#include <stdlib.h>

#define IBEACON_RSSI 0xc8 /**< Default Calibrated RSSI @ 1m for iBeacon */
#define STACKSIZE 2048    /**< Default stack size for the Bluetooth thread */
#define PRIORITY 7        /**< Default priority for the Bluetooth thread */

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)
static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

#define MSG_SIZE 256 // Size of UART message buffer

/* Queue to store up to 10 messages (aligned to 4-byte boundary) */
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE, 10, 4);

/* receive buffer used in UART ISR callback */
static char rx_buf[MSG_SIZE];
static int rx_buf_pos;
static int uart_rx_len;

// Bluetooth initialization function
void bt_init(void);

// Thread stack definition
K_THREAD_STACK_DEFINE(thread_stack, STACKSIZE);

// iBeacon advertisement data
static uint8_t data[] = {
    0x4c,
    0x00, // Beacons
    0x40,
    0x11, // project code
    0xba,
    0x01, //  BASE UUID
    0x05,
    0xc2, // Targic UUID
    0x00, // USER data (9th)
    0x00, // status type
    0x00, // month
    0x00, // day
    0x00, // hour
    0x00, // minute
    0x00, // Barcode data (15 byte)
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
    0x00, // Calibrated RSSI @ 1m
};

void print_uart(char *buf)
{
    uint8_t msg_len = strlen(buf); // Calculate the length of the message

    // Send each character of the message
    for (int i = 0; i < msg_len; i++)
    {
        uart_poll_out(uart_dev, buf[i]);
    }
}
//
/**
 * @description: Advertisement data structure
 */
static struct bt_data ad[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, data, 29),
};

void fill_data_with_ascii(uint8_t *data, const char *str, size_t start_index);

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
void serial_cb(const struct device *dev, void *user_data)
{
    uint8_t c;

    if (!uart_irq_update(uart_dev))
    {
        return;
    }

    if (!uart_irq_rx_ready(uart_dev))
    {
        return;
    }

    /* read until FIFO empty */
    while (uart_fifo_read(uart_dev, &c, 1) == 1)
    {
        if ((c == '\n' || c == '\r') && rx_buf_pos > 0)
        {
            /* terminate string */
            rx_buf[rx_buf_pos] = '\0';

            /* if queue is full, message is silently dropped */
            k_msgq_put(&uart_msgq, &rx_buf, K_NO_WAIT);

            /* reset the buffer (it was copied to the msgq) */
            uart_rx_len = rx_buf_pos;
            rx_buf_pos = 0;
        }
        else if (rx_buf_pos < (sizeof(rx_buf) - 1))
        {
            rx_buf[rx_buf_pos++] = c;
        }
        /* else: characters beyond buffer size are dropped */
    }
}

/**
 * @brief Initializes the UART device and sets up interrupt-driven reception.
 *
 * This function initializes the UART device and configures it for interrupt-driven reception.
 * If the UART device is not found or if setting up the interrupt callback fails, appropriate error messages are printed.
 */
void uart_init(void)
{
    // Check if the UART device is ready
    if (!device_is_ready(uart_dev))
    {
        printk("UART device not found!");
        return;
    }

    int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

    if (ret < 0)
    {
        if (ret == -ENOTSUP)
        {
            printk("Interrupt-driven UART API support not enabled\n");
        }
        else if (ret == -ENOSYS)
        {
            printk("UART device does not support interrupt-driven API\n");
        }
        else
        {
            printk("Error setting UART callback: %d\n", ret);
        }
        return;
    }

    uart_irq_rx_enable(uart_dev);
}

char time[32];
char barcodeId[32];
char status[32];
/**
 * @brief Processes a JSON string and extracts fields to fill the data array.
 *
 * This function parses the provided JSON string to extract `userid`, `time`, `barcodeid`, and `status` fields.
 * It then converts these fields to appropriate data types and fills the `data` array at specified indices.
 *
 * @param json_str Pointer to the JSON string to be parsed.
 */
void process_json(const char *json_str)
{
    // Parse the JSON string
    cJSON *json = cJSON_Parse(json_str);
    if (json == NULL)
    {
        print_uart("JSON parsing failed\n");
        return;
    }

    // Extract and convert the 'userid' field to uint8_t
    uint8_t userid;
    const cJSON *userid_json = cJSON_GetObjectItemCaseSensitive(json, "userid");
    if (cJSON_IsString(userid_json) && (userid_json->valuestring != NULL))
    {
        unsigned long userid_temp = strtoul(userid_json->valuestring, NULL, 10);
        if (userid_temp <= 255)
        {
            userid = (uint8_t)userid_temp;
            data[8] = userid;
            printk("Success userid: %d\n", userid);
        }
        else
        {
            printk("User ID value out of range\n");
        }
    }
    else
    {
        printk("Invalid JSON format for userid\n");
    }

    // Extract and parse the 'time' field to get month, day, hour, and minute
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    const cJSON *time_json = cJSON_GetObjectItemCaseSensitive(json, "time");
    if (cJSON_IsString(time_json) && (time_json->valuestring != NULL))
    {
        // Assuming the time format is always "YYYY/MM/DD HH:MM:SS"
        char temp_time[20];
        strncpy(temp_time, time_json->valuestring, sizeof(temp_time) - 1);
        temp_time[sizeof(temp_time) - 1] = '\0'; // Ensure null-termination

        // Use sscanf to extract month, day, hour, and minute
        int temp_month, temp_day, temp_hour, temp_minute;
        if (sscanf(temp_time, "%*d/%d/%d %d:%d", &temp_month, &temp_day, &temp_hour, &temp_minute) == 4)
        {
            month = (uint8_t)temp_month;
            day = (uint8_t)temp_day;
            hour = (uint8_t)temp_hour;
            minute = (uint8_t)temp_minute;
            data[10] = month;
            data[11] = day;
            data[12] = hour;
            data[13] = minute;
            printk("Success time\n");
        }
        else
        {
            printk("Invalid time format\n");
        }
    }
    else
    {
        printk("Invalid JSON format for time\n");
    }

    // Extract and fill the 'barcodeid' field into the data array starting at index 14
    const cJSON *barcodeId_json = cJSON_GetObjectItemCaseSensitive(json, "barcodeid");
    if (cJSON_IsString(barcodeId_json) && (barcodeId_json->valuestring != NULL))
    {
        strncpy(barcodeId, barcodeId_json->valuestring, sizeof(barcodeId) - 1);
        barcodeId[sizeof(barcodeId) - 1] = '\0'; // Ensure null-termination
        fill_data_with_ascii(data, barcodeId, 14);
        printk("Success Barcode ID: %s\n", barcodeId);
    }
    else
    {
        printk("Invalid JSON format for barcodeId\n");
    }

    // Extract and convert the 'status' field to uint8_t
    uint8_t status;
    const cJSON *status_json = cJSON_GetObjectItemCaseSensitive(json, "status");
    if (cJSON_IsString(status_json) && (status_json->valuestring != NULL))
    {
        if (strcmp(status_json->valuestring, "0") == 0)
        {
            status = 0;
        }
        else if (strcmp(status_json->valuestring, "1") == 0)
        {
            status = 1;
        }
        else
        {
            printk("Invalid status value\n");
            status = 0xFF; // Use an invalid value to indicate an error, e.g., 0xFF
        }
        data[9] = status;
        printk("Success status: %d\n", status);
    }
    else
    {
        printk("Invalid JSON format for status\n");
    }

    // Clean up the JSON object
    cJSON_Delete(json);
}

/**
 * @brief Main function for Bluetooth operation.
 *
 * This function initializes Bluetooth and creates a thread to handle data reception.
 *
 * @return int Returns 0 on success.
 */
int main(void)
{

    int err = bt_enable(NULL);
    if (err)
    {
        printk("Bluetooth init failed (err %d)\n", err);
    }

    uart_init();
    bt_init();
    char tx_buf[MSG_SIZE];
    while (k_msgq_get(&uart_msgq, &tx_buf, K_FOREVER) == 0)
    {

        // Stop advertisement and disable Bluetooth
        err = bt_le_adv_stop();
        if (err)
        {
            printk("Failed to stop advertising (err %d)\n", err);
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

        process_json(tx_buf);

        // Reinitialize Bluetooth for advertisement
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

/**
 * @brief Fills a portion of the data array with ASCII values of the given string and pads the rest with zeros.
 *
 * This function copies the ASCII values of the characters in the given string `str` into the `data` array starting
 * at the specified `start_index`. If the string length is less than `max_len`, the remaining portion of the data
 * array is padded with zeros. If the string length is greater than `max_len`, it is truncated to `max_len`.
 *
 * @param data Pointer to the data array to be filled.
 * @param str Pointer to the null-terminated string to be copied into the data array.
 * @param start_index The starting index in the data array where the string's ASCII values will be copied.
 */
void fill_data_with_ascii(uint8_t *data, const char *str, size_t start_index)
{
    size_t len = strlen(str);
    size_t i;
    size_t max_len = 15; // Maximum length of the portion to be filled in the data array

    // If the string length exceeds max_len, truncate it
    if (len > max_len)
    {
        len = max_len;
    }

    // Copy the string into the data array
    for (i = 0; i < len; i++)
    {
        data[start_index + i] = (uint8_t)str[i];
    }

    // Pad the rest of the data array with zeros
    for (; i < max_len; i++)
    {
        data[start_index + i] = 0;
    }
}
