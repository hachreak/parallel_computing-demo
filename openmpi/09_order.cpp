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
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include "mpi.h"
using namespace std;

int main(int argc, char *argv[]){
  int rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // init random seed
  srand(time(NULL)+rank);

  // generate random number
  int value = rand() % 100;
  // create vector
  int randoms[size], backup[size], order[size];

  // return hit random value to root process
  MPI_Gather(&value, 1, MPI_INT, &randoms, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // backup of randoms
  std::copy(randoms, randoms + size, backup);
 
  // order array
  sort(randoms, randoms + size);

  if(rank == 0){
    cout<<"Original: ";
    for(int i=0; i<size; i++){
      cout<<"["<<backup[i]<<"]\t";
    }
    cout<<endl;
    cout<<"Ordered: ";
    for(int i=0; i<size; i++){
      cout<<"["<<randoms[i]<<"]\t";
    }
    cout<<endl;

    for(int i=0; i<size; i++){
      int n = randoms[i];
     int num = 0;
      for(int j=0; j<size; j++){
        if(backup[j] == randoms[i]){
          order[j] = i;
          cout<<"process["<<j<<"] > "<<randoms[i]<<endl;
        }
      }
    }
    cout<<"index: ";
    for(int i=0; i<size; i++){
      cout<<"["<<order[i]<<"]\t";
    }
    cout<<endl;

  }

  // send the position
  MPI_Scatter(order, 1, MPI_INT, &value, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // view result!
  std::cout<< "process["<<rank<<"] > ("<<value<<") \n";

  MPI_Finalize();
  return 0;
}
