/*
 * Copyright (c) 2020, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_i2c.h"
#include "sfm3003.h"
#include <stdio.h>

int main() {

    const char* driver_version = sfm_common_get_driver_version();
    if (driver_version) {
        printf("SGPC3 driver version %s\n", driver_version);
    } else {
        printf("fatal: Getting driver version failed\n");
        return -1;
    }

    /* Initialize I2C bus */
    sensirion_i2c_init();

    while (sfm3003_probe()) {
        printf("SFM sensor probing failed\n");
    }

    SfmConfig sfm3003 = sfm3003_create();

    int16_t error = sfm_common_stop_continuous_measurement(&sfm3003);
    if (error) {
        printf("Failed to stop previous measurement\n");
    }

    error = sfm_common_start_continuous_measurement(
        &sfm3003, SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS0);
    if (error) {
        printf("Failed to start measurement\n");
    }

    for (;;) {
        int16_t flow;
        int16_t temperature;
        uint16_t status;
        error =
            sfm_common_read_measurement(&sfm3003, &flow, &temperature, &status);
        printf(" %4i %4i %04x\n", flow, temperature, status);
    }
    return 0;
}
