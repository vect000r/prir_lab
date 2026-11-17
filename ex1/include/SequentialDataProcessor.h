#ifndef SEQ_DATAPROCESSOR_H_
#define SEQ_DATAPROCESSOR_H_

#include "DataProcessor.h"

class SequentialDataProcessor : public DataProcessor {
private:
	void singleStep();
  void blockData( int rowM, int colM );
  void blockMargin( int rowM, int colM );
  void expandValue( int rowM, int colM );

  double blockDataVector[ 9 ];
  double blockMarginVector[ 4 ];
public:

	SequentialDataProcessor() {};
	~SequentialDataProcessor() {}
  
	void shareData() {}
	void collectData() {}
  
	void processData( int repetitions );
};

#endif /* SEQ_DATAPROCESSOR_H_ */
