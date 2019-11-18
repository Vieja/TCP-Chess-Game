package bierki;

import logic.Operacje;

import java.util.ArrayList;

public class Pion extends Bierka {

    private static final String nazwa_bierki = "pion";

    public Pion(boolean czy_czarny, int[] polozenie, String pol) {
        super(czy_czarny, polozenie, 1, pol);
    }

    @Override
    public ArrayList<String> znajdzMozliweRuchy(int[][] tab) {
        ArrayList<String> mozliwe_ruchy = new ArrayList<>();
        int [] pol = getPolozenie();
        if (isKolorCzarny()) {
            if (pol[1] != 1) {
                if (tab[pol[0]][pol[1] - 1] == 0) {
                    if (getIlosc_wykon_ruchow() == 0 && tab[pol[0]][pol[1] - 2] == 0)
                        mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0], pol[1] - 2));
                    mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0], pol[1] - 1));
                }
                if (pol[0] - 1 > 0 && pol[1] - 1 > 0)
                    if (tab[pol[0] - 1][pol[1] - 1] == -1)
                        mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0] - 1, pol[1] - 1));
                if (pol[0] + 1 < 9 && pol[1] - 1 > 0)
                    if (tab[pol[0] + 1][pol[1] - 1] == -1)
                        mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0] + 1, pol[1] - 1));
            }
        } else {
            if (pol[1] != 8) {
                if (tab[pol[0]][pol[1] + 1] == 0) {
                    if (getIlosc_wykon_ruchow() == 0 && tab[pol[0]][pol[1] + 2] == 0)
                        mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0], pol[1] + 2));
                    mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0], pol[1] + 1));
                }
                if (pol[0] - 1 > 0 && pol[1] + 1 < 9)
                    if (tab[pol[0] - 1][pol[1] + 1] == -1)
                        mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0] - 1, pol[1] + 1));
                if (pol[0] + 1 < 9 && pol[1] + 1 < 9)
                    if (tab[pol[0] + 1][pol[1] + 1] == -1)
                        mozliwe_ruchy.add(Operacje.zakodujPozycje(pol[0] + 1, pol[1] + 1));
            }
        }
        return mozliwe_ruchy;
    }

    @Override
    public String getNazwaBierki() {
        return nazwa_bierki;
    }
}
