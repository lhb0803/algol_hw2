/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);
  void DFSMatches(const Graph &data, const Graph &query, const CandidateSet &cs,
                  std::map<Vertex, bool> &mark_v, std::map<Vertex, bool> &mark_u,
                  Vertex start_u, Vertex start_v);
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
