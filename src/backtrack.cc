/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"
#define DEBUG_BUTTON 0 // 0: off / 1: on / 2: check FixCandidateSpace()
#define NEIGHBOR_REMEMBER 0

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

static int cnt = 0;

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query, const CandidateSet &cs) {
    // implement your code here.
#if NEIGHBOR_REMEMBER==1
    CandidateSetQueueRemember cs_queue = CandidateSetQueueRemember();
    for (size_t i=0; i<query.GetNumVertices(); i++) {
        auto u = (Vertex) i;
        auto u_cs_size = cs.GetCandidateSize(u);

        Neighbors neighbors = Neighbors();
        for (size_t j=query.GetNeighborStartOffset(u); j<query.GetNeighborEndOffset(u); j++) {
            neighbors.push_back(query.GetNeighbor(j));
        }

        CandidateSpace candidates_space = CandidateSpace();
        for (size_t j=0; j<u_cs_size; j++) {
            candidates_space.push_back(cs.GetCandidate(u, j));
        }
        NeighborsAndCandidateSpace n_and_cs = NeighborsAndCandidateSpace(neighbors, candidates_space);
        CandidateSizeWithNeighborsAndSpace candidate_sz_with_vertex = CandidateSizeWithNeighborsAndSpace(u_cs_size, n_and_cs);
        cs_queue.push({u, candidate_sz_with_vertex});
    }
#else
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
#endif

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

#if NEIGHBOR_REMEMBER == 1
void Backtrack::BackTrackMatches(const Graph &data, const Graph &query, CandidateSetQueueRemember csq, Embedding embedding,
                                 std::map<Vertex, bool> &mark) {
    if (embedding.size() == query.GetNumVertices()) { // |M| = |V(q)| then Report M
        cnt++;
#if DEBUG_BUTTON == 1
        std::cout << "embedding counts: " << cnt << std::endl;
#endif
        if (cnt >= 100000) {
            throw "Done!\n";
        }
        printEmbedding(embedding);
    } else if (embedding.empty()) { //|M| = 0
#if DEBUG_BUTTON == 1
        std::cout << "INITIATE" << std::endl;
#endif
        CandidateMappingRemember candidate_mapping = GetExtendableVertex(data, query, csq, embedding, mark);
        Vertex r = candidate_mapping.first;
        if (r == -1) { // No Extendable Vertex
#if DEBUG_BUTTON == 1
            std::cout << "No Extendable Vertex" << std::endl;
#endif
            return;
        }
        size_t cs_sz = candidate_mapping.second.first;
        CandidateSpace candidate_space = candidate_mapping.second.second.second;
        for (size_t i = 0; i < cs_sz; i++) {
            Vertex v = candidate_space[i];
            embedding[r] = v;
            mark[v] = true;
#if DEBUG_BUTTON == 1
            std::cout << "Select r: " << r << " v: "<< v << std::endl;
            std::cout << "Embedding Size: |" << embedding.size() << "|" << std::endl;
#endif
            BackTrackMatches(data, query, csq, embedding, mark);
            mark[v] = false;
        }
    } else {
        // u = extendable vertex with minimum candidate size |C(u)|
        CandidateMappingRemember candidate_mapping = GetExtendableVertex(data, query, csq, embedding, mark);
        Vertex u = candidate_mapping.first;
        if (u == -1) { // No Extendable Vertex
#if DEBUG_BUTTON == 1
            std::cout << "No Extendable Vertex" << std::endl;
#endif
            return;
        }
        size_t cs_sz = candidate_mapping.second.first;
        CandidateSpace candidate_space = candidate_mapping.second.second.second;
        for (size_t i = 0; i < cs_sz; i++) {
            Vertex v = candidate_space[i];

            embedding[u] = v;
            mark[v] = true;
#if DEBUG_BUTTON == 1
            std::cout << "Select u: " << u << " v: "<< v << std::endl;
            std::cout << "Embedding Size: |" << embedding.size() << "|" << std::endl;
#endif
            BackTrackMatches(data, query, csq, embedding, mark);
            mark[v] = false;

        }
    }
}
#else
void Backtrack::BackTrackMatches(const Graph &data, const Graph &query, CandidateSetQueue csq, std::map<Vertex, Vertex> embedding,
                             std::map<Vertex, bool> &mark) {

    if (embedding.size() == query.GetNumVertices()) { // |M| = |V(q)| then Report M
        cnt++;
#if DEBUG_BUTTON==1
        std::cout << "embedding counts: " << cnt << std::endl;
#endif
        if (cnt >= 100000) {
            throw "Done!\n";
        }
        printEmbedding(embedding);
    }

    else if (embedding.empty()) { //|M| = 0
#if DEBUG_BUTTON==1
        std::cout << "INITIATE" << std::endl;
#endif
        CandidateMapping candidate_mapping = GetExtendableVertex(data, query, csq, embedding, mark);
        Vertex r = candidate_mapping.first;
        if (r == -1) { // No Extendable Vertex
#if DEBUG_BUTTON==1
            std::cout << "No Extendable Vertex" << std::endl;
#endif
            return;
        }
        size_t cs_sz = candidate_mapping.second.first;
        CandidateSpace candidate_space = candidate_mapping.second.second;
        for (size_t i=0; i<cs_sz; i++) {
            Vertex v = candidate_space[i];
            embedding[r] = v; mark[v] = true;
#if DEBUG_BUTTON==1
            std::cout << "Select r: " << r << " v: "<< v << std::endl;
            std::cout << "Embedding Size: |" << embedding.size() << "|" << std::endl;
#endif
            BackTrackMatches(data, query, csq, embedding, mark); mark[v] = false;
        }
    }

    else {
        // u = extendable vertex with minimum candidate size |C(u)|
        CandidateMapping candidate_mapping = GetExtendableVertex(data, query, csq, embedding, mark);
        Vertex u = candidate_mapping.first;
        if (u == -1) { // No Extendable Vertex
#if DEBUG_BUTTON==1
            std::cout << "No Extendable Vertex" << std::endl;
#endif
            return;
        }
        size_t cs_sz = candidate_mapping.second.first;
        CandidateSpace candidate_space = candidate_mapping.second.second;
        for (size_t i=0; i<cs_sz; i++) {
            Vertex v = candidate_space[i];
            if (!mark[v]) {
                embedding[u] = v; mark[v] = true;
#if DEBUG_BUTTON==1
                std::cout << "Select u: " << u << " v: "<< v << std::endl;
                std::cout << "Embedding Size: |" << embedding.size() << "|" << std::endl;
#endif
                BackTrackMatches(data, query, csq, embedding, mark); mark[v] = false;
            }
        }
    }
}
#endif

#if NEIGHBOR_REMEMBER == 1
CandidateMappingRemember Backtrack::GetExtendableVertex(const Graph &data, const Graph &query, CandidateSetQueueRemember &csq,
                                      const Embedding &embedding, const std::map<Vertex, bool> &mark) {
    // 1. u* : which has minimum candidate space size
    CandidateMappingRemember candidate_mapping = csq.top(); csq.pop();
    Vertex u_star = candidate_mapping.first;
#if DEBUG_BUTTON==1
    std::cout << "u_star: " << u_star << std::endl;
#endif
    CandidateSizeWithNeighborsAndSpace cs_sz_with_cs = candidate_mapping.second;
    NeighborsAndCandidateSpace n_candidate_space = cs_sz_with_cs.second;
    CandidateSpace candidate_space = n_candidate_space.second;

    Embedding u_star_neighbors_embedding = Embedding();
    for (Vertex neighbor : n_candidate_space.first) {
        if (embedding.count(neighbor) > 0) {
            u_star_neighbors_embedding[neighbor] = embedding.at(neighbor);
        }
    }
    // 2. fix candidate space
#if DEBUG_BUTTON==1
    std::cout << "candidate space before fixing: ";
    for (auto v : candidate_space) std::cout << v << " ";
    std::cout << std::endl;
#endif
    FixCandidateSpace(data, query, u_star, candidate_space, u_star_neighbors_embedding, mark);
#if DEBUG_BUTTON==1
    std::cout << "candidate space after fixing: ";
    for (auto v : candidate_space) std::cout << v << " ";
    std::cout << std::endl;
#endif

    // 3-A. if candidate space empty: No Extendable Vertex
    if (candidate_space.empty()) { // No Extendable Vertex
        return CandidateMappingRemember(-1, CandidateSizeWithNeighborsAndSpace(-1, NeighborsAndCandidateSpace(Neighbors(), CandidateSpace())));
    }
    else {
        n_candidate_space = NeighborsAndCandidateSpace(n_candidate_space.first ,candidate_space);
        CandidateSizeWithNeighborsAndSpace to_return_cs_sz_with_cs = CandidateSizeWithNeighborsAndSpace(candidate_space.size(), n_candidate_space);
        CandidateMappingRemember to_return_candi_map = CandidateMappingRemember(u_star, to_return_cs_sz_with_cs);
        return to_return_candi_map;
    }
}

#else
CandidateMapping Backtrack::GetExtendableVertex(const Graph &data, const Graph &query, CandidateSetQueue &csq,
                                      const Embedding &embedding, const std::map<Vertex, bool> &mark) {
    // 1. u* : which has minimum candidate space size
    CandidateMapping candidate_mapping = csq.top(); csq.pop();
    Vertex u_star = candidate_mapping.first;
#if DEBUG_BUTTON == 1
    std::cout << "u_star: " << u_star << std::endl;
#endif
    CandidateSizeWithSpace cs_sz_with_cs = candidate_mapping.second;
    CandidateSpace candidate_space = cs_sz_with_cs.second;
    // 2. fix candidate space
#if DEBUG_BUTTON == 1
    std::cout << "candidate space before fixing: ";
    for (auto v : candidate_space) std::cout << v << " ";
    std::cout << std::endl;
#endif
    FixCandidateSpace(data, query, u_star, candidate_space, embedding, mark);
#if DEBUG_BUTTON == 1
    std::cout << "candidate space after fixing: ";
    for (auto v : candidate_space) std::cout << v << " ";
    std::cout << std::endl;
#endif

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
#endif

void Backtrack::FixCandidateSpace(const Graph &data, const Graph &query, const Vertex &u_star, CandidateSpace &candidate_space,
                                  const Embedding &embedding, const std::map<Vertex, bool> &mark) {
    // 2-1. get rid of all v* which are already embedded.
    CandidateSpace new_candidate_space = CandidateSpace();
    for (Vertex v_star : candidate_space) {
        if (mark.at(v_star)) {
#if DEBUG_BUTTON==2
            std::cout << v_star << " is visited!" << std::endl;
#endif
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
#if DEBUG_BUTTON==2
                    std::cout << v_star << "'s relationship is not same with " << u_star << " :(" << std::endl;
#endif
                    break;
                }
            }
            if (can_be_candidate) {
                candidate_space.push_back(v_star);
            }
        }
    }
}

