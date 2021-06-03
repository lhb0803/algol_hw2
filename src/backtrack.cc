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
    CandidateSetQueue cs_queue ;
    for (size_t i=0; i<query.GetNumVertices(); i++) {
        auto u = (Vertex) i;
        auto u_cs_size = cs.GetCandidateSize(u);
        std::queue<Vertex> candidates_queue;
        for (size_t j=0; j<u_cs_size; j++) {
            candidates_queue.push(cs.GetCandidate(u, j));
        }
        std::pair<size_t, std::queue<Vertex>> candidate_sz_with_vertex = std::pair<size_t, std::queue<Vertex>>(u_cs_size, candidates_queue);
        cs_queue.push({u, candidate_sz_with_vertex});
    }

    std::cout << "t " << query.GetNumVertices() << "\n";

    std::map<Vertex, Vertex> embedding = std::map<Vertex, Vertex>();
    std::map<Vertex, bool> mark = std::map<Vertex, bool>();
    for (size_t i=0; i<data.GetNumVertices(); i++) {
        auto v = (Vertex) i;
        mark[v] = false;
    }
    BackTrackMatches(query, cs, embedding, cs_queue, mark);
}

void Backtrack::BackTrackMatches(const Graph &query, const CandidateSet &cs, std::map<Vertex, Vertex>&embedding,
                                 CandidateSetQueue &csq, std::map<Vertex, bool> &mark) {
    if (embedding.size() == query.GetNumVertices()) { // |M| = |V(q)| then Report M
        printEmbedding(embedding);
    }

    else if (embedding.empty()) { //|M| = 0
        std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>> csq_top = csq.top(); csq.pop();
        Vertex r = csq_top.first;
        size_t cs_sz = csq_top.second.first;
        for (size_t i=0; i<cs_sz; i++) {
            //Vertex v = cs.GetCandidate(r, i);
            Vertex v = csq_top.second.second.front(); csq_top.second.second.pop();
            embedding[r] = v; mark[v] = true;

            BackTrackMatches(query, cs, embedding, csq,mark); mark[v] = false; csq_top.second.second.push(v);
            std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>> csq_new_top = std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>>(r, csq_top.second);
            csq.push(csq_new_top);
        }
    }

    else {
        // u = extendable vertex with minimum candidate size |C(u)|
        std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>> csq_top = csq.top(); csq.pop();
        Vertex u = csq_top.first;
        size_t cs_sz = csq_top.second.first;
        for (size_t i=0; i<cs_sz; i++) {
            //Vertex v = cs.GetCandidate(r, i);
            Vertex v = csq_top.second.second.front(); csq_top.second.second.pop();
            if (!mark[v]) {
                embedding[u] = v; mark[v] = true; csq_top.second.second.push(v);

                BackTrackMatches(query, cs, embedding, csq, mark); mark[v] = false; csq_top.second.second.push(v);
                std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>> csq_new_top = std::pair<Vertex, std::pair<size_t, std::queue<Vertex>>>(u, csq_top.second);
                csq.push(csq_new_top);
            }
        }
        csq.pop();
    }
}

