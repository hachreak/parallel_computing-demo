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
  int tama = 8;
  vector<long> vectorA, vectorB, subvectorA, subvectorB;

  // Init MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // compute how divide the two vectors
  long size_subvector = tama / size;

  if(rank == 0){
    cout << "Size of a single subvector: " << size_subvector << "\n";

    // init vectors
    vectorA.resize(tama, 0);
    vectorB.resize(tama, 0);
 
    // init vectors
    for(long i=0; i<tama; i++){
      vectorA[i] = i + 1;
      vectorB[i] = tama - i; //(i + 1)*10;
    }
  }

  // init subvectors
  subvectorA.resize(size_subvector, 0);
  subvectorB.resize(size_subvector, 0);

  // scatter the vectors
  MPI_Scatter(vectorA.data(), size_subvector, MPI_LONG, subvectorA.data(), size_subvector, MPI_LONG, 0, MPI_COMM_WORLD);
  MPI_Scatter(vectorB.data(), size_subvector, MPI_LONG, subvectorB.data(), size_subvector, MPI_LONG, 0, MPI_COMM_WORLD);

  // compute 
  long total = 0, subtotal = 0;
  for(int i=0; i<size_subvector; i++){
    subtotal += subvectorA[i] * subvectorB[i];
  }

  // reduce!
  MPI_Reduce(&subtotal, &total, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == 0){
    cout << "Total: "<< total << "\n";
  }

  MPI_Finalize();
  return 0;
}
