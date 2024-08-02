# HLS Client in C++

by Tobi Lehman <mail@tobilehman.com>

This is a demo HLS (HTTP Live Stream) player written in C++. It has a basic HLS server too.

Apple created HLS to make a video streaming format that was built on top of HTTP(S).
In a nutshell, the backend is an HTTP server that serves a playlist, the playlist has a bunch of little video file segments, and then the client has to fetch those segments, download them, decode them and play them.

To keep this repo focused on the client side, we will take a 12 second MP4 file and produce an HLS stream of 3 segments using FFmpeg:

## Streaming Client
To build the client, run:
```shell
make client
```
Then to run it, run:
```shell
./bin/hls_client
```

See [Streaming Server](/streaming-server) to get the server running, the client will connect to the server.

### Architecture of the client

The client connects to the server, requests the manifest, and then loops over all the segments, requesting them and buffering them for the player.

```mermaid
sequenceDiagram
    client->>server: GET /manifest.m3u8
    server-->>client:
    client->>server: GET /segment_1.ts
    server-->>client:
    client->>server: GET /segment_2.ts
    server-->>client:

```

The "H" in HLS is for HTTP, those right arrows are regular HTTP GET requests


## Streaming Server

The streaming server is a basic [civetweb](https://github.com/civetweb/civetweb) server that serves up the manifest and segments created by this command:


```shell
(cd data && ffmpeg -i F-35.mp4 -c:v libx264 -g 105 -keyint_min 105 -hls_time 2 -hls_list_size 0 -f hls output.m3u8)
```

To build the server, run:
```shell
make server
```
Then to run it, run:
```shell
./bin/hls_server
```

This will create an HLS endpoint at http://localhost:8080/output.m3u8

## Extracting Keyframes from mp4

To extract all the keyframes from an mp4 file, run this ffmpeg command:


```shell
(cd data/ && ffmpeg -i F-35.mp4 -vf "select='eq(pict_type\,I)'" -vsync vfr keyframe_%04d.png)
```

On the example video I gave, there were only 4 keyframes. This will limit how many segments you can emit from your streaming server.
