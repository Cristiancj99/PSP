#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string>
#include <semaphore.h>
#include <vector>

#define RESET_COLOR    "\x1b[0m"
#define ROJO_T     "\x1b[31m"
#define VERDE_T     	"\x1b[32m"


using namespace std;	
sem_t mutex;
sem_t vacio;
sem_t lleno;
int * buffer;
int numeroProducidas;
int dim_buffer;

/**
	@brief método que sirve para imprimir el buffer con sus respectivos colores dependiendo si es consumidora o productora.
	@param int posicion. Posición que acabara de ser actualizada
	@param bool producida. Booleano que nos marcará si es producida o no
	@param bool consumida. Booleano que nos marcará si es consumid o no
	@pre. deberá de haber  un cambio previo en el buffer 
	@return nada. es un procedimiento
	@post el Buffer será imprimido con la respectiva posición de su respectivo color.
*/

void imprimirBuffer (int posicion, bool producida, bool consumida){

	for (int i = 0; i < dim_buffer; ++i)
	{
		if(i==posicion-1 && producida){
			cout<<VERDE_T<<buffer[i]<<RESET_COLOR;
		}
		else if (i==posicion-1 && consumida)
		{
			cout<<ROJO_T<<buffer[i]<<RESET_COLOR;
		}
		else{
			cout<<buffer[i];
		}
	}
	cout<<RESET_COLOR<<endl;
}
/**
	@brief método que sirve para actualizar el buffer realizando una produccion
	@param int i(por referencia) La utilizaremos parasaber en que posicion del buffer produciremos
	@pre. previamente se deberá de hacer un wait en vacio y en mutex para garantizar la exclusión mutua. También deberá de haber hueco en el buffer
	@return nada. es un procedimiento
	@post el Buffer será actualizado en caso de que haya hueco con una produccion.
*/
void actualizarBufferProducir(int &i){
	bool producido=false;
	for ( i = 0; (i < dim_buffer && !producido); i++)
	{
		if (buffer[i]==0)
		{
			buffer[i]=1;
			producido=true;
			numeroProducidas++;
		}	
	}
}

/**
	@brief método que sirve para realizar una produccion
	@param void *p puntero a void que en nuestro caso será una posicion de un vector de int donde tendremos el id de la hebra
	@pre. se deberá de asignar a una hebra
	@return devolverá un puntero a null
	@post se realizará una produccion y se podrá consumir.
*/
void* producir (void *p){
	bool producido;
	int *posicion_hebra;
	posicion_hebra = (int *) p;	int i;

	sem_wait(&vacio);
	sem_wait( &mutex);
	actualizarBufferProducir(i);
	cout<<"La hebra "<<VERDE_T<<"productora " <<RESET_COLOR<< *posicion_hebra<< " produce en posicion: " << i<<endl;
	imprimirBuffer(i, true, false);
	sem_post(&mutex);
	sem_post(&lleno);
	return 0;
}

/**
	@brief método que sirve para realizar una produccion de forma infinta
	@param void *p puntero a void que en nuestro caso será una posicion de un vector de int donde tendremos el id de la hebra
	@pre. se deberá de asignar a una hebra
	@return devolverá un puntero a null
	@post se realizará una produccion y se podrá consumir.
*/
void* producirInf (void *p){

	bool producido;
	int *posicion_hebra;
	posicion_hebra = (int *) p;	
	int i;

	while(1){
		sem_wait(&vacio);
		sem_wait( &mutex);
		actualizarBufferProducir(i);	
		cout<<"La hebra "<<VERDE_T<<"productora " <<RESET_COLOR<< *posicion_hebra<< " produce en posicion: " << i<<endl;
		imprimirBuffer(i, true, false);
		sem_post(&mutex);
		sem_post(&lleno);
	}

	return 0;


}
/**
	@brief método que sirve para actualizar el buffer realizando una consumicion
	@param int i(por referencia) La utilizaremos para saber en que posicion del buffer consumiremos
	@pre. previamente se deberá de hacer un wait en lleno y en mutex para garantizar la exclusión mutua. También deberá de haber algo producido
	@return nada. es un procedimiento
	@post el Buffer será actualizado en caso de que haya hueco con una consumicion.
*/
void actualizarBufferConsumir(int &i){
	bool consumido=false;

	for (i = 0; (i < dim_buffer && !consumido) ; i++)
	{

		if (buffer[i]==1)
		{
			buffer[i]=0;
			consumido=true;
			numeroProducidas--;
		}	
	}

}

