
package cerrojos;

import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Buffer {
//baderas de colores
    private final String red="\033[31m"; 
    private final String green="\033[32m";
    private final String yellow="\033[33m";
    private final String blue="\033[34m"; 
    private final String purple="\033[35m"; 
    private final String cyan="\033[36m"; 
    private final String reset="\u001B[0m";
    
    
    
    int salida;
    int entrada;
    int dimension;
    int[] buffer;
    int numeroProducidas;
    
    Lock mutex;
    Condition vacio;
    Condition lleno;

    public Buffer(int dimension) {
        this.dimension = dimension;
        buffer = new int[dimension];
        for (int i = 0; i < dimension; i++) { // inicializamos el array a 0
            buffer[i] = 0;
        }
        // inicializacion cerrojo y condiciones
        mutex = new ReentrantLock();
        vacio = mutex.newCondition();
        lleno = mutex.newCondition();
        entrada=0;
        salida=0;

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

    public Lock getMutex() {
        return mutex;
    }

    public void setMutex(Lock mutex) {
        this.mutex = mutex;
    }

    public Condition getVacio() {
        return vacio;
    }

    public void setVacio(Condition vacio) {
        this.vacio = vacio;
    }

    public Condition getLleno() {
        return lleno;
    }

    public void setLleno(Condition lleno) {
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
        mutex.lock(); // cerramos el cerrojo mutex para garantizar la exclusión mutua 
        while (numeroProducidas == dimension) { // si hay el mismo numero de producidas que la dimension, tendremos el buffer lleno, 
            //con lo cual, deberemos de cerrar la condicion vacia
            System.out.println("---------Buffer lleno-------");
            vacio.await();
        }
        buffer[entrada] = 1;
        numeroProducidas++;
        System.out.println("...................." + this.getGreen()+ "productora " + name + this.getReset()+ " produce en" + entrada + "..........................");
        imprimirBuffer( entrada, true);
        this.setEntrada((entrada+1) % dimension);// formula buffer circular
        
        lleno.signal(); // si ha producido, significa que esta lleno, con lo cual mandamos una señal a lleno
        mutex.unlock(); // desbloqueamos el cerrojo mutex

    }

    public void Consumir(int i, String name) throws InterruptedException {


        mutex.lock();// cerramos el cerrojo mutex para garantizar la exclusión mutua 
        while (numeroProducidas == 0) {// si hay el mismo numero de producidas que 0, tendremos el buffer vacio,
                        //con lo cual, deberemos de cerrar la condicion lleno
            System.out.println("---------Buffer vacio-------");
            lleno.await() ;
        }

        buffer[salida] = 0;
        numeroProducidas--;

        System.out.println("...................."+ this.getRed()+ "consumidora "+ name + this.getReset()+ " consume en" + salida + "..........................");
        imprimirBuffer( salida, false);
        this.setSalida((salida+1) % dimension);// formula buffer circular

        vacio.signal();// si ha consumido, significa que esta vacio, con lo cual mandamos una señal a vacio
        mutex.unlock();// desbloqueamos el cerrojo mutex

    }

}


