#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <time.h>
#include <unistd.h> // Para fork() y wait()
#include <string.h> // Agrega esta línea para incluir <string.h>

void registrarCliente(PGconn *conn) {
        char nombre[255];
    char ap_pat[255];
    char ap_mat[255];
    char rfc[13];
    char direccion[255];
    char telefono[15];

    printf("Nombre: ");
    scanf("%s", nombre);
    printf("Apellido Paterno: ");
    scanf("%s", ap_pat);
    printf("Apellido Materno: ");
    scanf("%s", ap_mat);
    printf("RFC: ");
    scanf("%s", rfc);
    printf("Dirección: ");
    scanf("%s", direccion);
    
    printf("Ingrese el tipo de teléfono\n1. casa\n2. personal\n3. oficina\n");
    int seleccion;
    do {
        printf("Elija una opción (1/2/3): ");
        scanf("%d", &seleccion);

        switch (seleccion) {
            case 1:
                strcpy(telefono, "casa");
                break;
            case 2:
                strcpy(telefono, "personal");
                break;
            case 3:
                strcpy(telefono, "oficina");
                break;
            default:
                printf("Opción no válida. Inténtelo de nuevo.\n");
                break;
        }
    } while (seleccion < 1 || seleccion > 3);  
    
    // Validar el tipo de teléfono ingresado
    if (strcmp(telefono, "casa") != 0 && strcmp(telefono, "personal") != 0 && strcmp(telefono, "oficina") != 0) {
        printf("Tipo de teléfono no válido. Debe ser 'casa', 'personal' u 'oficina'.\n");
        return;
    }

    char numero[15];
    printf("Ingrese el número de teléfono: ");
    scanf("%s", numero);

    // Construir y ejecutar una consulta SQL INSERT para agregar un cliente a la base de datos.
    char consulta[500];
    snprintf(consulta, sizeof(consulta), "INSERT INTO Cliente (nombre, ap_pat, ap_mat, rfc, direccion) VALUES ('%s', '%s', '%s', '%s', '%s')",
            nombre, ap_pat, ap_mat, rfc, direccion);

    PGresult *res = PQexec(conn, consulta);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Error al realizar el alta del cliente: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);

    // Construir y ejecutar una consulta SQL INSERT para agregar el número de teléfono a la tabla Telefono.
    snprintf(consulta, sizeof(consulta), "INSERT INTO Telefonos (id_cliente, numero_telefono, tipo_telefono) SELECT id_cliente, '%s', '%s' FROM Cliente WHERE rfc = '%s'",
             numero, telefono, rfc);         

    res = PQexec(conn, consulta);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Error al agregar el número de teléfono: %s\n", PQerrorMessage(conn));
    } else {
        printf("Cliente y número de teléfono agregados exitosamente.\n");
    }

    PQclear(res);
}

void mostrarListaHabitaciones(PGconn *conn, int seleccion) {
    char buffer[100];  // Ajusta el tamaño según tus necesidades

    snprintf(buffer, sizeof(buffer), "SELECT numero_habitacion, estado FROM Lista_Habitaciones WHERE id_habitacion=%d", seleccion);

    PGresult *res = PQexec(conn, buffer);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("Error al realizar la consulta: %s\n", PQerrorMessage(conn));
    } else {
        int filas = PQntuples(res);

        printf("\tLista de Habitaciones:\n\tDisponible = + Ocupado = -\n\n");
        for (int i = 0; i < filas; i++) {
            int numeroHabitacion = atoi(PQgetvalue(res, i, 0)); // Obtener el valor de la columna 'numero_habitacion'.
            char *estado = PQgetvalue(res, i, 1); // Obtener el valor de la columna 'estado'.

            printf("\tNúmero de Habitación: %d, Estado: %s\n", numeroHabitacion, estado);
        }
    }

    PQclear(res);
}