/**
	@brief método que sirve para realizar una consumicion 
	@param void *p puntero a void que en nuestro caso será una posicion de un vector de int donde tendremos el id de la hebra
	@pre. se deberá de asignar a una hebra
	@return devolverá un puntero a null
	@post se realizará una consumicion y se podrá producir.
*/
void* consumir (void *p){
	bool consumido=false;
	int *posicion_hebra;
	posicion_hebra = (int *) p;
	int i;

	sem_wait(&lleno);
	sem_wait( &mutex);
	actualizarBufferConsumir(i);
	cout<<"La hebra "<<ROJO_T<<"consumidora " << RESET_COLOR<<*posicion_hebra<< " consume en posicion: " << i<<endl;
	imprimirBuffer(i, false, true);
	sem_post(&mutex);
	sem_post(&vacio);
	return 0;

}

/**
	@brief método que sirve para realizar consumiciones de forma infinita
	@param void *p puntero a void que en nuestro caso será una posicion de un vector de int donde tendremos el id de la hebra
	@pre. se deberá de asignar a una hebra
	@return devolverá un puntero a null
	@post se realizará una consumicion y se podrá producir.
*/
void* consumirInf (void *p){
	bool consumido=false;
	int *posicion_hebra;
	posicion_hebra = (int *) p;
	int i;
	while(1){
		sem_wait(&lleno);
		sem_wait( &mutex);
		actualizarBufferConsumir(i);		
		cout<<"La hebra "<<ROJO_T<<"consumidora " << RESET_COLOR<<*posicion_hebra<< " consume en posicion: " << i<<endl;
		imprimirBuffer(i, false, true);
		sem_post(&mutex);
		sem_post(&vacio);
	}

	return 0;

}

int main(int argc, char * argv[]){


    dim_buffer = atoi(argv[1]);
    int buffer_auxiliar[dim_buffer];
    int nProductoras = atoi(argv[2]);
    int productorasInfinitas = atoi(argv[3]);
    int nConsumidoras = atoi(argv[4]);
    int consumidorasInfinitas = atoi(argv[5]);
    string mensaje_productor="soy el productor";
    string mensaje_consumidor="voy a consumir";


    pthread_t hebras_consumidoras[nConsumidoras];
    pthread_t hebras_productoras[nProductoras];

    // inicializacion de los semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&vacio, 0, dim_buffer);
    sem_init(&lleno, 0, 0);

    // inicializaremos el buffer
    buffer=buffer_auxiliar;
    for (int i = 0; i < dim_buffer; i++)
    {
    	buffer[i]=0;
    }
//***************************************
//***************************************
    int idConsumidoras[nConsumidoras];
    for (int i = 0; i < nConsumidoras; i++) 
    {
    	idConsumidoras[i]=i;		
    }										//creamos los id que utilizaremos en nuestras hebras.
    int idProductoras[nProductoras];
    for (int i = 0; i < nProductoras; i++) 
    {
    	idProductoras[i]=i;
    }
//**************************************
//**************************************



    if (productorasInfinitas==0){

    	for (int i = 0; i < nProductoras; i++){
    		pthread_create(&hebras_productoras[i], NULL, producir,(void*)&idProductoras[i] );
    	}
    }
    else{
    	for (int i = 0; i < nProductoras; i++){
    		pthread_create(&hebras_productoras[i], NULL, producirInf,(void*)&idProductoras[i] );
   		}   	
    }



    if (consumidorasInfinitas==0){
    	for (int i = 0; i < nConsumidoras; i++){
    		pthread_create(&hebras_consumidoras[i], NULL, consumir,(void*) &idConsumidoras[i] );
    	}    
	}

	else{

    	for (int i = 0; i < nConsumidoras; i++){
    		pthread_create(&hebras_consumidoras[i], NULL, consumirInf,(void*)&idConsumidoras[i]);
    	}	
	}



	for (int i = 0; i < nProductoras; i++){
	   	pthread_join(hebras_productoras[i], NULL);
	}	


    for (int i = 0; i < nConsumidoras; i++)
    {
    	pthread_join(hebras_consumidoras[i], NULL );
    }

    // destruimos los semáforos
    sem_destroy(&mutex);
    sem_destroy(&vacio);
    sem_destroy(&lleno);

}