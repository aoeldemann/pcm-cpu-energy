EXE = pcm-cpu-energy

all:
ifndef PCM
	$(error Environment variable 'PCM' must point to Processor Counter \
	  Monitor installation (https://github.com/opcm/pcm))
endif

	g++ -c -std=c++11 -I$(PCM) $(EXE).cpp
	g++ -o $(EXE) $(EXE).o -L$(PCM) -lPCM -lpthread

clean:
	rm -f *.o $(EXE)
