# root directory of MATLAB installation
MATLABROOT=/usr/pkg/matlab-2016b-update

all: mpiTime

mpiTime: mpiTool.c
	mpicc -O3 mpiTool.c

clean:
	rm -f mpiTime *.o
