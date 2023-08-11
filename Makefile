# Variables to hold the compiler and the flags.
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude/Binance -Iinclude/ -Ilibs -Llibs/

# List all your sources here.
SOURCES = source/main.cpp source/Binance/BinanceClient.cpp source/Binance/Position.cpp source/Binance/Trade.cpp \
          source/Parser/ExcelParser.cpp source/Parser/Parser.cpp source/Parser/ParserFactory.cpp source/Parser/TextParser.cpp

# List all your object files here.
OBJECTS = source/main.o source/Binance/BinanceClient.o source/Binance/Position.o source/Binance/Trade.o \
          source/Parser/ExcelParser.o source/Parser/Parser.o source/Parser/ParserFactory.o source/Parser/TextParser.o

# The name of the output file.
OUTPUT = bin/main

# The libraries to link with.
LIBS = -lxlnt -lcurl -lcrypto -lssl

# The rule to build everything.
all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(OBJECTS) $(LIBS)

# The rules to build the object files.
source/main.o: source/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Binance/BinanceClient.o: source/Binance/BinanceClient.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Binance/Position.o: source/Binance/Position.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Binance/Trade.o: source/Binance/Trade.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Parser/ExcelParser.o: source/Parser/ExcelParser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Parser/Parser.o: source/Parser/Parser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Parser/ParserFactory.o: source/Parser/ParserFactory.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

source/Parser/TextParser.o: source/Parser/TextParser.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# The rule to clean up everything.
clean:
	rm -f $(OUTPUT) $(OBJECTS)
