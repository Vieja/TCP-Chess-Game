package logic;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class Main extends Application {

    private Stage primaryStage;
    private Socket socket;
    LoginController loginController;

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Chess Game");
        this.primaryStage = primaryStage;
        initializeLoginView();
    }

    public void initializeLoginView(){
        try {
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("../layout/LoginView.fxml"));
            BorderPane sign = loader.load();
            loginController = loader.getController();
            loginController.setMain(this);
            Scene scene = new Scene(sign);
            primaryStage.setScene(scene);
            primaryStage.setResizable(false);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void initializeGameView(String yourLogin, String enemyLogin, String color) {
        try {
            primaryStage.close();
            primaryStage = new Stage();
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("../layout/GameView.fxml"));
            BorderPane rootLayout = loader.load();
            Controller gameController = loader.getController();
            gameController.setConnectionThread(socket);
            gameController.setLogins(yourLogin, enemyLogin);
            gameController.startGame(color);
            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.setResizable(false);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public int connect(String login, String address, String port) {
        try {
            socket = new Socket(address, Integer.parseInt(port));

            System.out.format("[Client] Connected to server!\n");
            final Thread newGameThread = new Thread() {
                @Override
                public void run() {
                    System.out.println("Started...");
                    InputStream is;
                    OutputStream os;
                    String response;
                    int ile;
                    byte[] buffer = new byte[5];
                    byte[] buffer_login = new byte[9];
                    byte[] buffer_login_size = new byte[1];
                    try {
                        is = socket.getInputStream();
                        os = socket.getOutputStream();

                        ile = is.read(buffer);
                        String s1 = new String(buffer);
                        if (s1.equals("login")) {
                            response = login;
                            os.write(response.getBytes());
                        } else {
                            System.out.println("Błąd: niezrozumiałe polecenie, przerwano logowanie");
                            loginController.threadExists = false;
                            this.interrupt();
                        }
                        ile = is.read(buffer_login_size);
                        int size = buffer_login_size[0] - 48;
                        System.out.println("Size: "+size);
                        ile = is.read(buffer_login);
                        String s2 = new String(buffer_login,0,size);
                        final String enemyLogin = s2;
                        System.out.println("login: "+enemyLogin);
                        System.out.println("length: " + enemyLogin.length());
                        ile = is.read(buffer);
                        System.out.println("ile wczytałem"+ile);
                        String s3 = new String(buffer);
                        final String color = s3;

                        Platform.runLater(new Runnable() {
                            @Override
                            public void run() {
                                initializeGameView(login, enemyLogin, color);
                            }
                        });

                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                };
            };
            newGameThread.start();

        } catch (IOException e) {
            return -1;
        }
        return 0;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
