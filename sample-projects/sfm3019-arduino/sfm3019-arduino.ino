#include "sfm3019.h"

int error = 0;
SfmConfig sfm3019;

void setup() {
    Serial.begin(9600);
    const char* driver_version = sfm_common_get_driver_version();
    if (driver_version) {
        Serial.print("SFM driver version: ");
        Serial.println(driver_version);
    } else {
        Serial.println("fatal: Getting driver version failed");
        error = -1;
    }
    sensirion_i2c_init();

    /* Reset all I2C devices */
    int16_t error = sensirion_i2c_general_call_reset();
    if (error) {
        Serial.println("General call reset failed");
    }

    /* Wait for the SFM3019 to initialize */
    sensirion_sleep_usec(SFM3019_SOFT_RESET_TIME_US);

    while (sfm3019_probe()) {
        Serial.println("SFM sensor probing failed");
        sensirion_sleep_usec(100000);
    }

    uint32_t product_number = 0;
    uint8_t serial_number[8] = {};
    error = sfm_common_read_product_identifier(SFM3019_I2C_ADDRESS,
                                               &product_number, &serial_number);
    if (error) {
        Serial.println("Failed to read product identifier");
    } else {
        Serial.print("product: 0x");
        Serial.print(product_number, HEX);
        Serial.print("serial: 0x");
        for (size_t i = 0; i < 8; ++i) {
            Serial.print(serial_number[i], HEX);
        }
        Serial.println("");
    }
    
    sfm3019 = sfm3019_create();

    error = sfm_common_start_continuous_measurement(
        &sfm3019, SFM3019_CMD_START_CONTINUOUS_MEASUREMENT_AIR);

    if (error) {
        Serial.println("Failed to start measurement");
    }
 
    /* Wait for the first measurement to be available. Wait for
     * SFM3019_MEASUREMENT_WARM_UP_TIME_US instead for more reliable results */
    sensirion_sleep_usec(SFM3019_MEASUREMENT_INITIALIZATION_TIME_US);

}

void loop() {

    // do nothing if there were errors during setup
    if (error) {
      return;
    }

    int16_t flow_raw;
    int16_t temperature_raw;
    uint16_t status;
    error = sfm_common_read_measurement_raw(&sfm3019, &flow_raw,
                                            &temperature_raw, &status);
    if (error) {
        Serial.println("Error while reading measurement");
    } else {
        float flow;
        float temperature;
        error = sfm_common_convert_flow_float(&sfm3019, flow_raw, &flow);
        if (error) {
            Serial.println("Error while converting flow");
        }
        temperature = sfm_common_convert_temperature_float(temperature_raw);
        Serial.print(" Flow: ");
        Serial.print(flow);
        Serial.print(" Temperature: ");
        Serial.print(temperature);
        Serial.print(" Status: ");
        Serial.print(status, HEX);
    }

}
