#include "civetweb.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

void fetchManifest(const std::string& uri) {
    char error[128];
    struct mg_connection *conn;

    int port = 8080;
    int use_ssl = 0;
    // Connect to the URI
    conn = mg_connect_client(uri.c_str(), port, use_ssl, (char*)&error, sizeof(error));
    if (conn == NULL) {
        std::cerr << "Failed to connect to " << uri << ": " << error << std::endl;
        return;
    }

    // Send a GET request
    mg_printf(conn, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", uri.c_str(), uri.c_str());

    // Read the response
    char buffer[1024];
    int bytes_read;
    std::ofstream manifestFile("manifest.m3u8");
    bool headers_done = false;
    while ((bytes_read = mg_read(conn, buffer, sizeof(buffer))) > 0) {
        if (!headers_done) {
            // Find the end of headers (double newline)
            char* body_start = strstr(buffer, "\r\n\r\n");
            if (body_start) {
                headers_done = true;
                body_start += 4; // Move past the double newline
                manifestFile.write(body_start, bytes_read - (body_start - buffer));
            }
        } else {
            manifestFile.write(buffer, bytes_read);
        }
    }
    manifestFile.close();

    // Clean up
    mg_close_connection(conn);
}

int main(int argc, char *argv[]) {
    string uri("http://127.0.0.1:8080/output.m3u8");
    cout << "HLS client connecting to localhost on port 8080..." << endl;
    fetchManifest(uri);
    return 0;
}