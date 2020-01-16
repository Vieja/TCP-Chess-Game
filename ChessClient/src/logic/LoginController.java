package logic;

import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;

public class LoginController {
    public TextField loginField;
    public TextField addressField;
    public TextField portField;
    private Main main;

    public void setMain (Main m) {
        this.main = m;
    }

    public void joinGame() {
        String login = loginField.getText();
        String address = addressField.getText();
        String port = portField.getText();
        int result = main.connect();
        System.out.println(result);
    }
}
