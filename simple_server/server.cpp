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
#include <queue>

#define SERVER_PORT 1234
#define QUEUE_SIZE 5
#define BUFF_SIZE 5
#define LOGIN_SIZE 8
#define DESCRIPTION_ARRAY_SIZE 100
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t at_least_two_players = PTHREAD_COND_INITIALIZER;

using namespace std;

struct polozenie {
    int k;
    int w;
};

polozenie rozkodujPozycje(string pozycja) {
    int tab[2];
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
    tab[1] = pozycja.at(1) - '0';
    polozenie pol;
    pol.k = tab[0];
    pol.w = tab[1];
    return pol;
}

string zakodujPozycje(int a, int b) {
    string pozycja;
    switch (a) {
        case 1:
            pozycja.append("A");
            break;
        case 2:
            pozycja.append("B");
            break;
        case 3:
            pozycja.append("C");
            break;
        case 4:
            pozycja.append("D");
            break;
        case 5:
            pozycja.append("E");
            break;
        case 6:
            pozycja.append("F");
            break;
        case 7:
            pozycja.append("G");
            break;
        case 8:
            pozycja.append("H");
            break;
    }
    pozycja.append(to_string(b));
    return pozycja;
}

class Bierka {
public:
    bool kolor_czarny;
    polozenie pozycja;
    int ilosc_wykon_ruchow = 0;

    void wykonanoRuch(polozenie nowa) {
        ilosc_wykon_ruchow += 1;
        pozycja = nowa;
    };

    virtual vector<string> znajdzMozliweRuchy(int glupia_szachownica[][9]) = 0;

    virtual string getNazwaBierki() = 0;
};

class Pion : public Bierka {
public:
    Pion(bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };

    string getNazwaBierki() {
        return "pion";
    };

    vector<string> znajdzMozliweRuchy(int tab[][9]) {
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
    Goniec(bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };

    string getNazwaBierki() {
        return "goniec";
    };

    vector<string> znajdzMozliweRuchy(int tab[][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0] - 1;
        int j = pol[1] + 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i--;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] + 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i++;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] - 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i++;
            j--;
        }

        i = pol[0] - 1;
        j = pol[1] - 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
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
    Hetman(bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };

    string getNazwaBierki() {
        return "hetman";
    };

    vector<string> znajdzMozliweRuchy(int tab[][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0] - 1;
        int j = pol[1] + 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i--;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] + 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i++;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] - 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i++;
            j--;
        }

        i = pol[0] - 1;
        j = pol[1] - 1;
        while (i > 0 && i < 9 && j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i--;
            j--;
        }

        i = pol[0] + 1;
        j = pol[1];
        while (i > 0 && i < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i++;
        }

        i = pol[0] - 1;
        j = pol[1];
        while (i > 0 && i < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i--;
        }

        i = pol[0];
        j = pol[1] + 1;
        while (j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            j++;
        }

        i = pol[0];
        j = pol[1] - 1;
        while (j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            j--;
        }

        return mozliwe_ruchy;
    };
};

class Krol : public Bierka {
public:
    Krol(bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };

    string getNazwaBierki() {
        return "krol";
    };

    vector<string> znajdzMozliweRuchy(int tab[][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0];
        int j = pol[1];
        if (i - 1 > 0 && j - 1 > 0)
            if (tab[i - 1][j - 1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i - 1, j - 1));
        if (i - 1 > 0)
            if (tab[i - 1][j] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i - 1, j));
        if (i - 1 > 0 && j + 1 < 9)
            if (tab[i - 1][j + 1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i - 1, j + 1));
        if (j + 1 < 9)
            if (tab[i][j + 1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i, j + 1));
        if (i + 1 < 9 && j + 1 < 9)
            if (tab[i + 1][j + 1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i + 1, j + 1));
        if (i + 1 < 9)
            if (tab[i + 1][j] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i + 1, j));
        if (i + 1 < 9 && j - 1 > 0)
            if (tab[i + 1][j - 1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i + 1, j - 1));
        if (j - 1 > 0)
            if (tab[i][j - 1] != 1) mozliwe_ruchy.push_back(zakodujPozycje(i, j - 1));
        return mozliwe_ruchy;
    };
};

class Skoczek : public Bierka {
public:
    Skoczek(bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };

    string getNazwaBierki() {
        return "skoczek";
    };

