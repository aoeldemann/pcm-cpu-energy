# pcm-cpu-energy

``pcm-cpu-energy`` is a simple utility that measures/estimates the CPU
energy/power consumption using the
[Processor Counter Monitor library](https://github.com/opcm/pcm) (via *MSR*,
*RAPL*). Evaluation is started when the application is called. It is stopped
and results are printed out when the application receives a *SIGINT* signal
(i.e. ``CTRL-C`` is pressed). The returned values represent the consumed
energy/power of all CPU sockets.

## Installation

1. Download [Processor Counter Monitor library](https://github.com/opcm/pcm)
   and compile a static library via ``make lib``.

2. Compile ``pcm-cpu-energy`` using the provided Makefile while passing the
   location of the *Processor Counter Monitor* library: ``PCM=<location> make``

3. Done!

## Using the application

Application must be called as *root*, i.e. ``sudo ./pcm-cpu-energy``. To stop
the evaluation, press ``CTRL-C``.
