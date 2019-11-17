package logic;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;

import java.io.File;
import java.net.URL;
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

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        String ty;
        String przeciwnik;
        if (Game.czy_biale) {
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

    public void clickA1(MouseEvent mouseEvent) {
        int wyn = Game.onClick("A1");
        switch (wyn) {
            case 1 : {
                moveTower();
                break;
            }
        }
    }

    public void  moveTower() {
        a1.setImage(null);
        a2.setImage(new Image("file:images/wieza_bialy.png"));
    }
}
