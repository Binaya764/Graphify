#!/bin/bash
# Graphify Build Script
# Usage: ./build.sh [debug|release]

BUILD_TYPE="${1:-release}"
BUILD_DIR="build-${BUILD_TYPE}"

echo "╔══════════════════════════════════════╗"
echo "║       GRAPHIFY Build System          ║"
echo "╚══════════════════════════════════════╝"
echo ""

# Check dependencies
check_dep() {
    if ! command -v "$1" &>/dev/null; then
        echo "❌  $1 not found. Please install it."
        exit 1
    fi
    echo "✓  $1 found"
}

check_dep cmake
check_dep ninja || check_dep make

echo ""
echo "► Build type: ${BUILD_TYPE}"
echo "► Output dir: ${BUILD_DIR}/"
echo ""

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

CMAKE_BUILD_TYPE="Release"
[ "${BUILD_TYPE}" = "debug" ] && CMAKE_BUILD_TYPE="Debug"

cmake .. \
    -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    $(command -v ninja &>/dev/null && echo "-GNinja")

if [ $? -ne 0 ]; then
    echo ""
    echo "❌  CMake configuration failed."
    exit 1
fi

echo ""
echo "► Compiling..."
cmake --build . --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -eq 0 ]; then
    echo ""
    echo "╔══════════════════════════════════════╗"
    echo "║       ✓  Build Successful!           ║"
    echo "╚══════════════════════════════════════╝"
    echo ""
    echo "Run with:  ./${BUILD_DIR}/Graphify"
else
    echo ""
    echo "❌  Build failed. Check errors above."
    exit 1
fi
