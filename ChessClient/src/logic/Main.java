package logic;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

import java.io.IOException;
import java.net.Socket;

public class Main extends Application {

    private Stage primaryStage;
    private Socket socket;

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
            LoginController controller = loader.getController();
            controller.setMain(this);
            Scene scene = new Scene(sign);
            primaryStage.setScene(scene);
            primaryStage.setResizable(false);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void initializeGameView() {
        try {
            initializeGameView();
            System.out.format("[Client] Connected to server!\n");

            final Thread outThread = new Thread() {
                @Override
                public void run() {
                    System.out.println("Started...");
                };
            };
            outThread.start();

            primaryStage.close();
            primaryStage = new Stage();
            FXMLLoader loader = new FXMLLoader();
            loader.setLocation(Main.class.getResource("../layout/GameView.fxml"));
            BorderPane rootLayout = loader.load();
            Scene scene = new Scene(rootLayout);
            primaryStage.setScene(scene);
            primaryStage.setResizable(false);
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public int connect() {
        try {
            socket = new Socket("127.0.0.1", 1234);
        } catch (IOException e) {
            return -1;
        }
        return 0;
    }

    public static void main(String[] args) {
        launch(args);
    }
}
