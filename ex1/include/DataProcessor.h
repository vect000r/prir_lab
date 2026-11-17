#ifndef DATAPROCESSOR_H_
#define DATAPROCESSOR_H_

#include "BlockProcessor.h"

class DataProcessor {
protected:
	double **data;
	BlockProcessor *blockProcessor;
	int cols;
	int rows;
public:

	DataProcessor();
	virtual ~DataProcessor();
	void setBlockProcessor( BlockProcessor *processor );
	
	virtual void setData( double **data, int rows, int cols );

	virtual void shareData() = 0;
	virtual void collectData() = 0;
	virtual void processData( int repetitions ) = 0;

	virtual double get(int row, int col );
	double safeGet( int row, int col );
};

#endif /* DATAPROCESSOR_H_ */
