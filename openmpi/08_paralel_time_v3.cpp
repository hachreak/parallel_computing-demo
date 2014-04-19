/**
 * Copyright (C) 2014 Leonardo Rossi <leonardo.rossi@studenti.unipr.it>
 *
 * This source code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This source code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this source code; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <iostream>
#include <math.h>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]){
  int rank, size, tag = 10;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // init vector
  int vector[size];
  
  if(rank == 0){
    // init matrix
    for(int i=0; i<size; i++){
      vector[i] = i;
    }
  }

  // start
  double start = MPI_Wtime();

  // broadcast vector using send/recv
  if(rank == 0){
    int j = rank;
    for(int i=0; i<sqrt(size); i++){
      j = rank + exp2(i);
      MPI_Send(&i, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
      MPI_Send(&vector, size, MPI_INT, j, tag, MPI_COMM_WORLD);
    }
  }else{
    int n = 0;
    MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    n++;
    MPI_Recv(&vector, size, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int j = rank;
    for(int i=n; i<sqrt(size); i++){
      j = rank + exp2(i);
      if(j < size){
          MPI_Send(&n, 1, MPI_INT, j, tag, MPI_COMM_WORLD);
          n++;
          MPI_Send(&vector, size, MPI_INT, j, tag, MPI_COMM_WORLD);
      }
    }
  }

  // barrier to sync
  MPI_Barrier(MPI_COMM_WORLD);
  
  if(rank == 0){
    // end
    double end = MPI_Wtime();

    cout<<"Execution time: "<<(end-start)<<endl;
  }

  MPI_Finalize();
  return 0;
}
