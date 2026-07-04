#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build-win64"

cmake -B "$BUILD_DIR" -S . \
    -DCMAKE_TOOLCHAIN_FILE=toolchain/mingw64.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build "$BUILD_DIR" --parallel "$(nproc)"

echo ""
echo "--- Outputs ---"
ls -lh "$BUILD_DIR/bin/"

echo ""
echo "--- Copying to ~/vm_share ---"
cp "$BUILD_DIR/bin/"*.exe ~/vm_share/ 2>/dev/null && echo "  husk_external.exe copied" || true
cp "$BUILD_DIR/bin/"*.dll ~/vm_share/ 2>/dev/null && echo "  husk_internal.dll copied" || true
