#include <sys/printk.h>
#include <zephyr.h>
#include <zephyr/types.h>

#include "bluetooth.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

static struct bt_pwmService_cb pwmService_cb;

static ssize_t writeDuty(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags){
    if(pwmService_cb.pwm_cb){
        pwmService_cb.pwm_cb(*(uint32_t *)buf);
    }
    return len;
}

BT_GATT_SERVICE_DEFINE(pwmService,
BT_GATT_PRIMARY_SERVICE(BT_UUID_SERVICE),
BT_GATT_CHARACTERISTIC(BT_UUID_PWM, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, writeDuty, NULL),
);

int bt_pwmService_init(struct bt_pwmService_cb *callbacks){
    if (callbacks){
        pwmService_cb.pwm_cb = callbacks->pwm_cb;
    }
    return 0;
}

