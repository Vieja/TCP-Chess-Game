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
#define BUFF_SIZE 5
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
    void wykonanoRuch(polozenie nowa) {
        ilosc_wykon_ruchow+=1;
        pozycja = nowa;
    };
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

class Goniec : public Bierka {
public:
    Goniec (bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };
    string getNazwaBierki() {
        return "goniec";
    };
    vector<string> znajdzMozliweRuchy(int tab [][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0]-1;
        int j = pol[1]+1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i--;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] + 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i++;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] - 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i++;
            j--;
        }

        i = pol[0] - 1;
        j = pol[1] - 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i--;
            j--;
        }
        return mozliwe_ruchy;
    };
};

class Hetman : public Bierka {
public:
    Hetman (bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };
    string getNazwaBierki() {
        return "hetman";
    };
    vector<string> znajdzMozliweRuchy(int tab [][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0]-1;
        int j = pol[1]+1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i--;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] + 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i++;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] - 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i++;
            j--;
        }

        i = pol[0] - 1;
        j = pol[1] - 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i--;
            j--;
        }

        i = pol[0]+1;
        j = pol[1];
        while(i > 0 && i < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i++;
        }

        i = pol[0]-1;
        j = pol[1];
        while(i > 0 && i < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i--;
        }

        i = pol[0];
        j = pol[1]+1;
        while(j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            j++;
        }

        i = pol[0];
        j = pol[1]-1;
        while(j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            j--;
        }

        return mozliwe_ruchy;
    };
};

class Krol : public Bierka {
public:
    Krol (bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };
    string getNazwaBierki() {
        return "krol";
    };
    vector<string> znajdzMozliweRuchy(int tab [][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0];
        int j = pol[1];
        if (i-1 > 0 && j - 1 > 0)
            if (tab[i-1][j-1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i-1,j-1));
        if (i-1 > 0)
            if (tab[i-1][j] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i-1,j));
        if (i-1 > 0 && j + 1 < 9)
            if (tab[i-1][j+1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i-1,j+1));
        if (j + 1 < 9)
            if (tab[i][j+1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i,j+1));
        if (i+1 < 9 && j + 1 < 9)
            if (tab[i+1][j+1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i+1,j+1));
        if (i+1 < 9)
            if (tab[i+1][j] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i+1,j));
        if (i+1 < 9 && j - 1 > 0)
            if (tab[i+1][j-1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i+1,j-1));
        if (j - 1 > 0)
            if (tab[i][j-1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i,j-1));
        return mozliwe_ruchy;
    };
};

class Skoczek : public Bierka {
public:
    Skoczek (bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };
    string getNazwaBierki() {
        return "skoczek";
    };
    vector<string> znajdzMozliweRuchy(int tab [][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        for (int i = 1;i < 9;i++)
            for (int j = 1;j < 9;j++) {
                if (tab[i][j] != 1) {
                    if (i-2 == pol[0] && j+1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i-1 == pol[0] && j+2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i+1 == pol[0] && j+2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i+2 == pol[0] && j+1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i+2 == pol[0] && j-1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i+1 == pol[0] && j-2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i-1 == pol[0] && j-2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                    if (i-2 == pol[0] && j-1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                }
            }
        return mozliwe_ruchy;
    };
};

class Wieza : public Bierka {
public:
    Wieza (bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };
    string getNazwaBierki() {
        return "wieza";
    };
    vector<string> znajdzMozliweRuchy(int tab [][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0]+1;
        int j = pol[1];
        while(i > 0 && i < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i++;
        }

        i = pol[0]-1;
        j = pol[1];
        while(i > 0 && i < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            i--;
        }

        i = pol[0];
        j = pol[1]+1;
        while(j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            j++;
        }

        i = pol[0];
        j = pol[1]-1;
        while(j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i,j));
                break;
            } else break;
            j--;
        }

        return mozliwe_ruchy;
    };
};

bool sprawdzCzyPoprawne(char* bufor) {
    bool odp = false;
    int z1 = (int) bufor[0];
    int z2 = (int) bufor[1];
    int z3 = (int) bufor[2];
    int z4 = (int) bufor[3];
    int z5 = (int) bufor[4];
    //cout << z1 << " " << z2 << " " << z3 << " " << z4 << " " << z5 << " " << endl;
    if ( ( 64 < z1 ) && ( z1 < 73 ) &&
         ( 64 < z4 ) && ( z4 < 73 ) &&
         ( 48 < z2 ) && ( z2 < 57 ) &&
         ( 48 < z5 ) && ( z5 < 57 ) &&
         (z3 == 58) ) {
        odp = true;
    }
    return odp;
}

