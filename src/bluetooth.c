#include <sys/printk.h>
#include <zephyr.h>
#include <zephyr/types.h>

#include "bluetooth.h"
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>

static struct bt_ledService_cb ledService_cb;

static ssize_t writeLed(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, uint16_t len, uint16_t offset, uint8_t flags){
    if(ledService_cb.led_cb){
        ledService_cb.led_cb(*(uint32_t *)buf);
    }
    return len;
}

BT_GATT_SERVICE_DEFINE(ledService,
BT_GATT_PRIMARY_SERVICE(BT_UUID_SERVICE),
BT_GATT_CHARACTERISTIC(BT_UUID_LED, BT_GATT_CHRC_WRITE, BT_GATT_PERM_WRITE, NULL, writeLed, NULL),
);

int bt_ledService_init(struct bt_ledService_cb *callbacks){
    if (callbacks){
        ledService_cb.led_cb = callbacks->led_cb;
    }
    return 0;
}

