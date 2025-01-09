add_definitions(-DSTATICLIB)

# Enable testing for this directory
enable_testing()

# Set modern test configurations
set(CMAKE_TESTING_ENABLED ON)
set(BUILD_TESTING ON)

# Find GTest package
find_package(GTest REQUIRED)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../version)

# Set test categories
set(TEST_CATEGORIES
    Core
    Integration
    NodeRpcProxy
    Performance
    System
    Difficulty
    Functional
)

# Collect test sources
file(GLOB_RECURSE CoreTests CoreTests/*)
file(GLOB_RECURSE IntegrationTestLibrary IntegrationTestLib/*)
file(GLOB_RECURSE IntegrationTests IntegrationTests/*)
file(GLOB_RECURSE FunctionalTests FunctionalTests/*)
file(GLOB_RECURSE NodeRpcProxyTests NodeRpcProxyTests/*)
file(GLOB_RECURSE PerformanceTests PerformanceTests/*)
file(GLOB_RECURSE SystemTests System/*)
file(GLOB_RECURSE TestGenerator TestGenerator/*)
file(GLOB_RECURSE CryptoNoteProtocol ../src/CryptoNoteProtocol/*)
file(GLOB_RECURSE P2p ../src/P2p/*)

# Organize source groups
source_group("Core Tests" FILES ${CoreTests})
source_group("Integration Tests" FILES ${IntegrationTests})
source_group("Functional Tests" FILES ${FunctionalTests})
source_group("Node RPC Proxy Tests" FILES ${NodeRpcProxyTests})
source_group("Performance Tests" FILES ${PerformanceTests})
source_group("System Tests" FILES ${SystemTests})
source_group("Test Generator" FILES ${TestGenerator})
source_group("Protocol" FILES ${CryptoNoteProtocol})
source_group("P2P" FILES ${P2p})

# Build test libraries
add_library(IntegrationTestLibrary ${IntegrationTestLibrary})
add_library(TestGenerator ${TestGenerator})

# Build test executables
add_executable(CoreTests ${CoreTests})
add_executable(IntegrationTests ${IntegrationTests})
add_executable(NodeRpcProxyTests ${NodeRpcProxyTests})
add_executable(PerformanceTests ${PerformanceTests})
add_executable(SystemTests ${SystemTests})
add_executable(DifficultyTests Difficulty/Difficulty.cpp)
add_executable(FunctionalTests ${FunctionalTests})

# Set common compiler flags for tests
if(MSVC)
    set(TEST_COMPILE_FLAGS "/W4")
else()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(TEST_COMPILE_FLAGS "-Wall -Wextra -Werror")
    endif()
endif()

# Set common properties for all test targets
foreach(TEST_TARGET
    CoreTests
    IntegrationTests
    NodeRpcProxyTests
    PerformanceTests
    SystemTests
    DifficultyTests
    FunctionalTests
)
    set_target_properties(${TEST_TARGET} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    if(DEFINED TEST_COMPILE_FLAGS)
        set_target_properties(${TEST_TARGET} PROPERTIES COMPILE_FLAGS "${TEST_COMPILE_FLAGS}")
    endif()
endforeach()

# Link libraries
target_link_libraries(CoreTests 
    PRIVATE
    TestGenerator
    CryptoNoteCore
    Serialization
    System
    Logging
    Common
    crypto
    BlockchainExplorer
    ${Boost_LIBRARIES}
    GTest::GTest
    GTest::Main
)

target_link_libraries(IntegrationTests 
    PRIVATE
    IntegrationTestLibrary
    Wallet
    NodeRpcProxy
    InProcessNode
    P2P
    Rpc
    Http
    Transfers
    Serialization
    System
    CryptoNoteCore
    Logging
    Common
    crypto
    BlockchainExplorer
    gtest
    upnpc-static
    ${Boost_LIBRARIES}
)

target_link_libraries(NodeRpcProxyTests 
    PRIVATE
    NodeRpcProxy
    CryptoNoteCore
    Rpc
    Http
    Serialization
    System
    Logging
    Common
    crypto
    ${Boost_LIBRARIES}
)

target_link_libraries(PerformanceTests 
    PRIVATE
    CryptoNoteCore
    Serialization
    Logging
    Common
    crypto
    ${Boost_LIBRARIES}
)

target_link_libraries(SystemTests 
    PRIVATE
    System
    GTest::GTest
    GTest::Main
)

target_link_libraries(DifficultyTests 
    PRIVATE
    CryptoNoteCore
    Serialization
    crypto
    Logging
    Common
    ${Boost_LIBRARIES}
)

# Windows-specific libraries
if (MSVC)
    target_link_libraries(SystemTests PRIVATE ws2_32)
    target_link_libraries(NodeRpcProxyTests PRIVATE ws2_32)
    target_link_libraries(CoreTests PRIVATE ws2_32)
endif()

# Add test discovery
include(GoogleTest)
gtest_discover_tests(CoreTests)
gtest_discover_tests(IntegrationTests)
gtest_discover_tests(NodeRpcProxyTests)
gtest_discover_tests(SystemTests)
gtest_discover_tests(DifficultyTests)
gtest_discover_tests(FunctionalTests)

# Create test labels
foreach(CATEGORY ${TEST_CATEGORIES})
    add_custom_target(test_${CATEGORY}
        COMMAND ctest -L ${CATEGORY} --output-on-failure
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endforeach()

# Main test target
add_custom_target(tests 
    DEPENDS 
        CoreTests
        IntegrationTests
        NodeRpcProxyTests
        PerformanceTests
        SystemTests
        DifficultyTests
        FunctionalTests
)

# Set target properties for better organization
set_target_properties(
    tests
    CoreTests
    IntegrationTestLibrary
    TestGenerator
    IntegrationTests
    NodeRpcProxyTests
    PerformanceTests
    SystemTests
    DifficultyTests
    FunctionalTests
    PROPERTIES
    FOLDER "Tests"
)

# Add test coverage if enabled
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add coverage flags
    target_compile_options(tests PRIVATE --coverage -O0 -g)
    target_link_libraries(tests PRIVATE --coverage)
endif()
