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

class Bierka {
public:
    bool kolor_czarny;
    int polozenie[2];
    int ilosc_wykon_ruchow=0;
    virtual vector<string> znajdzMozliweRuchy(int glupia_szachownica [][9])=0;
    virtual string getNazwaBierki()=0;
};

class Pion : public Bierka {
public:
    string getNazwaBierki() {
        return "pion";
    };
    vector<string> znajdzMozliweRuchy(int glupia_szachownica [][9]) {
        vector<string> mozliwe;
        mozliwe.push_back("jeden ");
        mozliwe.push_back("dwa ");
        return mozliwe;
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

//    Szachownica gra;
//    string test = gra.getNazwa();
//    cout << test << endl;
    int szachownica [9][9] = {
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
            {1,1,0,0,0,0,-1,-1},
   };
    Pion pionek;
    vector<string> vectorek = pionek.znajdzMozliweRuchy(szachownica);
    cout <<"oto vectorek: " << vectorek[0] << vectorek[1];
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
