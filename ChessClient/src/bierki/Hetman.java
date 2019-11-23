package bierki;

import logic.Operacje;

import java.util.ArrayList;

public class Hetman extends Bierka {

    private static final String nazwa_bierki = "hetman";

    public Hetman(boolean czy_czarny, int[] polozenie, String pol) {
        super(czy_czarny, polozenie, 9, pol);
    }

    @Override
    public ArrayList<String> znajdzMozliweRuchy(int[][] tab) {
        ArrayList<String> mozliwe_ruchy = new ArrayList<>();
        int [] pol = getPolozenie();

        int i = pol[0]-1;
        int j = pol[1]+1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            i--;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] + 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            i++;
            j++;
        }

        i = pol[0] + 1;
        j = pol[1] - 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            i++;
            j--;
        }

        i = pol[0] - 1;
        j = pol[1] - 1;
        while(i > 0 && i < 9 && j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            i--;
            j--;
        }

        i = pol[0]+1;
        j = pol[1];
        while(i > 0 && i < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            i++;
        }

        i = pol[0]-1;
        j = pol[1];
        while(i > 0 && i < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            i--;
        }

        i = pol[0];
        j = pol[1]+1;
        while(j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            j++;
        }

        i = pol[0];
        j = pol[1]-1;
        while(j > 0 && j < 9) {
            if(tab[i][j] == 0) mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
            else if(tab[i][j] == -1) {
                mozliwe_ruchy.add(Operacje.zakodujPozycje(i,j));
                break;
            } else break;
            j--;
        }

        return mozliwe_ruchy;
    }


    @Override
    public String getNazwaBierki() {
        return nazwa_bierki;
    }
}
