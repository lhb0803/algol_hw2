/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

static int cnt = 0;

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    // implement your code here.
    CandidateSetQueue cs_queue = CandidateSetQueue();
    for (size_t i=0; i<query.GetNumVertices(); i++) {
        auto u = (Vertex) i;
        auto u_cs_size = cs.GetCandidateSize(u);
        CandidateSpace candidates_space = CandidateSpace();
        for (size_t j=0; j<u_cs_size; j++) {
            candidates_space.push_back(cs.GetCandidate(u, j));
        }
        CandidateSizeWithSpace candidate_sz_with_vertex = CandidateSizeWithSpace(u_cs_size, candidates_space);
        cs_queue.push({u, candidate_sz_with_vertex});
    }

    std::cout << "t " << query.GetNumVertices() << std::endl;

    std::map<Vertex, Vertex> embedding = std::map<Vertex, Vertex>();
    std::map<Vertex, bool> mark = std::map<Vertex, bool>();
    for (size_t i=0; i<data.GetNumVertices(); i++) {
        auto v = (Vertex) i;
        mark[v] = false;
    }

    BackTrackMatches(data, query, cs_queue, embedding, mark);
    throw "Done!\n";
}

void Backtrack::BackTrackMatches(const Graph &data, const Graph &query, CandidateSetQueue csq, std::map<Vertex, Vertex> embedding,
                                 std::map<Vertex, bool> &mark) {
    if (embedding.size() == query.GetNumVertices()) { // |M| = |V(q)| then Report M
        cnt++;
        // std::cout << "embedding counts: " << cnt << std::endl;
        if (cnt >= 100000) {
            //exit(EXIT_SUCCESS);
            throw "Done!\n";
            //exit(1);
        }
        printEmbedding(embedding);
    }

    else if (embedding.empty()) { //|M| = 0
//        std::cout << "INITIATE" << std::endl;
        CandidateMapping candidate_mapping = GetExtendableVertex(data, query, csq, embedding, mark);
        Vertex r = candidate_mapping.first;
        if (r == -1) { // No Extendable Vertex
//            std::cout << "No Extendable Vertex" << std::endl;
            return;
        }
        size_t cs_sz = candidate_mapping.second.first;
        CandidateSpace candidate_space = candidate_mapping.second.second;
        for (size_t i=0; i<cs_sz; i++) {
            Vertex v = candidate_space[i];
            embedding[r] = v; mark[v] = true;
//            std::cout << "Select r: " << r << " v: "<< v << std::endl;
//            std::cout << "Embedding Size: |" << embedding.size() << "|" << std::endl;
            BackTrackMatches(data, query, csq, embedding, mark); mark[v] = false;
        }
    }

    else {
        // u = extendable vertex with minimum candidate size |C(u)|
        CandidateMapping candidate_mapping = GetExtendableVertex(data, query, csq, embedding, mark);
        Vertex u = candidate_mapping.first;
        if (u == -1) { // No Extendable Vertex
//            std::cout << "No Extendable Vertex" << std::endl;
            return;
        }
        size_t cs_sz = candidate_mapping.second.first;
        CandidateSpace candidate_space = candidate_mapping.second.second;
        for (size_t i=0; i<cs_sz; i++) {
            Vertex v = candidate_space[i];
            if (!mark[v]) {
                embedding[u] = v; mark[v] = true;
//                std::cout << "Select u: " << u << " v: "<< v << std::endl;
//                std::cout << "Embedding Size: |" << embedding.size() << "|" << std::endl;
                BackTrackMatches(data, query, csq, embedding, mark); mark[v] = false;
            }
        }
    }
}

CandidateMapping Backtrack::GetExtendableVertex(const Graph &data, const Graph &query, CandidateSetQueue &csq,
                                      const std::map<Vertex, Vertex> &embedding, const std::map<Vertex, bool> &mark) {
    // 1. u* : which has minimum candidate space size
//    std::cout << "debug" << std::endl;
    CandidateMapping candidate_mapping = csq.top(); csq.pop();
    Vertex u_star = candidate_mapping.first;
//    std::cout << "u_star: " << u_star << std::endl;
    CandidateSizeWithSpace cs_sz_with_cs = candidate_mapping.second;
    CandidateSpace candidate_space = cs_sz_with_cs.second;

    // 2. fix candidate space
//    std::cout << "candidate space before fixing: ";
//    for (auto v : candidate_space) std::cout << v << " ";
//    std::cout << std::endl;
    FixCandidateSpace(data, query, u_star, candidate_space, embedding, mark);
//    std::cout << "candidate space after fixing: ";
//    for (auto v : candidate_space) std::cout << v << " ";
//    std::cout << std::endl;

    // 3-A. if candidate space empty: No Extendable Vertex
    if (candidate_space.empty()) { // No Extendable Vertex
        return CandidateMapping(-1, CandidateSizeWithSpace(-1, CandidateSpace()));
    }
    // 3-B. else: return CandidateMapping
    else {
        CandidateSizeWithSpace to_return_cs_sz_with_cs = CandidateSizeWithSpace(candidate_space.size(), candidate_space);
        CandidateMapping to_return_candi_map = CandidateMapping(u_star, to_return_cs_sz_with_cs);
        return to_return_candi_map;
    }
}

void Backtrack::FixCandidateSpace(const Graph &data, const Graph &query, const Vertex &u_star, CandidateSpace &candidate_space,
                                  const std::map<Vertex, Vertex> &embedding, const std::map<Vertex, bool> &mark) {
    // 2-1. get rid of all v* which are already embedded.
    CandidateSpace new_candidate_space = CandidateSpace();
    for (Vertex v_star : candidate_space) {
        if (mark.at(v_star)) {
//            std::cout << v_star << " is visited!" << std::endl;
            continue;
        }
        else {
            new_candidate_space.push_back(v_star);
        }
    }
    candidate_space.clear();
    // 2-2. check the edge between u* and u with the edge between v* and v.
    if (embedding.empty()) {
        candidate_space = new_candidate_space;
    }
    else {
        for (Vertex v_star : new_candidate_space) {
            bool can_be_candidate = true;
            for (auto mapping : embedding) {
                if (query.IsNeighbor(mapping.first, u_star) && !data.IsNeighbor(mapping.second, v_star)) {
                    can_be_candidate = false;
//                    std::cout << v_star << "'s relationship is not same with " << u_star << " :(" << std::endl;
                    break;
                }
            }
            if (can_be_candidate) {
                candidate_space.push_back(v_star);
            }
        }
    }
}

