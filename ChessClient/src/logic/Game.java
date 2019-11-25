package logic;

import bierki.*;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public class Game {
    public boolean czyTyToBiale;
    public boolean etap_wybierania_piona = true;
    private boolean dluga_roszada = false;
    private boolean krotka_roszada = false;
    private boolean wykonano_roszade = false;

    private int[][] glupia_szachownica;

    private Bierka wybrana_bierka = null;
    ArrayList<String> mozliwe_ruchy = new ArrayList<>();
    ArrayList<String> mozliwe = new ArrayList<>();

    private ArrayList<Bierka> biale_bierki = new ArrayList<>();
    private ArrayList<Bierka> czarne_bierki = new ArrayList<>();

    public Game() {
        glupia_szachownica = new int[9][9];
        for (int i = 1; i < 9; i++)
            for (int j = 1; j < 3; j++) {
                glupia_szachownica[i][j] = 1;
            }
        for (int i = 1; i < 9; i++)
            for (int j = 3; j < 7; j++) {
                glupia_szachownica[i][j] = 0;
            }
        for (int i = 1; i < 9; i++)
            for (int j = 7; j < 9; j++) {
                glupia_szachownica[i][j] = -1;
            }


        for (int i = 1; i < 9; i++) {
            int[] tab = {i, 2};
            biale_bierki.add(new Pion(false, tab, Operacje.zakodujPozycje(tab[0], tab[1])));
        }

        for (int i = 1; i < 9; i++) {
            int[] tab = {i, 7};
            czarne_bierki.add(new Pion(true, tab, Operacje.zakodujPozycje(tab[0], tab[1])));
        }

        biale_bierki.add(new Wieza(false, Operacje.rozkodujPozycje("A1"), "A1"));
        biale_bierki.add(new Skoczek(false, Operacje.rozkodujPozycje("B1"), "B1"));
        biale_bierki.add(new Goniec(false, Operacje.rozkodujPozycje("C1"), "C1"));
        biale_bierki.add(new Hetman(false, Operacje.rozkodujPozycje("D1"), "D1"));
        biale_bierki.add(new Krol(false, Operacje.rozkodujPozycje("E1"), "E1"));
        biale_bierki.add(new Goniec(false, Operacje.rozkodujPozycje("F1"), "F1"));
        biale_bierki.add(new Skoczek(false, Operacje.rozkodujPozycje("G1"), "G1"));
        biale_bierki.add(new Wieza(false, Operacje.rozkodujPozycje("H1"), "H1"));

        czarne_bierki.add(new Wieza(true, Operacje.rozkodujPozycje("A8"), "A8"));
        czarne_bierki.add(new Skoczek(true, Operacje.rozkodujPozycje("B8"), "B8"));
        czarne_bierki.add(new Goniec(true, Operacje.rozkodujPozycje("C8"), "C8"));
        czarne_bierki.add(new Hetman(true, Operacje.rozkodujPozycje("D8"), "D8"));
        czarne_bierki.add(new Krol(true, Operacje.rozkodujPozycje("E8"), "E8"));
        czarne_bierki.add(new Goniec(true, Operacje.rozkodujPozycje("F8"), "F8"));
        czarne_bierki.add(new Skoczek(true, Operacje.rozkodujPozycje("G8"), "G8"));
        czarne_bierki.add(new Wieza(true, Operacje.rozkodujPozycje("H8"), "H8"));

    }

    public int onClick(String where) {
        int odp;
        if (etap_wybierania_piona) {
            odp = wybieraniePiona(where);
            mozliwe_ruchy.addAll(mozliwe);
        } else {
            int mozeNowaBierka;
            Bierka wybrana = wybrana_bierka;
            mozeNowaBierka = wybieraniePiona(where);
            if (mozeNowaBierka == 2) {
                odp = 4;
            } else { //obsługa wybierania ruchu
                wybrana_bierka = wybrana;
                //
                //

                mozliwe_ruchy.clear();
                etap_wybierania_piona = true;
                odp = 1;

            }
        }
        return odp;
    }

    public int wybieraniePiona(String where) {
        int odp = -1;
        wybrana_bierka = null;
        mozliwe.clear();
        if (czyTyToBiale) wybrana_bierka = sprawdzCoTuJest(biale_bierki, Operacje.rozkodujPozycje(where));
        else wybrana_bierka = sprawdzCoTuJest(czarne_bierki, Operacje.rozkodujPozycje(where));
        if (wybrana_bierka != null) { //wybrano pionek, nie wiemy czy ma możliwe ruchy
            /* TODO tutaj mozna wsadzic bicie w przelocie "" kazBierceZnalezcMozliweRuchy "" */
            mozliwe= wybrana_bierka.znajdzMozliweRuchy(glupia_szachownica);
            if (mozliwe.isEmpty()) {
                wybrana_bierka = null;
            } else { //wybrano pionek, mamy listę możliwych ruchów
                boolean wybranoKrola = false;
                if (wybrana_bierka.getNazwaBierki().equals("krol")) wybranoKrola = true;
                mozliweRuchyBezSzachaZOdkrycia(wybrana_bierka.getPolozenie(), wybranoKrola);
                if (!mozliwe.isEmpty()) {
                    //jeżeli coś jest na liście to zakończ etap wybierania piona
                    etap_wybierania_piona = false;
                    odp = 2;
                }
            }
        }
        return odp;
    }

    public Bierka sprawdzCzyJestTuTwojPion(String where) {
        if (czyTyToBiale) {
            for (Bierka bierka : biale_bierki) {
                String pozycjaBierki = bierka.getPolozenieJakoString();
                if (pozycjaBierki.equals(where)) return bierka;
            } return null;
        } else {
            for (Bierka bierka : czarne_bierki) {
                String pozycjaBierki = bierka.getPolozenieJakoString();
                if (pozycjaBierki.equals(where)) return bierka;
            } return null;
        }
    }

    public int[] znajdzPozycjeNaszegoKrola() {
        int[] pozycjaKrola = new int[2];
        ArrayList<Bierka> lista;

        if (czyTyToBiale) lista = biale_bierki;
        else lista = czarne_bierki;

        for (Bierka bierka : lista)
            if (bierka.getNazwaBierki().equals("krol")) pozycjaKrola = bierka.getPolozenie();

        return pozycjaKrola;
    }

    public void mozliweRuchyBezSzachaZOdkrycia(int[] polozenie, boolean wybranoKrola) {
        int[][] tmp_glupia = new int[9][9];
        boolean czy;
        int[] nowa_pozycja;
        int[] poz = znajdzPozycjeNaszegoKrola();
        int[] pozycjaKrola = new int[2];
        ArrayList<String> doUsuniecia = new ArrayList<>();

        for (String ruch : mozliwe_ruchy) {
            for (int i = 1; i < 9; i++)
                for (int j = 1; j < 9; j++) tmp_glupia[i][j] = glupia_szachownica[i][j];
            nowa_pozycja = Operacje.rozkodujPozycje(ruch);
            tmp_glupia[polozenie[0]][polozenie[1]] = 0;
            tmp_glupia[nowa_pozycja[0]][nowa_pozycja[1]] = 1;

            if (wybranoKrola) {
                pozycjaKrola[0] = nowa_pozycja[0];
                pozycjaKrola[1] = nowa_pozycja[1];
            } else {
                pozycjaKrola[0] = poz[0];
                pozycjaKrola[1] = poz[1];
            }

            czy = sprawdzCzySzach(tmp_glupia, nowa_pozycja, pozycjaKrola);
            if (czy) {
                doUsuniecia.add(ruch);
                //Log.v("mozliweRuchyBezSzacha","Usuwam "+ruch);
            }
        }
        if (wybranoKrola) dodajMozliweRoszady();
        mozliwe_ruchy.removeAll(doUsuniecia);
    }

    public boolean sprawdzCzySzach() {
        odwrocGlupiaSzachownice();

        ArrayList<String> tmp;
        boolean odp;

        tmp = stworzListeAtakowanychPol(czyTyToBiale);
        if (tmp != null) {
            odp = sprawdzCzyKrolWsrodPolAtakowanych(tmp, znajdzPozycjeNaszegoKrola());
            if (odp) {
                odwrocGlupiaSzachownice();
                return true;
            }
        }
        odwrocGlupiaSzachownice();
        return false;
    }

    //przejrzyj liste bierek konkretnego koloru i znajdz tą która znajduje sie na zadanej pozycji
    //zwraca null jeżeli brak takiej bierki
    public Bierka sprawdzCoTuJest(List<Bierka> kolor, int[] sprawdzanaPozycja) {
        for (Bierka bierka : kolor) {
            int[] pozycjaBierki = bierka.getPolozenie();
            if (sprawdzanaPozycja[0] == pozycjaBierki[0] && sprawdzanaPozycja[1] == pozycjaBierki[1]) {
                System.out.println("Na tej pozycji jest " + bierka.getNazwaBierki());
                return bierka;
            }
        }
        return null;
    }

    public boolean sprawdzCzySzach(int[][] tab, int[] potancjalneBicie, int[] pozycjaKrola) {
        int[][] szachownica = new int[9][9];
        for (int i = 1; i < 9; i++)
            for (int j = 1; j < 9; j++)
                szachownica[i][j] = (-1) * tab[i][j];

        HashSet<String> polaAtakowane = new HashSet<>();
        ArrayList<String> tmp = new ArrayList<>();
        ArrayList<Bierka> listaBierek;

        if (czyTyToBiale) listaBierek = czarne_bierki;
        else listaBierek = biale_bierki;

        Bierka potencjalnaBita = sprawdzCoTuJest(listaBierek, potancjalneBicie);

        for (Bierka bierka : listaBierek) {
            if (bierka != potencjalnaBita) {
                tmp.clear();
                tmp = bierka.znajdzMozliweRuchy(szachownica);
                polaAtakowane.addAll(tmp);
            }
        }
        tmp.clear();
        tmp.addAll(polaAtakowane);

        boolean odp;

        if (!tmp.isEmpty()) {
            odp = sprawdzCzyKrolWsrodPolAtakowanych(tmp, pozycjaKrola);
            return odp;
        }
        return false;
    }

    public boolean sprawdzCzyKrolWsrodPolAtakowanych(ArrayList<String> polaAtakowane, int[] pozycjaKrola) {
        int[] pozycja;
        if (!polaAtakowane.isEmpty())
            for (String pole : polaAtakowane) {
                pozycja = Operacje.rozkodujPozycje(pole);
                if (pozycja[0] == pozycjaKrola[0] && pozycja[1] == pozycjaKrola[1])
                    return true;
            }
        return false;
    }

    public ArrayList<String> stworzListeAtakowanychPol(boolean kolorAtakujacyToCzarny) {
        HashSet<String> polaAtakowane = new HashSet<>();
        ArrayList<String> tmp = new ArrayList<>();
        ArrayList<Bierka> listaBierek;

        if (kolorAtakujacyToCzarny) listaBierek = czarne_bierki;
        else listaBierek = biale_bierki;

        for (Bierka bierka : listaBierek) {
            tmp.clear();
            tmp = bierka.znajdzMozliweRuchy(glupia_szachownica);
            polaAtakowane.addAll(tmp);
        }
        tmp.clear();
        tmp.addAll(polaAtakowane);
        return tmp;
    }

    public void dodajMozliweRoszady() {
        ArrayList<Bierka> lista;
        if (czyTyToBiale) lista = biale_bierki;
        else lista = czarne_bierki;

        ArrayList<String> polePrzejscia = new ArrayList<>();

        if (wybrana_bierka.getIlosc_wykon_ruchow() == 0)
            for (Bierka bierka : lista) {
                if (bierka.getPoleStartu().equals("A1")) {
                    if (bierka.getIlosc_wykon_ruchow() == 0 &&
                            glupia_szachownica[2][1] == 0 &&
                            glupia_szachownica[3][1] == 0 &&
                            glupia_szachownica[4][1] == 0) {
                        polePrzejscia.add("D1");
                        polePrzejscia.add("C1");
                        polePrzejscia.add("E1");
                        czyRoszadaMozliwa(polePrzejscia, true);
                        polePrzejscia.clear();
                    }
                } else if (bierka.getPoleStartu().equals("A8")) {
                    if (bierka.getIlosc_wykon_ruchow() == 0 &&
                            glupia_szachownica[2][8] == 0 &&
                            glupia_szachownica[3][8] == 0 &&
                            glupia_szachownica[4][8] == 0) {
                        polePrzejscia.add("D8");
                        polePrzejscia.add("C8");
                        polePrzejscia.add("E8");
                        czyRoszadaMozliwa(polePrzejscia, true);
                        polePrzejscia.clear();
                    }
                } else if (bierka.getPoleStartu().equals("H8")) {
                    if (bierka.getIlosc_wykon_ruchow() == 0 &&
                            glupia_szachownica[6][8] == 0 &&
                            glupia_szachownica[7][8] == 0) {
                        polePrzejscia.add("F8");
                        polePrzejscia.add("G8");
                        polePrzejscia.add("E8");
                        czyRoszadaMozliwa(polePrzejscia, false);
                        polePrzejscia.clear();
                    }
                } else if (bierka.getPoleStartu().equals("H1")) {
                    if (bierka.getIlosc_wykon_ruchow() == 0 &&
                            glupia_szachownica[6][1] == 0 &&
                            glupia_szachownica[7][1] == 0) {
                        polePrzejscia.add("F1");
                        polePrzejscia.add("G1");
                        polePrzejscia.add("E1");
                        czyRoszadaMozliwa(polePrzejscia, false);
                        polePrzejscia.clear();
                    }
                }
            }

        if (czyTyToBiale) {
            if (krotka_roszada) mozliwe_ruchy.add("G1");
            if (dluga_roszada) mozliwe_ruchy.add("C1");
        } else {
            if (krotka_roszada) mozliwe_ruchy.add("G8");
            if (dluga_roszada) mozliwe_ruchy.add("C8");
        }
    }

    public void czyRoszadaMozliwa(ArrayList<String> polePrzejscia, boolean dluga) {
        int[][] tmp_glupia = new int[9][9];
        int[] nowa_pozycja;
        int[] polozenie = wybrana_bierka.getPolozenie();
        int licz = 0;
        boolean czy;

        for (String przejscie : polePrzejscia) {
            for (int i = 1; i < 9; i++)
                for (int j = 1; j < 9; j++)
                    tmp_glupia[i][j] = glupia_szachownica[i][j];
            nowa_pozycja = Operacje.rozkodujPozycje(przejscie);
            tmp_glupia[polozenie[0]][polozenie[1]] = 0;
            tmp_glupia[nowa_pozycja[0]][nowa_pozycja[1]] = 1;

            czy = sprawdzCzySzach(tmp_glupia, nowa_pozycja, nowa_pozycja);
            if (!czy) licz++;
        }
        if (licz == 3) {
            if (dluga) dluga_roszada = true;
            else krotka_roszada = true;
        }
    }

    public void zaktualizujGlupiaSzachownice(int[] stara, int[] nowa) {
        int i = stara[0];
        int j = stara[1];
        glupia_szachownica[i][j] = 0;
        i = nowa[0];
        j = nowa[1];
        glupia_szachownica[i][j] = 1;
    }

    public void odwrocGlupiaSzachownice() {
        for (int i = 1; i < 9; i++)
            for (int j = 1; j < 9; j++)
                glupia_szachownica[i][j] = (-1) * glupia_szachownica[i][j];
    }

    // wyczysc informacje o roszadzie - nie doszło do niej
    public void resetujRoszade() {
        wykonano_roszade = false;
        dluga_roszada = false;
        krotka_roszada = false;
    }

    public void czyscMozliweRuchy() {
        mozliwe_ruchy.clear();
    }

    public void setCzyBiale(boolean czy) {
        this.czyTyToBiale = czy;
    }

    public ArrayList<String> getMozliweRuchy() {
        return mozliwe_ruchy;
    }
}
