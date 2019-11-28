package logic;

import java.util.Scanner;

public class EnemyAction implements Runnable{

    private Game game;

    public EnemyAction(Game gra) {
        this.game = gra;
    }

    @Override
    public void run() {
        while (true) {
            if (game.poraNaWroga) {
                System.out.println("KNUJĘ COŚ NIEDOBREGO HEHEHEHE");
                Scanner scan = new Scanner(System.in);
                String start = scan.nextLine();
                String koniec = scan.nextLine();
                game.wrogWykonalRuch(start, koniec);
                game.poraNaWroga = false;
            }

            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
