#ifndef SIMPLE_BLOCK_PROCESSOR_H_
#define SIMPLE_BLOCK_PROCESSOR_H_

#include"BlockProcessor.h"

class SimpleBlockProcessor : public BlockProcessor {
  public:
  double process( double *data, double *margin );
};

#endif