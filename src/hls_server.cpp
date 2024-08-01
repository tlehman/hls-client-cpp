#include "civetweb.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Function to read the content of a file
string readFile(const string &filePath) {
    ifstream file(filePath);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Callback for handling HTTP requests
int requestHandler(struct mg_connection *conn) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    string uri(request_info->local_uri);

    // Serve the playlist file
    if (uri == "/output.m3u8") {
        string content = readFile("./data/output.m3u8");
        // HLS Headers
        mg_send_http_ok(conn, "application/vnd.apple.mpegurl", content.size());
        // HLS Body
        mg_write(conn, content.c_str(), content.size());
        return 1;
    }

    // Serve the segment files
    if (uri.find("/output_") == 0 && uri.find(".ts") != string::npos) {
        string filePath = "./data" + uri; // Adjust path if necessary
        cout << "Serving " << filePath << endl;
        string content = readFile(filePath);
        // HLS Headers
        mg_send_http_ok(conn, "video/MP2T", content.size());
        // HLS Body
        mg_write(conn, content.c_str(), content.size());
        return 1;
    }

    // 404 Not Found for other URIs
    mg_printf(conn, "HTTP/1.1 404 Not Found\r\n"
                    "Content-Length: 0\r\n\r\n");
    return 1;
}

int main(int argc, char *argv[]) {
    const char *options[] = {"document_root", ".", "listening_ports", "8080", nullptr};
    struct mg_callbacks callbacks;
    struct mg_context *ctx;

    // Initialize the callbacks structure to zero
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = requestHandler;

    // Start the Civetweb server
    ctx = mg_start(&callbacks, nullptr, options);

    if (ctx == nullptr) {
        cout << "Failed to start Civetweb server." << endl;
        return 1;
    }

    cout << "HLS server started on port 8080" << endl;
    cout << "Access the HLS stream at http://localhost:8080/output.m3u8" << endl;

    // Wait until the user hits enter
    getchar();

    // Stop the server
    mg_stop(ctx);

    return 0;
}
