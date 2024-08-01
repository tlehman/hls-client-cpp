CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wdeprecated-declarations -Iinclude -DNO_SSL
SOURCES = src/hls_server.cpp src/civetweb.c
TARGET = bin/hls_server

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
