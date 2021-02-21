/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include "bluetooth.h"
#include <sys/printk.h>
#include <drivers/gpio.h>
#include <soc.h>
#include <zephyr/types.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <dk_buttons_and_leds.h>

#define DEVICE_NAME     CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LENGTH     (sizeof(DEVICE_NAME) -1)

#define RUN_STATUS_LED          DK_LED1
#define CON_STATUS_LED          DK_LED2
#define RUN_LED_BLINK_INTERVAL  1000
#define USER_LED          DK_LED3

/*PWM0*/
#include <drivers/pwm.h>
const struct device *pwm;
uint32_t data;
#define PWM_LED0_NODE	DT_ALIAS(pwm_led0)

#if !DT_NODE_HAS_STATUS(PWM_LED0_NODE, okay)
#error "Unsupported board: pwm-servo devicetree alias is not defined or enabled"
#define PWM_LABEL ""
#else
#define PWM_LABEL	DT_PWMS_LABEL(PWM_LED0_NODE)
#define PWM_CHANNEL	DT_PWMS_CHANNEL(PWM_LED0_NODE)
#define PWM_FLAGS	DT_PWMS_FLAGS(PWM_LED0_NODE)
#endif
#define PERIOD_USEC	(200U * USEC_PER_MSEC)

static const struct bt_data sd[] = {
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_SERVICE_VAL),
};

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LENGTH),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		printk("Connection failed (err %u)\n", err);
		return;
	}

	printk("Connected\n");

	dk_set_led_on(CON_STATUS_LED);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	printk("Disconnected (reason %u)\n", reason);

	dk_set_led_off(CON_STATUS_LED);
}



static struct bt_conn_cb conn_callbacks = {
	.connected        = connected,
	.disconnected     = disconnected,
};
//PWM set
void pwm_set(){
        int ret;
        ret = pwm_pin_set_usec(pwm, PWM_CHANNEL, PERIOD_USEC, PERIOD_USEC/2, PWM_FLAGS);
}

//Calling the callback 
static void app_led_cb(uint32_t ledState){
    bool ledBool;
    if(ledState == 1){
        ledBool = true;
        pwm_set();
    }
    else{
        ledBool = false;
    }
    dk_set_led(USER_LED, ledBool);
}

static struct bt_ledService_cb service_callbacks = {
    .led_cb = app_led_cb,
};

void main(void)
{
    int blinkStatus = 0;
    int err;
    err = dk_leds_init();

    if(err){
        printk("Leds init failed (err %d) \n", err);
        return;
    }
     
	printk("Starting Bluetooth Peripheral LBS example\n");
        /*PWM INIT*/
        pwm = device_get_binding(PWM_LABEL);
	if (!pwm) {
		printk("Error: didn't find %s device\n", PWM_LABEL);
		return;
	}

    bt_conn_cb_register(&conn_callbacks);

    err = bt_enable(NULL);
       if(err){
        printk("Bluetooth init failed (err %d) \n", err);
        return;
    }
    
    printk("Bluetooth initialized\n");

	//if (IS_ENABLED(CONFIG_SETTINGS)) {
	//	settings_load();
	//}

	err = bt_ledService_init(&service_callbacks);
	if (err) {
		printk("Failed to init LBS (err:%d)\n", err);
		return;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

	printk("Advertising successfully started\n");

	for (;;) {
		dk_set_led(RUN_STATUS_LED, (++blinkStatus) % 2);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
	}
}
