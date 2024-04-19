# embedded-sfm [![GitHub license](https://img.shields.io/badge/license-BSD3-blue.svg)](https://raw.githubusercontent.com/Sensirion/embedded-sfm/master/LICENSE)

Note: This repository is deprecated. Please use our new driver for flow sensors
https://github.com/Sensirion/embedded-i2c-sfm-sf06


This repository contains the embedded driver sources for Sensirion's SFM product
line.

***DOWNLOAD THE RELEASE ZIP FROM THE [RELEASE
PAGE](https://github.com/Sensirion/embedded-sfm/releases)***

## Clone this repository
```
 git clone --recursive https://github.com/Sensirion/embedded-sfm.git
```

## Repository content

* embedded-common (submodule repository for the common embedded driver HAL)
* sfm3003 (SFM3003 driver)
* sfm3019 (SFM3019 driver)
* sfm-common (functionality common across SFM sensors)

## Collecting resources
```
make release
```
This will create the `release` folder with the necessary driver files in it,
including a Makefile. That way, you have just one folder with all the sources
ready to build your driver for your platform. You can find a pre-built release
package on the release page linked at the top of this document.

## Files to adjust (from embedded-common)
You only need to touch the following files:

* `sensirion_arch_config.h` (architecture specifics, you need to specify
the integer sizes)

and depending on your i2c implementation either of the following:

* `embedded-common/hw_i2c/sensirion_hw_i2c_implementation.c`
  functions for hardware i2c communication if your platform supports that
* `embedded-common/sw_i2c/sensirion_sw_i2c_implementation.c`
  functions for software i2c communication via GPIOs

## Building the driver
1. Adjust `sensirion_arch_config.h` if you don't have the `<stdint.h>` header
   file available
2. Implement necessary functions in one of the `*_implementation.c` files
   described above
3. make

---

Please check the [embedded-common](https://github.com/Sensirion/embedded-common)
repository for further information and sample implementations.

---
