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

#ifndef SFM3003_H
#define SFM3003_H

#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sfm_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SFM3003_I2C_ADDRESS 0x28

#define SFM3003_CMD_START_CONTINUOUS_MEASUREMENT_O2 \
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS0
#define SFM3003_CMD_START_CONTINUOUS_MEASUREMENT_AIR \
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS1
#define SFM3003_CMD_START_CONTINUOUS_MEASUREMENT_AIR_O2_MIX \
    SFM_CMD_START_CONTINUOUS_MEASUREMENT_GAS_MIX_0

#define SFM3003_SOFT_RESET_TIME_US 2000

#define SFM3003_MEASUREMENT_INITIALIZATION_TIME_US 12000

#define SFM3003_MEASUREMENT_WARM_UP_TIME_US 30000

/**
 * Detects if a sensor is connected by reading out the ID register.
 * If the sensor does not answer or if the answer is not the expected value,
 * the test fails.
 *
 * @return 0 if a sensor was detected
 */
int16_t sfm3003_probe(void);

/**
 * Create a new SFM3003 instance
 */
SfmConfig sfm3003_create(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SFM3003_H */
