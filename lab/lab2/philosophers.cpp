// code authored by professor
#include<unistd.h>
#include<signal.h>
#include<iostream>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

using namespace std;

#define COMMON_FILE_NAME "/bin/bash"

int main( void )
{
  const int LICZBA_FILOZOFOW = 5;
  
  key_t key = ftok( COMMON_FILE_NAME, 'A' );
  cout << "Klucz : " << key << endl;
  
// ten proces tworzy semafory. Jesli juz sa to gotowe to zglaszany jest blad
  int semid = semget( key, LICZBA_FILOZOFOW, 0666 | IPC_CREAT | IPC_EXCL ); 
 
  if ( semid == -1 ) {
     cout << "Semafory juz wczesniej utworzono..." << endl;
     cout << "Mozna je usunac za pomoca polecenia: ipcrm -S kluczHEX" << endl;
     cout << "             lub z pomoca polecenia: ipcrm -s IDsemafora" << endl;
     cout << "Potrzebne dane mozna poznac za pomoca polecenia: ipcs" << endl;
     return 1;
  } else {
     cout << "SemID = " << semid << endl;
  }

  struct sembuf s_ini = { 0, 1, 0 }; // sem_num, sem_op, sem_flg
  // mamy jedna sztuke struktury sembuf, wiec musimy wykonac semop na kazdym
  // z semaforow z osobna
  for ( ; s_ini.sem_num < LICZBA_FILOZOFOW; s_ini.sem_num++ ) 
     semop( semid, &s_ini, 1 );
 
 
  cout << "Warto uruchomic dodatkowy terminal i wykonac polecenie ps -xf" << endl;

  // tworzymy dodatkowych filozofow

  int numerFilozofa;
  pid_t id;
  for ( numerFilozofa = 0; numerFilozofa < LICZBA_FILOZOFOW-1; numerFilozofa++ ) {
     id = fork();
     if ( id == 0 ) {
        break; // proces dziecka dalej sie nie rozwidla
     }
  }

  // tu juz program ma wiele procesow
  // jeden proces - jeden filozof

  cout << "Tu filozof " << numerFilozofa << " moje procesID to " << getpid() << endl;

  int widelec1 = numerFilozofa;
  int widelec2 = ( numerFilozofa + 1 ) % LICZBA_FILOZOFOW;

/*
  // powiazanie Filozof - Id widelcy
  // F0 - 0 1
  // F1 = 1 2
  // F2 = 2 3
  // F3 = 3 4
  // F4 = 4 0 lub po odkomentowaniu ponizszej czesci 0 4
   
  if ( widelec1 > widelec2 ) {
     widelec1 = widelec2;
     widelec2 = numerFilozofa;
  }
*/

// struktury do obslugi widelca 1
  struct sembuf w1_reserv = { widelec1, -1, SEM_UNDO };
  struct sembuf w1_release = { widelec1, 1, SEM_UNDO };

// struktury do obslugi widelca 2
  struct sembuf w2_reserv = { widelec2, -1, SEM_UNDO };
  struct sembuf w2_release = { widelec2, 1, SEM_UNDO };

  for ( int i = 1; i < 100; i++ ) {
     cout << "Tu filozof " << numerFilozofa << " prosze o widelec id " 
          << widelec1 << endl;
     semop( semid, &w1_reserv, 1 );
     cout << "Tu filozof " << numerFilozofa << "      mam widelec id " 
          << widelec1 << endl;
     sleep( 1 );
     cout << "Tu filozof " << numerFilozofa << "    prosze o widelec id " 
          << widelec2 << endl;
     semop( semid, &w2_reserv, 1 );
     cout << "Tu filozof " << numerFilozofa << "         mam widelec id " 
          << widelec2 << endl;
     sleep( 1 );
 
     cout << "Tu filozof " << numerFilozofa << "     zwracam widelec id " 
          << widelec1 << endl;
     semop( semid, &w1_release, 1 );
  
     cout << "Tu filozof " << numerFilozofa << "  zwracam widelec id " 
          << widelec2 << endl;
     semop( semid, &w2_release, 1 );

     cout << " - - - - - - - - - Tu filozof " << numerFilozofa << " zjadlem " 
          << i << " porcji" << endl;
  }
 
  return 0;
}
