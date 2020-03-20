# See user_config.inc for build customization
-include user_config.inc
include default_config.inc


.PHONY: all clean

all: sfm3003_example_usage

sfm3003_example_usage: clean
	$(CC) $(CFLAGS) -o $@ ${sfm3003_sources} ${${CONFIG_I2C_TYPE}_sources} ${sfm3003_dir}/sfm3003_example_usage.c

clean:
	$(RM) sfm3003_example_usage
