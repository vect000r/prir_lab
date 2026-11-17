#include "SequentialDataProcessor.h"

#include<iostream>

using namespace std;

void SequentialDataProcessor::processData(int repetitions) {
  for (int r = 0; r < repetitions; r++)
    singleStep();
}

void SequentialDataProcessor::singleStep() {
  for (int row = 1; row < rows; row += 3)
    for (int col = 1; col < cols; col += 3) {
      blockData(row, col);
      blockMargin(row, col);
      data[row][col] = blockProcessor->process(blockDataVector, blockMarginVector);          
    }
  for (int row = 1; row < rows; row += 3)
    for (int col = 1; col < cols; col += 3)
      expandValue(row, col);
}

void SequentialDataProcessor::blockData(int rowM, int colM) {
  int idx = 0;
  for (int row = -1; row < 2; row++)
    for (int col = -1; col < 2; col++)
      blockDataVector[idx++] = data[row + rowM][col + colM];
}

void SequentialDataProcessor::blockMargin(int rowM, int colM) {
  blockMarginVector[0] = safeGet(rowM - 2, colM);
  blockMarginVector[1] = safeGet(rowM, colM - 2);
  blockMarginVector[2] = safeGet(rowM, colM + 2);
  blockMarginVector[3] = safeGet(rowM + 2, colM);
}

void SequentialDataProcessor::expandValue(int rowM, int colM) {
  double v = data[rowM][colM];

  for (int col = -1; col < 2; col++) {
    data[rowM - 1][colM + col] = v;
    data[rowM + 1][colM + col] = v;
  }
  data[rowM][colM - 1] = v;
  data[rowM][colM + 1] = v;
}
