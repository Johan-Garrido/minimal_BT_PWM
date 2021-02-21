#include <zephyr/types.h>

//UUID service declaration
#define BT_UUID_SERVICE_VAL BT_UUID_128_ENCODE(0x00001526, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_SERVICE BT_UUID_DECLARE_128 (BT_UUID_SERVICE_VAL)

//UUID Led characteristic declaration
#define BT_UUID_PWM_VAL BT_UUID_128_ENCODE(0x00001527, 0x1212, 0xefde, 0x1523, 0x785feabcd123)
#define BT_UUID_PWM BT_UUID_DECLARE_128 (BT_UUID_PWM_VAL)

//Callback declaration
typedef void (*pwm_cb_t) (uint32_t ledState);
struct bt_pwmService_cb{pwm_cb_t pwm_cb;};

int bt_pwmService_init (struct bt_pwmService_cb *callbacks);
