cmake_minimum_required(VERSION 3.10)

project(CryptoParser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(source/Binance)

include_directories(include/Binance)
include_directories(include/)
include_directories(libs)

link_directories(libs)

set(SOURCES 
    source/main.cpp 
    source/Binance/BinanceClient.cpp 
    source/Binance/Position.cpp 
    source/Binance/Trade.cpp
    source/Parser/ExcelParser.cpp 
    source/Parser/Parser.cpp 
    source/Parser/ParserFactory.cpp 
    source/Parser/TextParser.cpp
)

add_executable(cparser ${SOURCES})

target_link_libraries(cparser Binance xlnt curl crypto ssl)

add_custom_command(
	TARGET cparser 
	POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cparser ${CMAKE_CURRENT_SOURCE_DIR}/cparser # 1 link (in the Root)
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cparser $ENV{HOME}/Desktop/cparser          # 2 link (in the Desktop)
    )

    


    
    
    