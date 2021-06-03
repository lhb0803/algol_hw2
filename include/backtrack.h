/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
struct cmp {
    bool operator()(std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>>&u1, std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>>&u2) {
        if(u1.second.first == u2.second.first) {
            return u1.first < u2.first;
        }
        else {
            return u1.second.first > u2.second.first;
        }
    }
};
using CandidateSetQueue = std::priority_queue<std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>>, std::vector<std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>>>, cmp>;
// csq = {{u, {candidate_sz, {u0, u1, u2, ... }}}, ... }

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void BackTrackMatches(const Graph &query, const CandidateSet &cs, std::map<Vertex, Vertex>&embedding,
                        CandidateSetQueue &csq, std::map<Vertex, bool> &mark);

  inline void printEmbedding(std::map<Vertex, Vertex> &embedding);
};

inline void Backtrack::printEmbedding(std::map<Vertex, Vertex> &embedding) {
    size_t n = embedding.size();
    std::cout << "a ";
    for (size_t i=0; i<n; i++) {
        Vertex u = (Vertex) i;
        Vertex v = embedding[u];
        std::cout << v;
        if (i < n-1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}




#endif  // BACKTRACK_H_
