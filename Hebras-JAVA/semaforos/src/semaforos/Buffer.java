/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package semaforos;

import java.util.concurrent.Semaphore;

public class Buffer {
//baderas de colores

    private final String red = "\033[31m";
    private final String green = "\033[32m";
    private final String yellow = "\033[33m";
    private final String blue = "\033[34m";
    private final String purple = "\033[35m";
    private final String cyan = "\033[36m";
    private final String reset = "\u001B[0m";
    private int salida;
    private int entrada;
    private int dimension;
    private int[] buffer;
    private int numeroProducidas;
    private Semaphore mutex;
    private Semaphore vacio;
    private Semaphore lleno;

    public Buffer(int dimension) {
        this.dimension = dimension;
        buffer = new int[dimension];
        for (int i = 0; i < dimension; i++) {// inicializamos el array a 0
            buffer[i] = 0;
        }
        // inicializacion semaforos
        mutex = new Semaphore(1);
        vacio = new Semaphore(dimension);
        lleno = new Semaphore(0);
        salida = 0;
        entrada = 0;
    }

    public int getSalida() {
        return salida;
    }

    public void setSalida(int salida) {
        this.salida = salida;
    }

    public int getEntrada() {
        return entrada;
    }

    public void setEntrada(int entrada) {
        this.entrada = entrada;
    }

    public int getDimension() {
        return dimension;
    }

    public void setDimension(int dimension) {
        this.dimension = dimension;
    }

    public int[] getBuffer() {
        return buffer;
    }

    public void setBuffer(int[] buffer) {
        this.buffer = buffer;
    }

    public int getNumeroProducidas() {
        return numeroProducidas;
    }

    public void setNumeroProducidas(int numeroProducidas) {
        this.numeroProducidas = numeroProducidas;
    }

    public Semaphore getMutex() {
        return mutex;
    }

    public void setMutex(Semaphore mutex) {
        this.mutex = mutex;
    }

    public Semaphore getVacio() {
        return vacio;
    }

    public void setVacio(Semaphore vacio) {
        this.vacio = vacio;
    }

    public Semaphore getLleno() {
        return lleno;
    }

    public void setLleno(Semaphore lleno) {
        this.lleno = lleno;
    }

    public String getRed() {
        return red;
    }

    public String getGreen() {
        return green;
    }

    public String getYellow() {
        return yellow;
    }

    public String getBlue() {
        return blue;
    }

    public String getPurple() {
        return purple;
    }

    public String getCyan() {
        return cyan;
    }

    public String getReset() {
        return reset;
    }

    public void imprimirBuffer(int posicion, boolean producido) {

        for (int i = 0; i < dimension; i++) {
            if(posicion == i && producido){ // si producido es true, producimos
            System.out.print(this.getGreen()+buffer[i]+this.getReset());    
            }
            else if (posicion == i && !producido){ // si producido es false, hemos consumido
                System.out.print(this.getRed()+buffer[i]+this.getReset());
            }
            else {
                System.out.print(buffer[i]);
            }
        }
        System.out.println("");
    }

    public void Producir(int i, String name) throws InterruptedException {
        boolean producido = false;
        vacio.acquire(); // bloqueamos el semaforo vacio
        mutex.acquire(); // bloqueamos el semaforo mutex para que no entren las dos hebras a la vez y asi garantizar la exclusion mutua

        buffer[entrada] = 1;
        numeroProducidas++;
        System.out.println("....................productora " + name + " produce en" + entrada + "..........................");
        imprimirBuffer(entrada, true);
        this.setEntrada((entrada + 1) % dimension);// formula buffer circular

        mutex.release();//mandamos una señal a mutex, ya que hemos terminado en la seccion crítica
        lleno.release();// si ha producido, significa que esta vacio, con lo cual mandamos una señal a lleno

    }

    public void Consumir(int i, String name) throws InterruptedException {

        boolean consumido = false;
        lleno.acquire();// bloqueamos el semaforo vacio
        mutex.acquire();// bloqueamos el semaforo mutex para que no entren las dos hebras a la vez y asi garantizar la exclusion mutua

        buffer[salida] = 0;
        numeroProducidas--;

        System.out.println("....................consumidora " + name + " consume en" + salida + "..........................");
        imprimirBuffer(salida, false);
        this.setSalida((salida + 1) % dimension);// formula buffer circular

        mutex.release();//mandamos una señal a mutex, ya que hemos terminado en la seccion crítica
        vacio.release();// si ha consumido, significa que esta vacio, con lo cual mandamos una señal a lleno

    }

}