void hacerReservacion(PGconn *conn) {
    //char fecha[11];
    char rfcCliente[13];
    //char hora[9];
    int numeroHabitacion;
    char tipoHabitacion[50];
    char fechaLlegada[11];
    char fechaSalida[11];
    
    // Mostrar opciones de tipo de habitación y permitir al usuario elegir.
    printf("Seleccione el tipo de habitación:\n");
    printf("1. Lujo\n2. Confort\n3. Básico\n");

    int seleccion;
    do {
        printf("Elija una opción (1/2/3): ");
        scanf("%d", &seleccion);

        switch (seleccion) {
            case 1:
                strcpy(tipoHabitacion, "Lujo");
                break;
            case 2:
                strcpy(tipoHabitacion, "Confort");
                break;
            case 3:
                strcpy(tipoHabitacion, "Básico");
                break;
            default:
                printf("Opción no válida. Inténtelo de nuevo.\n");
                break;
        }
    } while (seleccion < 1 || seleccion > 3);
    mostrarListaHabitaciones(conn,seleccion);

    printf("Elige una habitacion: ");
    scanf("%d",&numeroHabitacion);
    //printf("Ingrese la fecha de la reservación (YYYY-MM-DD): ");
    //scanf("%s", Fecha);

    printf("Ingrese su RFC: ");
    scanf("%s", rfcCliente);

    //printf("Ingrese la hora de la reservación (HH:MM): ");
    //scanf("%s", hora);
 
    printf("Ingrese la fecha de llegada (YYYY-MM-DD): ");
    scanf("%s", fechaLlegada);

    printf("Ingrese la fecha de salida (YYYY-MM-DD): ");
    scanf("%s", fechaSalida);

    //Aqui se obtiene la fecha y hora del sistema para que el usuario no lo haga
    time_t t;
    struct tm *info;
    char Hora[10];
    char Fecha[50];

    // Obtiene el tiempo actual
    time(&t);
    info = localtime(&t);

    // Formatea el tiempo como una cadena y guárdalo en el búfer
    strftime(Hora, 10, "%H:%M:%S", info);
    strftime(Fecha,50,"%Y-%m-%d", info);

    printf("\tHora del sistema: %s\n", Hora);
    printf("\tFecha del sistema: %s\n", Fecha);

    //Con los dator recolectados se hace la insercion a base de datos
    char consulta[200];
    snprintf(consulta, sizeof(consulta), "INSERT INTO Reservacion (fecha, rfc, hora, id_habitacion, numero_habitacion, fecha_llegada, fecha_salida) VALUES ('%s', '%s', '%s', %d, %d, '%s', '%s')",
         Fecha, rfcCliente, Hora, seleccion, numeroHabitacion, fechaLlegada, fechaSalida);

    PGresult *res = PQexec(conn, consulta);
    int validarReservacion;
     if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Error al realizar la reservación: %s\n", PQerrorMessage(conn));
        validarReservacion=-1;
    } else {
        printf("Reservación exitosa.\n");
        validarReservacion=1;
    }
    if(validarReservacion==1){
        char buffer[200];
        snprintf(buffer, sizeof(buffer), "UPDATE Lista_Habitaciones SET estado = '-' WHERE numero_habitacion = %d",numeroHabitacion);

        PGresult *res2 = PQexec(conn, buffer);
        if (PQresultStatus(res2) != PGRES_COMMAND_OK) {
        printf("Error al realizar la reservación(Err2): %s\n", PQerrorMessage(conn));
        } else {
            printf("Reservación exitosa.\n");
        }
    }

   
    

    PQclear(res);
}

int obtenerNumeroHabitacion(PGconn *conn, int idReservacion) {
    char consulta[100];
    snprintf(consulta, sizeof(consulta), "SELECT numero_habitacion FROM Reservacion WHERE id_res = %d", idReservacion);


    PGresult *res = PQexec(conn, consulta);

    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        int numeroHabitacion = atoi(PQgetvalue(res, 0, 0)); // Obtener el valor de la columna 'numero_habitacion'.
        PQclear(res);
        return numeroHabitacion;
    } else {
        printf("Error al obtener el número de habitación: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return -1; // Valor de retorno para indicar un error.
    }
}


void cancelarReservacion(PGconn *conn) {
    int idReservacion,numeroHabitacion;
    printf("Ingrese el ID de la reserva que desea cancelar: ");
    scanf("%d", &idReservacion);
    numeroHabitacion=obtenerNumeroHabitacion(conn,idReservacion);
    
     // Actualizar el estado de la habitación en la tabla "Lista_Habitaciones"
    char updateConsulta[100];
    snprintf(updateConsulta, sizeof(updateConsulta), "UPDATE Lista_Habitaciones SET estado = '+' WHERE numero_Habitacion = %d", numeroHabitacion);

    PGresult *updateRes = PQexec(conn, updateConsulta);

    if (PQresultStatus(updateRes) != PGRES_COMMAND_OK) {
        printf("Error al actualizar el estado de la habitación: %s\n", PQerrorMessage(conn));
    } else {
        printf("Estado de la habitación actualizado exitosamente.\n");
    }

    PQclear(updateRes);

    // Construir y ejecutar una consulta SQL DELETE para cancelar una reserva por su ID.
    char consulta[100];
    snprintf(consulta, sizeof(consulta), "DELETE FROM Reservacion WHERE id_res = %d", idReservacion);

    PGresult *res = PQexec(conn, consulta);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Error al cancelar la reservación: %s\n", PQerrorMessage(conn));
    } else {
        printf("Reservación cancelada exitosamente.\n");
    }

    PQclear(res);
}



