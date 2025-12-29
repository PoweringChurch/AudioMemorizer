#include "comparator.h"

size_t hash_fingerprint(float anchor, const vector<float>& pattern) {
    size_t h = hash<int>{}(static_cast<int>(anchor / FREQ_TOLERANCE));
    for (float f : pattern) {
        h ^= hash<int>{}(static_cast<int>(f / FREQ_TOLERANCE)) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
}
unordered_map<size_t, vector<Fingerprint>> compute_fingerprints(const vector<vector<float>>& chunk) {
    unordered_map<size_t, vector<Fingerprint>> fingerprints;
    for (int i = 0; i < chunk.size() - 1; i++) {
        for (float anchorFreq : chunk[i]) {
            vector<float> pattern;
            for (int j = i + 1; j < min((int)chunk.size(), i + 4); j++) {
                if (!chunk[j].empty()) {
                    pattern.push_back(chunk[j][0]);
                }
            }
            if (pattern.size() < 3) continue;
            
            Fingerprint fp = {anchorFreq, pattern, i};
            size_t hash = hash_fingerprint(anchorFreq, pattern);
            fingerprints[hash].push_back(fp);
        }
    }
    return fingerprints;
}
int Comparator::find_best_match(const vector<vector<float>>& queryChunk) {
    int bestMatch = -1;
    float bestScore = 0;
    //construct query AudioClip
    AudioClip queryClip = AudioClip();
    queryClip.fingerprints = compute_fingerprints(queryChunk);
    if (queryClip.fingerprints.size() <= 4) return -2; //too short, invalid chunk
    //compare
    cout << "fingerprints size : " << queryClip.fingerprints.size() << endl;
    for (int i = 0; i < storedClips.size(); i++) {
        float score = compare_fingerprints(queryClip.fingerprints, storedClips[i].fingerprints);
        if (score > 0.001)
            cout << storedClips[i].clipId << " comparison score : " << score << endl;
        if (score > bestScore && score > SIMILARITY_MINIMUM) { //if it has the best score so far and its higher than the minimum
            bestScore = score;
            bestMatch = storedClips[i].clipId;
        }
    }
    //output
    if (bestMatch == -1) {
        //store
        queryClip.clipId = nextId;
        cout << RED << "match not found, stored clip at id : " << nextId << RESET << endl;
        nextId++;
        storedClips.push_back(move(queryClip));

    } else {
        cout << GREEN << "match found, id: " << bestMatch << RESET << endl;
    }
    cout << "best score: "<< bestScore << endl;
    return bestMatch;
}

float Comparator::compare_fingerprints(const unordered_map<size_t, vector<Fingerprint>>& a, const unordered_map<size_t, vector<Fingerprint>>& b) {
    if (a.empty() || b.empty()) return 0.0f;

    size_t fpCountA = 0;
    size_t fpCountB = 0;
    for (const auto& [_, fps] : a) fpCountA += fps.size();
    for (const auto& [_, fps] : b) fpCountB += fps.size();
    if (fpCountA == 0 || fpCountB == 0) return 0.0f;
    
    int matchScore = 0;
    //for each fingerprint (kv pair) in a
    for (const auto& [hash, fingerprints_a] : a) {
        //check if this hash exists in fingerprint set b
        auto it = b.find(hash); //look up hash in b
        if (it == b.end()) continue; //if doesnt exist in b, skip
        const vector<Fingerprint>& fingerprints_b = it->second; //get a read only reference to the fingerprints stored
        
        //compare fingerprints (value) a and b
        for (const Fingerprint& fp_a : fingerprints_a) {
            for (const Fingerprint& fp_b : fingerprints_b) {
                //verify anchor frequency is actually close (handling hash collisions)
                if (fabs(fp_a.anchorFreq - fp_b.anchorFreq) >= FREQ_TOLERANCE) continue; //probably gonna have to improve
                //count how many pattern frequencies match
                int localMatches = 0;
                int minSize = min(fp_a.pattern.size(), fp_b.pattern.size());
                if (minSize == 0) continue; //this shouldnt ever occur

                for (int px = 0; px < minSize; px++) {
                    if (fabs(fp_a.pattern[px] - fp_b.pattern[px]) < FREQ_TOLERANCE) {
                        localMatches++;
                    }
                }
                float ratio = localMatches / float(minSize);
                if (ratio >= 0.6f) {matchScore++; break;}
            }
        }
    }
    return (float)matchScore / max(fpCountA,fpCountB);
}


/* js that this is based on
note that noisePrints[name] is effectively equivalent to a

in other news, i loathe js
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