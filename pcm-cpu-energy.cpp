//
// Project:        pcm-cpu-energy
// File:           pcm-cpu-energy.cpp
// Date Create:    November 2nd 2017
// Date Modified:  November 3rd 2017
// Author:         Andreas Oeldemann, TUM <andreas.oeldemann@tum.de>
//
// Description:
//
// see README.md
//

#include <cpucounters.h>

// pcm
PCM *m;

// counter states
SocketCounterState *stateSocketBefore;
SocketCounterState *stateSocketAfter;
CoreCounterState *stateCoreBefore;
CoreCounterState *stateCoreAfter;

// times
uint64 timeBefore, timeAfter;

// number of CPU sockets
int nSockets;

// number of (logical) CPU cores
int nCores;

void sigHandler(int signo) {
  // get socket counter states
  for (int i = 0; i < nSockets; i++) {
    stateSocketAfter[i] = m->getSocketCounterState(i);
  }

  // get core counter states
  for (int i = 0; i < nCores; i++) {
    stateCoreAfter[i] = m->getCoreCounterState(i);
  }

  // get time
  timeAfter = m->getTickCount();

  // get consumed energy by all sockets
  double consumedJoulesTotal = 0.0;
  for (int i = 0; i < nSockets; i++) {
    consumedJoulesTotal +=
        getConsumedJoules(stateSocketBefore[i], stateSocketAfter[i]);
  }

  // calculate average power
  double avgPower =
      1000.0 * consumedJoulesTotal / double(timeAfter - timeBefore);

  // get per-core average frequency
  double *avgCoreFrequencies = new double[nCores];
  for (int i = 0; i < nCores; i++) {
    avgCoreFrequencies[i] =
        getAverageFrequency(stateCoreBefore[i], stateCoreAfter[i]);
  }

  std::cout << std::endl;
  std::cout << "Duration: " << double(timeAfter - timeBefore) / 1000.0
            << std::endl;
  std::cout << "Energy: " << consumedJoulesTotal << std::endl;
  std::cout << "Power: " << avgPower << std::endl;
  for (int i = 0; i < nCores; i++) {
    std::cout << "Frequency Core " << i << " : " << avgCoreFrequencies[i]
              << std::endl;
  }

  delete[] stateSocketBefore;
  delete[] stateSocketAfter;
  delete[] stateCoreBefore;
  delete[] stateCoreAfter;
  delete[] avgCoreFrequencies;

  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  // init pcm
  m = PCM::getInstance();

  // make sure everything is good
  if (!m->good()) {
    std::cerr << "ERROR: could not access CPU counters" << std::endl;
    return -1;
  }

  if (m->program() != PCM::Success) {
    std::cerr << "ERROR: could not program" << std::endl;
    return -1;
  }

  // get number of CPU sockets
  nSockets = m->getNumSockets();

  // get number of (logical) cores
  nCores = m->getNumCores();

  // create socket counter states
  stateSocketBefore = new SocketCounterState[nSockets];
  stateSocketAfter = new SocketCounterState[nSockets];

  // create core counter states
  stateCoreBefore = new CoreCounterState[nCores];
  stateCoreAfter = new CoreCounterState[nCores];

  // save 'before' socket counter states
  for (int i = 0; i < nSockets; i++) {
    stateSocketBefore[i] = m->getSocketCounterState(i);
  }

  // save 'before' core counter states
  for (int i = 0; i < nCores; i++) {
    stateCoreBefore[i] = m->getCoreCounterState(i);
  }

  // save 'before' time
  timeBefore = m->getTickCount();

  // register sigint handler
  signal(SIGINT, sigHandler);

  while (true) {
    // sleep. pretty much forever.
    sleep(4294967295);
  }

  return 0;
}
