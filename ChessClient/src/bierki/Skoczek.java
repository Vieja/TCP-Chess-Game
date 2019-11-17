package bierki;

import java.util.ArrayList;

public class Skoczek extends Bierka {

    private static final String nazwa_bierki = "skoczek";

    public Skoczek(boolean czy_czarny, int[] polozenie, String pol) {
        super(czy_czarny, polozenie, 3, pol);
    }

    @Override
    public ArrayList<String> znajdzMozliweRuchy(int[][] tab) {
        ArrayList<String> mozliwe_ruchy = new ArrayList<>();
        int [] pol = getPolozenie();
        for (int i = 1;i < 9;i++)
            for (int j = 1;j < 9;j++) {
                if (tab[i][j] != 1) {
                    if (i-2 == pol[0] && j+1 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i-1 == pol[0] && j+2 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i+1 == pol[0] && j+2 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i+2 == pol[0] && j+1 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i+2 == pol[0] && j-1 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i+1 == pol[0] && j-2 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i-1 == pol[0] && j-2 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                    if (i-2 == pol[0] && j-1 == pol[1]) mozliwe_ruchy.add(this.zakodujPozycje(i,j));
                }
            }
        return mozliwe_ruchy;
    }

    @Override
    public String getNazwaBierki() {
        return nazwa_bierki;
    }
}
