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

#ifndef SFM_COMMON_H
#define SFM_COMMON_H

#include "sensirion_arch_config.h"

#define SFM_CMD_READ_PRODUCT_IDENTIFIER 0xE102

#define SFM_CMD_READ_SCALE_FACTOR_OFFSET_AND_FLOW_UNIT 0x3661

#define SFM_CMD_STOP_CONTINUOUS_MEASUREMENT 0x3FF9

typedef enum {
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS0 = 0x3603,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS1 = 0x3608,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS2 = 0x3615,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS3 = 0x361E,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS4 = 0x3624,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS5 = 0x362F,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS_MIX_0 = 0x3632,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS_MIX_1 = 0x3639,
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS_MIX_2 = 0x3646,
} SfmCmdStartContinuousMeasurement;

typedef struct {
    uint8_t i2c_address;
    int16_t flow_scale;
    int16_t flow_offset;
} SfmConfig;

/**
 * Return the driver version
 * @return  Driver version string
 */
const char* sfm_common_get_driver_version(void);

/**
 * Detects if a sensor is connected by reading out the ID register.
 * If the sensor does not answer or if the answer is not the expected value,
 * the test fails.
 *
 * @return 0 if a sensor was detected
 */
int16_t sfm_common_probe(uint8_t i2c_address);

/**
 * Read the product identifier, consisting of a product number and a serial
 * number.
 */
int16_t sfm_common_read_product_identifier(uint8_t i2c_address,
                                           uint32_t* product_number,
                                           uint8_t (*serial_number)[8]);

/**
 * Read the scale factor, offset and unit for the given measurement type.
 *
 * @return  0 on success, an error code otherwise
 */
int16_t sfm_common_read_scale_factor_offset_and_unit(
    const SfmConfig* sfm_config,
    SfmCmdStartContinuousMeasurement measurement_cmd, int16_t* flow_scale,
    int16_t* flow_offset, uint16_t* unit);

/**
 * Convert the raw flow ticks to slm.
 *
 * @return  0 on success, an error code otherwise
 */
int16_t sfm_common_convert_flow_float(const SfmConfig* sfm_config,
                                      int16_t flow_raw, float* flow);

/**
 * Starts a continuous measurement with the given gas configuration.
 */
int16_t sfm_common_start_continuous_measurement(
    SfmConfig* sfm_config, SfmCmdStartContinuousMeasurement measurement_cmd);

/**
 * Read results of a continuous measurement
 */
int16_t sfm_common_read_measurement(const SfmConfig* sfm_config, int16_t* flow,
                                    int16_t* temperature, uint16_t* status);

/**
 * Stops a continuous measurement.
 */
int16_t sfm_common_stop_continuous_measurement(SfmConfig* sfm_config);

#endif /* SFM_COMMON_H */
