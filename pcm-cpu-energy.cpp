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
SocketCounterState *stateBefore;
SocketCounterState *stateAfter;

// times
uint64 timeBefore, timeAfter;

// number of CPU sockets
int nSockets;

void sigHandler(int signo) {
  // get counter state
  for (int i = 0; i < nSockets; i++) {
    stateAfter[i] = m->getSocketCounterState(i);
  }

  // get time
  timeAfter = m->getTickCount();

  // get consumed energy by all sockets
  double consumedJoulesTotal = 0.0;
  for (int i = 0; i < nSockets; i++) {
    consumedJoulesTotal += getConsumedJoules(stateBefore[i], stateAfter[i]);
  }

  // calculate average power
  double avgPower =
      1000.0 * consumedJoulesTotal / double(timeAfter - timeBefore);

  std::cout << std::endl;
  std::cout << "Duration: " << double(timeAfter - timeBefore) / 1000.0
            << std::endl;
  std::cout << "Energy: " << consumedJoulesTotal << std::endl;
  std::cout << "Power: " << avgPower << std::endl;

  delete[] stateBefore;
  delete[] stateAfter;

  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  // init pcm
  m = PCM::getInstance();

  // get cpu model and make sure its supported
  if (m->hasPCICFGUncore() == false) {
    std::cerr << "ERROR: unsupported CPU" << std::endl;
    exit(EXIT_FAILURE);
  }

  // get number of CPU sockets
  nSockets = m->getNumSockets();

  // create counter states
  stateBefore = new SocketCounterState[nSockets];
  stateAfter = new SocketCounterState[nSockets];

  // save 'before' counter state
  for (int i = 0; i < nSockets; i++) {
    stateBefore[i] = m->getSocketCounterState(i);
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