void Tiempo(){
    
}
void registrarEstancia(PGconn *conn) {
    char fecha[20];
    char nombreCliente[50];
    char hora[10];
    int tipoHabitacion;
    char fechaLlegada[20];
    char fechaSalida[20];

    printf("Ingrese la fecha (AAAA-MM-DD): ");
    scanf("%s", fecha);
    
    printf("Ingrese el nombre del cliente: ");
    scanf("%s", nombreCliente);
    
    printf("Ingrese la hora (HH:MM:SS): ");
    scanf("%s", hora);
    
    printf("Seleccione el tipo de habitación:\n");
    printf("1. Lujo\n");
    printf("2. Confort\n");
    printf("3. Básico\n");
    scanf("%d", &tipoHabitacion);
    
    printf("Ingrese la fecha de llegada (AAAA-MM-DD): ");
    scanf("%s", fechaLlegada);
    
    printf("Ingrese la fecha de salida (AAAA-MM-DD): ");
    scanf("%s", fechaSalida);

    // Mapea el tipo de habitación seleccionado a su nombre correspondiente
    char *tipoHabitacionNombre;
    switch (tipoHabitacion) {
        case 1:
            tipoHabitacionNombre = "Lujo";
            break;
        case 2:
            tipoHabitacionNombre = "Confort";
            break;
        case 3:
            tipoHabitacionNombre = "Básico";
            break;
        default:
            printf("Tipo de habitación no válido.\n");
            return;
    }

    // Construir y ejecutar una consulta SQL INSERT para registrar la estancia en la tabla.
    char consulta[1000];
	snprintf(consulta, sizeof(consulta), "INSERT INTO Reservacion (fecha, nombre_cliente, hora, tipo_habitacion, fecha_llegada, fecha_salida) VALUES ('%s', '%s', '%s', '%s', '%s', '%s')",
    fecha, nombreCliente, hora, tipoHabitacionNombre, fechaLlegada, fechaSalida);



}

void facturar(PGconn *conn) {

}

void liberarHabitacion(PGconn *conn) {

}

int main() {
    pid_t pid;

    // Crear un proceso hijo
    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Error al crear el proceso hijo\n");
        return 1;
    } else if (pid == 0) {
        // Este es el código del proceso hijo
        PGconn *conn;

        // Establecer una conexión a la base de datos PostgreSQL 'hotel'.
        conn = PQsetdbLogin("localhost","5432",NULL,NULL,"hotel","postgres","unach123");

        if (PQstatus(conn) == CONNECTION_BAD) {
            fprintf(stderr, "No se pudo conectar a la base de datos: %s\n", PQerrorMessage(conn));
            PQfinish(conn);
            exit(1); // Salir del proceso hijo con un código de error
        }

        printf("\nConexión a la base de datos 'hotel' exitosa.\n\n");        

        int opcion;
        do {
            printf("\nMenú de Operaciones:\n");
            printf("1. Registrar Cliente\n");
            printf("2. Hacer Reservación\n");
            printf("3. Cancelar Reservación\n");
            printf("4. Registrar Estancia en el Hotel\n");
            printf("5. Facturación\n");
            printf("6. Liberación de Habitación\n");
            printf("7. Salir\n");
            scanf("%d", &opcion);

            switch (opcion) {
                case 1:
                    registrarCliente(conn);
                    break;
                case 2:
                    hacerReservacion(conn);
                    break;
                case 3:
                    cancelarReservacion(conn);
                    break;
                case 4:
                    registrarEstancia(conn);
                    break;
                case 5:
                    facturar(conn);
                    break;
                case 6: 
                    Tiempo();
                    //liberarHabitacion(conn);
                    break;    
                case 7:
                    printf("Saliendo del programa.\n");
                    PQfinish(conn);
                    break;
                default:
                    printf("Opción no válida.\n");
            }
        } while (opcion != 7);

        // Salir del proceso hijo
        exit(0);
    } else {
        // Este es el código del proceso padre
        int status;
        waitpid(pid, &status, 0); // Esperar a que el proceso hijo termine
    }

    return 0;
}
