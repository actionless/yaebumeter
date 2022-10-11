#!/bin/bash
SCRIPT_DIR="$(readlink -e "$(dirname "$0")")"

mkdir -p "${SCRIPT_DIR}/fake_share/share"
ln -s "$SCRIPT_DIR/share" "${SCRIPT_DIR}/fake_share/share/ebumeter"

sed -e '/native/d' \
  -i source/Makefile
#make -C source PREFIX=/usr
make clean -C source
make -C source PREFIX="${SCRIPT_DIR}/fake_share/"
