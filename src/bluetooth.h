#include <zephyr/types.h>

//UUID service declaration
#define BT_UUID_SERVICE_VAL BT_UUID_128_ENCODE(0x00001526, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_SERVICE BT_UUID_DECLARE_128 (BT_UUID_SERVICE_VAL)

//UUID Led characteristic declaration
#define BT_UUID_LED_VAL BT_UUID_128_ENCODE(0x00001527, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_LED BT_UUID_DECLARE_128 (BT_UUID_LED_VAL)

//Declaración del Callback. Callback declaration
typedef void (*led_cb_t) (uint32_t ledState);
struct bt_ledService_cb{led_cb_t led_cb;};

int bt_ledService_init (struct bt_ledService_cb *callbacks);
