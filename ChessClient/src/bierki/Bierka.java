package bierki;

import logic.Operacje;

import java.util.ArrayList;

public abstract class Bierka {

    private boolean kolor_czarny;
    private int[] polozenie;
    private int wartosc;
    private int ilosc_wykon_ruchow;
    private String poleStartu;

    public Bierka(boolean kolor_czarny, int[] polozenie, int wartosc, String poleStartu) {
        this.kolor_czarny = kolor_czarny;
        this.polozenie = polozenie;
        this.wartosc = wartosc;
        this.ilosc_wykon_ruchow = 0;
        this.poleStartu = poleStartu;
    }

    public abstract ArrayList<String> znajdzMozliweRuchy(int [][] glupia_tablica);

    public abstract String getNazwaBierki();


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

    public void setPolozenie(int[] polozenie) {
        this.polozenie = polozenie;
    }

    public String getPolozenieJakoString() {
        return Operacje.zakodujPozycje(polozenie[0],polozenie[1]);
    }

    public boolean isKolorCzarny() {
        return kolor_czarny;
    }

}
