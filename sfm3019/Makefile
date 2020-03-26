# See user_config.inc for build customization
-include user_config.inc
include default_config.inc


.PHONY: all clean

all: sfm3019_example_usage

sfm3019_example_usage: clean
	$(CC) $(CFLAGS) -o $@ ${sfm3019_sources} ${${CONFIG_I2C_TYPE}_sources} ${sfm3019_dir}/sfm3019_example_usage.c

clean:
	$(RM) sfm3019_example_usage
