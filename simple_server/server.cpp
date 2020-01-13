// gcc -pthread -o serwer server.c

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <vector>

#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define BUFF_SIZE 3
#define LOGIN_SIZE 8
#define DESCRIPTION_ARRAY_SIZE 100

using namespace std;

struct polozenie
{
    int k;
    int w;
};

polozenie rozkodujPozycje(string pozycja) {
    int tab [2];
    switch (pozycja.at(0)) {
        case 'A':
        tab[0] = 1;
            break;
        case 'B':
        tab[0] = 2;
            break;
        case 'C':
        tab[0] = 3;
            break;
        case 'D':
        tab[0] = 4;
            break;
        case 'E':
        tab[0] = 5;
            break;
        case 'F':
        tab[0] = 6;
            break;
        case 'G':
        tab[0] = 7;
            break;
        case 'H':
        tab[0] = 8;
            break;
    }
    tab[1] = pozycja.at(1)-'0';
    polozenie pol;
    pol.k = tab[0];
    pol.w = tab[1];
    return pol;
}

string zakodujPozycje(int a, int b) {
    string pozycja;
    switch (a) {
        case 1: pozycja.append("A");
            break;
        case 2: pozycja.append("B");
            break;
        case 3: pozycja.append("C");
            break;
        case 4: pozycja.append("D");
            break;
        case 5: pozycja.append("E");
            break;
        case 6: pozycja.append("F");
            break;
        case 7: pozycja.append("G");
            break;
        case 8: pozycja.append("H");
            break;
    }
    pozycja.append(to_string(b));
    return pozycja;
}

class Bierka {
public:
    bool kolor_czarny;
    polozenie pozycja;
    int ilosc_wykon_ruchow=0;
    virtual vector<string> znajdzMozliweRuchy(int glupia_szachownica [][9])=0;
    virtual string getNazwaBierki()=0;
};

class Pion : public Bierka {
public:
    Pion (bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };
    string getNazwaBierki() {
        return "pion";
    };
    vector<string> znajdzMozliweRuchy(int tab [][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        if (kolor_czarny) {
            if (pol[1] != 1) {
                if (tab[pol[0]][pol[1] - 1] == 0) {
                    if (ilosc_wykon_ruchow == 0 && tab[pol[0]][pol[1] - 2] == 0)
                        mozliwe_ruchy.push_back(zakodujPozycje(pol[0], pol[1] - 2));
                    mozliwe_ruchy.push_back(zakodujPozycje(pol[0], pol[1] - 1));
                }
                if (pol[0] - 1 > 0 && pol[1] - 1 > 0)
                    if (tab[pol[0] - 1][pol[1] - 1] == -1)
                        mozliwe_ruchy.push_back(zakodujPozycje(pol[0] - 1, pol[1] - 1));
                if (pol[0] + 1 < 9 && pol[1] - 1 > 0)
                    if (tab[pol[0] + 1][pol[1] - 1] == -1)
                        mozliwe_ruchy.push_back(zakodujPozycje(pol[0] + 1, pol[1] - 1));
            }
        } else {
            if (pol[1] != 8) {
                if (tab[pol[0]][pol[1] + 1] == 0) {
                    if (ilosc_wykon_ruchow == 0 && tab[pol[0]][pol[1] + 2] == 0)
                        mozliwe_ruchy.push_back(zakodujPozycje(pol[0], pol[1] + 2));
                    mozliwe_ruchy.push_back(zakodujPozycje(pol[0], pol[1] + 1));
                }
                if (pol[0] - 1 > 0 && pol[1] + 1 < 9)
                    if (tab[pol[0] - 1][pol[1] + 1] == -1)
                        mozliwe_ruchy.push_back(zakodujPozycje(pol[0] - 1, pol[1] + 1));
                if (pol[0] + 1 < 9 && pol[1] + 1 < 9)
                    if (tab[pol[0] + 1][pol[1] + 1] == -1)
                        mozliwe_ruchy.push_back(zakodujPozycje(pol[0] + 1, pol[1] + 1));
            }
        }
        return mozliwe_ruchy;
    };
};



class Szachownica {
private:
    string nazwa = "test";
    int glupia_szachownica[9][9];
    double kapital; // początkowy
public:
    string getNazwa() {
        return nazwa;
    }
};

//struktura zawierająca dane, które zostaną przekazane do wątku
struct data_thread_join
{
// -2 gdy nie istnieje
// -1 gdy istnieje, ale nie ma przeciwnika
//  0 -  DESCRIPTION_ARRAY_SIZE deskryptor przeciwnika
int * descriptor_array;
};

struct data_thread_game
{
    int first_socket_descriptor;
    int second_socket_descriptor;
};

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    int czy_polaczony=1;
    struct data_thread_game *th_data = (struct data_thread_game*)t_data;
    //dostęp do pól struktury: (*th_data).pole
    while(czy_polaczony)
    {
        int read_result;
        printf("Powstałem!\n");
        char * login_1 = (char *) malloc(sizeof(char)*LOGIN_SIZE);
        char * login_2 = (char *) malloc(sizeof(char)*LOGIN_SIZE);
        write(th_data->first_socket_descriptor,"log",BUFF_SIZE);
        read_result = read(th_data->first_socket_descriptor, login_1, LOGIN_SIZE);
        if (read_result>0) {
            printf("Login gracza pierwszego: %s\n",login_1);
        } else printf("Blad");

        write(th_data->second_socket_descriptor,"log",BUFF_SIZE);
        read_result = read(th_data->second_socket_descriptor, login_2, LOGIN_SIZE);
        if (read_result>0) {
            printf("Login gracza drugiego: %s\n",login_2);
        } else printf("Blad");

        write(th_data->first_socket_descriptor,login_2,LOGIN_SIZE);
        write(th_data->second_socket_descriptor,login_1,LOGIN_SIZE);
        // create game
        int szachownica [9][9] = {
                {0,0,0,0,0,0,0,0,0},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
                {0,1,1,0,0,0,0,-1,-1},
        };
        polozenie poz;
        poz.k=1;
        poz.w=2;
        Pion pionek(false, poz);
        vector<string> vectorek = pionek.znajdzMozliweRuchy(szachownica);
        //cout <<"oto vectorek: " << vectorek[0] << vectorek[1];
        printf("%s, %s\n",vectorek[0].c_str(),vectorek[1].c_str());
        write(th_data->first_socket_descriptor,"bia",BUFF_SIZE);
        write(th_data->second_socket_descriptor,"cza",BUFF_SIZE);
        while (1) {
            sleep(1);
        }
    }
    free(th_data);
    pthread_exit(NULL);
}

