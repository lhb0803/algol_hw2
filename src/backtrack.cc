/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    // implement your code here.
//    std::cout << "<G> ID: " << data.GetGraphID() << "\n";
//    std::cout << "<G> Num of Vertices: " << data.GetNumVertices() << "\n";
//    std::cout << "<G> Num of Edges: " << data.GetNumEdges() << "\n";
//    std::cout << "<G> Num of Labels: " << data.GetNumLabels() << "\n" << "\n";
//
//    std::cout << "<q> ID: " << query.GetGraphID() << "\n";
//    std::cout << "<q> Num of Vertices: " << query.GetNumVertices() << "\n";
//    std::cout << "<q> Num of Edges: " << query.GetNumEdges() << "\n";
//    std::cout << "<q> Num of Labels: " << query.GetNumLabels() << "\n" << "\n";
//    CandidateSetQueue cs_queue ;
//    for (size_t i=0; i<query.GetNumVertices(); i++) {
//        auto u = (Vertex) i;
//        auto u_cs_size = cs.GetCandidateSize(u);
//        std::queue<Vertex> candidates_queue;
//        for (size_t j=0; j<u_cs_size; j++) {
//            candidates_queue.push(cs.GetCandidate(u, j));
//        }
//        std::pair<size_t, std::queue<Vertex>> candidate_sz_with_vertex = std::pair<size_t, std::queue<Vertex>>(u_cs_size, candidates_queue);
//        cs_queue.push({u, candidate_sz_with_vertex});
//    }

    std::cout << "t " << query.GetNumVertices() << std::endl;

    std::map<Vertex, Vertex> embedding = std::map<Vertex, Vertex>();
    std::map<Vertex, bool> mark = std::map<Vertex, bool>();
    for (size_t i=0; i<data.GetNumVertices(); i++) {
        auto v = (Vertex) i;
        mark[v] = false;
    }
    BackTrackMatches(query, cs, embedding, mark);
}

void Backtrack::BackTrackMatches(const Graph &query, const CandidateSet &cs, std::map<Vertex, Vertex> embedding,
                                 std::map<Vertex, bool> &mark) {
    if (embedding.size() == query.GetNumVertices()) { // |M| = |V(q)| then Report M
        printEmbedding(embedding);
    }

    else if (embedding.empty()) { //|M| = 0
        Vertex r = GetExtendableVertex(query, cs, embedding);

        if (r == -1) return;
        for (size_t i=0; i<cs.GetCandidateSize(r); i++) {
            Vertex v = cs.GetCandidate(r, i);
            embedding[r] = v; mark[v] = true;
            BackTrackMatches(query, cs, embedding, mark); mark[v] = false;
        }
    }

    else {
        // u = extendable vertex with minimum candidate size |C(u)|
        Vertex u = GetExtendableVertex(query, cs, embedding);
        if (u == -1) return;
        for (size_t i=0; i<cs.GetCandidateSize(u); i++) {
            Vertex v = cs.GetCandidate(u, i);
            if (!mark[v]) {
                embedding[u] = v; mark[v] = true;
                BackTrackMatches(query, cs, embedding, mark); mark[v] = false;
            }
        }
    }
}

Vertex Backtrack::GetExtendableVertex(const Graph &query, const CandidateSet &cs,
                                      const std::map<Vertex, Vertex> &embedding) {

    size_t min_candi_sz = std::numeric_limits<size_t>::max();
    Vertex extendable_u = -1;

    for (size_t i=0; i<query.GetNumVertices(); i++) { // O(|V|)
        auto u = (Vertex) i;

        if (embedding.count(u)==0 && cs.GetCandidateSize(u)<min_candi_sz) { // if u is not embedded and smallest size
            min_candi_sz = cs.GetCandidateSize(u);
            extendable_u = u;
        }
    }

    return extendable_u;
}

