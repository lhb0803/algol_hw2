/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
using CandidateSpace = std::vector<Vertex>;
using CandidateSizeWithSpace = std::pair<size_t, CandidateSpace>;
using CandidateMapping = std::pair<Vertex, CandidateSizeWithSpace>;
struct cmp {
    bool operator()(CandidateMapping &u1, CandidateMapping &u2) {
        if(u1.second.first == u2.second.first) {
            return u1.first > u2.first;
        }
        else {
            return u1.second.first > u2.second.first;
        }
    }
};
using CandidateSetQueue = std::priority_queue<CandidateMapping, std::vector<CandidateMapping>, cmp>;
// csq = {{u, {candidate_sz, {u0, u1, u2, ... }}}, ... }

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void BackTrackMatches(const Graph &data, const Graph &query, CandidateSetQueue csq, std::map<Vertex, Vertex> embedding,
                        std::map<Vertex, bool> &mark);

  CandidateMapping GetExtendableVertex(const Graph &data, const Graph &query, CandidateSetQueue &csq,
                                       const std::map<Vertex, Vertex> &embedding, const std::map<Vertex, bool> &mark);

  void FixCandidateSpace(const Graph &data, const Graph &query, const Vertex &u_star, CandidateSpace &candidate_space,
                         const std::map<Vertex, Vertex> &embedding, const std::map<Vertex, bool> &mark);

  inline void printEmbedding(std::map<Vertex, Vertex> &embedding);
};

inline void Backtrack::printEmbedding(std::map<Vertex, Vertex> &embedding) {
    size_t n = embedding.size();
    std::cout << "a ";
    for (size_t i=0; i<n; i++) {
        auto u = (Vertex) i;
        Vertex v = embedding[u];
        std::cout << v;
        if (i < n-1) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}

#endif  // BACKTRACK_H_