bool sprawdzCzySzachNaszegoKrolaGdyTakiRuch(int tab[][9], polozenie potancjalneBicie, polozenie pozycjaKrola, vector<Bierka*> bierki_przeciwnika) {
    int szachownica [9][9];
    for (int i = 1; i < 9; i++)
        for (int j = 1; j < 9; j++)
            szachownica[i][j] = (-1) * tab[i][j];
    bool czyKrolJestAtakowany = false;
    for (vector<Bierka*>::iterator bierka = bierki_przeciwnika.begin(); bierka != bierki_przeciwnika.end(); ++bierka) {
        if ( (*bierka)->pozycja.k != potancjalneBicie.k || (*bierka)->pozycja.w != potancjalneBicie.w ) {
            vector <string> tmp = (*bierka)->znajdzMozliweRuchy(szachownica);
            for (vector<string>::iterator ruch = tmp.begin(); ruch != tmp.end(); ++ruch) {
                polozenie pole = rozkodujPozycje(*ruch);
                if (pole.k == pozycjaKrola.k && pole.w == pozycjaKrola.w) {
                    czyKrolJestAtakowany = true;
                }
            }
        }
    }
    return czyKrolJestAtakowany;
}

vector<string> mozliweRuchyBezSzachaZOdkrycia(polozenie pozycjaBierki, vector<string> mozliwe, bool wybranoKrola, int glupia_szachownica[][9], polozenie potencjalnyKrol, vector<Bierka*> bierki_przeciwnika) {
    vector<string> mozliwe_bez_szacha = mozliwe;
    for (vector<string>::iterator it = mozliwe_bez_szacha.begin(); it != mozliwe_bez_szacha.end(); ++it) {
        int tmp_glupia[9][9];
        for (int i = 1; i < 9; i++)
            for (int j = 1; j < 9; j++) tmp_glupia[i][j] = glupia_szachownica[i][j];
        polozenie nowa_pozycja = rozkodujPozycje(*it);
        tmp_glupia[pozycjaBierki.k][pozycjaBierki.w] = 0;
        tmp_glupia[nowa_pozycja.k][nowa_pozycja.w] = 1;
        polozenie pozycjaKrola;
        if (wybranoKrola) {
            pozycjaKrola.k = nowa_pozycja.k;
            pozycjaKrola.w = nowa_pozycja.w;
        } else {
            pozycjaKrola.k = potencjalnyKrol.k;
            pozycjaKrola.w = potencjalnyKrol.w;
        }

        bool czy = sprawdzCzySzachNaszegoKrolaGdyTakiRuch(tmp_glupia, nowa_pozycja, pozycjaKrola, bierki_przeciwnika);
        if (czy) {
            mozliwe_bez_szacha.erase(it--);
        }
    }
    return mozliwe_bez_szacha;
}

