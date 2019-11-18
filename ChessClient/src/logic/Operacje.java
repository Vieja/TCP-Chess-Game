package logic;

public class Operacje {

//     metoda zamieniająca współrządne szachowe na tablicę dwóch liczb
//     pozycja - współrzędna w postaci np E4
//     return - tablica[2] intów ze współrzędnymi liczbowymi

    public static int[] rozkodujPozycje(String pozycja) {
        int[] tab = new int[2];

        switch (pozycja.charAt(0)) {
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
        tab[1] = Integer.parseInt(pozycja.substring(1, 2));
        return tab;
    }

    public static String zakodujPozycje(int a, int b) {
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
}
