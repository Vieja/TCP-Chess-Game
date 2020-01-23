package logic;

import bierki.Bierka;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.Pane;

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    public ImageView a1, a2, a3, a4, a5, a6, a7, a8,
            b1, b2, b3, b4, b5, b6, b7, b8,
            c1, c2, c3, c4, c5, c6, c7, c8,
            d1, d2, d3, d4, d5, d6, d7, d8,
            e1, e2, e3, e4, e5, e6, e7, e8,
            f1, f2, f3, f4, f5, f6, f7, f8,
            g1, g2, g3, g4, g5, g6, g7, g8,
            h1, h2, h3, h4, h5, h6, h7, h8;

    public Pane a1pane, a2pane, a3pane, a4pane, a5pane, a6pane, a7pane, a8pane,
            b1pane, b2pane, b3pane, b4pane, b5pane, b6pane, b7pane, b8pane,
            c1pane, c2pane, c3pane, c4pane, c5pane, c6pane, c7pane, c8pane,
            d1pane, d2pane, d3pane, d4pane, d5pane, d6pane, d7pane, d8pane,
            e1pane, e2pane, e3pane, e4pane, e5pane, e6pane, e7pane, e8pane,
            f1pane, f2pane, f3pane, f4pane, f5pane, f6pane, f7pane, f8pane,
            g1pane, g2pane, g3pane, g4pane, g5pane, g6pane, g7pane, g8pane,
            h1pane, h2pane, h3pane, h4pane, h5pane, h6pane, h7pane, h8pane;

    public Label yourLogin;
    public Label whoNow;


    private Game game = new Game();
    private ArrayList<String> niebieskie_pola = new ArrayList<>();


    @Override
    public void initialize(URL location, ResourceBundle resources) {

        EnemyAction enemy = new EnemyAction(game);
        Thread enemyThread = new Thread(enemy);
        enemyThread.start();

        game.Przeciwnik_wykonal_ruch.addListener((observable, oldValue, newValue) -> {
            // Only if completed
            if (newValue) {
                przesunBierkePrzeciwnika();
                game.Przeciwnik_wykonal_ruch.set(false);
            }
        });


        game.setCzyBiale(false);
        game.poraNaWroga = true;
        game.odwrocGlupiaSzachownice();
        String ty;
        String przeciwnik;
        if (game.czyTyToBiale) {
            ty = "bialy";
            przeciwnik = "czarny";
            //label_kto.setText("białe");
            d1.setImage(new Image("file:images/hetman_"+ty+".png"));
            e1.setImage(new Image("file:images/krol_"+ty+".png"));
            d8.setImage(new Image("file:images/hetman_"+przeciwnik+".png"));
            e8.setImage(new Image("file:images/krol_"+przeciwnik+".png"));
        } else {
            ty = "czarny";
            przeciwnik = "bialy";
            //label_kto.setText("czarne");
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

    private void przesunBierkePrzeciwnika() {
        String start = game.Przeciwnik_START;
        String koniec = game.Przeciwnik_KONIEC;
        if (!game.czyTyToBiale) {
            start = odwrocWhere(start);
            koniec = odwrocWhere(koniec);
        }
        czyscBierke(start);
        movePiece(game.Przeciwnik_BIERKA, koniec);
    }

    public void  movePiece(Bierka bierka, String where) {
        String kolor, nazwa;
        if (bierka.isKolorCzarny()) kolor = "_czarny";
        else kolor = "_bialy";
        nazwa = bierka.getNazwaBierki();
        String calosc = "file:images/"+nazwa+kolor+".png";
        rysujBierke(where, calosc);
    }

    public void clickOn(String what) {
        if(!game.czyTyToBiale) what = odwrocWhere(what);
        System.out.println(what);
        if(!game.poraNaWroga) {
            int wyn = game.onClick(what);
            switch (wyn) {
                case 0: //WYBRANO BIERKĘ, ALE NIE MA ON MOZLIWYCH RUCHÓW
                    System.out.println("Brak możliwych ruchów dla wybranego piona");
                    odkolorujWszystko();
                    break;
                case 1: // bierka wykonuje ruch
                    odkolorujWszystko();
                    if (!game.czyTyToBiale) {
                        czyscBierke(odwrocWhere(game.wybrana_bierka.getPolozenieJakoString()));
                        what = odwrocWhere(what);
                    } else czyscBierke(game.wybrana_bierka.getPolozenieJakoString());
                    movePiece(game.wybrana_bierka, what);
                    if(!game.czyTyToBiale) what = odwrocWhere(what);
                    game.zaktualizujPolozenieBierki(what);
                    game.czyscMozliweRuchy();
                    game.poraNaWroga = true;
                    break;
                case 2: //WYBRANO PRAWIDLOWO BIERKE
                    System.out.println("Wybrano piona z możliwymi ruchami");
                    odkolorujWszystko();
                    pokolorujNaNiebiesko();
                    break;

                case 3: //ODKLIKNIECIE (WYBRANO POLE NIEBĘDĄCE NA LIŚCIE MOZLIWYCH RUCHÓW)
                    odkolorujWszystko();
                    game.czyscMozliweRuchy();
                    game.resetujRoszade();
                    game.etap_wybierania_piona = true;
                    break;

                case 4: //WYBRANO PRAWIDLOWO BIERKE PO INNEJ
                    System.out.println("Wybrano piona z możliwymi ruchami (po innej)");
                    odkolorujWszystko();
                    pokolorujNaNiebiesko();
                    break;
            }
        }
    }

    private String odwrocWhere(String where) {
        //System.out.println("przed: "+where);
        int fir = 9 - (where.charAt(0) - 64) + 64;
        int sec = 9 - (where.charAt(1) - 48);
        where = (char) fir + Integer.toString(sec);
        //System.out.println("po: "+where);
        return where;
    }

    //usun niebieskie pola
    public void odkolorujWszystko() {
        kolorujPola(niebieskie_pola, "#f0dbc0", "#6a4c4c");
        niebieskie_pola.clear();
    }

    // pokoloruj mozliwe ruchy na niebiesko
    public void pokolorujNaNiebiesko() {
        if (!game.czyTyToBiale) {
            for (String ruch : game.getMozliweRuchy()) {
                ruch = odwrocWhere(ruch);
                niebieskie_pola.add(ruch);
            }
        } else {
            niebieskie_pola.addAll(game.getMozliweRuchy());
        }

        kolorujPola(niebieskie_pola, "#5199FF", "#1771F1");
    }

    public void rysujBierke(String pole, String calosc) {
        switch (pole) {
            case "A1": {
                a1.setImage(new Image(calosc));
                break;
            }
            case "B1": {
                b1.setImage(new Image(calosc));
                break;
            }
            case "C1": {
                c1.setImage(new Image(calosc));
                break;
            }
            case "D1": {
                d1.setImage(new Image(calosc));
                break;
            }
            case "E1": {
                e1.setImage(new Image(calosc));
                break;
            }
            case "F1": {
                f1.setImage(new Image(calosc));
                break;
            }
            case "G1": {
                g1.setImage(new Image(calosc));
                break;
            }
            case "H1": {
                h1.setImage(new Image(calosc));
                break;
            }

            case "A2": {
                a2.setImage(new Image(calosc));
                break;
            }
            case "B2": {
                b2.setImage(new Image(calosc));
                break;
            }
            case "C2": {
                c2.setImage(new Image(calosc));
                break;
            }
            case "D2": {
                d2.setImage(new Image(calosc));
                break;
            }
            case "E2": {
                e2.setImage(new Image(calosc));
                break;
            }
            case "F2": {
                f2.setImage(new Image(calosc));
                break;
            }
            case "G2": {
                g2.setImage(new Image(calosc));
                break;
            }
            case "H2": {
                h2.setImage(new Image(calosc));
                break;
            }

            case "A3": {
                a3.setImage(new Image(calosc));
                break;
            }
            case "B3": {
                b3.setImage(new Image(calosc));
                break;
            }
            case "C3": {
                c3.setImage(new Image(calosc));
                break;
            }
            case "D3": {
                d3.setImage(new Image(calosc));
                break;
            }
            case "E3": {
                e3.setImage(new Image(calosc));
                break;
            }
            case "F3": {
                f3.setImage(new Image(calosc));
                break;
            }
            case "G3": {
                g3.setImage(new Image(calosc));
                break;
            }
            case "H3": {
                h3.setImage(new Image(calosc));
                break;
            }

            case "A4": {
                a4.setImage(new Image(calosc));
                break;
            }
            case "B4": {
                b4.setImage(new Image(calosc));
                break;
            }
            case "C4": {
                c4.setImage(new Image(calosc));
                break;
            }
            case "D4": {
                d4.setImage(new Image(calosc));
                break;
            }
            case "E4": {
                e4.setImage(new Image(calosc));
                break;
            }
            case "F4": {
                f4.setImage(new Image(calosc));
                break;
            }
            case "G4": {
                g4.setImage(new Image(calosc));
                break;
            }
            case "H4": {
                h4.setImage(new Image(calosc));
                break;
            }

            case "A5": {
                a5.setImage(new Image(calosc));
                break;
            }
            case "B5": {
                b5.setImage(new Image(calosc));
                break;
            }
            case "C5": {
                c5.setImage(new Image(calosc));
                break;
            }
            case "D5": {
                d5.setImage(new Image(calosc));
                break;
            }
            case "E5": {
                e5.setImage(new Image(calosc));
                break;
            }
            case "F5": {
                f5.setImage(new Image(calosc));
                break;
            }
            case "G5": {
                g5.setImage(new Image(calosc));
                break;
            }
            case "H5": {
                h5.setImage(new Image(calosc));
                break;
            }

            case "A6": {
                a6.setImage(new Image(calosc));
                break;
            }
            case "B6": {
                b6.setImage(new Image(calosc));
                break;
            }
            case "C6": {
                c6.setImage(new Image(calosc));
                break;
            }
            case "D6": {
                d6.setImage(new Image(calosc));
                break;
            }
            case "E6": {
                e6.setImage(new Image(calosc));
                break;
            }
            case "F6": {
                f6.setImage(new Image(calosc));
                break;
            }
            case "G6": {
                g6.setImage(new Image(calosc));
                break;
            }
            case "H6": {
                h6.setImage(new Image(calosc));
                break;
            }

            case "A7": {
                a7.setImage(new Image(calosc));
                break;
            }
            case "B7": {
                b7.setImage(new Image(calosc));
                break;
            }
            case "C7": {
                c7.setImage(new Image(calosc));
                break;
            }
            case "D7": {
                d7.setImage(new Image(calosc));
                break;
            }
            case "E7": {
                e7.setImage(new Image(calosc));
                break;
            }
            case "F7": {
                f7.setImage(new Image(calosc));
                break;
            }
            case "G7": {
                g7.setImage(new Image(calosc));
                break;
            }
            case "H7": {
                h7.setImage(new Image(calosc));
                break;
            }

            case "A8": {
                a8.setImage(new Image(calosc));
                break;
            }
            case "B8": {
                b8.setImage(new Image(calosc));
                break;
            }
            case "C8": {
                c8.setImage(new Image(calosc));
                break;
            }
            case "D8": {
                d8.setImage(new Image(calosc));
                break;
            }
            case "E8": {
                e8.setImage(new Image(calosc));
                break;
            }
            case "F8": {
                f8.setImage(new Image(calosc));
                break;
            }
            case "G8": {
                g8.setImage(new Image(calosc));
                break;
            }
            case "H8": {
                h8.setImage(new Image(calosc));
                break;
            }
        }
    }

    public void czyscBierke(String pole) {
        switch (pole) {
            case "A1": {
                a1.setImage(null);
                break;
            }
            case "B1": {
                b1.setImage(null);
                break;
            }
            case "C1": {
                c1.setImage(null);
                break;
            }
            case "D1": {
                d1.setImage(null);
                break;
            }
            case "E1": {
                e1.setImage(null);
                break;
            }
            case "F1": {
                f1.setImage(null);
                break;
            }
            case "G1": {
                g1.setImage(null);
                break;
            }
            case "H1": {
                h1.setImage(null);
                break;
            }

            case "A2": {
                a2.setImage(null);
                break;
            }
            case "B2": {
                b2.setImage(null);
                break;
            }
            case "C2": {
                c2.setImage(null);
                break;
            }
            case "D2": {
                d2.setImage(null);
                break;
            }
            case "E2": {
                e2.setImage(null);
                break;
            }
            case "F2": {
                f2.setImage(null);
                break;
            }
            case "G2": {
                g2.setImage(null);
                break;
            }
            case "H2": {
                h2.setImage(null);
                break;
            }

            case "A3": {
                a3.setImage(null);
                break;
            }
            case "B3": {
                b3.setImage(null);
                break;
            }
            case "C3": {
                c3.setImage(null);
                break;
            }
            case "D3": {
                d3.setImage(null);
                break;
            }
            case "E3": {
                e3.setImage(null);
                break;
            }
            case "F3": {
                f3.setImage(null);
                break;
            }
            case "G3": {
                g3.setImage(null);
                break;
            }
            case "H3": {
                h3.setImage(null);
                break;
            }

            case "A4": {
                a4.setImage(null);
                break;
            }
            case "B4": {
                b4.setImage(null);
                break;
            }
            case "C4": {
                c4.setImage(null);
                break;
            }
            case "D4": {
                d4.setImage(null);
                break;
            }
            case "E4": {
                e4.setImage(null);
                break;
            }
            case "F4": {
                f4.setImage(null);
                break;
            }
            case "G4": {
                g4.setImage(null);
                break;
            }
            case "H4": {
                h4.setImage(null);
                break;
            }

            case "A5": {
                a5.setImage(null);
                break;
            }
            case "B5": {
                b5.setImage(null);
                break;
            }
            case "C5": {
                c5.setImage(null);
                break;
            }
            case "D5": {
                d5.setImage(null);
                break;
            }
            case "E5": {
                e5.setImage(null);
                break;
            }
            case "F5": {
                f5.setImage(null);
                break;
            }
            case "G5": {
                g5.setImage(null);
                break;
            }
            case "H5": {
                h5.setImage(null);
                break;
            }

            case "A6": {
                a6.setImage(null);
                break;
            }
            case "B6": {
                b6.setImage(null);
                break;
            }
            case "C6": {
                c6.setImage(null);
                break;
            }
            case "D6": {
                d6.setImage(null);
                break;
            }
            case "E6": {
                e6.setImage(null);
                break;
            }
            case "F6": {
                f6.setImage(null);
                break;
            }
            case "G6": {
                g6.setImage(null);
                break;
            }
            case "H6": {
                h6.setImage(null);
                break;
            }

            case "A7": {
                a7.setImage(null);
                break;
            }
            case "B7": {
                b7.setImage(null);
                break;
            }
            case "C7": {
                c7.setImage(null);
                break;
            }
            case "D7": {
                d7.setImage(null);
                break;
            }
            case "E7": {
                e7.setImage(null);
                break;
            }
            case "F7": {
                f7.setImage(null);
                break;
            }
            case "G7": {
                g7.setImage(null);
                break;
            }
            case "H7": {
                h7.setImage(null);
                break;
            }

            case "A8": {
                a8.setImage(null);
                break;
            }
            case "B8": {
                b8.setImage(null);
                break;
            }
            case "C8": {
                c8.setImage(null);
                break;
            }
            case "D8": {
                d8.setImage(null);
                break;
            }
            case "E8": {
                e8.setImage(null);
                break;
            }
            case "F8": {
                f8.setImage(null);
                break;
            }
            case "G8": {
                g8.setImage(null);
                break;
            }
            case "H8": {
                h8.setImage(null);
                break;
            }
        }
    }

    public void kolorujPola(ArrayList<String> mozliwe_ruchy, String kolor_bialy, String kolor_czarny) {
        for (String pole : mozliwe_ruchy) {
            char a = pole.charAt(0);
            char b = pole.charAt(1);
            String kolor;
            if (a == 'A' || a == 'C' || a == 'E' || a == 'G') {
                if (b == '2' || b == '4' || b == '6' || b == '8') kolor = kolor_bialy;
                else kolor = kolor_czarny;
            } else {
                if (b == '2' || b == '4' || b == '6' || b == '8') kolor = kolor_czarny;
                else kolor = kolor_bialy;
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

    public void clickA1( ) {
        clickOn("A1");
    }
    public void clickB1( ) {
        clickOn("B1");
    }
    public void clickC1( ) {
        clickOn("C1");
    }
    public void clickD1( ) {
        clickOn("D1");
    }
    public void clickE1( ) {
        clickOn("E1");
    }
    public void clickF1( ) {
        clickOn("F1");
    }
    public void clickG1( ) {
        clickOn("G1");
    }
    public void clickH1( ) {
        clickOn("H1");
    }

    public void clickA2( ) {
        clickOn("A2");
    }
    public void clickB2( ) {
        clickOn("B2");
    }
    public void clickC2( ) {
        clickOn("C2");
    }
    public void clickD2( ) {
        clickOn("D2");
    }
    public void clickE2( ) {
        clickOn("E2");
    }
    public void clickF2( ) {
        clickOn("F2");
    }
    public void clickG2( ) {
        clickOn("G2");
    }
    public void clickH2( ) {
        clickOn("H2");
    }

    public void clickA3( ) {
        clickOn("A3");
    }
    public void clickB3( ) {
        clickOn("B3");
    }
    public void clickC3( ) {
        clickOn("C3");
    }
    public void clickD3( ) {
        clickOn("D3");
    }
    public void clickE3( ) {
        clickOn("E3");
    }
    public void clickF3( ) {
        clickOn("F3");
    }
    public void clickG3( ) {
        clickOn("G3");
    }
    public void clickH3( ) {
        clickOn("H3");
    }

    public void clickA4( ) {
        clickOn("A4");
    }
    public void clickB4( ) {
        clickOn("B4");
    }
    public void clickC4( ) {
        clickOn("C4");
    }
    public void clickD4( ) {
        clickOn("D4");
    }
    public void clickE4( ) {
        clickOn("E4");
    }
    public void clickF4( ) {
        clickOn("F4");
    }
    public void clickG4( ) {
        clickOn("G4");
    }
    public void clickH4( ) {
        clickOn("H4");
    }

    public void clickA5( ) {
        clickOn("A5");
    }
    public void clickB5( ) {
        clickOn("B5");
    }
    public void clickC5( ) {
        clickOn("C5");
    }
    public void clickD5( ) {
        clickOn("D5");
    }
    public void clickE5( ) {
        clickOn("E5");
    }
    public void clickF5( ) {
        clickOn("F5");
    }
    public void clickG5( ) {
        clickOn("G5");
    }
    public void clickH5( ) {
        clickOn("H5");
    }

    public void clickA6( ) {
        clickOn("A6");
    }
    public void clickB6( ) {
        clickOn("B6");
    }
    public void clickC6( ) {
        clickOn("C6");
    }
    public void clickD6( ) {
        clickOn("D6");
    }
    public void clickE6( ) {
        clickOn("E6");
    }
    public void clickF6( ) {
        clickOn("F6");
    }
    public void clickG6( ) {
        clickOn("G6");
    }
    public void clickH6( ) {
        clickOn("H6");
    }

    public void clickA7( ){
        clickOn("A7");
    }
    public void clickB7( ){
        clickOn("B7");
    }
    public void clickC7( ) {
        clickOn("C7");
    }
    public void clickD7( ) {
        clickOn("D7");
    }
    public void clickE7( ) {
        clickOn("E7");
    }
    public void clickF7( ) {
        clickOn("F7");
    }
    public void clickG7( ) {
        clickOn("G7");
    }
    public void clickH7( ) {
        clickOn("H7");
    }

    public void clickA8( ) {
        clickOn("A8");
    }
    public void clickB8( ) {
        clickOn("B8");
    }
    public void clickC8( ) {
        clickOn("C8");
    }
    public void clickD8( ) {
        clickOn("D8");
    }
    public void clickE8( ) {
        clickOn("E8");
    }
    public void clickF8( ) {
        clickOn("F8");
    }
    public void clickG8( ) {
        clickOn("G8");
    }
    public void clickH8( ) {
        clickOn("H8");
    }

    public void executeSurrender( ) {
    }
}
