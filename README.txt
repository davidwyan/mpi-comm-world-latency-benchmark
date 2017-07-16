MPI TIMING TOOL

By: Rob Martorano and David Yan

GETTING STARTED
===============

First, set up an RSA key on your machine to allow this script to access multiple
virtual machines in the same cluster without requiring you to sign in to every
machine every time.

The steps for this are simple:

ssh-keygen -t rsa

followed by

cat ~/.ssh/id_rsa.pub | ssh NETID@MACHINE_NAME.DEPARTMENT.duke.edu "mkdir -p ~/.ssh && cat >>  ~/.ssh/authorized_keys"

Example usage:
ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub | ssh NETID@linux1.cs.duke.edu "mkdir -p ~/.ssh && cat >>  ~/.ssh/authorized_keys"

MPD.HOSTS FILE
==============

The MPD.hosts file allows you to specify which set of machines you want to use
as your particular network. The machines specified in this file will specify
the set of machines that this MPI tool will perform timing on.

Example usage:
linux1
linux2
linux3
linux4

The above setup says that we want to run our MPI timing tool on a network of
machines linux1, linux2, linux3, and linux4.

RUNNING THE PROGRAM
===================

Our MPI tool allows you to time the MPI send process in a specified cluster of
machines. Once you have your MPD.hosts file set up, you are ready to run the
tool and begin timing your network.

To do this, open the mpiMeasure.sh bash file. Inside, you will see three parameters,
numProcesses, numIterations, and dataSize. The numProcesses variable specifies how many
copies of the program to run on the given nodes. The numIterations variable
specifies how many times to run and time the send processes. We use this variable
in the MPI tool to average the results. In short, the more iterations you choose
to perform, the higher accuracy in results you may receive. Finally, the dataSize
parameter represents the size of data that you would like to send. We use the
dataSize parameter to create a character array of size dataSize that we will
use as the data to send for our MPI send calls.

The first input to the bash script call will be for the number of processes.
The second input to the bash script call will be for the number of iterations.
The third input to the bash script will be for the data size.

Example bash script call:
bash mpiMeasure.sh 4 10 2048

which corresponds to arguments:
numProcesses = 4
numIterations = 10
dataSize = 2048


Alternately,
Compile with:
mpicc -O3 mpiTool.c
then run with
mpirun --np numProcesses --default-hostfile mpd.hosts ./a.out numIterations dataSize

Result will be a Matlab Adjacency Plot showing which particular nodes and processors
are slower in relation to the others.

MATLAB FUNCTION
===============
Currently, we have a Matlab function plotGraph.m that reads in the binary data file that our
MPI tool saves, opens the data, and performs a simple plot of the adjacency
matrix using imagesc(). You can feel free to change any parts of plotGraph.m to
display plots of your preference.
