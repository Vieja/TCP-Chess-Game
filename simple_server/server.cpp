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

#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define BUFF_SIZE 3
#define DESCRIPTION_ARRAY_SIZE 100

using namespace std;

class Szachownica {
private:
    string nazwa = "test";
    double kapital; // początkowy
public:
    string getNazwa() {
        return nazwa;
    }
};

//struktura zawierająca dane, które zostaną przekazane do wątku
struct thread_data_t
{
// -2 gdy nie istnieje
// -1 gdy istnieje, ale nie ma przeciwnika
//  0 -  DESCRIPTION_ARRAY_SIZE deskryptor przeciwnika
int connection_socket_descriptor;
int * descriptor_array;
};

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data)
{
    pthread_detach(pthread_self());
    int czy_polaczony=1;
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
    char * buffor = (char *) malloc(sizeof(char)*BUFF_SIZE);
    //dostęp do pól struktury: (*th_data).pole
    //TODO (przy zadaniu 1) klawiatura -> wysyłanie albo odbieranie -> wyświetlanie
    while(czy_polaczony)
    {
        while (th_data->descriptor_array[th_data->connection_socket_descriptor] == -2) {
            sleep(1);
        }
        printf("Poloczono\n");
        while (1) {
            sleep(1);
        }
    }
    free(th_data);
    free(buffor);
    pthread_exit(NULL);
}

void *ThreadJoin(void *t_data)
{
    pthread_detach(pthread_self());
    struct thread_data_t *th_data = (struct thread_data_t*)t_data;
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
        }
    }
}

//funkcja obsługująca połączenie z nowym klientem
void handleConnection(int connection_socket_descriptor,int * descriptor_array,int which) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;

    //uchwyt na wątek
    pthread_t thread1;
    //dane, które zostaną przekazane do wątku
    //TODO dynamiczne utworzenie instancji struktury thread_data_t o nazwie t_data (+ w odpowiednim miejscu zwolnienie pamięci)
    //TODO wypełnienie pól struktury
    struct thread_data_t* t_data= (struct thread_data_t *) malloc(sizeof(struct thread_data_t));
    t_data->descriptor_array=descriptor_array;
    t_data->connection_socket_descriptor=connection_socket_descriptor; 
    if (which==0) {
        create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *) t_data);
    } else {
        create_result = pthread_create(&thread1, NULL, ThreadJoin, (void *) t_data);
    }
    if (create_result){
    printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
    exit(-1);
    }
    //TODO (przy zadaniu 1) odbieranie -> wyświetlanie albo klawiatura -> wysyłanie
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
    Szachownica gra;
    string test = gra.getNazwa();
    cout << test << endl;
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

    handleConnection(0,descriptor_array,1);

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
       handleConnection(connection_socket_descriptor,descriptor_array,0);
   }
   free(descriptor_array);
   close(server_socket_descriptor);
   return(0);
}
