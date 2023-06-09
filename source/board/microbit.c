/**
 * @file    microbit.c
 * @brief   board ID for the BBC Microbit board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "fsl_device_registers.h"
#include "IO_Config.h"
#include "DAP.h"
#include "target_family.h"
#include "target_board.h"

const char * const board_id_mb_1_3 = "9900";
const char * const board_id_mb_1_5 = "9901";

typedef enum {
    BOARD_VERSION_1_3 = 0,
    BOARD_VERSION_1_5 = 1,
} mb_version_t;

// Enables Board Type Pin, reads it and disables it
// Depends on gpio_init() to have been executed already
static uint8_t read_board_type_pin(void) {
    uint8_t pin_state = 0;
    // GPIO mode, Pull enable, pull down, input
    PIN_BOARD_TYPE_PORT->PCR[PIN_BOARD_TYPE_BIT] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(0);
    PIN_BOARD_TYPE_GPIO->PDDR &= ~PIN_BOARD_TYPE;
    // Wait to stabilise, based on gpio.c busy_wait(), at -O2 10000 iterations delay ~850us
    for (volatile uint32_t i = 10000; i > 0; i--);
    // Read pin
    pin_state = (PIN_BOARD_TYPE_GPIO->PDIR & PIN_BOARD_TYPE);
    // Revert and disable
    PIN_BOARD_TYPE_PORT->PCR[PIN_BOARD_TYPE_BIT] = PORT_PCR_MUX(0) | PORT_PCR_PE(0);
    return pin_state;
}

static void set_board_id(mb_version_t board_version) {
    switch (board_version) {
        case BOARD_VERSION_1_3:
            g_board_info.target_cfg->rt_board_id = board_id_mb_1_3;
            break;
        case BOARD_VERSION_1_5:
            g_board_info.target_cfg->rt_board_id = board_id_mb_1_5;
            break;
        default:
            g_board_info.target_cfg->rt_board_id = board_id_mb_1_5;
            break;
    }
}

// Called in main_task() to init before USB and files are configured
static void prerun_board_config(void) {
    // With only two boards the digital pin read maps directly to the type
    mb_version_t board_version = (mb_version_t)read_board_type_pin();
    set_board_id(board_version);
}

extern target_cfg_t target_device_nrf51822_16;

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .family_id = kNordic_Nrf51_FamilyID,
    .daplink_url_name = "MICROBITHTM",
    .daplink_drive_name = "MICROBIT   ",
    .daplink_target_url = "https://microbit.org/device/?id=@B&v=@V",
    .prerun_board_config = prerun_board_config,
    .target_cfg = &target_device_nrf51822_16,
    .board_vendor = "Micro:bit Educational Foundation",
    .board_name = "BBC micro:bit V1",
};
