#include "graph.h"
#include "inputFunctions.h"
#include "outputFunctions.h"
#include "vertexCoverSolver.h"
#include <iostream>
#include <pthread.h>

Graph *g = nullptr;
bool cnfThreadExited = false;
std::vector<int> minVertexCoverCNF;
std::vector<int> minVertexCoverApprox1;
std::vector<int> minVertexCoverApprox2;
std::vector<double> cnfRunTimeArray;
std::vector<double> approxVC1RunTimeArray;
std::vector<double> approxVC2RunTimeArray;
std::vector<double> approx_vc1;
std::vector<double> approx_vc2;
double cnfCurrentRunTime, approxVC1CurrentRunTime, approxVC2CurrentRunTime;
int vertex_no;

#define REPORT std::getenv("REPORT") != NULL

void *cnfSatThread(void *arg) {
  clock_t start, finish;
  start = clock();

  cnfThreadExited = false;
  VertexCoverSolver vcs(g);
  minVertexCoverCNF = vcs.findMinVertexCover();
  cnfThreadExited = true;

  finish = clock();

  cnfCurrentRunTime = (double)((finish - start) * 1000000) /
                      CLOCKS_PER_SEC; // microsecond calculation
  cnfRunTimeArray.push_back(cnfCurrentRunTime);
  return nullptr;
}

void *approxVertexCover1Thread(void *arg) {
  clock_t start, finish;
  start = clock();

  minVertexCoverApprox1 = g->approximateMinimumVertexCover1();

  finish = clock();

  approxVC1CurrentRunTime =
      (double)((finish - start) * 1000000) / CLOCKS_PER_SEC;
  approxVC1RunTimeArray.push_back(approxVC1CurrentRunTime);
  return nullptr;
}

void *approxVertexCover2Thread(void *arg) {
  clock_t start, finish;
  start = clock();

  minVertexCoverApprox2 = g->approximateMinimumVertexCover2();

  finish = clock();

  approxVC2CurrentRunTime =
      (double)((finish - start) * 1000000) / CLOCKS_PER_SEC;
  approxVC2RunTimeArray.push_back(approxVC2CurrentRunTime);
  return nullptr;
}

clockid_t get_cpu_clockid_for_thread(pthread_t thread_id) {
  clockid_t cid;
  if (pthread_getcpuclockid(thread_id, &cid)) {
    std::cerr << "Error: can't get the cpu clock" << std::endl;
  }
  return cid;
}

void thread_timeout_watcher(pthread_t thread_id, int time_limit) {
  clockid_t cid = get_cpu_clockid_for_thread(thread_id);
  struct timespec tspec;

  while (!cnfThreadExited) {
    clock_gettime(cid, &tspec);
    if (tspec.tv_sec > time_limit) {
      VertexCoverSolver::cnf_timeout_flag = true;
      cnfThreadExited = true;
    }
  }
}

void threaded_vc() {
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

  pthread_create(&thread1, NULL, &cnfSatThread, nullptr);
  pthread_create(&thread2, NULL, &approxVertexCover1Thread, nullptr);
  pthread_create(&thread3, NULL, &approxVertexCover2Thread, nullptr);

  thread_timeout_watcher(thread1, 120);

  // // Timeout function for CNF-SAT-VC approach (Check "Formal DEBUG" pdf on
  // Github repository for more information)
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 120;

  // // Awaiting thread termination
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  pthread_timedjoin_np(thread1, NULL, &ts);

  if (!VertexCoverSolver::cnf_timeout_flag) {
    printMinimumVertexCover("CNF-SAT-VC: ", minVertexCoverCNF);
  } else {
    std::cout << "CNF-SAT-VC: timeout" << std::endl;
    VertexCoverSolver::cnf_timeout_flag = false;
  }

  printMinimumVertexCover("APPROX-VC-1: ", minVertexCoverApprox1);
  printMinimumVertexCover("APPROX-VC-2: ", minVertexCoverApprox2);

  if (REPORT) {
    std::cout << "CNF-SAT-VC Runtime: " << cnfCurrentRunTime << " microseconds"
              << std::endl;
    std::cout << "APPROX-VC-1 Runtime: " << approxVC1CurrentRunTime
              << " microseconds" << std::endl;
    std::cout << "APPROX-VC-2 Runtime: " << approxVC2CurrentRunTime
              << " microseconds" << std::endl;
  }

  approx_vc1.push_back(Graph::approx_1_size / VertexCoverSolver::cnf_size);
  approx_vc2.push_back(Graph::approx_2_size / VertexCoverSolver::cnf_size);

  return;
}

int main() {
  while (!std::cin.eof()) {
    std::string line;

    while (std::getline(std::cin, line)) {
      // if cmd length is less than 2 characters
      if (line.length() <= 2) {
        std::cerr << "Error: Command length should be minimum 3 characters\n";
      }
      try {
        // Check cmd cases
        if (line[0] == 'V') {
          readVertixNumberInput(g, line);
          vertex_no = g->getVertixNumber();
        } else if (line[0] == 'E') {
          if (g == nullptr) {
            std::cerr << "Error: Graph not yet defined.\n";
          } else if (!(g->getEdgeCommandGiven())) {
            if (readEdgeInput(g, line)) {
              threaded_vc();
            }
          }
        } else {
          std::cerr << "Error: unknown command\n";
        }
      } catch (...) {
        std::cerr << "Error: Some error occurred during the parsing of the "
                     "given command.\n";
      }
    }
  }

  if (REPORT) {
    std::cout << "Vertex no" << vertex_no << std::endl;

    printStats("CNF-SAT-VC: ", cnfRunTimeArray);
    printStats("APPROX-VC-1: ", approxVC1RunTimeArray);
    printStats("APPROX-VC-2: ", approxVC2RunTimeArray);
    printStats("approximate ratio of APPROX-VC-1: ", approx_vc1);
    printStats("approximate ratio of APPROX-VC-2: ", approx_vc2);
  }

  return 0;
}
