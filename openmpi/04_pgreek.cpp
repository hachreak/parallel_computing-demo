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
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]){
  int rank, size;
  double PI25DT = 3.141592653589793238462643;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n = 1;

  if(rank == 0){
    cin >> n;

    if(n > size){
      cout<<"We don't have enought process... we use precision: "<<size<<"\n";
      n = size;
    }
  }

  // Broadcast precision
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

  double sum = 0.0;
  double val = 0.0;
  double h = 1.0 / (double)n;

  if(rank < n){
    // compute val only by first n process
    int i = rank;
    double x = h * ((double)(i + 1) - 0.5);
    val = 4.0 / (1.0 + x*x);
  }else{
    // exclude other processes
    val = 0;
  }

  MPI_Reduce(&val, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if(rank == 0){
    double pi = sum * h;

    cout << "PI: "<< pi << " Error: "<<fabs(pi - PI25DT)<<"\n";
  }

  MPI_Finalize();
  return 0;
}
