#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <cmath>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#include <iostream> 
#include <chrono>

using namespace std;

const float FREQ_TOLERANCE = 80.0f;  // fully arbitrary :3 (should def do some calculations)
constexpr float SIMILARITY_MINIMUM = 0.95f; //for use in find_best_match to filter out the garbo


struct Fingerprint {
    float anchorFreq;
    vector<float> pattern;  //the 3+ following peaks
    int timeOffset;         //which slice this starts at
};

struct AudioClip {
    unordered_map<size_t, vector<Fingerprint>> fingerprints; //the computed fingerprints of an audio clip
    int clipId; //the id associated with the clip
};

/// @brief hashes a fingerprint from an anchor and a pattern
/// @param anchor anchor point
/// @param pattern peak frequencies
/// @return a size_t hash key
size_t hash_fingerprint(float anchor, const vector<float>& pattern);

/// @brief computes fingerprints for an input chunk
/// @param chunk chunk to compute for
/// @return hash map of fingerprints for a chunk
unordered_map<size_t, vector<Fingerprint>> compute_fingerprints(const vector<vector<float>>& chunk);

class Comparator {
public:
    /// @brief Returns clipID of best match in stored clips
    ///
    ///(ideally switch to a hash system or similar later)
    /// @param queryChunk The audio chunk to match against stored clips
    /// @return The clip ID with the highest match score, or -1 if no clips are similar enough within SIMILARITY_THRESHOLD
    int find_best_match(const vector<vector<float>>& queryChunk);
private:
     //returns similarity as a float 0-1, uses jaccard frequency matching
    float compare_fingerprints(const unordered_map<size_t, vector<Fingerprint>>& a, const unordered_map<size_t, vector<Fingerprint>>& b);
    //to be replaced with a real storage system
    vector<AudioClip> storedClips;
    int nextId = 0;
};
#endif