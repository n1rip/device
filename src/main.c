#include <stdlib.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "class/hid/hid_device.h"
#include "driver/gpio.h"

#include "definitions.h"

/**
 * @brief hid report descriptor
 */
const uint8_t hid_report_descriptor[] = {
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(HID_ITF_PROTOCOL_MOUSE))
};

/**
 * @brief string descriptor
 */
const char* hid_string_descriptor[5] = {
    (char[]){0x09, 0x04}, // means supported language is english (0x0409)
    "n1", // manufacturer
    "n1 mouse", // product
    "1337", // serials, should use chip id
    "n1 mouse hid", // hid
};

/**
 * @brief configuration descriptor that defines 1 configuration and 1 HID interface
 */
static const uint8_t hid_configuration_descriptor[] = {
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, (TUD_CONFIG_DESC_LEN + CFG_TUD_HID * TUD_HID_DESC_LEN), TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(0, 4, false, sizeof(hid_report_descriptor), 0x81, 16, 10),
};

/**
 * @brief invoked when received GET_HID_REPORT_DESCRIPTOR request
 * @returns pointer to report descriptor
*/
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    return hid_report_descriptor;
}

/**
 * @brief invoked when received GET_REPORT control request
 * @returns 0 to stall request
*/
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
  return 0;
}

/**
 * @brief invoked when received SET_REPORT control request or received data on out endpoint (report if = 0, type = 0)
*/
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
    // return nothing
}

/**
 * @brief stub for the input forwarding function
*/
void forward_input() {
    return 0;
}

void app_main(void) {
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,
        .string_descriptor = hid_string_descriptor,
        .string_descriptor_count = sizeof(hid_string_descriptor) / sizeof(hid_string_descriptor[0]),
        .external_phy = false,
        .configuration_descriptor = hid_configuration_descriptor,
    };

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    while (1) {
        if (tud_mounted()) {
            forward_inputs();
        }
    }
}
