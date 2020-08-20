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

#include "sfm_common.h"
#include "sensirion_common.h"
#include "sfm_git_version.h"

const char* sfm_common_get_driver_version(void) {
    return SFM_DRV_VERSION_STR;
}

int16_t sfm_common_probe(uint8_t i2c_address) {
    uint16_t buf[6];
    return sensirion_i2c_read_cmd(i2c_address, SFM_CMD_READ_PRODUCT_IDENTIFIER,
                                  buf, 6);
}

int16_t sfm_common_read_product_identifier(uint8_t i2c_address,
                                           uint32_t* product_number,
                                           uint8_t (*serial_number)[8]) {
    uint8_t buf[6 * 2];
    int16_t error =
        sensirion_i2c_write_cmd(i2c_address, SFM_CMD_READ_PRODUCT_IDENTIFIER);
    if (error) {
        return error;
    }
    error = sensirion_i2c_read_words_as_bytes(i2c_address, buf, 6);
    if (error) {
        return error;
    }
    if (product_number) {
        *product_number = ((uint32_t)buf[0] << 24) + ((uint32_t)buf[1] << 16) +
                          ((uint32_t)buf[2] << 8) + (uint32_t)buf[3];
    }
    if (serial_number) {
        for (size_t i = 0; i < 8; ++i) {
            (*serial_number)[i] = buf[i + 4];
        }
    }
    return 0;
}

int16_t sfm_common_read_scale_factor_offset_and_unit(
    const SfmConfig* sfm_config,
    SfmCmdStartContinuousMeasurement measurement_cmd, int16_t* flow_scale,
    int16_t* flow_offset, uint16_t* unit) {

    uint16_t measurement_cmd_word = (uint16_t)measurement_cmd;

    uint16_t buf[3];
    int16_t error = sensirion_i2c_write_cmd_with_args(
        sfm_config->i2c_address, SFM_CMD_READ_SCALE_FACTOR_OFFSET_AND_FLOW_UNIT,
        &measurement_cmd_word, 1);
    if (error) {
        return error;
    }
    error =
        sensirion_i2c_read_words(sfm_config->i2c_address, buf, ARRAY_SIZE(buf));

    if (error) {
        return error;
    }
    if (flow_scale) {
        *flow_scale = (int16_t)buf[0];
    }
    if (flow_offset) {
        *flow_offset = (int16_t)buf[1];
    }
    if (unit) {
        *unit = buf[2];
    }
    return 0;
}

int16_t sfm_common_convert_flow_float(const SfmConfig* sfm_config,
                                      int16_t flow_raw, float* flow) {
    if (sfm_config->flow_scale == 0) {
        return -1;
    }

    *flow = ((int32_t)flow_raw - (int32_t)sfm_config->flow_offset) /
            (float)(sfm_config->flow_scale);

    return 0;
}

float sfm_common_convert_temperature_float(int16_t temperature_raw) {
    return temperature_raw / 200.0f;
}

int16_t sfm_common_start_continuous_measurement(
    SfmConfig* sfm_config, SfmCmdStartContinuousMeasurement measurement_cmd) {

    int16_t error = sfm_common_read_scale_factor_offset_and_unit(
        sfm_config, measurement_cmd, &sfm_config->flow_scale,
        &sfm_config->flow_offset, NULL);
    if (error) {
        return error;
    }

    return sensirion_i2c_write_cmd(sfm_config->i2c_address, measurement_cmd);
}

int16_t sfm_common_read_measurement_raw(const SfmConfig* sfm_config,
                                        int16_t* flow_raw,
                                        int16_t* temperature_raw,
                                        uint16_t* status) {
    uint16_t buf[3] = {};
    int16_t error = sensirion_i2c_read_words(sfm_config->i2c_address, buf, 3);
    if (error) {
        return error;
    }
    if (flow_raw) {
        *flow_raw = (int16_t)buf[0];
    }
    if (temperature_raw) {
        *temperature_raw = (int16_t)buf[1];
    }
    if (status) {
        *status = buf[2];
    }
    return 0;
}

int16_t sfm_common_stop_continuous_measurement(SfmConfig* sfm_config) {
    sfm_config->flow_scale = 0;
    sfm_config->flow_offset = 0;
    return sensirion_i2c_write_cmd(sfm_config->i2c_address,
                                   SFM_CMD_STOP_CONTINUOUS_MEASUREMENT);
}
