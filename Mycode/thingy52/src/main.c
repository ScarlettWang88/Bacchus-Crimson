/*
 *
 *    ┏┓　　　┏┓
 *  ┏┛┻━━━┛┻┓
 *  ┃　　　　　　　┃
 *  ┃　　　━　　　┃
 *  ┃　＞　　　＜　┃
 *  ┃　　　　　　　┃
 *  ┃...　⌒　...　┃
 *  ┃　　　　　　　┃
 *  ┗━┓　　　┏━┛
 *      ┃　　　┃　
 *      ┃　　　┃
 *      ┃　　　┃
 *      ┃　　　┃  神兽保佑
 *      ┃　　　┃  代码无bug　　
 *      ┃　　　┃
 *      ┃　　　┗━━━┓
 *      ┃　　　　　　　┣┓
 *      ┃　　　　　　　┏┛
 *      ┗┓┓┏━┳┓┏┛
 *        ┃┫┫　┃┫┫
 *        ┗┻┛　┗┻┛
 *
 *
 * @Author: s4627347 s4627347@student.uq.edu.au
 * @Date: 2024-03-12 09:50:17
 * @LastEditors: s4627347 s4627347@student.uq.edu.au
 * @LastEditTime: 2024-05-18 20:38:19
 * @FilePath: /csse4011/prac3/mycode/apps/final/thingy52/prac2/src/main.c
 * @Description: Main application entry point
 *
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include "btscan52.h"
#include "bt52.h"

#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/**
 * @brief Function to execute the Bluetooth functionality in a thread.
 *
 * @return int Returns 0 on success.
 */
int bt_thread(void)
{
	bt();
	return 0;
}

void led_blink_thread(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led))
	{
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0)
	{
		return 0;
	}

	while (1)
	{
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0)
		{
			return 0;
		}
		led_state = !led_state;
		k_msleep(1000);
	}
}

K_THREAD_DEFINE(led_blink_id, STACKSIZE, led_blink_thread, NULL, NULL, NULL, PRIORITY, 0, 0);

K_THREAD_DEFINE(bt_id, STACKSIZE, bt_thread, NULL, NULL, NULL,
				PRIORITY, 0, 0);

// Main function
int main(void)
{
	//	k_thread_start(btscan_id);
	k_thread_start(bt_id);
	k_thread_start(led_blink_id);

	return 0;
}
