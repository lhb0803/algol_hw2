/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"
using Embedding = std::map<Vertex, Vertex>;
using CandidateSpace = std::vector<Vertex>;
using Neighbors = std::vector<Vertex>;
using NeighborsAndCandidateSpace = std::pair<Neighbors, CandidateSpace>;
using CandidateSizeWithSpace = std::pair<size_t, CandidateSpace>;
using CandidateSizeWithNeighborsAndSpace = std::pair<size_t, NeighborsAndCandidateSpace>;
using VertexWithWeight = std::pair<Vertex, size_t>;
using CandidateMapping = std::pair<VertexWithWeight, CandidateSizeWithSpace>;
using CandidateMappingRemember = std::pair<VertexWithWeight , CandidateSizeWithNeighborsAndSpace>;
struct cmp {
    // CandidateMapping u : u1.first (Vertex v, Weight) / u1.second (Candidate Size, Candidate Space)
    bool operator()(CandidateMapping &u1, CandidateMapping &u2) {
        if(u1.second.first == u2.second.first) {
            return u1.first.second < u2.first.second;
        }
        else {
            return u1.first.second > u2.second.first;
        }
    }
};
struct cmp_remember {
    // CandidateMapping u : u1.first (Vertex v, Weight) / u1.second (Candidate Size, Candidate Space)
    bool operator()(CandidateMappingRemember &u1, CandidateMappingRemember &u2) {
        if(u1.second.first == u2.second.first) {
            return u1.first.second < u2.first.second;
        }
        else {
            return u1.second.first > u2.second.first;
        }
    }
};

using CandidateSetQueue = std::priority_queue<CandidateMapping, std::vector<CandidateMapping>, cmp>;
using CandidateSetQueueRemember = std::priority_queue<CandidateMappingRemember, std::vector<CandidateMappingRemember>, cmp_remember>;

// csq = {{u, {candidate_sz, {u0, u1, u2, ... }}}, ... }

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void BackTrackMatches(const Graph &data, const Graph &query, CandidateSetQueue csq, Embedding embedding,
                        std::map<Vertex, bool> &mark);
  void BackTrackMatches(const Graph &data, const Graph &query, CandidateSetQueueRemember csq, Embedding embedding,
                          std::map<Vertex, bool> &mark);
  CandidateMapping GetExtendableVertex(const Graph &data, const Graph &query, CandidateSetQueue &csq,
                                       const Embedding &embedding, const std::map<Vertex, bool> &mark);
  CandidateMappingRemember GetExtendableVertex(const Graph &data, const Graph &query, CandidateSetQueueRemember &csq,
                                         const Embedding &embedding, const std::map<Vertex, bool> &mark);

  bool RefineAllCandidateSpace(const Graph &data, const Graph &query, CandidateSetQueueRemember &csq,
                               const Embedding &embedding, const std::map<Vertex, bool> &mark);

  void RefineCandidateSpace(const Graph &data, const Graph &query, const Vertex &u_star, CandidateSpace &candidate_space,
                            const Embedding &embedding, const std::map<Vertex, bool> &mark);
  inline void printEmbedding(std::map<Vertex, Vertex> &embedding);
  inline size_t GetOtherLabelNum(const Vertex &a, const Graph &query);
  inline size_t calculateWeight(const Vertex &u, const Graph &query);
};

inline void Backtrack::printEmbedding(std::map<Vertex, Vertex> &embedding) {
    // Printing Embedding
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

inline size_t Backtrack::GetOtherLabelNum(const Vertex &a, const Graph &query) {
    // Getting # of Vertex connected to target Vertex w/ other Label
    Label label = query.GetLabel(a);
    size_t other = 0;
    for(size_t i = query.GetNeighborStartOffset(a); i < query.GetNeighborEndOffset(a); i++) {
        Vertex neighbor = query.GetNeighbor(i);
        if(label != query.GetLabel(neighbor)) {
            other++;
        }
    }
    return other;
}

inline size_t Backtrack::calculateWeight(const Vertex &u, const Graph &query) {
    // first weight : Label of Vertex
    // Second weight : # of Vertex connected to target Vertex
    return 1000*query.GetLabel(u)+GetOtherLabelNum(u, query);
}

#endif  // BACKTRACK_H_