    vector<string> znajdzMozliweRuchy(int tab[][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        for (int i = 1; i < 9; i++)
            for (int j = 1; j < 9; j++) {
                if (tab[i][j] != 1) {
                    if (i - 2 == pol[0] && j + 1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i - 1 == pol[0] && j + 2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i + 1 == pol[0] && j + 2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i + 2 == pol[0] && j + 1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i + 2 == pol[0] && j - 1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i + 1 == pol[0] && j - 2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i - 1 == pol[0] && j - 2 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                    if (i - 2 == pol[0] && j - 1 == pol[1]) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                }
            }
        return mozliwe_ruchy;
    };
};

class Wieza : public Bierka {
public:
    Wieza(bool czarny, polozenie start) {
        kolor_czarny = czarny;
        pozycja = start;
    };

    string getNazwaBierki() {
        return "wieza";
    };

    vector<string> znajdzMozliweRuchy(int tab[][9]) {
        vector<string> mozliwe_ruchy;
        int pol[2];
        pol[0] = pozycja.k;
        pol[1] = pozycja.w;
        int i = pol[0] + 1;
        int j = pol[1];
        while (i > 0 && i < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i++;
        }

        i = pol[0] - 1;
        j = pol[1];
        while (i > 0 && i < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            i--;
        }

        i = pol[0];
        j = pol[1] + 1;
        while (j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            j++;
        }

        i = pol[0];
        j = pol[1] - 1;
        while (j > 0 && j < 9) {
            if (tab[i][j] == 0) mozliwe_ruchy.push_back(zakodujPozycje(i, j));
            else if (tab[i][j] == -1) {
                mozliwe_ruchy.push_back(zakodujPozycje(i, j));
                break;
            } else break;
            j--;
        }

        return mozliwe_ruchy;
    };
};

bool sprawdzCzyPoprawne(char *bufor) {
    bool odp = false;
    int z1 = (int) bufor[0];
    int z2 = (int) bufor[1];
    int z3 = (int) bufor[2];
    int z4 = (int) bufor[3];
    int z5 = (int) bufor[4];
    if ((64 < z1) && (z1 < 73) &&
        (64 < z4) && (z4 < 73) &&
        (48 < z2) && (z2 < 57) &&
        (48 < z5) && (z5 < 57) &&
        (z3 == 58)) {
        odp = true;
    }
    return odp;
}

bool sprawdzCzySzachNaszegoKrolaGdyTakiRuch(int tab[][9], polozenie potancjalneBicie, polozenie pozycjaKrola, vector<Bierka *> bierki_przeciwnika) {
    int szachownica[9][9];
    for (int i = 1; i < 9; i++)
        for (int j = 1; j < 9; j++)
            szachownica[i][j] = (-1) * tab[i][j];
    bool czyKrolJestAtakowany = false;
    for (vector<Bierka *>::iterator bierka = bierki_przeciwnika.begin(); bierka != bierki_przeciwnika.end(); ++bierka) {
        if ((*bierka)->pozycja.k != potancjalneBicie.k || (*bierka)->pozycja.w != potancjalneBicie.w) {
            vector<string> tmp = (*bierka)->znajdzMozliweRuchy(szachownica);
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

vector<string> mozliweRuchyBezSzachaZOdkrycia(polozenie pozycjaBierki, vector<string> mozliwe, bool wybranoKrola,int glupia_szachownica[][9], polozenie potencjalnyKrol,vector<Bierka *> bierki_przeciwnika) {
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

bool sprawdzCzySzachMat(vector<Bierka *> bierki_twoje, vector<Bierka *> bierki_przeciwnika, int tab[][9], polozenie krol) {
    for (vector<Bierka *>::iterator bierka = bierki_twoje.begin(); bierka != bierki_twoje.end(); ++bierka) {
        bool wybranoKrola = false;
        if ((*bierka)->getNazwaBierki().compare("krol") == 0) wybranoKrola = true;
        vector<string> mozliwe = (*bierka)->znajdzMozliweRuchy(tab);
        vector<string> mozliwe_bez_szacha = mozliweRuchyBezSzachaZOdkrycia((*bierka)->pozycja, mozliwe, wybranoKrola,
                                                                           tab, krol, bierki_przeciwnika);
        if (mozliwe_bez_szacha.size() > 0) return false;
    }
    return true;
}

//struktura zawierająca dane, które zostaną przekazane do wątku
struct data_thread_join {
    queue<int> &players_queue;
    data_thread_join(queue<int> &players) : players_queue(players) {}
};

struct data_thread_game {
    int first_socket_descriptor;
    int second_socket_descriptor;
};

//funkcja opisującą zachowanie wątku - musi przyjmować argument typu (void *) i zwracać (void *)
void *ThreadBehavior(void *t_data) {
    pthread_detach(pthread_self());
    struct data_thread_game *th_data = (struct data_thread_game *) t_data;
    char *login_1 = (char *) malloc(sizeof(char) * LOGIN_SIZE);
    char *login_2 = (char *) malloc(sizeof(char) * LOGIN_SIZE);
    char *login_request = (char *) malloc(sizeof(char) * BUFF_SIZE);
    strcpy(login_request,"LOGIN");

    int bytes_read = 0;
    int bytes_write = 0;
    int result_read;
    int result_write;

    //Poproś graczy kolejno o ich loginy
    bytes_write = 0;
    while (bytes_write < BUFF_SIZE) {
        result_write = write(th_data->first_socket_descriptor, login_request + bytes_write, BUFF_SIZE - bytes_write);
        if (result_write < 1) break;
        bytes_write += result_write;
    }
    if (result_write < 1) {
        //nie weryfikujemy wyników write, ponieważ i tak zamykamy grę
        write(th_data->second_socket_descriptor,"_INFO",BUFF_SIZE);
        write(th_data->second_socket_descriptor,"_CONN",BUFF_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(login_request);
        free(th_data);
        free(login_1);
        free(login_2);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    bytes_read = 0;
    while (bytes_read < BUFF_SIZE) {
        result_read = read(th_data->first_socket_descriptor, login_1 + bytes_read, LOGIN_SIZE - bytes_read);
        if (result_read < 1) break;
        bytes_read += result_read;
    }
    if (result_read < 1) {
        write(th_data->second_socket_descriptor,"_INFO",BUFF_SIZE);
        write(th_data->second_socket_descriptor,"_CONN",BUFF_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(login_request);
        free(th_data);
        free(login_1);
        free(login_2);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    printf("[%d-%d] White login: %s\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor,login_1);

    bytes_write = 0;
    while (bytes_write < BUFF_SIZE) {
        result_write = write(th_data->second_socket_descriptor, login_request + bytes_write, BUFF_SIZE - bytes_write);
        if (result_write < 1) break;
        bytes_write += result_write;
    }
    if (result_write < 1) {
        write(th_data->first_socket_descriptor,"_INFO",BUFF_SIZE);
        write(th_data->first_socket_descriptor,"_CONN",BUFF_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(login_request);
        free(th_data);
        free(login_1);
        free(login_2);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    bytes_read = 0;
    while (bytes_read < BUFF_SIZE) {
        result_read = read(th_data->second_socket_descriptor, login_2 + bytes_read, LOGIN_SIZE - bytes_read);
        if (result_read < 1) break;
        bytes_read += result_read;
    }
    if (result_read < 1) {
        write(th_data->first_socket_descriptor,"_INFO",BUFF_SIZE);
        write(th_data->first_socket_descriptor,"_CONN",BUFF_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(login_request);
        free(th_data);
        free(login_1);
        free(login_2);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    printf("[%d-%d] Black login: %s\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor,login_2);
    free(login_request);

    //Prześlij każdemu z graczy login jego przeciwnika
    bytes_write = 0;
    while (bytes_write < LOGIN_SIZE) {
        result_write = write(th_data->first_socket_descriptor, login_2 + bytes_write, LOGIN_SIZE - bytes_write);
        if (result_write < 1) break;
        bytes_write += result_write;
    }
    if (result_write < 1) {
        write(th_data->second_socket_descriptor,"_INFO_LOG",LOGIN_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(th_data);
        free(login_1);
        free(login_2);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    bytes_write = 0;
    while (bytes_write < LOGIN_SIZE) {
        result_write = write(th_data->second_socket_descriptor, login_1 + bytes_write, LOGIN_SIZE - bytes_write);
        if (result_write < 1) break;
        bytes_write += result_write;
    }
    if (result_write < 1) {
        write(th_data->first_socket_descriptor,"_INFO_LOG",LOGIN_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(th_data);
        free(login_1);
        free(login_2);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    free(login_1);
    free(login_2);
    // Utwórz początkowy stan gry
    int szachownica[9][9] = {
            {0, 0, 0, 0, 0, 0, 0, 0,  0},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
            {0, 1, 1, 0, 0, 0, 0, -1, -1},
    };
    vector<Bierka *> biale_bierki;
    vector<Bierka *> czarne_bierki;
    polozenie krol_bialy = rozkodujPozycje("E1");
    polozenie krol_czarny = rozkodujPozycje("E8");

    for (int i = 1; i < 9; i++) {
        int tab[2] = {i, 2};
        biale_bierki.push_back(new Pion(false, rozkodujPozycje(zakodujPozycje(tab[0], tab[1]))));
    }

    for (int i = 1; i < 9; i++) {
        int tab[2] = {i, 7};
        czarne_bierki.push_back(new Pion(true, rozkodujPozycje(zakodujPozycje(tab[0], tab[1]))));
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

    //Prześlij informację graczom o ich kolorze
    //Tożsame z rozpoczęciem gry - po tym będziemy oczekiwać na ruch gracza białego
    char *c_white = (char *) malloc(sizeof(char) * BUFF_SIZE);
    strcpy(c_white,"white");
    char *c_black = (char *) malloc(sizeof(char) * BUFF_SIZE);
    strcpy(c_black,"black");

    bytes_write = 0;
    while (bytes_write < BUFF_SIZE) {
        result_write = write(th_data->first_socket_descriptor, c_white + bytes_write, BUFF_SIZE - bytes_write);
        if (result_write < 1) break;
        bytes_write += result_write;
    }
    if (result_write < 1) {
        write(th_data->second_socket_descriptor,"_INFO",BUFF_SIZE);
        write(th_data->second_socket_descriptor,"_CONN",BUFF_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(c_white);
        free(c_black);
        free(th_data);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);
    }
    bytes_write = 0;
    while (bytes_write < BUFF_SIZE) {
        result_write = write(th_data->second_socket_descriptor, c_black + bytes_write, BUFF_SIZE - bytes_write);
        if (result_write < 1) break;
        bytes_write += result_write;
    }
    if (result_write < 1) {
        write(th_data->first_socket_descriptor,"_INFO",BUFF_SIZE);
        write(th_data->first_socket_descriptor,"_CONN",BUFF_SIZE);
        printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
        free(c_white);
        free(c_black);
        free(th_data);
        close(th_data->first_socket_descriptor);
        close(th_data->second_socket_descriptor);
        pthread_exit(NULL);;
    }
    free(c_white);
    free(c_black);

    //Rozpocznij grę

    bool aktualny_gracz_to_biale = true;
    bool gra_trwa = true;
    bool first_is_dead = false;
    bool second_is_dead = false;
    char *buffor = (char *) malloc(sizeof(char) * BUFF_SIZE);

    while (gra_trwa) {
        //przypisz deskryptory do gracza i jego przeciwnika
        //first_socket_descriptor zawsze należy do białego gracza
        int player;
        int enemy;
        if (aktualny_gracz_to_biale) {
            player = th_data->first_socket_descriptor;
            enemy = th_data->second_socket_descriptor;
        } else {
            enemy = th_data->first_socket_descriptor;
            player = th_data->second_socket_descriptor;
        }
        // czytaj ruch gracza
        bytes_read = 0;
        while (bytes_read < BUFF_SIZE) {
            result_read = read(player, buffor + bytes_read, BUFF_SIZE - bytes_read);
            if (result_read < 1) break;
            bytes_read += result_read;
        }
        // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
        // z danym graczem więc zamykamy pokój
        if (result_read < 1) {
            gra_trwa = false;
            if (aktualny_gracz_to_biale)
                first_is_dead = true;
            else second_is_dead = true;
            break;
        }
        //weryfikujemy, czy nadesłana wiadomość to poprawnie zapisany ruch
        //czy zawiera dwa pola od A1 do H8 przedzielone znakiem ':' np. A2:A4
        //na typ etapie nie weryfikujemy jeszcze czy jest to poprawny ruch
        //w sensie logiki i stanu gry
        bool poprawny_zapis = sprawdzCzyPoprawne(buffor);
        if (!poprawny_zapis) {
            //gdy zapis niepoprawny zwracamy informację do nadawcy o błędzie (ASC od ASCII)
            //nie zrywamy połączenia, ponownie będziemy czekać na podanie ruchu_
            char * error = (char *) malloc(sizeof(char) * BUFF_SIZE);
            strcpy(error,"E:ASC");
            bytes_write = 0;
            while (bytes_write < BUFF_SIZE) {
                result_write = write(player, error + bytes_write, BUFF_SIZE - bytes_write);
                if (result_write < 1) break;
                bytes_write += result_write;
            }
            free(error);
            // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
            // z danym graczem więc zamykamy pokój
            if (result_write < 1) {
                gra_trwa = false;
                if (aktualny_gracz_to_biale)
                    first_is_dead = true;
                else second_is_dead = true;
                break;
            }
        } else {
            string start_s = string() + buffor[0] + buffor[1];
            string koniec_s = string() + buffor[3] + buffor[4];
            polozenie start = rozkodujPozycje(start_s);
            polozenie koniec = rozkodujPozycje(koniec_s);
            Bierka *wybrana_bierka;
            bool wybrana = false;

            //jeżeli ruch wykonuje biały
            if (aktualny_gracz_to_biale) {
                // znajdź bierkę gracza znajdującą się na pierwszym polu wysłanym przez gracza
                for (vector<Bierka *>::iterator it = biale_bierki.begin(); it != biale_bierki.end(); ++it) {
                    if ((*it)->pozycja.k == start.k && (*it)->pozycja.w == start.w) {
                        wybrana = true;
                        wybrana_bierka = *it;
                        break;
                    }
                }
                // musi taka istnieć, jeżeli nie, to ruch jest niepoprawny, STA od START, pola startowego
                if (!wybrana) {
                    char * error = (char *) malloc(sizeof(char) * BUFF_SIZE);
                    strcpy(error,"E:STA");
                    bytes_write = 0;
                    while (bytes_write < BUFF_SIZE) {
                        result_write = write(player, error + bytes_write, BUFF_SIZE - bytes_write);
                        if (result_write < 1) break;
                        bytes_write += result_write;
                    }
                    free(error);
                    // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
                    // z danym graczem więc zamykamy pokój
                    if (result_write < 1) {
                        gra_trwa = false;
                        first_is_dead = true;
                        break;
                    }
                } else {
                    // poproś bierkę aby stworzyła listę swoich możliwych ruchów na podstawie macierzy pokrycia szachownicy
                    vector<string> mozliwe_wedlug_bierki = wybrana_bierka->znajdzMozliweRuchy(szachownica);
                    // zmniejsz uzyskany wektor możliwych o ruchy, które spowodują szach na naszym królu (tzw. szach z odkrycia)
                    // jeżeli wybrana bierka to właśnie nasz król, zmienia to zachowanie funkcji
                    bool wybranoKrola = false;
                    if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) wybranoKrola = true;
                    vector<string> vectorek = mozliweRuchyBezSzachaZOdkrycia(wybrana_bierka->pozycja,
                                                                             mozliwe_wedlug_bierki, wybranoKrola,
                                                                             szachownica, krol_bialy, czarne_bierki);
                    // sprawdz czy ruch wysłany przez gracza znajduje się w wektorze ruchów możliwych
                    bool poprawny_ruch = false;
                    for (vector<string>::iterator wyb = vectorek.begin(); wyb != vectorek.end(); ++wyb) {
                        if (koniec_s.compare(*wyb) == 0) {
                            poprawny_ruch = true;
                            break;
                        }
                    }
                    //jeżeli nie znajduje się, wybrano nieprawidłowy ruch; KON od KONIEC, pozycja końcowa
                    if (!poprawny_ruch) {
                        char * error = (char *) malloc(sizeof(char) * BUFF_SIZE);
                        strcpy(error,"E:KON");
                        bytes_write = 0;
                        while (bytes_write < BUFF_SIZE) {
                            result_write = write(player, error + bytes_write, BUFF_SIZE - bytes_write);
                            if (result_write < 1) break;
                            bytes_write += result_write;
                        }
                        free(error);
                        // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
                        // z danym graczem więc zamykamy pokój
                        if (result_write < 1) {
                            gra_trwa = false;
                            first_is_dead = true;
                            break;
                        }
                    } else {
                        //znajdz i usun ewentualną bierkę przeciwnika znajdującą się na polu końcowym
                        for (vector<Bierka *>::iterator wroga = czarne_bierki.begin();
                             wroga != czarne_bierki.end(); ++wroga) {
                            if (koniec_s.compare(zakodujPozycje((*wroga)->pozycja.k, (*wroga)->pozycja.w)) == 0) {
                                czarne_bierki.erase(wroga);
                                break;
                            }
                        }
                        //zapisz aktualną pozycję bierki
                        wybrana_bierka->wykonanoRuch(koniec);
                        //jezeli to krol sie przesunął zapamietaj jego pozycję w zmiennej
                        // (jest ona potrzebna gdy badamy szach z odkrycia)
                        if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) krol_bialy = wybrana_bierka->pozycja;
                        //przygotuj wszystko przed ruchem następnego gracza
                        //zmien aktywny kolor, odwróc macierz pokrycia szachownicy
                        aktualny_gracz_to_biale = false;
                        szachownica[start.k][start.w] = 0;
                        szachownica[koniec.k][koniec.w] = 1;
                        for (int i = 1; i < 9; i++)
                            for (int j = 1; j < 9; j++)
                                szachownica[i][j] = (-1) * szachownica[i][j];
                        //sprawdz, czy Szach Mat - tzn. czy przeciwnik ma jakikolwiek możliwy ruch
                        bool szach_mat = sprawdzCzySzachMat(czarne_bierki, biale_bierki, szachownica, krol_czarny);
                        if (szach_mat) {
                            //jeżeli koniec gry, poinformuj o tym obu graczy, drugiemu graczowi wyślij informację o ruchu i zakończ grę
                            printf("[%d-%d] White wins\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
                            // jeżeli wynik write < 1 to nic z tym nie robimy: gra i tak się skończyła
                            char * info = (char *) malloc(sizeof(char) * BUFF_SIZE);
                            strcpy(info,"_INFO");
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(player, info + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, info + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            free(info);

                            char * info2 = (char *) malloc(sizeof(char) * BUFF_SIZE);
                            strcpy(info2,"WIN:W");
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(player, info2 + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, info2 + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            free(info2);

                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, buffor + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            gra_trwa = false;
                        } else {
                            //jeżeli brak mata, przekaż ruch drugiemuu graczowi
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, buffor + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
                            // z danym graczem więc zamykamy pokój
                            if (result_write < 1) {
                                gra_trwa = false;
                                second_is_dead = true;
                                break;
                            }
                        }
                    }
                }
            //jeżeli ruch wykonuje czarny
            } else {
                // znajdź bierkę gracza znajdującą się na pierwszym polu wysłanym przez gracza
                for (vector<Bierka *>::iterator it = czarne_bierki.begin(); it != czarne_bierki.end(); ++it) {
                    if ((*it)->pozycja.k == start.k && (*it)->pozycja.w == start.w) {
                        wybrana = true;
                        wybrana_bierka = *it;
                        break;
                    }
                }
                // musi taka istnieć, jeżeli nie, to ruch jest niepoprawny, STA od START, pola startowego
                if (!wybrana) {
                    char * error = (char *) malloc(sizeof(char) * BUFF_SIZE);
                    strcpy(error,"E:STA");
                    bytes_write = 0;
                    while (bytes_write < BUFF_SIZE) {
                        result_write = write(player, error + bytes_write, BUFF_SIZE - bytes_write);
                        if (result_write < 1) break;
                        bytes_write += result_write;
                    }
                    free(error);
                    // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
                    // z danym graczem więc zamykamy pokój
                    if (result_write < 1) {
                        gra_trwa = false;
                        second_is_dead = true;
                        break;
                    }
                } else {
                    // poproś bierkę aby stworzyła listę swoich możliwych ruchów na podstawie macierzy pokrycia szachownicy
                    vector<string> mozliwe_wedlug_bierki = wybrana_bierka->znajdzMozliweRuchy(szachownica);
                    // zmniejsz uzyskany wektor możliwych o ruchy, które spowodują szach na naszym królu (tzw. szach z odkrycia)
                    // jeżeli wybrana bierka to właśnie nasz król, zmienia to zachowanie funkcji
                    bool wybranoKrola = false;
                    if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) wybranoKrola = true;
                    vector<string> vectorek = mozliweRuchyBezSzachaZOdkrycia(wybrana_bierka->pozycja,
                                                                             mozliwe_wedlug_bierki, wybranoKrola,
                                                                             szachownica, krol_czarny, biale_bierki);
                    // sprawdz czy ruch wysłany przez gracza znajduje się w wektorze ruchów możliwych
                    bool poprawny_ruch = false;
                    for (vector<string>::iterator wyb = vectorek.begin(); wyb != vectorek.end(); ++wyb) {
                        if (koniec_s.compare(*wyb) == 0) {
                            poprawny_ruch = true;
                            break;
                        }
                    }
                    //jeżeli nie znajduje się, wybrano nieprawidłowy ruch; KON od KONIEC, pozycja końcowa
                    if (!poprawny_ruch) {
                        char * error = (char *) malloc(sizeof(char) * BUFF_SIZE);
                        strcpy(error,"E:KON");
                        bytes_write = 0;
                        while (bytes_write < BUFF_SIZE) {
                            result_write = write(player, error + bytes_write, BUFF_SIZE - bytes_write);
                            if (result_write < 1) break;
                            bytes_write += result_write;
                        }
                        free(error);
                        // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
                        // z danym graczem więc zamykamy pokój
                        if (result_write < 1) {
                            gra_trwa = false;
                            second_is_dead = true;
                            break;
                        }
                    } else {
                        //znajdz i usun ewentualną bierkę przeciwnika znajdującą się na polu końcowym
                        for (vector<Bierka *>::iterator wroga = biale_bierki.begin();
                             wroga != biale_bierki.end(); ++wroga) {
                            if (koniec_s.compare(zakodujPozycje((*wroga)->pozycja.k, (*wroga)->pozycja.w)) == 0) {
                                biale_bierki.erase(wroga);
                                break;
                            }
                        }
                        //zapisz aktualną pozycję bierki
                        wybrana_bierka->wykonanoRuch(koniec);
                        //jezeli to krol sie przesunął zapamietaj jego pozycję w zmiennej
                        // (jest ona potrzebna gdy badamy szach z odkrycia)
                        if (wybrana_bierka->getNazwaBierki().compare("krol") == 0) krol_czarny = wybrana_bierka->pozycja;
                        //przygotuj wszystko przed ruchem następnego gracza
                        //zmien aktywny kolor, odwróc macierz pokrycia szachownicy
                        aktualny_gracz_to_biale = true;
                        szachownica[start.k][start.w] = 0;
                        szachownica[koniec.k][koniec.w] = 1;
                        for (int i = 1; i < 9; i++)
                            for (int j = 1; j < 9; j++)
                                szachownica[i][j] = (-1) * szachownica[i][j];
                        //sprawdz, czy Szach Mat - tzn. czy przeciwnik ma jakikolwiek możliwy ruch
                        bool szach_mat = sprawdzCzySzachMat(biale_bierki, czarne_bierki, szachownica, krol_bialy);
                        if (szach_mat) {
                            //jeżeli koniec gry, poinformuj o tym obu graczy, drugiemu graczowi wyślij informację o ruchu i zakończ grę
                            printf("[%d-%d] Black wins\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
                            // jeżeli wynik write < 1 to nic z tym nie robimy: gra i tak się skończyła
                            char * info = (char *) malloc(sizeof(char) * BUFF_SIZE);
                            strcpy(info,"_INFO");
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(player, info + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, info + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            free(info);

                            char * info2 = (char *) malloc(sizeof(char) * BUFF_SIZE);
                            strcpy(info2,"WIN:B");
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(player, info2 + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, info2 + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            free(info2);

                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, buffor + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            gra_trwa = false;
                        //jeżeli brak mata, przekaż ruch drugiemuu graczowi
                        } else {
                            //jeżeli brak mata, przekaż ruch drugiemuu graczowi
                            bytes_write = 0;
                            while (bytes_write < BUFF_SIZE) {
                                result_write = write(enemy, buffor + bytes_write, BUFF_SIZE - bytes_write);
                                if (result_write < 1) break;
                                bytes_write += result_write;
                            }
                            // 0 i -1 uznajemy za taki sam rezultat - nie jesteśmy w stanie kontynuować gry
                            // z danym graczem więc zamykamy pokój
                            if (result_write < 1) {
                                gra_trwa = false;
                                first_is_dead = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("[%d-%d] Zamknięcie pokoju\n", th_data->first_socket_descriptor,th_data->second_socket_descriptor);
    if (first_is_dead || second_is_dead) {
        char * info = (char *) malloc(sizeof(char) * BUFF_SIZE);
        strcpy(info,"_INFO");
        bytes_write = 0;
        while (bytes_write < BUFF_SIZE) {
            if (first_is_dead)
                result_write = write(th_data->second_socket_descriptor, info + bytes_write, BUFF_SIZE - bytes_write);
            else
                result_write = write(th_data->first_socket_descriptor, info + bytes_write, BUFF_SIZE - bytes_write);
            if (result_write < 1) break;
            bytes_write += result_write;
        }
        info = (char *) malloc(sizeof(char) * BUFF_SIZE);
        strcpy(info,"_CONN");
        bytes_write = 0;
        while (bytes_write < BUFF_SIZE) {
            if (first_is_dead)
                result_write = write(th_data->second_socket_descriptor, info + bytes_write, BUFF_SIZE - bytes_write);
            else
                result_write = write(th_data->first_socket_descriptor, info + bytes_write, BUFF_SIZE - bytes_write);
            if (result_write < 1) break;
            bytes_write += result_write;
        }
        free(info);
    }
    free(buffor);
    free(th_data);
    close(th_data->first_socket_descriptor);
    close(th_data->second_socket_descriptor);
    pthread_exit(NULL);
}

//wątek odpowiedzialny za łączenie pary graczy w pokoje
void *ThreadJoin(void *t_data) {
    pthread_detach(pthread_self());
    struct data_thread_join *th_data = (struct data_thread_join *) t_data;
    while (1) {
        //jeżeli w kolejce jest mniej niż dwóch graczy czekaj na sygnał
        pthread_mutex_lock(&queue_mutex);
        while (th_data->players_queue.size() < 2) {
            pthread_cond_wait(&at_least_two_players,&queue_mutex);
        }
        //stworz nowy wątek, który będzie odpowiedzialny za rozgrywkę
        // uchwyt na wątek
        pthread_t thread1;
        //dane, które zostaną przekazane do wątku
        struct data_thread_game *game_data = (struct data_thread_game *) malloc(sizeof(struct data_thread_game));
        //dane przekazdywane do wątku do deskryptory pierwszego i drugiego gracza
        game_data->first_socket_descriptor = th_data->players_queue.front();
        th_data->players_queue.pop();
        game_data->second_socket_descriptor = th_data->players_queue.front();
        th_data->players_queue.pop();
        pthread_mutex_unlock(&queue_mutex);

        cout<<"["<<game_data->first_socket_descriptor<<"-"<<game_data->second_socket_descriptor<<"] --- NOWA GRA ---"<<endl;
        int create_result = 0;
        create_result = pthread_create(&thread1, NULL, ThreadBehavior, (void *) game_data);

        if (create_result) {
            printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
        }
    }
}

//utworzenie wątku, który będzie łączyć graczy w pray i tworzył dla nich nową grę
void createThreadJoin(queue<int> &new_players_sockets) {
    //wynik funkcji tworzącej wątek
    int create_result = 0;
    //uchwyt na wątek
    pthread_t thread1;
    //dane, które zostaną przekazane do wątku
    //to wektor podłączonych i oczekujących na grę graczy
    struct data_thread_join *t_data = new data_thread_join(new_players_sockets);
    create_result = pthread_create(&thread1, NULL, ThreadJoin, (void *) t_data);
    if (create_result) {
        printf("Błąd przy próbie utworzenia wątku, kod błędu: %d\n", create_result);
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    queue <int> new_players_sockets;
    int server_socket_descriptor;
    int connection_socket_descriptor;
    int bind_result;
    int listen_result;
    char reuse_addr_val = 1;
    struct sockaddr_in server_address;

    //inicjalizacja gniazda serwera
    memset(&server_address, 0, sizeof(struct sockaddr));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);

    server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_descriptor < 0) {
        fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
        exit(1);
    }
    setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse_addr_val, sizeof(reuse_addr_val));
    printf("Socket serwera: %d\n", server_socket_descriptor);
    bind_result = bind(server_socket_descriptor, (struct sockaddr *) &server_address, sizeof(struct sockaddr));
    if (bind_result < 0) {
        fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
        exit(1);
    }

    listen_result = listen(server_socket_descriptor, QUEUE_SIZE);
    if (listen_result < 0) {
        fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
        exit(1);
    }
    createThreadJoin(new_players_sockets);
    while (1) {
        //dodawaj nowo podłączonych graczy do kolejki
        //jeżeli w kolejce jest 2 lub więcej graczy wysyłaj sygnał
        connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
        if (connection_socket_descriptor < 0) {
            fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
        } else {
            printf("Nastąpiło połączenie na sockecie: %d\n", connection_socket_descriptor);
            pthread_mutex_lock(&queue_mutex);
            new_players_sockets.push(connection_socket_descriptor);
            if (new_players_sockets.size() >= 2) pthread_cond_signal(&at_least_two_players);
            pthread_mutex_unlock(&queue_mutex);
        }
    }
}
