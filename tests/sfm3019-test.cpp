#include "sensirion_test_setup.h"
#include "sfm3019.h"

TEST_GROUP(SFM3019) {
    void setup() {
        sensirion_i2c_init();
        int16_t error = sensirion_i2c_mux_set_single_channel(0x72, 2);
        CHECK_EQUAL_TEXT(0, error, "sensirion_i2c_mux_set_single_channel(0x72, 2)");
        error = sensirion_i2c_general_call_reset();
        CHECK_EQUAL_TEXT(0, error, "sensirion_i2c_general_call_reset");
        /* Wait for the SFM3019 to initialize */
        sensirion_sleep_usec(SFM3019_SOFT_RESET_TIME_US);
    }

    void teardown() {
        sensirion_i2c_release();
    }
};

TEST(SFM3019, probe) {
    int16_t error = sfm3019_probe();
    CHECK_EQUAL_TEXT(0, error, "probe failed");
}


TEST(SFM3019, read_product_identifier) {
    uint32_t product_number = 0;
    uint8_t serial_number[8] = {};
    int16_t error = sfm_common_read_product_identifier(
            SFM3019_I2C_ADDRESS, &product_number, &serial_number);
    CHECK_EQUAL(0, error);
}

TEST(SFM3019, measurement) {
    SfmConfig sfm3019 = sfm3019_create();

    int16_t error = sfm_common_start_continuous_measurement(
        &sfm3019, SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR);
    CHECK_EQUAL_TEXT(0, error, "start measurement failed");

    /* Wait for the first measurement to be available. Wait for
     * SFM3019_MEASUREMENT_WARM_UP_TIME_US instead for more reliable results */
    sensirion_sleep_usec(SFM3019_MEASUREMENT_INITIALIZATION_TIME_US);

    int16_t flow_raw;
    int16_t temperature_raw;
    uint16_t status;
    error = sfm_common_read_measurement_raw(&sfm3019, &flow_raw,
                                            &temperature_raw, &status);
    CHECK_EQUAL_TEXT(0, error, "read measurement failed");
}
