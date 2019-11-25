package logic;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;

import java.io.File;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    public Label label_kto;

    public ImageView a1;
    public ImageView a2;
    public ImageView a3;
    public ImageView a4;
    public ImageView a5;
    public ImageView a6;
    public ImageView a7;
    public ImageView a8;

    public ImageView b1;
    public ImageView b2;
    public ImageView b3;
    public ImageView b4;
    public ImageView b5;
    public ImageView b6;
    public ImageView b7;
    public ImageView b8;

    public ImageView c1;
    public ImageView c2;
    public ImageView c3;
    public ImageView c4;
    public ImageView c5;
    public ImageView c6;
    public ImageView c7;
    public ImageView c8;

    public ImageView d1;
    public ImageView d2;
    public ImageView d3;
    public ImageView d4;
    public ImageView d5;
    public ImageView d6;
    public ImageView d7;
    public ImageView d8;

    public ImageView e1;
    public ImageView e2;
    public ImageView e3;
    public ImageView e4;
    public ImageView e5;
    public ImageView e6;
    public ImageView e7;
    public ImageView e8;

    public ImageView f1;
    public ImageView f2;
    public ImageView f3;
    public ImageView f4;
    public ImageView f5;
    public ImageView f6;
    public ImageView f7;
    public ImageView f8;

    public ImageView g1;
    public ImageView g2;
    public ImageView g3;
    public ImageView g4;
    public ImageView g5;
    public ImageView g6;
    public ImageView g7;
    public ImageView g8;

    public ImageView h1;
    public ImageView h2;
    public ImageView h3;
    public ImageView h4;
    public ImageView h5;
    public ImageView h6;
    public ImageView h7;
    public ImageView h8;

    public Pane a1pane;
    public Pane a2pane;
    public Pane a3pane;
    public Pane a4pane;
    public Pane a5pane;
    public Pane a6pane;
    public Pane a7pane;
    public Pane a8pane;

    public Pane b1pane;
    public Pane b2pane;
    public Pane b3pane;
    public Pane b4pane;
    public Pane b5pane;
    public Pane b6pane;
    public Pane b7pane;
    public Pane b8pane;

    public Pane c1pane;
    public Pane c2pane;
    public Pane c3pane;
    public Pane c4pane;
    public Pane c5pane;
    public Pane c6pane;
    public Pane c7pane;
    public Pane c8pane;

    public Pane d1pane;
    public Pane d2pane;
    public Pane d3pane;
    public Pane d4pane;
    public Pane d5pane;
    public Pane d6pane;
    public Pane d7pane;
    public Pane d8pane;

    public Pane e1pane;
    public Pane e2pane;
    public Pane e3pane;
    public Pane e4pane;
    public Pane e5pane;
    public Pane e6pane;
    public Pane e7pane;
    public Pane e8pane;

    public Pane f1pane;
    public Pane f2pane;
    public Pane f3pane;
    public Pane f4pane;
    public Pane f5pane;
    public Pane f6pane;
    public Pane f7pane;
    public Pane f8pane;

    public Pane g1pane;
    public Pane g2pane;
    public Pane g3pane;
    public Pane g4pane;
    public Pane g5pane;
    public Pane g6pane;
    public Pane g7pane;
    public Pane g8pane;

    public Pane h1pane;
    public Pane h2pane;
    public Pane h3pane;
    public Pane h4pane;
    public Pane h5pane;
    public Pane h6pane;
    public Pane h7pane;
    public Pane h8pane;

    private Game game = new Game();

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        game.setCzyBiale(true);

        String ty;
        String przeciwnik;
        if (game.czyTyToBiale) {
            ty = "bialy";
            przeciwnik = "czarny";
            label_kto.setText("białe");
            d1.setImage(new Image("file:images/hetman_"+ty+".png"));
            e1.setImage(new Image("file:images/krol_"+ty+".png"));
            d8.setImage(new Image("file:images/hetman_"+przeciwnik+".png"));
            e8.setImage(new Image("file:images/krol_"+przeciwnik+".png"));
        } else {
            ty = "czarny";
            przeciwnik = "bialy";
            label_kto.setText("czarne");
            d1.setImage(new Image("file:images/krol_"+ty+".png"));
            e1.setImage(new Image("file:images/hetman_"+ty+".png"));
            d8.setImage(new Image("file:images/krol_"+przeciwnik+".png"));
            e8.setImage(new Image("file:images/hetman_"+przeciwnik+".png"));
        }
        a1.setImage(new Image("file:images/wieza_"+ty+".png"));
        b1.setImage(new Image("file:images/skoczek_"+ty+".png"));
        c1.setImage(new Image("file:images/goniec_"+ty+".png"));
        f1.setImage(new Image("file:images/goniec_"+ty+".png"));
        g1.setImage(new Image("file:images/skoczek_"+ty+".png"));
        h1.setImage(new Image("file:images/wieza_"+ty+".png"));
        a2.setImage(new Image("file:images/pion_"+ty+".png"));
        b2.setImage(new Image("file:images/pion_"+ty+".png"));
        c2.setImage(new Image("file:images/pion_"+ty+".png"));
        d2.setImage(new Image("file:images/pion_"+ty+".png"));
        e2.setImage(new Image("file:images/pion_"+ty+".png"));
        f2.setImage(new Image("file:images/pion_"+ty+".png"));
        g2.setImage(new Image("file:images/pion_"+ty+".png"));
        h2.setImage(new Image("file:images/pion_"+ty+".png"));

        a8.setImage(new Image("file:images/wieza_"+przeciwnik+".png"));
        b8.setImage(new Image("file:images/skoczek_"+przeciwnik+".png"));
        c8.setImage(new Image("file:images/goniec_"+przeciwnik+".png"));
        f8.setImage(new Image("file:images/goniec_"+przeciwnik+".png"));
        g8.setImage(new Image("file:images/skoczek_"+przeciwnik+".png"));
        h8.setImage(new Image("file:images/wieza_"+przeciwnik+".png"));
        a7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        b7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        c7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        d7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        e7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        f7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        g7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
        h7.setImage(new Image("file:images/pion_"+przeciwnik+".png"));
    }

    public void  moveTower() {
        a1.setImage(null);
        a2.setImage(new Image("file:images/wieza_bialy.png"));
    }

    public void clickOn(String what) {
        int wyn = game.onClick(what);
        switch (wyn) {
            case -1 : {
                System.out.println("Brak możliwych ruchów dla wybranego piona");
                break;
            }
            case 2 : {
                System.out.println("Wybrano piona z możliwymi ruchami");
                ArrayList<String> mozliwe_ruchy = game.getMozliweRuchy();
                pokolorujNaNiebiesko(mozliwe_ruchy);
                break;
            }
        }
    }

    public void pokolorujNaNiebiesko(ArrayList<String> mozliwe_ruchy) {
        for (String pole : mozliwe_ruchy) {
            String kolor;
            char a = pole.charAt(0);
            char b = pole.charAt(1);
            if (a == 'A' || a == 'C' || a == 'E' || a == 'G') {
                if (b == '2' || b == '4' || b == '6' || b == '8') kolor = "#5199FF";
                else kolor = "#1771F1";
            } else {
                if (b == '2' || b == '4' || b == '6' || b == '8') kolor = "#1771F1";
                else kolor = "#5199FF";
            }

            switch (pole) {
                case "A1": {
                    a1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B1": {
                    b1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C1": {
                    c1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D1": {
                    d1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E1": {
                    e1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F1": {
                    f1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G1": {
                    g1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H1": {
                    h1pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A2": {
                    a2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B2": {
                    b2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C2": {
                    c2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D2": {
                    d2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E2": {
                    e2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F2": {
                    f2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G2": {
                    g2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H2": {
                    h2pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A3": {
                    a3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B3": {
                    b3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C3": {
                    c3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D3": {
                    d3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E3": {
                    e3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F3": {
                    f3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G3": {
                    g3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H3": {
                    h3pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A4": {
                    a4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B4": {
                    b4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C4": {
                    c4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D4": {
                    d4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E4": {
                    e4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F4": {
                    f4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G4": {
                    g4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H4": {
                    h4pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A5": {
                    a5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B5": {
                    b5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C5": {
                    c5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D5": {
                    d5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E5": {
                    e5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F5": {
                    f5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G5": {
                    g5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H5": {
                    h5pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A6": {
                    a6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B6": {
                    b6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C6": {
                    c6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D6": {
                    d6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E6": {
                    e6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F6": {
                    f6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G6": {
                    g6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H6": {
                    h6pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A7": {
                    a7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B7": {
                    b7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C7": {
                    c7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D7": {
                    d7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E7": {
                    e7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F7": {
                    f7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G7": {
                    g7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H7": {
                    h7pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }

                case "A8": {
                    a8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "B8": {
                    b8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "C8": {
                    c8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "D8": {
                    d8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "E8": {
                    e8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "F8": {
                    f8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "G8": {
                    g8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
                case "H8": {
                    h8pane.setStyle("-fx-background-color: " + kolor);
                    break;
                }
            }
            }
        }


//            SquareLayout sl = findViewById(getResources().getIdentifier(pole, "id", getPackageName()));
//
//            String kolor;
//            char a = pole.charAt(0);
//            char b = pole.charAt(1);
//            if (a == 'A' || a == 'C' || a == 'E' || a == 'G') {
//                if (b == '2' || b == '4' || b == '6' || b == '8') kolor = "blueOnWhiteSquare";
//                else kolor = "blueOnBlackSquare";
//            } else {
//                if (b == '2' || b == '4' || b == '6' || b == '8') kolor = "blueOnBlackSquare";
//                else kolor = "blueOnWhiteSquare";
//            }
//            int kolor_id = getResources().getIdentifier(kolor, "color", getPackageName());
//            sl.setBackgroundColor(getResources().getColor(kolor_id));


 //   }

    public void clickA1(MouseEvent mouseEvent) {
        clickOn("A1");
    }
    public void clickB1(MouseEvent mouseEvent) {
        clickOn("B1");
    }
    public void clickC1(MouseEvent mouseEvent) {
        clickOn("C1");
    }
    public void clickD1(MouseEvent mouseEvent) {
        clickOn("D1");
    }
    public void clickE1(MouseEvent mouseEvent) {
        clickOn("E1");
    }
    public void clickF1(MouseEvent mouseEvent) {
        clickOn("F1");
    }
    public void clickG1(MouseEvent mouseEvent) {
        clickOn("G1");
    }
    public void clickH1(MouseEvent mouseEvent) {
        clickOn("H1");
    }

    public void clickA2(MouseEvent mouseEvent) {
        clickOn("A2");
    }
    public void clickB2(MouseEvent mouseEvent) {
        clickOn("B2");
    }
    public void clickC2(MouseEvent mouseEvent) {
        clickOn("C2");
    }
    public void clickD2(MouseEvent mouseEvent) {
        clickOn("D2");
    }
    public void clickE2(MouseEvent mouseEvent) {
        clickOn("E2");
    }
    public void clickF2(MouseEvent mouseEvent) {
        clickOn("F2");
    }
    public void clickG2(MouseEvent mouseEvent) {
        clickOn("G2");
    }
    public void clickH2(MouseEvent mouseEvent) {
        clickOn("H2");
    }

    public void clickA3(MouseEvent mouseEvent) {
        clickOn("A3");
    }
    public void clickB3(MouseEvent mouseEvent) {
        clickOn("B3");
    }
    public void clickC3(MouseEvent mouseEvent) {
        clickOn("C3");
    }
    public void clickD3(MouseEvent mouseEvent) {
        clickOn("D3");
    }
    public void clickE3(MouseEvent mouseEvent) {
        clickOn("E3");
    }
    public void clickF3(MouseEvent mouseEvent) {
        clickOn("F3");
    }
    public void clickG3(MouseEvent mouseEvent) {
        clickOn("G3");
    }
    public void clickH3(MouseEvent mouseEvent) {
        clickOn("H3");
    }

    public void clickA4(MouseEvent mouseEvent) {
        clickOn("A4");
    }
    public void clickB4(MouseEvent mouseEvent) {
        clickOn("B4");
    }
    public void clickC4(MouseEvent mouseEvent) {
        clickOn("C4");
    }
    public void clickD4(MouseEvent mouseEvent) {
        clickOn("D4");
    }
    public void clickE4(MouseEvent mouseEvent) {
        clickOn("E4");
    }
    public void clickF4(MouseEvent mouseEvent) {
        clickOn("F4");
    }
    public void clickG4(MouseEvent mouseEvent) {
        clickOn("G4");
    }
    public void clickH4(MouseEvent mouseEvent) {
        clickOn("H4");
    }

    public void clickA5(MouseEvent mouseEvent) {
        clickOn("A5");
    }
    public void clickB5(MouseEvent mouseEvent) {
        clickOn("B5");
    }
    public void clickC5(MouseEvent mouseEvent) {
        clickOn("C5");
    }
    public void clickD5(MouseEvent mouseEvent) {
        clickOn("D5");
    }
    public void clickE5(MouseEvent mouseEvent) {
        clickOn("E5");
    }
    public void clickF5(MouseEvent mouseEvent) {
        clickOn("F5");
    }
    public void clickG5(MouseEvent mouseEvent) {
        clickOn("G5");
    }
    public void clickH5(MouseEvent mouseEvent) {
        clickOn("H5");
    }

    public void clickA6(MouseEvent mouseEvent) {
        clickOn("A6");
    }
    public void clickB6(MouseEvent mouseEvent) {
        clickOn("B6");
    }
    public void clickC6(MouseEvent mouseEvent) {
        clickOn("C6");
    }
    public void clickD6(MouseEvent mouseEvent) {
        clickOn("D6");
    }
    public void clickE6(MouseEvent mouseEvent) {
        clickOn("E6");
    }
    public void clickF6(MouseEvent mouseEvent) {
        clickOn("F6");
    }
    public void clickG6(MouseEvent mouseEvent) {
        clickOn("G6");
    }
    public void clickH6(MouseEvent mouseEvent) {
        clickOn("H6");
    }

    public void clickA7(MouseEvent mouseEvent) {
        clickOn("A7");
    }
    public void clickB7(MouseEvent mouseEvent) {
        clickOn("B7");
    }
    public void clickC7(MouseEvent mouseEvent) {
        clickOn("C7");
    }
    public void clickD7(MouseEvent mouseEvent) {
        clickOn("D7");
    }
    public void clickE7(MouseEvent mouseEvent) {
        clickOn("E7");
    }
    public void clickF7(MouseEvent mouseEvent) {
        clickOn("F7");
    }
    public void clickG7(MouseEvent mouseEvent) {
        clickOn("G7");
    }
    public void clickH7(MouseEvent mouseEvent) {
        clickOn("H7");
    }

    public void clickA8(MouseEvent mouseEvent) {
        clickOn("A8");
    }
    public void clickB8(MouseEvent mouseEvent) {
        clickOn("B8");
    }
    public void clickC8(MouseEvent mouseEvent) {
        clickOn("C8");
    }
    public void clickD8(MouseEvent mouseEvent) {
        clickOn("D8");
    }
    public void clickE8(MouseEvent mouseEvent) {
        clickOn("E8");
    }
    public void clickF8(MouseEvent mouseEvent) {
        clickOn("F8");
    }
    public void clickG8(MouseEvent mouseEvent) {
        clickOn("G8");
    }
    public void clickH8(MouseEvent mouseEvent) {
        clickOn("H8");
    }
}
