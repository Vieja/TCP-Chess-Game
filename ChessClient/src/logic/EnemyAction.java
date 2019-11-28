package logic;

public class EnemyAction implements Runnable{

    public void EnemyAction() {
    }

    @Override
    public void run() {
        while (true) {
            System.out.println("KNUJĘ COŚ NIEDOBREGO HEHEHEHE");
            try {
                Thread.sleep(5000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
