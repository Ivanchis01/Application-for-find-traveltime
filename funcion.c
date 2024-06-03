#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define SHM_KEY 0X1234
#define MAX_SIZE 997

struct Nodo{
	int origen; 
	int destino;
	int hora;
    float tiempo_medio;
	struct Nodo* siguiente;
}; //estructura nodo


struct datos{
    int origen;
    int destino;
    int hora;

    float tiempo_medio;
}; //estructura para guardar los datos (args)


//funcion hash
int funcion_hash(int partida, int destino, int hora){
    return (partida+destino+hora) % MAX_SIZE;
}

//funcion search
float search(int aux[], FILE *archivo, int origen, int destino, int hora){
    int key, j, contador,tiempo_medio;
    struct Nodo auxiliar;

    key=funcion_hash(origen,destino,hora);
    
    if(key==0){
        j=0;
        while(j<aux[0]){
            fseek(archivo,j*sizeof(struct Nodo),SEEK_SET);
            fread(&auxiliar,sizeof(struct Nodo),1,archivo);
            if(auxiliar.origen==origen&&auxiliar.destino==destino&&auxiliar.hora==hora){
                return(auxiliar.tiempo_medio);
            }
            j++;
        }
        return(-1.0);
    } else{
        j=0;
        contador=0;
        while(j<key){
            contador=contador+aux[j];
            j++;
        }
        j=0;
        while(j<aux[key]){
            fseek(archivo,(contador+j)*sizeof(struct Nodo),SEEK_SET);
            fread(&auxiliar,sizeof(struct Nodo),1,archivo);
            if(auxiliar.origen==origen&&auxiliar.destino==destino&&auxiliar.hora==hora){
                return(auxiliar.tiempo_medio);
            }
            j++;
        }
        return(-1.0);
    }
}

int main() {
    int shmid,r,linea,curr_line;
    struct datos *datosh;
    int arr_aux[MAX_SIZE];

    //Creamos el espacio de memoria compartida y retornamos el valor en shmid
    shmid=shmget(SHM_KEY,sizeof(struct datos),0644|IPC_CREAT); 
    if(shmid==-1){
        perror("Error al crear memoria compartida (shmget)");
        exit(-1);
    }

    //Asociamos la memoria compartida al proceso
    datosh=shmat(shmid,NULL,0);
    if(datosh == (void*) -1){
        perror("Error al asociar la memoria compartida (shmat)");
        exit(-1);
    }

    //Cargamos el archivo de la array auxiliar con los tamaños de las linkedlists
    FILE *archivo=fopen("array_auxiliar.c1","rb");
    if(archivo==NULL){
        perror("Error al abrir el archivo binario");
        exit(-1);
    }
    
    fread(arr_aux,sizeof(int),MAX_SIZE,archivo);
    FILE *archivo_tabla=fopen("tabla_hash.c1","rb");
    datosh->tiempo_medio=search(arr_aux,archivo_tabla,datosh->origen,datosh->destino,datosh->hora);

    fclose(archivo_tabla);
    fclose(archivo);

    //=================================================================================================
    //Desasociamos el espacio de memoria compartida
    r=shmdt(datosh);
    if(r<0){
        perror("Error al desasociar memoria compartida (shmdt)");
        exit(-1);
    }
    return 0;
}