void *ThreadJoin(void *t_data)
{
    pthread_detach(pthread_self());
    struct data_thread_join *th_data = (struct data_thread_join*)t_data;
    while(1)
    {
        int first = -1;
        int second = -1;
        for (int i=0;i<DESCRIPTION_ARRAY_SIZE;i++) {
            if(th_data->descriptor_array[i]==-2) {
                if (first == -1) {
                    first = i;
                } else {
                    second = i;
                    break;
                }
            }
        }
        if (second!=-1) {
            printf("%d oraz %d moga grac\n",first,second);
            th_data->descriptor_array[first]=second;
            th_data->descriptor_array[second]=first;

            int create_result = 0;
            //uchwyt na wątek
            pthread_t thread1;
            //dane, które zostaną przekazane do wątku
            struct data_thread_game* game_data= (struct data_thread_game *) malloc(sizeof(struct data_thread_game));
            game_data->first_socket_descriptor=first;
            game_data->second_socket_descriptor=second;
            create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *) game_data);

            if (create_result) {
                printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
                exit(-1);
            }

        }
    }
}

//utworzenie wątku który będzie tworzył nowe pokoje
void handleConnection(int * descriptor_array) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    //dane, które zostaną przekazane do wątku
    struct data_thread_join* t_data= (struct data_thread_join *) malloc(sizeof(struct data_thread_join));
    t_data->descriptor_array=descriptor_array;
    create_result = pthread_create(&thread1, NULL, ThreadJoin, (void *) t_data);

    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
}

int main(int argc, char* argv[])
{   int * descriptor_array = (int *) malloc(sizeof(int)*DESCRIPTION_ARRAY_SIZE);
   int server_socket_descriptor;
   int connection_socket_descriptor;
   int bind_result;
   int listen_result;
   char reuse_addr_val = 1;
   struct sockaddr_in server_address;

   //inicjalizacja gniazda serwera
   memset(descriptor_array, -1, sizeof(int)*DESCRIPTION_ARRAY_SIZE);
   memset(&server_address, 0, sizeof(struct sockaddr));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(SERVER_PORT);

   server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_descriptor < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
       exit(1);
   }
    //////////////////////


//    int szachownica [9][9] = {
//            {0,0,0,0,0,0,0,0,0},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//            {0,1,1,0,0,0,0,-1,-1},
//   };
//   polozenie poz;
//   poz.k=1;
//   poz.w=2;
//    Pion pionek(false, poz);
//    vector<string> vectorek = pionek.znajdzMozliweRuchy(szachownica);
//    cout <<"oto vectorek: " << vectorek[0] << vectorek[1];
    //////////////////////
   setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));
   printf("Socket serwera: %d\n",server_socket_descriptor);
   bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
   if (bind_result < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
       exit(1);
   }

   listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
   if (listen_result < 0) {
       fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
       exit(1);
   }

    handleConnection(descriptor_array);

   while(1)
   {
       connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
       printf("Nastąpiło połączenie na sockecie: %d\n",connection_socket_descriptor);
       if (connection_socket_descriptor < 0)
       {
           fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
           exit(1);
       }

       descriptor_array[connection_socket_descriptor]=-2;
   }
   free(descriptor_array);
   close(server_socket_descriptor);
   return(0);
}
