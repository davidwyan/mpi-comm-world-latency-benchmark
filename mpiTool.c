/*
 mpiTool.c
 This file contains an implementation of a tool that allows a user to time the
 MPI send process in a specified cluster of machines.
 More information about setup and running this tool can be found in the
 README file.
*/

/*
------- ----------------------
By: Rob Martorano and David Yan
-----------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"
#include <math.h>
#include </usr/pkg/matlab-2016b-update/extern/include/engine.h>

int main(int argc, char** argv){
  int pid;
  int nproc;

  MPI_Status status;
  int tag = 50;

  double t1, t2;
  t1 = MPI_Wtime();

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  int numOfTrials = atoi(argv[1]);
  int messageSize = atoi(argv[2]);
  char message[messageSize];

  int rc = system("hostname");
  double timesForNode[nproc];

  timesForNode[pid] = 0;

  //Outer through each process (this will be the the sender) for the iteration.
  int sender;
  for(sender = 0; sender< nproc; sender++) {
    //Loop through number of trials
    int trial;
    for(trial = 0;trial<numOfTrials;trial++) {
      if(pid != sender) {
          MPI_Recv(message, sizeof(char)*messageSize, MPI_CHAR, sender, tag, MPI_COMM_WORLD, &status);
      }
      else {
        /* Loop is through each process (this will be the the receiver). We want
        the sender to send a message to every other node so we must send to all
        receivers */
        int receiver;
        for(receiver = 0; receiver<nproc; receiver++) {
          if(pid != receiver) {
            double startTime, endTime;
            startTime = MPI_Wtime();
            MPI_Ssend(message, sizeof(char)*messageSize, MPI_CHAR, receiver, tag, MPI_COMM_WORLD);
            endTime = MPI_Wtime();
            timesForNode[receiver] = timesForNode[receiver] + (endTime-startTime);
          }
        }
      }
    }
  }
  if(pid == 0) {
    /* Compile all the times into adjacency matrix on first node. */
    double timesMatrix[nproc][nproc];
    double tempTimesPerNode[nproc];

    int indFirstNodeTimes;
    for(indFirstNodeTimes = 0; indFirstNodeTimes < nproc; indFirstNodeTimes ++) {
      timesMatrix[0][indFirstNodeTimes] = timesForNode[indFirstNodeTimes];
    }

    int nodeNum;
    for(nodeNum = 1; nodeNum<nproc; nodeNum++) {
      MPI_Recv(&tempTimesPerNode, nproc, MPI_DOUBLE, nodeNum, tag, MPI_COMM_WORLD, &status);
      int ind;
      for(ind = 0; ind<nproc; ind++) {
        if(nodeNum == ind) {
          timesMatrix[nodeNum][ind] = 0;
        }
        else {
          timesMatrix[nodeNum][ind] = tempTimesPerNode[ind] / numOfTrials;
        }
      }
    }

    /*
    Write data to binary file
    */
    FILE *myFile = fopen("data","wb");
    int row,col;
    fwrite(timesMatrix,nproc*nproc*sizeof(double), 1, myFile);
    fclose(myFile);
    char command[1024];
    // Invoke Matlab plotting function plotGraph.m using system call
    sprintf(command, "matlab -r \"[data,dataOrg] = plotGraph('data',%d);\"",nproc);
    printf("%s\n",command);
    int systemRet = system(command);

  }
  else {
    /* If not the first node, send your list of times to first node to be compiled
    into adjacency matrix. */
    MPI_Ssend(timesForNode, nproc, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return(0);
}
