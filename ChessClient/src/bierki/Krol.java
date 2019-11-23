package bierki;

import logic.Operacje;

import java.util.ArrayList;

public class Krol extends Bierka {

    private static final String nazwa_bierki = "krol";

    public Krol(boolean czy_czarny, int[] polozenie, String pol) {
        super(czy_czarny, polozenie, 0, pol);
    }

    @Override
    public ArrayList<String> znajdzMozliweRuchy(int[][] tab) {
        ArrayList<String> mozliwe_ruchy = new ArrayList<>();
        int [] pol = getPolozenie();
        int i = pol[0];
        int j = pol[1];
        if (i-1 > 0 && j - 1 > 0)
            if (tab[i-1][j-1] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i-1,j-1));
        if (i-1 > 0)
            if (tab[i-1][j] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i-1,j));
        if (i-1 > 0 && j + 1 < 9)
            if (tab[i-1][j+1] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i-1,j+1));
        if (j + 1 < 9)
            if (tab[i][j+1] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j+1));
        if (i+1 < 9 && j + 1 < 9)
            if (tab[i+1][j+1] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i+1,j+1));
        if (i+1 < 9)
            if (tab[i+1][j] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i+1,j));
        if (i+1 < 9 && j - 1 > 0)
            if (tab[i+1][j-1] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i+1,j-1));
        if (j - 1 > 0)
            if (tab[i][j-1] != 1) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j-1));
        return mozliwe_ruchy;
    }


    @Override
    public String getNazwaBierki() {
        return nazwa_bierki;
    }
}
