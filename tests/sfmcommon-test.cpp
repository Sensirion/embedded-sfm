#include "sensirion_test_setup.h"
#include "sfm_common.h"

TEST_GROUP(SFMCommon) {
    void setup() {
    }

    void teardown() {
    }
};

TEST(SFMCommon, driver_version) {
    const char* driver_version = sfm_common_get_driver_version();
    TEST_ASSERT(driver_version != NULL, "driver_version NULL");
    printf("SFM driver version %s\n", driver_version);
}
