/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


import java.util.logging.Level;
import java.util.logging.Logger;

class ProductorHebra implements Runnable // opcionalmente: extends ....
{

    static int id;
    public Thread thr; // objeto hebra encapsulado
    Buffer b;
    int finita;

    public ProductorHebra(String nombre, Buffer bu, int finita) {
        b = bu;
        thr = new Thread(this, nombre + id);
        id++;
        this.finita = finita;

    }

    public void run() {


        int i = 0;

        try {
            if (finita == 1) { // si finita es uno, significa que la hebra morirá una vez termina de ejecutarse
                b.Producir(i, thr.getName());
            } else if (finita == 0) { // si finita es 0, la hebra no finalizará 
                while (true) {
                    b.Producir(i, thr.getName());
                }
            }
        } catch (InterruptedException ex) {
            Logger.getLogger(ProductorHebra.class.getName()).log(Level.SEVERE, null, ex);
        }

    }
}
