package logic;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class ConnectionHandler implements Runnable {

    private Game game;
    private Socket socket;
    boolean stop;

    public ConnectionHandler(Socket socket, Game gra) {
        this.socket = socket;
        this.game = gra;
        this.stop = false;
    }

    @Override
    public void run() {
        try {
            InputStream is = socket.getInputStream();
            OutputStream os = socket.getOutputStream();
            byte[] buffer = new byte[5];
            int ile;
            while (!stop) {
                if (game.poraNaWroga) {
                    ile = is.read(buffer);
                    if (buffer[2] == ':') {
                        game.wrogWykonalRuch(new String(buffer,0,2), new String(buffer,3,2));
                        game.poraNaWroga = false;
                    } else {
                        String received = new String(buffer,0,5);
                        System.out.println(received);
                        if (received.equals("_INFO")) {
                            ile = is.read(buffer);
                            received = new String(buffer,0,5);
                            System.out.println(received);
                            if (received.equals("_CONN")) {
                                game.theEnd("nikt");
                            } else if (received.equals("WIN:W")) {
                                game.theEnd("white");
                            } else if (received.equals("WIN:B")) {
                                game.theEnd("black");
                            } else {
                                System.out.println("Otrzymabo błędną wiadomość...");
                            }
                        }
                    }
                } else if (game.ruchGotowyDoWysylki) {
                    os.write(game.wysylka.getBytes());
                    game.poraNaWroga = true;
                    game.ruchGotowyDoWysylki = false;
                }
                //System.out.println("working...");
                Thread.sleep(1000);
            }
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
