package logic;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class ConnectionHandler implements Runnable {

    private Game game;
    private Socket socket;

    public ConnectionHandler(Socket socket, Game gra) {
        this.socket = socket;
        this.game = gra;
    }

    @Override
    public void run() {
        try {
            InputStream is = socket.getInputStream();
            OutputStream os = socket.getOutputStream();
            //String received;
            //String response;
            byte[] buffer = new byte[5];
            int ile;
            while (true) {
                if (game.poraNaWroga) {
                    ile = is.read(buffer);
                    //received = new String(buffer,0,5);
                    if (buffer[2] == ':') {
                        game.wrogWykonalRuch(new String(buffer,0,2), new String(buffer,3,2));
                        game.poraNaWroga = false;
                    } else {
                        System.out.println("Jakiś błąd...");
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
