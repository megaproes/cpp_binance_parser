# source/Binance/CMakeLists.txt
set(BINANCE_SOURCES
    BinanceClient.cpp
    Position.cpp
    Trade.cpp
)

# Create a library target for the Binance sources
add_library(Binance ${BINANCE_SOURCES})

# Specify the include directories for this library
target_include_directories(Binance PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include/Binance)
