/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    // implement your code here.
    std::cout << "<G> ID: " << data.GetGraphID() << "\n";
    std::cout << "<G> Num of Vertices: " << data.GetNumVertices() << "\n";
    std::cout << "<G> Num of Edges: " << data.GetNumEdges() << "\n";
    std::cout << "<G> Num of Labels: " << data.GetNumLabels() << "\n" << "\n";

    std::cout << "<q> ID: " << query.GetGraphID() << "\n";
    std::cout << "<q> Num of Vertices: " << query.GetNumVertices() << "\n";
    std::cout << "<q> Num of Edges: " << query.GetNumEdges() << "\n";
    std::cout << "<q> Num of Labels: " << query.GetNumLabels() << "\n" << "\n";

    std::cout << "<q> q's vertex 0's candidate Size: " << cs.GetCandidateSize(0) << "\n";
    std::cout << "<q> q's vertex 0(u0)'s candidates(v0, v1, ... ): ";
    for (size_t i=0; i<cs.GetCandidateSize(0); i++) {
        std::cout << cs.GetCandidate(0, i) << " ";
    }
    std::cout << "\n";

}
