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
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]){
  int rank, size;
  int tag = 10;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // send a msg to the next process
  int buff = rank;
  if(rank == 0){ 
    int dest = rank + 1;

    MPI_Send(&buff, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
  }else{
    int source = rank - 1;
    int dest = rank + 1;

    MPI_Recv(&buff, 1, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  
    cout<<"I'm the process "<<rank<<" and received "<<buff<<"\n";

    if(rank < (size - 1)){
      buff = rank;
      MPI_Send(&buff, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
  }

  MPI_Finalize();
  return 0;
}
