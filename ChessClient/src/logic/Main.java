package logic;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
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
            socket = new Socket("127.0.0.1", 1234);

            System.out.format("[Client] Connected to server!\n");
            final Thread newGameThread = new Thread() {
                @Override
                public void run() {
                    System.out.println("Started...");
                    InputStream is;
                    OutputStream os ;
                    String received;
                    String response;
                    int ile;
                    byte[] buffer = new byte[5];
                    try {
                        is = socket.getInputStream();
                        os = socket.getOutputStream();

                        ile = is.read(buffer);
                        received = new String(buffer,0,5);
                        if (received.equals("login")) {
                            response = login;
                            os.write(response.getBytes());
                        } else {
                            System.out.println("Błąd: niezrozumiałe polecenie, przerwano logowanie");
                            loginController.threadExists = false;
                            this.interrupt();
                        }

                        ile = is.read(buffer);
                        received = new String(buffer,0,5);
                        final String enemyLogin = received;

                        ile = is.read(buffer);
                        received = new String(buffer,0,5);
                        final String color = received;

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
