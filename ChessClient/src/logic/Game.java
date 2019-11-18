package logic;

import bierki.*;

import java.util.ArrayList;

public class Game {
    public boolean czyTyToBiale;
    public boolean etap_wybierania_piona = true;

    private int[][] glupia_szachownica;

    private Bierka wybrana_bierka = null;

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
        if (etap_wybierania_piona) {
            wybrana_bierka = null;
            wybrana_bierka = sprawdzCzyJestTuTwojPion(where);
            /* TODO
                sprawdz czy bierka ma jakies ruchy, w innym wypadku nie mozna jej wybrac */
            ArrayList<String> mozliweRuchy = wybrana_bierka.znajdzMozliweRuchy(glupia_szachownica);
            if (mozliweRuchy.isEmpty()) wybrana_bierka = null;
            if (wybrana_bierka != null) {

            }
        }
            int wyn = -1;
            if (where.equals("A1")) {
                wyn = 1;
            }
        return wyn;
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

    public void setCzyBiale(boolean czy) {
        czyTyToBiale = czy;
    }
}
