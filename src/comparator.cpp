#include "comparator.h"

constexpr float SIMILARITY_THRESHOLD = 0.1f;  // Adjust as needed
constexpr float QUANTISE = 1.0f;

int Comparator::find_best_match(const vector<vector<float>>& queryChunk) {
    int bestMatch = -1;
    float bestScore = 0;
    for (int i = 0; i < storedClips.size(); i++) {
        float score = compare_chunk(queryChunk, storedClips[i].chunk);
        if (score > bestScore && score > SIMILARITY_THRESHOLD) {
            bestScore = score;
            bestMatch = storedClips[i].clipId;
        }
    }
    return bestMatch;
}
float Comparator::compare_chunk(const vector<vector<float>>& a, const vector<vector<float>>& b) {
    if (a.size() <= 1 || b.size() <= 1) return 0.0f;
    int matchScore = 0;
    const float FREQ_TOLERANCE = 50.0f;  // Peaks within 50Hz are same enough
    // For each time slice in 'a'
    for (int i = 0; i < a.size() - 1; i++) {
        // For each peak in that slice
        for (float anchorFreq : a[i]) {
            // Look for the next 3 peaks in future slices (constellation)
            vector<float> pattern;
            for (int j = i + 1; j < min((int)a.size(), i + 4); j++) {
                if (!a[j].empty()) {
                    pattern.push_back(a[j][0]);  // Take first peak from next slices
                }
            }
            if (pattern.size() < 3) continue;  // Need at least 3 points
            // Search for this constellation in 'b'
            for (int ki = 0; ki < b.size() - 1; ki++) {
                // Look for anchor frequency in this slice
                for (float bAnchor : b[ki]) {
                    if (abs(anchorFreq - bAnchor) < FREQ_TOLERANCE) {
                        // Found anchor! Check if pattern matches
                        int localMatches = 0;
                        for (int px = 0; px < pattern.size() && ki + px + 1 < b.size(); px++) {
                            for (float bFreq : b[ki + px + 1]) {
                                if (abs(pattern[px] - bFreq) < FREQ_TOLERANCE) {
                                    localMatches++;
                                    break;
                                }
                            }
                        }
                        if (localMatches >= 2) matchScore++;
                    }
                }
            }
        }
    }
    return (float)matchScore / max(a.size(), b.size());
}
/* I HATE EVERYTHING
float Comparator::compare_chunk(const vector<vector<float>>& a, const vector<vector<float>>& b) {

    constexpr int lookForward = 1;
    constexpr int required_points = 4;
    constexpr int max_lookahead = 2;
    constexpr int min_local_matches = 3;

    //exit if too small
    if (a.size() <= lookForward || b.size() <= lookForward) {
        return 0.0f;
    }
    compute_indices(a, b);
    float matchScore = 0;
    
    //loop through slices of a
    for (int i = 0; i < a.size()-lookForward; i++) {
        for (int anchor : quantise_indices_a[i]) {
            //find a pattern in a
            vector<int> points;
            points.reserve(required_points);

            int j = 0;
            int search_start = anchor;
            
            while (points.size() < required_points && j < max_lookahead) {
                if (i + j >= a.size()) break; //bounds check

                auto& indices = quantise_indices_a[i + j];
                auto it = upper_bound(indices.begin(), indices.end(), search_start);
                if (it != indices.end()) {
                    points.push_back(*it);
                    search_start = *it;
                } else {
                    j++;
                    search_start = anchor; //reset search position for next row
                }
            }
            
            if (points.size() == required_points) {
            //search for pattern in 'b'
            for (int ki = 1; ki < b.size() - lookForward; ki++) {
                //check if anchor position in b has QUANTISE & use binary search on computed indeces
                auto& b_indices = quantise_indices_b[ki];
                if (binary_search(b_indices.begin(), b_indices.end(), anchor)) {
                    int localMatchScore = 0;

                    //check if the pattern points also match in b
                    for (int px = 0; px < required_points; px++) {
                        for (int sx = 0; sx < lookForward + 1; sx++) {
                            if (ki + sx >= b.size()) break; //bounds check
                            
                            auto& check_indices = quantise_indices_b[ki + sx];
                            if (binary_search(check_indices.begin(), check_indices.end(), points[px]))
                                localMatchScore++;
                        }
                    }
                    if (localMatchScore > min_local_matches)
                    matchScore++;
                }
            }
        }
    }
    return matchScore / b.size();
}
*/

/* js
note that noisePrints[name] is effectively equivalent to a
in other news, i loathe js.
function matchClip(name) {
    console.clear();
    $(".output").html("");
    var scores = {};
    scores.min = 99999999;
    scores.max = 0;
    Object.keys(noisePrints).forEach(function (k) { // go round each song in the 'database'
        if (!k.includes("clip")) {
            var matchScore = 0;
            var lookForward = 1;
            for (var i = 0; i < noisePrints[name].length - lookForward; i++) { // go round each slice of the noiseprint you want to match
                var anchor = -2;
                while (anchor !== -1) {
                    if (anchor === -2) anchor = -1;
                    anchor = noisePrints[name][i].indexOf(quantise, anchor + 1); // <--- what is this line doing?  quantise = 256
                    if (anchor !== -1) {
                        var points = [];
                        var ii = 0;
                        var offset = anchor;
                        while (points.length < 4 && ii < 2) {
                            var newPoint = noisePrints[name][i + ii].indexOf(quantise, offset + 1);
                            if (newPoint !== -1) {
                                points.push(newPoint);
                                offset = newPoint;
                            } else {
                                ii++;
                                offset = -1;
                            }
                        }
                        if (points.length === 4) {
                            for (var ki = 1; ki < noisePrints[k].length - lookForward; ki++) {
                                if (noisePrints[k][ki][anchor] === quantise) {
                                    var localMatchScore = 0;
                                    for (var px = 0; px < 4; px++) {
                                        for (var sx = 0; sx < lookForward + 1; sx++) {
                                            if (noisePrints[k][ki + sx][points[px]] === quantise) {
                                                localMatchScore++;
                                            }
                                        }
                                    }
                                    if (localMatchScore > 3) {
                                        matchScore++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            matchScore = (matchScore / noisePrints[k].length)
            console.log("score: " + k + " " + matchScore);
            scores.min = Math.min(scores.min, matchScore);
            scores.max = Math.max(scores.max, matchScore);
            scores[k] = {};
            scores[k].score = matchScore;
        }
    });
*/