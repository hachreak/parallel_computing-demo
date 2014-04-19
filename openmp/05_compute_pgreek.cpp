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
#include <math.h>
#include <iostream>

#define RANDMAX 1000000
#define REPETITIONS 10000000

using namespace std;

int main(int argc, char *argv[]){
  bool init_rand = false;

  double PI25DT = 3.1415926536;
  double total_inside_circle = 0;

  #pragma omp parallel for private(init_rand) reduction(+: total_inside_circle)
  for(long i=0; i<REPETITIONS; i++){
    if(!init_rand){
      // obtain thread number
      int tid = omp_get_thread_num();
      // init random seed
      srand(time(NULL)+tid);
      // finish the init
      init_rand = true;
    }

    // generate random number for X
    double x = ((double)(rand() % (RANDMAX+1))) / RANDMAX;
    // generate random number for Y
    double y = ((double)(rand() % RANDMAX)) / RANDMAX;

    // check if is inside the circle
    if((x*x + y*y) <= 1){
      total_inside_circle++;
    }
  }
 
  // compute PI approximate value
  double pi = (total_inside_circle / REPETITIONS) * 4;

  cout<<"Point total: "<<REPETITIONS<<endl;
  cout<<"Point inside the circle: "<<total_inside_circle<<endl;
  cout << "The approximate PI's value is: " << pi << " with a error of " << fabs(pi - PI25DT) << endl;
}
