#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct AudioClip {
    vector<vector<float>> chunk;
    int clipId;
};

class Comparator {
public:
    //stores chunk into memory
    void store_chunk(const vector<vector<float>>& chunk) {storedClips.push_back({chunk, nextId++}); };;;
    /// @brief Returns clipID of best match in stored clips.
    ///
    ///(ideally switch to a hash system or similar later)
    /// @param queryChunk The audio fingerprint chunk to match against stored clips
    /// @return The clip ID with the highest match score, or -1 if no clips are similar enough within SIMILARITY_THRESHOLD
    int find_best_match(const vector<vector<float>>& queryChunk);
private:
     //returns similarity as a float 0-1
    float compare_chunk(const vector<vector<float>>& a, const vector<vector<float>>& b);
    //to be replaced with a real storage system
    vector<AudioClip> storedClips;
    int nextId = 0;
};
#endif