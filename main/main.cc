/**
 * @file main.cc
 *
 */

#include "backtrack.h"
#include "candidate_set.h"
#include "common.h"
#include "graph.h"

#define PRINT_TIME 1

int main(int argc, char* argv[]) {
//    std::cout << "Hello, World!" <<std::endl;
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                 "<candidate set file>\n";
    return EXIT_FAILURE;
  }

  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string candidate_set_file_name = argv[3];

  Graph data(data_file_name);
  Graph query(query_file_name, true);
  CandidateSet candidate_set(candidate_set_file_name);

  Backtrack backtrack;
#if PRINT_TIME
  time_t start, end;
  start = clock();
#endif
  try {
      backtrack.PrintAllMatches(data, query, candidate_set);
  }
  catch (const char* e) {
#if PRINT_TIME
      end = clock();
      std::cout << e << std::endl;
      std::cout << "time: " << (end-start) << "ms" << std::endl;
#endif
  }
  return EXIT_SUCCESS;
}