bool sprawdzCzySzachMat(vector<Bierka*> bierki_twoje, vector<Bierka*> bierki_przeciwnika, int tab[][9],polozenie krol) {
    for (vector<Bierka*>::iterator bierka = bierki_twoje.begin(); bierka != bierki_twoje.end(); ++bierka) {
        bool wybranoKrola = false;
        if ((*bierka)->getNazwaBierki().compare("krol") == 0) wybranoKrola = true;
        vector<string> mozliwe = (*bierka)->znajdzMozliweRuchy(tab);
        vector<string> mozliwe_bez_szacha = mozliweRuchyBezSzachaZOdkrycia((*bierka)->pozycja,mozliwe,wybranoKrola,tab,krol,bierki_przeciwnika);
        cout <<(*bierka)->pozycja.k<<(*bierka)->pozycja.w<<" : "<< mozliwe_bez_szacha.size() << endl;
        if (mozliwe_bez_szacha.size() > 0) return false;
    }
    return true;
}

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
    int read_result;
    printf("Powstałem!\n");
    char * login_1 = (char *) malloc(sizeof(char)*LOGIN_SIZE);
    char * login_2 = (char *) malloc(sizeof(char)*LOGIN_SIZE);
    write(th_data->first_socket_descriptor,"login",BUFF_SIZE);
    read_result = read(th_data->first_socket_descriptor, login_1, LOGIN_SIZE);
    if (read_result>0) {
        printf("Login gracza pierwszego: %s\n",login_1);
    } else printf("Blad");

    write(th_data->second_socket_descriptor,"login",BUFF_SIZE);
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
    vector<Bierka*> biale_bierki;
    vector<Bierka*> czarne_bierki;
    polozenie krol_bialy = rozkodujPozycje("E1");
    polozenie krol_czarny = rozkodujPozycje("E8");

    for (int i = 1; i < 9; i++) {
        int tab[2] = {i, 2};
        biale_bierki.push_back(new Pion(false, rozkodujPozycje( zakodujPozycje(tab[0],tab[1]) )));
    }

    for (int i = 1; i < 9; i++) {
        int tab[2] = {i, 7};
        czarne_bierki.push_back(new Pion(true, rozkodujPozycje( zakodujPozycje(tab[0],tab[1]) )));
    }

    biale_bierki.push_back(new Wieza(false, rozkodujPozycje("A1")));
    biale_bierki.push_back(new Skoczek(false, rozkodujPozycje("B1")));
    biale_bierki.push_back(new Goniec(false, rozkodujPozycje("C1")));
    biale_bierki.push_back(new Hetman(false, rozkodujPozycje("D1")));
    biale_bierki.push_back(new Krol(false, rozkodujPozycje("E1")));
    biale_bierki.push_back(new Goniec(false, rozkodujPozycje("F1")));
    biale_bierki.push_back(new Skoczek(false, rozkodujPozycje("G1")));
    biale_bierki.push_back(new Wieza(false, rozkodujPozycje("H1")));

    czarne_bierki.push_back(new Wieza(true, rozkodujPozycje("A8")));
    czarne_bierki.push_back(new Skoczek(true, rozkodujPozycje("B8")));
    czarne_bierki.push_back(new Goniec(true, rozkodujPozycje("C8")));
    czarne_bierki.push_back(new Hetman(true, rozkodujPozycje("D8")));
    czarne_bierki.push_back(new Krol(true, rozkodujPozycje("E8")));
    czarne_bierki.push_back(new Goniec(true, rozkodujPozycje("F8")));
    czarne_bierki.push_back(new Skoczek(true, rozkodujPozycje("G8")));
    czarne_bierki.push_back(new Wieza(true, rozkodujPozycje("H8")));

    write(th_data->first_socket_descriptor,"white",BUFF_SIZE);
    write(th_data->second_socket_descriptor,"black",BUFF_SIZE);

    bool aktualny_gracz_to_biale = true;
    bool gra_trwa = true;
    while (gra_trwa) {
        int player;
        int enemy;
        if (aktualny_gracz_to_biale) {
            player = th_data->first_socket_descriptor;
            enemy = th_data->second_socket_descriptor;
        } else {
            enemy = th_data->first_socket_descriptor;
            player = th_data->second_socket_descriptor;
        }
        int success;
        char * buffor = (char *) malloc(sizeof(char)*BUFF_SIZE);
        success = read(player, buffor, BUFF_SIZE);
        bool poprawny_zapis = sprawdzCzyPoprawne(buffor);
        if (!poprawny_zapis) {
            write(player,"E:ASC",BUFF_SIZE);
        } else {
            //sprawdz czy tu jest figura
            string start_s = string() + buffor[0] + buffor[1];
            string koniec_s = string() + buffor[3] + buffor[4];
            polozenie start = rozkodujPozycje(start_s);
            polozenie koniec = rozkodujPozycje(koniec_s);
            Bierka* wybrana_bierka;
            bool wybrana = false;
            if (aktualny_gracz_to_biale) {
                for (vector<Bierka*>::iterator it = biale_bierki.begin(); it!=biale_bierki.end(); ++it) {
                    //cout << (*it)->pozycja.k << " "  << (*it)->pozycja.w << endl;
                    if ( (*it)->pozycja.k == start.k && (*it)->pozycja.w == start.w ) {
                        wybrana = true;
                        wybrana_bierka = *it;
                        break;
                    }
                }
                if (!wybrana) {
                    write(player,"E:STA",BUFF_SIZE);
                } else {
                    bool wybranoKrola = false;
                    if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) wybranoKrola = true;
                    vector<string> mozliwe_wedlug_bierki = wybrana_bierka->znajdzMozliweRuchy(szachownica);
                    vector<string> vectorek = mozliweRuchyBezSzachaZOdkrycia(wybrana_bierka->pozycja,mozliwe_wedlug_bierki,wybranoKrola,szachownica,krol_bialy,czarne_bierki);
                    bool poprawny_ruch = false;
                    for (vector<string>::iterator wyb = vectorek.begin(); wyb!=vectorek.end(); ++wyb) {
                        if ( koniec_s.compare(*wyb) == 0 ) {
                            poprawny_ruch = true;
                            break;
                        }
                        //cout << *wyb << " " << endl;
                    }
                    if (!poprawny_ruch) {
                        write(player,"E:KON",BUFF_SIZE);
                    } else {
                        //cout << "RUCH POPRAWNY"<<endl;
                        for (vector<Bierka*>::iterator wroga = czarne_bierki.begin(); wroga!=czarne_bierki.end(); ++wroga) {
                            if (koniec_s.compare(zakodujPozycje((*wroga)->pozycja.k, (*wroga)->pozycja.w)) == 0 ) {
                                czarne_bierki.erase(wroga);
                                break;
                            }
                        }
                        wybrana_bierka->wykonanoRuch(koniec);
                        if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) krol_bialy = wybrana_bierka->pozycja;
                        aktualny_gracz_to_biale = false;
                        szachownica[start.k][start.w] = 0;
                        szachownica[koniec.k][koniec.w] = 1;
                        for (int i = 1; i < 9; i++)
                            for (int j = 1; j < 9; j++)
                                szachownica[i][j] = (-1) * szachownica[i][j];
                        write(enemy,buffor,BUFF_SIZE);
                    }
                }
            } else {
                for (vector<Bierka*>::iterator it = czarne_bierki.begin(); it!=czarne_bierki.end(); ++it) {
                    //cout << (*it)->pozycja.k << " "  << (*it)->pozycja.w << endl;
                    if ( (*it)->pozycja.k == start.k && (*it)->pozycja.w == start.w ) {
                        wybrana = true;
                        wybrana_bierka = *it;
                        break;
                    }
                }
                if (!wybrana) {
                    write(player,"E:STA",BUFF_SIZE);
                } else {
                    bool wybranoKrola = false;
                    if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) wybranoKrola = true;
                    vector<string> mozliwe_wedlug_bierki = wybrana_bierka->znajdzMozliweRuchy(szachownica);
                    vector<string> vectorek = mozliweRuchyBezSzachaZOdkrycia(wybrana_bierka->pozycja,mozliwe_wedlug_bierki,wybranoKrola,szachownica,krol_czarny,biale_bierki);
                    bool poprawny_ruch = false;
                    for (vector<string>::iterator wyb = vectorek.begin(); wyb!=vectorek.end(); ++wyb) {
                        if ( koniec_s.compare(*wyb) == 0 ) {
                            poprawny_ruch = true;
                            break;
                        }
                        //cout << *wyb << " " << endl;
                    }
                    if (!poprawny_ruch) {
                        write(player,"E:KON",BUFF_SIZE);
                    } else {
                        //cout << "RUCH POPRAWNY"<<endl;
                        for (vector<Bierka*>::iterator wroga = biale_bierki.begin(); wroga!=biale_bierki.end(); ++wroga) {
                            if (koniec_s.compare(zakodujPozycje((*wroga)->pozycja.k, (*wroga)->pozycja.w)) == 0 ) {
                                biale_bierki.erase(wroga);
                                break;
                            }
                        }
                        wybrana_bierka->wykonanoRuch(koniec);
                        if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) krol_bialy = wybrana_bierka->pozycja;
                        aktualny_gracz_to_biale = true;
                        szachownica[start.k][start.w] = 0;
                        szachownica[koniec.k][koniec.w] = 1;
                        for (int i = 1; i < 9; i++)
                            for (int j = 1; j < 9; j++)
                                szachownica[i][j] = (-1) * szachownica[i][j];
                        bool szach_mat = sprawdzCzySzachMat(biale_bierki,czarne_bierki,szachownica,krol_bialy);
                        if (szach_mat) {
                            cout << "SZACH MAT KONIEEEEEC"<<endl;
                        }
                        write(enemy,buffor,BUFF_SIZE);
                    }
                }
            }
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
