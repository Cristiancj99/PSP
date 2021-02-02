#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#define NORMAL "\033[1;0m"
#define CYAN "\033[1;36m"
#define DEBUG "\033[1;31m"
#define YELLOW "\033[1;33m"
#define SOLUCION "\033[1;32m"
using namespace std;

void senal_abuelo(int total){
	cout<<"La señal del proceso abuelo "<<total<<endl;

}
void senal_padre(int total){
	
	cout<<"La señal del proceso hijo "<<endl;

}
void senal_nieto(int total){
	
	cout<<"La señal del proceso nieto "<<endl;

}


int* resize(int &dim_nueva, int vector[], int util){

	dim_nueva=util+5;
	int *aux=new int [dim_nueva];	


	if (util< dim_nueva){

		for (int i=0;i<util;i++){

			aux[i]=vector[i];
		}
	}

	
	delete []vector;
	return aux;	

	
}
int sumatoria(int *array, int util){
	
	int sumatoria=0;

	for (int i=0;i<util;i++){

		sumatoria+=array[i];
	}
	return sumatoria;
}
void limpiarVector(int *array, int &util){
	
	int sumatoria=0;

	for (int i=0;i<util;i++){

		array[i]=0;
	}
	util=0;

}

int* comprobacion(int *array, int &util){
	
	bool valido=true;
	int sumatoria2=0;
	cout<<YELLOW<<" COMPROBANDO VALIDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!"<<NORMAL<<endl;
	for (int i=0;((i<util) && (valido));i++){

		if (array[i] <0){
			valido=false;
			cout<<DEBUG<<"NO ES VALIDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!"<<NORMAL<<endl;
		}
	}

	if(valido){
		cout<<SOLUCION<<" ES VALIDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!!!!!!!!"<<NORMAL<<endl;
		sumatoria2= sumatoria(array, util);
		limpiarVector(array, util);
		array[util]=sumatoria2;
		util++;
	}
	return array;
}


void imprimirSenal(int *array, int util){
	cout<<SOLUCION<<util<<endl;
	for (int i=0;i<util;i++){	

		cout<<DEBUG<< array[i]<<" ";
		
	}
	cout<<endl;
}

void imprimirHijo(int *array, int util){
	
	if(util == 1){

		cout<< SOLUCION<< "senial correcta: "<<array[util-1]<<NORMAL<<endl;
	}
	else{
		for (int i=0;i<util;i++){	

			cout<<DEBUG<< array[i]<<" "<<NORMAL;
		
		}
		cout<<endl;
	}
}
int main(){
	pid_t pidPadre, pidNieto, pidAbuelo;

	int fd1[2]; //abuelo-padre
	int fd2[2]; //padre-hijo

	pipe(fd1);
	pipe(fd2);

	pidPadre=fork();//creamos desde el proceso abuelo al padre

	if (pidPadre ==-1){

		cout<<"no se ha podido crear el proceso"<<endl;
		exit(-1);
	}

	else if(pidPadre==0){//padre
		pidNieto=fork();

		switch (pidNieto){

			case -1:
				cout<<"no se ha podido crear el proceso"<<endl;
				exit(-1);
				break;

			case 0://nieto

				signal( SIGUSR1, senal_nieto);
				while(true){
					sleep(1);
					int v;

					close (fd2[1]);

					read(fd2[0], &v, sizeof(int)); //leo el pipe 	
					int senales_nieto[v];
					read(fd2[0], senales_nieto, sizeof(int)*v); //leo el pipe 
					imprimirHijo(senales_nieto, v);
					kill(pidAbuelo, SIGUSR1);			
					wait();
				 

				}



				break;

			default: 
				
				
				signal( SIGUSR1, senal_padre);

				while(true){
					sleep(1);


					
					bool valido;
					int util;
					int sumatoria1;
					read(fd1[0], &util, sizeof(int)); //leo el pipe
					int senales_padre[util];

					read(fd1[0], senales_padre, sizeof(int)*util);
 				 

					close (fd2[0]);

					comprobacion(senales_padre, util);

					write(fd2[1], &util, sizeof(int));
					write(fd2[1], senales_padre, sizeof(int)*util);


					kill(pidNieto, SIGUSR1);
					wait();

				}


				break;
		}
	}
	else if(pidPadre>0){//abuelo2
		
		int util=0;	
		int dimension=2;
		int *senales=new int[dimension];
		

		pidAbuelo=getpid();
		signal( SIGUSR1, senal_abuelo);
		while(true){
			sleep(1);

			close (fd1[0]);
			int aux=0;
			bool salir=false;
			cout<<YELLOW<<"Por favor, introduzca las señales"<<endl;
			util=0;
			for(int i=0;!salir;i++){
				
				cin>>aux;
				if (aux==-1){
					salir=true;
				}
				else{
					if(util>dimension){

						senales=resize(dimension, senales, util);
						senales[util]=aux;
						util++;
						cout<<SOLUCION<<senales[util-1]<<endl;	
					}
					else{
						
						senales[util]=aux;			
						util++;	
						cout<<SOLUCION<<senales[util-1]<<endl;	
					}				
				}			
	
			}
			write(fd1[1], &util, sizeof(int));			
			write(fd1[1], senales, sizeof(int)*util);
			kill(pidPadre, SIGUSR1);
			wait();
		}




	}
}
