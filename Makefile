CXX = g++

SERVER_CXXFLAGS = -std=c++11 -Wall -Wdeprecated-declarations -Iinclude -DNO_SSL
SERVER_SOURCES = src/hls_server.cpp src/civetweb.c
SERVER_TARGET = bin/hls_server

CLIENT_CXXFLAGS = -std=c++11 -Wall -Wdeprecated-declarations -Iinclude -DNO_SSL
CLIENT_CXXFLAGS += -I/opt/homebrew/Cellar/ffmpeg/7.0.1/include
CLIENT_LDFLAGS = -L/opt/homebrew/Cellar/ffmpeg/7.0.1/lib
CLIENT_LDLIBS = -lavformat -lavcodec -lavutil
CLIENT_SOURCES = src/hls_client.cpp src/civetweb.c
CLIENT_TARGET = bin/hls_client

all: server client

server: $(SERVER_TARGET)
client: $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES)
	$(CXX) $(SERVER_CXXFLAGS) -o $(SERVER_TARGET) $(SERVER_SOURCES)

$(CLIENT_TARGET): $(CLIENT_SOURCES)
	$(CXX) $(CLIENT_CXXFLAGS) $(CLIENT_LDFLAGS) -o $(CLIENT_TARGET) $(CLIENT_SOURCES) $(CLIENT_LDLIBS)

clean:
	rm -f $(SERVER_TARGET)
	rm -f $(CLIENT_TARGET)
