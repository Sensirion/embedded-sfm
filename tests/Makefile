include ../embedded-common/test-config/base_config.inc
sfm_driver_dir := ${driver_dir}/embedded-sfm
include ${sfm_driver_dir}/sfm3019/default_config.inc

sfm3019_test_binaries := sfm3019-test-hw_i2c sfm3019-test-sw_i2c

.PHONY: all clean prepare test

all: clean prepare test

prepare:
	cd ${sfm_driver_dir} && $(MAKE) prepare

sfm3019-test-hw_i2c: sfm3019-test.cpp ${sfm3019_sources} ${hw_i2c_sources} ${sensirion_test_sources}
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sfm3019-test-sw_i2c: CONFIG_I2C_TYPE := sw_i2c
sfm3019-test-sw_i2c: sfm3019-test.cpp ${sfm3019_sources} ${sw_i2c_sources} ${sensirion_test_sources}
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) ${sfm3019_test_binaries}

test: prepare ${sfm3019_test_binaries}
	set -ex; for test in ${sfm3019_test_binaries}; do echo $${test}; ./$${test}; echo; done;
