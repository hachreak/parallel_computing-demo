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
#include <cstdlib>
#include <vector>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]){
  int rank, size;

  // Init MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int rows = size, cols = 4;

  long vector[cols];
  long matrix[rows][cols];
  long single_row[cols];
  long result[cols];

  if(rank == 0){
    // init matrix
    cout<<"Matrix\n";
    for(int i=0; i<rows; i++){
      cout<<"|";
      for(int j=0; j<cols; j++){
        matrix[i][j] = i*rows + (j+1);
        cout<<matrix[i][j]<<"\t";
      }
      cout<<"|\n";
    }

    // init vector
    cout<<"Vector\n|";
    for(long i=0; i<cols; i++){
      vector[i] = i + 1;
      cout<<vector[i]<<"\t";
    }
    cout<<"|\n";
  }

  // broadcast vector
  MPI_Bcast(&vector, cols, MPI_LONG, 0, MPI_COMM_WORLD);

  // scatter the vectors
  MPI_Scatter(matrix[0], cols, MPI_LONG, single_row, cols, MPI_LONG, 0, MPI_COMM_WORLD);

  // compute
  long value = 0;
  for(int i=0; i<cols; i++){
    value += vector[i] * single_row[i];
  }

  // return result
  MPI_Gather(&value, 1, MPI_LONG, &result, 1, MPI_LONG, 0, MPI_COMM_WORLD);

  if(rank==0){
    // Print result!
    cout<<"Result\n|";
    for(long i=0; i<cols; i++){
      cout<<result[i]<<"\t";
    }
    cout<<"|\n";
  }

  MPI_Finalize();
  return 0;
}
