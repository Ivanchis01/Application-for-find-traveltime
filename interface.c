#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

#define SHM_KEY 0X1234

struct datos{
    int origen;
    int destino;
    int hora;
    float tiempo_medio;
}; //estructura para guardar los datos

int main() {
    int shmid, opcion, r; 
    struct datos *datosh;

    //Creamos el espacio de memoria compartida y retornamos el valor en shmid
    shmid = shmget(SHM_KEY, sizeof(struct datos), 0644|IPC_CREAT); 
    if(shmid == -1){
        perror("Error al crear memoria compartida (shmget)");
        exit(-1);
    }

    //Asociamos la memoria compartida al proceso
    datosh = shmat(shmid, NULL, 0);
    if(datosh == (void*) -1){
        perror("Error al asociar la memoria compartida (shmat)");
        exit(-1);
    }


    printf("\nBienvenido\n");
    do {
        printf("1. Ingresar origen\n");
        printf("2. Ingresar destino\n");
        printf("3. Ingresar hora\n");
        printf("4. Buscar tiempo de viaje medio\n");
        printf("5. Salir\n");
		
		printf("Ingrese una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
				printf("Ingrese ID Origen (1 - 1160): ");
                scanf("%d", &(datosh->origen));
                break;
            case 2:
				printf("Ingrese ID Destino (1 - 1160): ");
                scanf("%d", &(datosh->destino));
                break;
            case 3:
				printf("Ingrese Hora (0 - 23): ");
                scanf("%d", &(datosh->hora));
                break;
            case 4:
                system("./funcion");
                sleep(1.5);
				if(datosh->tiempo_medio >= 0){
					printf("El tiempo de viaje medio entre %d y %d a las %d horas es de %.1f horas.\n", datosh->origen, datosh->destino, datosh->hora, datosh->tiempo_medio);
				}else{
					printf("NA\n");
				}
                break;
            case 5:
                printf("Gracias por utilizar el programa.\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while(opcion != 5);

    //Desasociamos el espacio de memoria compartida
    r = shmdt(datosh);
    if(r < 0){
        perror("Error al desasociar memoria compartida (shmdt)");
        exit(-1);
    }

    return 0;
}
