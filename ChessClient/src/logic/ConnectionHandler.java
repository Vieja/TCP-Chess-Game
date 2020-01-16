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
            while (true) {
                System.out.println("working...");
                Thread.sleep(3000);
            }
        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }
}
