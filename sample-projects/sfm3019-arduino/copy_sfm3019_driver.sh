#!/bin/bash

set -euo pipefail

BASE_DIR=$(dirname "$0")

mkdir -p "$BASE_DIR"/
cp "$BASE_DIR/../../embedded-common/"*.[ch] "$BASE_DIR"/
cp "$BASE_DIR/../../embedded-common/hw_i2c/sample-implementations/arduino/"*.cpp "$BASE_DIR"/
cp "$BASE_DIR/../../sfm3019/sfm3019."[ch] "$BASE_DIR"/
cp "$BASE_DIR/../../sfm-common/"*.[ch] "$BASE_DIR"/
gitversion=$(git describe --always --dirty)
cat << EOF > "$BASE_DIR/sfm_git_version.c"
/* THIS FILE IS AUTOGENERATED */
#include "sfm_git_version.h"
const char * SFM_DRV_VERSION_STR = "$gitversion";
EOF
