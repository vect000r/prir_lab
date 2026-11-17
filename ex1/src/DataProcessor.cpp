#include "DataProcessor.h"

DataProcessor::DataProcessor() {}

DataProcessor::~DataProcessor() {}

void DataProcessor::setBlockProcessor(BlockProcessor *processor) {
  this->blockProcessor = processor;
}

void DataProcessor::setData(double **data, int rows, int cols) {
  this->data = data;
  this->rows = rows;
  this->cols = cols;
}

double DataProcessor::get(int row, int col) { return data[row][col]; }

double DataProcessor::safeGet(int row, int col) {
  if (row < 0 || row >= rows || col < 0 || col >= cols) {
    return 0.0;
  }

  return data[row][col];
}