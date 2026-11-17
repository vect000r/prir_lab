#include "SimpleBlockProcessor.h"

#include <math.h>

double SimpleBlockProcessor::process(double *data, double *margin) {
  double sum = data[0];

  for (int i = 1; i < 9; i++)
    sum += data[i];

  for (int i = 0; i < 4; i++)
    sum += 1.0 + sqrt( fabs( sin( 3.141592 * margin[i] ) ) );

  return sum / 9;
}
