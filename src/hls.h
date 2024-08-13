#include <vector>
#include <string>

using std::vector;
using std::string;

namespace hls {
    // An HLS Manifest is a text file that contains metadata
    // for a video stream and it indexes segments (the .ts files)
    //
    // Example:
    //   #EXTM3U
    //   #EXT-3-VERSION:3
    //   #EXT-X-TARGETDURATION:4
    //   #EXT-X-MEDIA-SEQUENCE:0
    //   #EXTINF:3.500000,         // duration,optional title
    //   output_0.ts               // 3.5 seconds
    //   #EXTINF:1.750000,
    //   output_1.ts               // 1.75 seconds
    //   ..
    //   #EXT-X-ENDLIST
    //   ^____________^
    //       (the #EXT-.. things are called tags)
    struct manifest {
        struct segment {
            int duration;    // duration in seconds
            string title;    // (optional)
            string filename; // output_1.ts
        };
        int version;
        int target_duration;
        int media_sequence;
        vector<segment> segments;
    };
};
