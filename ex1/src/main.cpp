#include <iostream>

#include "BlockProcessor.h"
#include "DataProcessor.h"
#include "SequentialDataProcessor.h"
#include "SimpleBlockProcessor.h"

#define __MPI_ON

#ifdef MPI_ON
#include "MPIDataProcessor.h"
#include <mpi.h>
#endif

using namespace std;

void initialize_data(double **data, int rows, int cols);

int main(int args, char **argv) {

  constexpr int cols = 1800;
  constexpr int rows = 3300;
  constexpr int steps = 500;
  constexpr int report_every = 50;

  int rank = 0;
  double **data;

#ifdef MPI_ON
  MPI_Init(&args, &argv);
  cout << "MPI version " << MPI_VERSION << endl;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  DataProcessor *processor = new MPIDataProcessor();
#else
  DataProcessor *processor = new SequentialDataProcessor();
#endif

  BlockProcessor *blockProcessor = new SimpleBlockProcessor();

// każdy z procesów dysponuje narzędziem do przetwarzania klocków
  processor->setBlockProcessor(blockProcessor);

  if (rank == 0) {
    data = new double *[rows];
    for (int row = 0; row < rows; row++)
      data[row] = new double[cols];

// początkową dużą tablicą dysponuje wyłącznie proces o rank=0
    initialize_data(data, rows, cols);
    processor->setData(data, rows, cols);
  }

// współdzielimy dane pomiędzy procesami
  processor->shareData();

  for (int step = 0; step < steps; step += report_every) {
// każdy z procesów liczy
    processor->processData(report_every);
// procesy przekazują dane do procesu rank = 0
    processor->collectData();
    if (rank == 0) {
// proces rank = 0 sprawdza dane zgromadzone w dużej tablicy.    
      cout << "Dana na pozycji 0,0 to : " << processor->get(0, 0) << endl;
      cout << "Dana na pozycji 100,100 to : " << processor->get(100, 100) << endl;
    }
  }

#ifdef MPI_ON
  MPI_Finalize();
#endif
}

void initialize_data(double **data, int rows, int cols) {
  cout << "Przygotowanie danych w dużej tablicy" << endl;
  for (int row = 0; row < rows; row++)
    for (int col = 0; col < cols; col++) {
      data[row][col] = col + row;
    }
  cout << "Przygotowanie danych w dużej tablicy - gotowe" << endl;
}
