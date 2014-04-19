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
#include <random>

#define RANDMAX 10000000
#define REPETITIONS 10000
#define DECIMAL_PRECISION 6
#define ITERATION_EACH_TIME 10000

using namespace std;

int main(int argc, char *argv[]){
  double PI25DT = 3.1415926536;
  double total_inside_circle = 0;

  long init_for = REPETITIONS * ITERATION_EACH_TIME;
  double pi = 0.0;
  double diff = 0.0;

  // set precision on cout
  cout.precision(DECIMAL_PRECISION);

  // set minimum error
  double precision = pow(10.0, -DECIMAL_PRECISION );

  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0, 1.0);

  do{
    #pragma omp parallel for private(init_rand) reduction(+: total_inside_circle)
    for(long i=0; i<REPETITIONS; i++){
      
      long local_total_inside_circle = 0;
      for(long j=0; j<ITERATION_EACH_TIME; j++){
        // generate random number for X
        double x = distribution(generator);
        // generate random number for Y
        double y = distribution(generator);

        // check if is inside the circle
        if((x*x + y*y) <= 1){
          local_total_inside_circle++;
        }
      }
      total_inside_circle += local_total_inside_circle;
    }

    // compute PI approximate value
    pi = (total_inside_circle / init_for) * 4;
    diff = fabs(pi - PI25DT);

    cout << "The approximate PI's value is: " << pi << " with a error of " << diff << endl;

    init_for += REPETITIONS * ITERATION_EACH_TIME;
  }while(diff > precision);
 
  cout<<"Point total: "<<init_for<<endl;
  cout<<"Point inside the circle: "<<total_inside_circle<<endl;
  cout<<"Precision: "<<precision<<endl;
  cout << "The approximate PI's value is: " << pi << " with a error of " << fabs(pi - PI25DT) << endl;
}
