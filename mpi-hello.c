#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char** argv){
  int pid;
  int nproc;

  char message[10240];
  MPI_Status status;
  int tag = 50;

  double t1, t2;
  t1 = MPI_Wtime();


  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  int rc = system("hostname");
  double times[nproc];
  times[pid] = 0;
  int x;
  for(x = 0; x< nproc; x++) {
    double t1, t2;
    if(pid != x) {
        MPI_Recv(message, 100, MPI_CHAR, x, tag, MPI_COMM_WORLD, &status);
        MPI_Ssend(message, strlen (message)+1, MPI_CHAR, x, tag, MPI_COMM_WORLD);
        printf("RECEIVED FROM: %d\n",status.MPI_SOURCE);
    }
    else {
      sprintf(message, "Hello from process %d!", pid);
      int y;
      for(y = 0; y<nproc; y++) {
        if(y != pid) {
          t1 = MPI_Wtime();
          MPI_Ssend(message, strlen (message)+1, MPI_CHAR, y, tag, MPI_COMM_WORLD);
          MPI_Recv(message, 100, MPI_CHAR, y, tag, MPI_COMM_WORLD, &status);
          t2 = MPI_Wtime();
          times[y] = t2-t1;
          printf("SENT\n");
        }
      }
    }
  }
  MPI_Finalize();
  return(0);
}
