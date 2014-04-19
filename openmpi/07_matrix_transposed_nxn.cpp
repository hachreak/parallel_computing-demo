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

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int matrix[size][size];
  int buffer_input[size];
  int buffer_output[size];

  if(rank == 0){
     cout<<"Init Matrix..\n";
     for(int i=0; i<size; i++){
       cout<<"|";
       for(int j=0; j<size; j++){
         matrix[i][j] = i*size + j;
         cout<<matrix[i][j]<<"\t";
       }
       cout<<"|\n";
     }
  }

  // send matrix
  MPI_Scatter(matrix[0], size, MPI_INT, buffer_input, size, MPI_INT, 0, MPI_COMM_WORLD);

  // recv transpost vector
  MPI_Alltoall(buffer_input, 1, MPI_INT, buffer_output, 1, MPI_INT, MPI_COMM_WORLD);

  // send back transposed matrix
  MPI_Gather(buffer_output, size, MPI_INT, matrix[0], size, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank == 0){
     cout<<"Transposed Matrix..\n";
     for(int i=0; i<size; i++){
       cout<<"|";
       for(int j=0; j<size; j++){
         cout<<matrix[i][j]<<"\t";
       }
       cout<<"|\n";
     }
  }

  MPI_Finalize();
  return 0;
}
