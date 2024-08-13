#include "civetweb.h"
#include "hls.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libavformat/avformat.h>

#define MAX_BUFFER_SIZE 8192


// read manifest from connection body, parse and store in *manifest struct
void parseManifest(struct mg_connection *conn, hls::manifest *manifest) {
    // loop over lines
}

int main() {
    char error_buffer[256];
    struct mg_connection *conn;
    int ret;
    char buffer[MAX_BUFFER_SIZE];
    int bytes_read;

    mg_init_library(0);

    conn = mg_connect_client("localhost", 8080, 0, error_buffer, sizeof(error_buffer));

    if (conn == NULL) {
        printf("Error connecting: %s\n", error_buffer);
        return 1;
    }

    mg_printf(conn, "GET /output.m3u8 HTTP/1.1\r\n"
                    "Host: localhost:8080\r\n"
                    "Connection: close\r\n\r\n");

    ret = mg_get_response(conn, error_buffer, sizeof(error_buffer), 10000);
    if (ret < 0) {
        printf("Error getting response: %s\n", error_buffer);
        mg_close_connection(conn);
        return 1;
    }

    // Read and print the response headers
    const struct mg_response_info *ri = mg_get_response_info(conn);

    hls::manifest manifest;
    parseManifest(conn, &manifest);

    // Close the connection
    mg_close_connection(conn);

    // Cleanup
    mg_exit_library();

    return 0;
}
