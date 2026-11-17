#ifndef BLOCK_PROCESSOR_H_
#define BLOCK_PROCESSOR_H_

class BlockProcessor {

  public:
  virtual double process( double *data, double *margin ) = 0;

};

#endif