// code authored by professor
#include<unistd.h>
#include<iostream>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

using namespace std;

#define COMMON_FILE_NAME "/bin/bash"
#define SLOTS 10
#define MAX_SLEEP 300000
#define MIN_SLEEP 200000
#define REPETITIONS 150

int wolne_miejsce_semid;
int sa_dane_semid;
struct sembuf down_op = { 0, -1, SEM_UNDO };
struct sembuf up_op = { 0, 1, SEM_UNDO };
int *buffer;

void down( int semafor_id ) {
  semop( semafor_id, &down_op, 1 );
}

void up( int semafor_id ) {
  semop( semafor_id, &up_op, 1 );
}

void my_sleep( long sleep_time ) {
  usleep( sleep_time % MAX_SLEEP + MIN_SLEEP );
}

void producent() {
  int idx = 0;
  int tmp = 0;
  srandom( 1000 );
  
  for ( int i = 0; i < REPETITIONS; i++ ) {
  
    down( wolne_miejsce_semid );
  
    // produkcja nowych danych
    tmp ++;
    // spowolnienie produkcji
    my_sleep( random() );
  
    cout << "Producent: Umieszczam pod indeksem " << idx << " wartoĹÄ " << tmp << endl;
    buffer[ idx ] = tmp;
    idx = ( idx + 1 ) % SLOTS;
  
    up( sa_dane_semid );
  }  
}

void konsument() {
  int idx = 0;
  int tmp;
  int licznik = 0;
  srandom( 2000 );
  
  for ( int i = 0; i < REPETITIONS; i++ ) {
  
    down( sa_dane_semid );
  
    tmp = buffer[ idx ];
    licznik++;

    cout << "Konsument: Pobranie numer " << licznik << " spod indeksu " << idx << " wartoĹÄ " << tmp << endl;  
    idx = ( idx + 1 ) % SLOTS;

    // spowolnienie konsumpcji
    my_sleep( random() );
  
    up( wolne_miejsce_semid );
  }  
}


int main( void )
{
  key_t key = ftok( COMMON_FILE_NAME, 'A' );
  cout << "Klucz : " << key << endl;

// tworzymy dwa semafory  
  sa_dane_semid = semget( key, 1, 0600 | IPC_CREAT | IPC_EXCL ); 
  wolne_miejsce_semid = semget( key+1, 1, 0600 | IPC_CREAT | IPC_EXCL ); 
 
  if ( sa_dane_semid == -1 ) {
     cout << "Semafory juz wczesniej utworzono..." << endl;
     cout << "Mozna je usunac za pomoca polecenia: ipcrm -S kluczHEX" << endl;
     cout << "             lub z pomoca polecenia: ipcrm -s IDsemafora" << endl;
     cout << "Potrzebne dane mozna poznac za pomoca polecenia: ipcs" << endl;
     return 1;
  } else {
     cout << "sa_dane_semid       = " << sa_dane_semid << endl;
     cout << "wolne_miejsce_semid = " << wolne_miejsce_semid << endl;
  }

// inicjalizacja semafora z wolnym miejscem
  struct sembuf wolne_miejsce_ini = { 0, SLOTS, 0 }; // sem_num, sem_op, sem_flg
  semop( wolne_miejsce_semid, &wolne_miejsce_ini, 1 );
  struct sembuf sa_dane_ini = { 0, 0, 0 }; // sem_num, sem_op, sem_flg
  semop( sa_dane_semid, &sa_dane_ini, 1 );
 
// tworzymy pamiÄÄ wspĂłĹdzielonÄ dla bufora
  int shmid = shmget( key, SLOTS * sizeof( int ), 0600 | IPC_CREAT );  // tworzymy blok pamieci wspoldzelonej
  buffer = (int *)shmat( shmid, (void *) 0, 0 ); // podlaczamy blok pamieci wspoldzielonej do naszego programu

  if ( fork() ) 
    producent();
  else
    konsument();
 
  return 0;
}
