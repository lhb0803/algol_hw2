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
    Vertex min_candi_u;
    size_t min_candi_num = SIZE_MAX;
    std::map<Vertex, std::vector<Vertex>> candidate_set;

    for (size_t u_i=0; u_i<query.GetNumVertices(); u_i++) {
        Vertex u = (Vertex) u_i;
        std::vector<Vertex> cs_of_u;

        for (size_t i=0; i<cs.GetCandidateSize(u); i++) {
            Vertex candidate = cs.GetCandidate(u, i);
            cs_of_u.push_back(candidate);
        }

        if (cs.GetCandidateSize(u) < min_candi_num) {
            min_candi_u = u;
        }

        candidate_set[u] = cs_of_u;
//        std::cout << "<q> q's vertex " << u << "'s candidate Size: " << cs.GetCandidateSize(u) << "\n";
//        std::cout << "<q> q's vertex " << u << "'s candidates(v0, v1, ... ): ";
//        for (Vertex v : candidate_set[u]) {
//            std::cout << v << " ";
//        }
//        std::cout << "\n";
    }


    std::cout << "t " << query.GetNumVertices() << "\n";
    std::map<Vertex, bool> mark_v;
    std::map<Vertex, bool> mark_u;
    for (size_t v_i=0; v_i<data.GetNumVertices(); v_i++) {
        Vertex v = (Vertex) v_i;
        mark_v[v] = false;
    }

    for (size_t u_i=0; u_i<query.GetNumVertices(); u_i++) {
        Vertex u = (Vertex) u_i;
        mark_u[u] = false;
    }

    DFSMatches(data, query, cs, mark_v, mark_u, 0, cs.GetCandidate(0,0));
}

void Backtrack::DFSMatches(const Graph &data, const Graph &query, const CandidateSet &cs,
                           std::map<Vertex, bool> &mark_v, std::map<Vertex, bool> &mark_u,
                           Vertex start_u, Vertex start_v) {
    std::stack<std::pair<Vertex, Vertex>> dfs_stack;
    std::pair<Vertex, Vertex> start_pair = std::pair<Vertex, Vertex>(start_u, start_v);
    dfs_stack.push(start_pair);
    mark_u[start_u] = true;
    mark_u[start_v] = true;
    std::map<Vertex, Vertex> embedding;
    embedding[start_u] = start_v;

    while (!dfs_stack.empty()) {
        // print embedding
        if (embedding.size() == query.GetNumVertices()) {
            printEmbedding(embedding);
        }

        // select unvisited u;
        Vertex new_u = -1;
        for (size_t i=0; i<mark_u.size(); i++) {
            Vertex u = (Vertex) i;
            if (!mark_u[u]) {
                new_u = u; //std::cout << "new_u: " << new_u << "\n";
                break;
            }
        }

        if (new_u == -1) {
            // pop dfs_stack
            std::pair<Vertex, Vertex> popped_pair = dfs_stack.top();
            embedding.erase(popped_pair.first);
            mark_u[popped_pair.first] = false;
            dfs_stack.pop();
            continue;
        }
        // find u's visited neighbors
        std::vector<Vertex> visited_neighbors;
//        std::cout << "neighbors: ";
        for (size_t i=query.GetNeighborStartOffset(new_u); i<query.GetNeighborEndOffset(new_u); i++) {
            Vertex neighbor = query.GetNeighbor(i);
            if (mark_u[neighbor]) {
                visited_neighbors.push_back(neighbor); //std::cout << neighbor << " ";
            }
        }
//        std::cout << "\n";

        // map all the neighbors to v
        std::vector<Vertex> visited_mapped_neighbors;
        //std::cout << "mapped neighbors: ";
        for (Vertex u : visited_neighbors) {
            visited_mapped_neighbors.push_back(embedding[u]); //std::cout << embedding[u] << " ";
        }
        //std::cout << "\n";

        // build u's candidate set
        std::vector<Vertex> new_u_candidates;
        //std::cout << "new u's candidates: ";
        for (size_t i=0; i<cs.GetCandidateSize(new_u); i++) {
            Vertex candidate = cs.GetCandidate(new_u, i);
            bool hasSameNeighborMapping = false;
            for (Vertex v : visited_mapped_neighbors) {
                if (!data.IsNeighbor(candidate, v)) {
                    hasSameNeighborMapping = false;
                    break;
                }
                else {
                    hasSameNeighborMapping = true;
                }
            }
            if (!mark_v[candidate] && hasSameNeighborMapping) {
                new_u_candidates.push_back(candidate); //std::cout << candidate << " ";
            }
        }
        //std::cout << "\n";

        if (new_u_candidates.empty()) {
            std::pair<Vertex, Vertex> popped_pair = dfs_stack.top();
            embedding.erase(popped_pair.first);
            mark_u[popped_pair.first] = false;
            dfs_stack.pop();
        }
        else {
            std::pair<Vertex, Vertex> new_pair = std::pair<Vertex, Vertex>(new_u, new_u_candidates[0]);
            dfs_stack.push(new_pair);
            embedding[new_u] = new_u_candidates[0];
            mark_u[new_u] = true;
            mark_v[new_u_candidates[0]] = true;
        }

    }
}

