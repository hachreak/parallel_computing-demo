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

#include <math.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  int n;
  
  cout<<"Enter the precision of the calculation (n > 0): ";
  cin>>n;

  double PI25DT = 3.141592653589793238462643;
  double h = 1.0 / (double) n;
  double sum = 0.0;

  #pragma omp parallel for reduction(+: sum)
  for(int i=1; i<=n; i++){
    double x = h * ((double)i - 0.5);
    sum += (4.0 / (1.0 + x*x));
  }

  double pi = sum * h;

  cout << "The approximate PI's value is: " << pi << " with a error of " << fabs(pi - PI25DT) << endl;
  return 0;
}
