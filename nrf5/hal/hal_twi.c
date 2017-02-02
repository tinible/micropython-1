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

#include "mphalport.h"
#include "hal_twi.h"

#ifdef HAL_TWI_MODULE_ENABLED

static const uint32_t hal_twi_frequency_lookup[] = {
    TWI_FREQUENCY_FREQUENCY_K100, // 100 kbps
    TWI_FREQUENCY_FREQUENCY_K250, // 250 kbps
    TWI_FREQUENCY_FREQUENCY_K400, // 400 kbps
};

void hal_twi_master_init(NRF_TWI_Type * p_instance, hal_twi_init_t const * p_twi_init) {
}

void hal_twi_master_tx(NRF_TWI_Type  * p_instance,
                       uint8_t         addr,
                       uint16_t        transfer_size,
                       const uint8_t * tx_data,
                       bool            stop) {

}

void hal_twi_master_rx(NRF_TWI_Type  * p_instance,
                       uint8_t         addr,
                       uint16_t        transfer_size,
                       const uint8_t * rx_data) {

}

void hal_twi_slave_init(NRF_TWI_Type * p_instance, hal_twi_init_t const * p_twi_init) {
}

#endif // HAL_TWI_MODULE_ENABLED
