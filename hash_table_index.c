#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024
#define MAX_SIZE 997

//Nodos
typedef struct Nodo{
	int origen; 
	int destino;
	int hora;
    float tiempo_medio;
	struct Nodo* siguiente;
}Nodo; //etiqueta para marcar un punto especifico y volver si hay un error al procesar el archivo csv

//funcion hash
int funcion_hash(int partida, int destino, int hora){
    return (partida+destino+hora) % MAX_SIZE;
}
//insertar en tbla hash
void insert(struct Nodo *tabla[], int partida, int destino, int hora, float tiempo_medio){
    int key=funcion_hash(partida,destino,hora);
    struct Nodo *nodo=(struct Nodo*) malloc(sizeof(struct Nodo)); //reservamos el espacio en memoria para almacenar un nuevo nodo
    //Ponemos los atributos del nodo
    nodo->origen=partida;
    nodo->destino=destino;
    nodo->hora=hora;
    nodo->tiempo_medio=tiempo_medio;
    nodo->siguiente=NULL;
    //Ponemos el nodo en la tabla hash
    if(tabla[key]==NULL){
        tabla[key]=nodo; //si el indice de la tabla hash está vacío ponemos el nodo allí
    } else {
        struct Nodo *aux=tabla[key];
        while (aux->siguiente != NULL){ //si el indice de la tabla hash está ocupado se maneja como una lista enlazada
            aux=aux->siguiente;
        }
        aux->siguiente=nodo; //ponemos el nuevo nodo al final
    }
}

int main(){
    //abrimos el archivo y confirmamos que se abra correctamente
    int aux[MAX_SIZE]; //array auxiliar para cargar los nodos
    FILE *archivo=fopen("csv_data.csv","r");
    if(archivo==NULL){
        perror("Error al abrir el archivo");
        exit(-1);
    }

    //inicializamos la tabla hash
    Nodo *tabla[MAX_SIZE];
    for(int i=0;i<MAX_SIZE;i++){
        tabla[i]=NULL;
    }

    //procesamos el archivo csv
    char linea[MAX_LINE_LENGTH]; //procesamos por lineas
    //saltar primera linea
    fgets(linea, MAX_LINE_LENGTH, archivo);

    while(fgets(linea,MAX_LINE_LENGTH,archivo)){
        //Dividimos la linea por las comas
        int origen, destino, hora; 
        float tiempo_medio; //creamos las variables
        char *token = strtok(linea,",");//separa los atributos en el csv y nos devuelve el apuntador para el siguiente
        origen = atoi(token); //convierte la primera porcion a int o float en el caso del tiempo_medio
        token = strtok(NULL,",");
        destino=atoi(token); //int
        token = strtok(NULL,",");
        hora=atoi(token); //int
        token = strtok(NULL,",");
        tiempo_medio=atof(token); //float
        
        //Insertamos el nodo en la tabla
        insert(tabla,origen,destino,hora,tiempo_medio);
    }
    //cerramos el archivo
    fclose(archivo);

    //Guardamos la tabla en el archivo binario por linea
    FILE *archivo_binario = fopen("tabla_hash.c1","wb");
    for (int i=0;i<MAX_SIZE;i++){
        struct Nodo *head = tabla[i]; //cargamos el apuntador al nodo cabeza
        int auxIndex=0; //indice que contará el numero de nodos en cada lista
        while(head!=NULL){ //escribe los nodos con una misma key en una misma linea
            fwrite(head,sizeof(struct Nodo),1,archivo_binario); //escribe los nodos en un archivo bbinario secuencialmente
            head=head->siguiente;
            auxIndex++; //aumentamos el contador por cada nodo guardado
        }
        aux[i]=auxIndex; //guarda el numero de nodos en el indice i de el array
    }
    fclose(archivo_binario);

    FILE *array_aux = fopen("array_auxiliar.c1","wb");
    fwrite(aux,sizeof(int),MAX_SIZE,array_aux); //guardamos el array auxiliar en un archivo binario
    fclose(array_aux);

    //Liberamos la memoria de la tabla
    //for(int i=0;i<MAX_SIZE;i++){
        //Nodo *aux = tabla[i];
        //while(aux != NULL){
            //Nodo *borrar_nodo=aux;
            //aux=aux->siguiente;
       // }
    //}


	return 0;
}
