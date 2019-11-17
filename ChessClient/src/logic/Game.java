package logic;

public class Game {
    public static boolean czy_biale = true;

    public static int onClick(String where) {
        int wyn = -1;
        if (where.equals("A1")) {
            wyn = 1;
        }
        return wyn;
    }
}
