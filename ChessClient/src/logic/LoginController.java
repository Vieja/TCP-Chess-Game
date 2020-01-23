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
    String textErrorConnection = "Błąd - nie można połączyć z serwerem";
    String textWaitingForPlayer = "Szukanie gracza...";
    String textErrorAddress = "Błędny adres lub port";
    String textErrorLogin = "Login musi mieć długość od 1 do 8 znaków";

    public void setMain (Main m) {
        this.main = m;
    }

    public void joinGame() {
        if (!threadExists) {
            String login = loginField.getText();
            String address = addressField.getText();
            String port = portField.getText();
            int result;
            if ( !validLogin(login) ) {
                result = -3;
            } else {
                if (validIP(address) & validPort(port)) {
                    result = main.connect(login, address, port);
                } else result = -2;
            }
            switch (result) {
                case 0: {
                    threadExists = true;
                    waitingLabel.setText(textWaitingForPlayer);
                    waitingLabel.setVisible(true);
                    break;
                }
                case -1: {
                    waitingLabel.setText(textErrorConnection);
                    waitingLabel.setVisible(true);
                    break;
                }
                case -2: {
                    waitingLabel.setText(textErrorAddress);
                    waitingLabel.setVisible(true);
                    break;
                }
                case -3: {
                    waitingLabel.setText(textErrorLogin);
                    waitingLabel.setVisible(true);
                    break;
                }
            }

        }
    }

    private boolean validLogin(String login) {
        if (login.isEmpty() || login.length() > 8 ) return false;
        return true;
    }

    private boolean validPort(String port) {
        if (port == null) {
            return false;
        }
        try {
            Integer.parseInt(port);
        } catch (NumberFormatException nfe) {
            return false;
        }
        return true;
    }

    public static boolean validIP (String ip) {
        try {
            if ( ip == null || ip.isEmpty() ) {
                return false;
            }

            String[] parts = ip.split( "\\." );
            if ( parts.length != 4 ) {
                return false;
            }

            for ( String s : parts ) {
                int i = Integer.parseInt( s );
                if ( (i < 0) || (i > 255) ) {
                    return false;
                }
            }
            if ( ip.endsWith(".") ) {
                return false;
            }

            return true;
        } catch (NumberFormatException nfe) {
            return false;
        }
    }
}
