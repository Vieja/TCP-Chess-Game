package bierki;

import java.util.ArrayList;

public abstract class Bierka {

    private boolean kolor_czarny;
    private int polozenie[];
    private int wartosc;
    private int ilosc_wykon_ruchow;
    private String poleStartu;

    public Bierka(boolean kolor_czarny, int polozenie[], int wartosc, String poleStartu) {
        this.kolor_czarny = kolor_czarny;
        this.polozenie = polozenie;
        this.wartosc = wartosc;
        this.ilosc_wykon_ruchow = 0;
        this.poleStartu = poleStartu;
    }

    public abstract ArrayList<String> znajdzMozliweRuchy(int [][] glupia_tablica);

    public abstract String getNazwaBierki();

    public String zakodujPozycje(int a, int b) {
        String pozycja="";
        switch (a) {
            case 1: pozycja="A";
                break;
            case 2: pozycja="B";
                break;
            case 3: pozycja="C";
                break;
            case 4: pozycja="D";
                break;
            case 5: pozycja="E";
                break;
            case 6: pozycja="F";
                break;
            case 7: pozycja="G";
                break;
            case 8: pozycja="H";
                break;
        }
        pozycja=pozycja+b;
        return pozycja;
    }

    //gettery i settery

    public String getPoleStartu() {
        return poleStartu;
    }

    public int getIlosc_wykon_ruchow() {
        return ilosc_wykon_ruchow;
    }

    public void wykonanoRuch() {
        this.ilosc_wykon_ruchow += 1;
    }

    public int getWartosc() {
        return wartosc;
    }

    public int[] getPolozenie() {
        return polozenie;
    }

    public String getPolozenieJakoString() {
        return zakodujPozycje(polozenie[0],polozenie[1]);
    }

    public void setPolozenie(int[] polozenie) {
        this.polozenie = polozenie;
    }

    public boolean isKolorCzarny() {
        return kolor_czarny;
    }
}
