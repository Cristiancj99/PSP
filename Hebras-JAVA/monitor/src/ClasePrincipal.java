

import java.util.ArrayList;

/**
 *
 * @author crist
 */
public class ClasePrincipal {

    public static void main(String[] args) throws InterruptedException {
        int tamanioBuffer = Integer.parseInt(args[0]); 
        int numeroProductoras = Integer.parseInt(args[1]);
        int productorasFinitas = Integer.parseInt(args[2]);
        int numeroConsumidoras = Integer.parseInt(args[3]);
        int ConsumidorasFinitas = Integer.parseInt(args[4]);

        
        Buffer bufferCompartido = new Buffer(tamanioBuffer); // creamos un buffer del tamaño introducido por el usuario

        ArrayList<ProductorHebra> productoras = new ArrayList<ProductorHebra>();//Arraylist de hebras que utilizaremos para 
        for (int i = 0; i < numeroProductoras; i++) { // inicializamos la hebra y la introducimos en el array

            ProductorHebra obj = new ProductorHebra("productora", bufferCompartido, productorasFinitas);
            productoras.add(obj);
            productoras.get(i).thr.start(); // lanzaremos las hebras
        }
        ArrayList<ConsumidorHebra> consumidoras = new ArrayList<ConsumidorHebra>();
        for (int i = 0; i < numeroConsumidoras; i++) {// inicializamos la hebra y la introducimos en el array
            ConsumidorHebra obj = new ConsumidorHebra("consumidora", bufferCompartido, ConsumidorasFinitas);
            consumidoras.add(obj);
            consumidoras.get(i).thr.start();
        }

    }

}
