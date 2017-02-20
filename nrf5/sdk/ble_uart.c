/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Glenn Ruben Bakke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <string.h>
#include "ble_uart.h"

#if MICROPY_PY_BLE_NUS

static ubluepy_uuid_obj_t uuid_obj_service = {
    .base.type = &ubluepy_uuid_type,
    .type = UBLUEPY_UUID_128_BIT,
    .value = {0x01, 0x00}
};

static ubluepy_uuid_obj_t uuid_obj_char_tx = {
    .base.type = &ubluepy_uuid_type,
    .type = UBLUEPY_UUID_128_BIT,
    .value = {0x02, 0x00}
};

static ubluepy_uuid_obj_t uuid_obj_char_rx = {
    .base.type = &ubluepy_uuid_type,
    .type = UBLUEPY_UUID_128_BIT,
    .value = {0x03, 0x00}
};

static ubluepy_service_obj_t ble_uart_service = {
    .base.type = &ubluepy_service_type,
    .p_uuid = &uuid_obj_service,
    .type = UBLUEPY_SERVICE_PRIMARY
};

static ubluepy_characteristic_obj_t ble_uart_char_tx = {
    .base.type = &ubluepy_characteristic_type,
    .p_uuid = &uuid_obj_char_tx,
    .props = UBLUEPY_PROP_WRITE | UBLUEPY_PROP_WRITE_WO_RESP,
     .attrs = 0,
};

static ubluepy_characteristic_obj_t ble_uart_char_rx = {
    .base.type = &ubluepy_characteristic_type,
    .p_uuid = &uuid_obj_char_rx,
    .props = UBLUEPY_PROP_NOTIFY,
    .attrs = UBLUEPY_ATTR_CCCD,
};

static ubluepy_peripheral_obj_t ble_uart_peripheral = {
    .base.type = &ubluepy_peripheral_type,
    .conn_handle = 0xFFFF,
};

int mp_hal_stdin_rx_chr(void) {
    return 0;
}

void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {

}

STATIC void gap_event_handler(mp_obj_t self_in, uint16_t event_id, uint16_t conn_handle, uint16_t length, uint8_t * data) {
    ubluepy_peripheral_obj_t * self = MP_OBJ_TO_PTR(self_in);

    if (event_id == 16) {                // connect event
        self->conn_handle = conn_handle;
    } else if (event_id == 17) {         // disconnect event
        self->conn_handle = 0xFFFF;      // invalid connection handle
    }
}

STATIC void gatts_event_handler(mp_obj_t self_in, uint16_t event_id, uint16_t attr_handle, uint16_t length, uint8_t * data) {
    ubluepy_peripheral_obj_t * self = MP_OBJ_TO_PTR(self_in);
    (void)self;
}

void ble_uart_init0(void) {
    uint8_t base_uuid[] = {0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x00, 0x00, 0x40, 0x6E};
    uint8_t uuid_vs_idx;

    (void)ble_drv_uuid_add_vs(base_uuid, &uuid_vs_idx);

    uuid_obj_service.uuid_vs_idx = uuid_vs_idx;
    uuid_obj_char_tx.uuid_vs_idx = uuid_vs_idx;
    uuid_obj_char_rx.uuid_vs_idx = uuid_vs_idx;

    (void)ble_drv_service_add(&ble_uart_service);
    ble_uart_service.char_list = mp_obj_new_list(0, NULL);

    // add TX characteristic
    ble_uart_char_tx.service_handle = ble_uart_service.handle;
    bool retval = ble_drv_characteristic_add(&ble_uart_char_tx);
    if (retval) {
    	ble_uart_char_tx.p_service = &ble_uart_service;
    }
    mp_obj_list_append(ble_uart_service.char_list, MP_OBJ_FROM_PTR(&ble_uart_char_tx));

    // add RX characteristic
    ble_uart_char_rx.service_handle = ble_uart_service.handle;
    retval = ble_drv_characteristic_add(&ble_uart_char_rx);
    if (retval) {
    	ble_uart_char_rx.p_service = &ble_uart_service;
    }
    mp_obj_list_append(ble_uart_service.char_list, MP_OBJ_FROM_PTR(&ble_uart_char_rx));



    // setup the peripheral
    (void)ble_uart_peripheral;

    ble_drv_gap_event_handler_set(MP_OBJ_FROM_PTR(&ble_uart_peripheral), gap_event_handler);
    ble_drv_gatts_event_handler_set(MP_OBJ_FROM_PTR(&ble_uart_peripheral), gatts_event_handler);

    ble_uart_peripheral.conn_handle = 0xFFFF;

    char device_name[] = "mpus";

    mp_obj_t service_list = mp_obj_new_list(0, NULL);
    mp_obj_list_append(service_list, MP_OBJ_FROM_PTR(&ble_uart_service));

    mp_obj_t * services = NULL;
    mp_uint_t  num_services;
    mp_obj_get_array(service_list, &num_services, &services);

    ubluepy_advertise_data_t adv_data = {
        .p_services = services,
        .num_of_services = num_services,
        .p_device_name = (uint8_t *)device_name,
        .device_name_len = strlen(device_name)
    };

    (void)device_name;
    (void)services;

    (void)ble_drv_advertise_data(&adv_data);
}


#endif // MICROPY_PY_BLE_NUS
