/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package semaforos;

import java.util.logging.Level;
import java.util.logging.Logger;
import semaforos.Buffer;

class ConsumidorHebra implements Runnable // opcionalmente: extends ....
{
    static int id = 0;
    public Thread thr; // objeto hebra encapsulado
    Buffer b;
    int finita;

    public ConsumidorHebra(String nombre, Buffer bu, int finita) {
        b = bu;
        thr = new Thread(this, nombre + id);
        id++;
        this.finita=finita;

    }

    public void run() {

        boolean consumido = false;
        int posicion_hebra;
        int i = 0;

        try {
            if (finita==1){// si finita es 1, la hebra hará su trabajo y se muere
            b.Consumir(i, thr.getName());
            }
            else if (finita==0){// si es 0, la hebra no morirá
                while(true){
                    b.Consumir(i, thr.getName());
                }
            }
        } catch (InterruptedException ex) {
            Logger.getLogger(ConsumidorHebra.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

}
