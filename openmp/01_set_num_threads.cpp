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

#include <omp.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  int n_threads = -1;
  int default_val = 2;

  if(argc > 1){
    n_threads = atoi(argv[1]);
    if(n_threads == 0)
      n_threads = default_val;
  }else{
    n_threads = default_val;
  }
 
  omp_set_num_threads(n_threads);

  #pragma omp parallel
  {
    // obtain thread number
    int tid = omp_get_thread_num();

    cout << "Hello world from thread "<<tid<<endl;
  }
}
