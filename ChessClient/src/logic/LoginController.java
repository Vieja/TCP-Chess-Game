package logic;

import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;

public class LoginController {
    public TextField loginField;
    public TextField addressField;
    public TextField portField;
    public Label waitingLabel;
    public boolean threadExists = false;
    private Main main;
    String textError = "Błąd - nie można połączyć z serwerem";
    String textWaiting = "Szukanie gracza...";

    public void setMain (Main m) {
        this.main = m;
    }

    public void joinGame() {
        if (!threadExists) {
            String login = loginField.getText();
            String address = addressField.getText();
            String port = portField.getText();
            int result = main.connect(login, address, port);
            System.out.println(result);
            if (result == 0) {
                threadExists = true;
                waitingLabel.setText(textWaiting);
                waitingLabel.setVisible(true);
            } else {
                waitingLabel.setText(textError);
                waitingLabel.setVisible(true);
            }
        }
    }
}
