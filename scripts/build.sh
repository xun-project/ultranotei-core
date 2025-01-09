#!/bin/bash
# UltraNote Infinity Core Build Script for Unix-based systems

set -e

# Default values
BUILD_TYPE="Release"
RUN_TESTS=false
CLEAN=false
PLATFORM=$(uname -s)

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --build-type=*)
            BUILD_TYPE="${1#*=}"
            ;;
        --run-tests)
            RUN_TESTS=true
            ;;
        --clean)
            CLEAN=true
            ;;
        *)
            echo "Unknown parameter: $1"
            exit 1
            ;;
    esac
    shift
done

# Print header function
print_header() {
    echo -e "\n=== $1 ===\n"
}

# Initialize build environment
init_build() {
    print_header "Initializing Build Environment"
    
    if [ "$CLEAN" = true ] && [ -d "build" ]; then
        echo "Cleaning previous build..."
        rm -rf build
    fi
    
    mkdir -p build
}

# Build for Linux
build_linux() {
    print_header "Building for Linux"
    
    cd build
    
    # Configure
    echo "Configuring CMake..."
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
    
    # Build
    echo "Building project..."
    make -j$(nproc)
    
    # Run tests if requested
    if [ "$RUN_TESTS" = true ]; then
        echo "Running tests..."
        ctest --output-on-failure
    fi
}

# Build for macOS
build_macos() {
    print_header "Building for macOS"
    
    cd build
    
    # Configure
    echo "Configuring CMake..."
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
          -DOPENSSL_ROOT_DIR=$(brew --prefix openssl) \
          -DBOOST_ROOT=$(brew --prefix boost@1.72) ..
    
    # Build
    echo "Building project..."
    make -j$(sysctl -n hw.ncpu)
    
    # Run tests if requested
    if [ "$RUN_TESTS" = true ]; then
        echo "Running tests..."
        ctest --output-on-failure
    fi
}

# Main execution
main() {
    init_build
    
    case "$PLATFORM" in
        Linux*)
            build_linux
            ;;
        Darwin*)
            build_macos
            ;;
        *)
            echo "Unsupported platform: $PLATFORM"
            exit 1
            ;;
    esac
    
    print_header "Build Completed Successfully!"
}

# Execute main function
main
