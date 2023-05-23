#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>
#include <stdbool.h>
#define MAX_BUF 1024
#define FIFO_SERVIDOR "fifo_servidor"
#define FIFO_CLIENTE "fifo_cliente"
#define FIFO_Model_autos "fifo_autos"

// Menu Principal
void MenuPrincipal();
// Sub_menu autos
void submenu_Autos();
// Sub_menu Clientes
void submenu_Clientes();
// sub_menu Accidentes
void submenu_Accidentes();

typedef struct
{
    char id_cli[5];
    char name[15];
    char fisrt_n[15];
    char last_n[15];
    char age[5];
    char sex[9];
    char address[50];
    char rfc[12];
    // datos para Telefonos del cliente
    char telhome[13];
    char cellphone[13];
    char teloffice[13];
} t_cliente;

typedef struct
{
    char placa[11]; // CHIS-93D-23
    char id_clie1[5];
    char color[20];
    char marca[20];
    char modelo[5];

} registro_autos;

typedef struct
{
    char id_acc[5];
    char placa1[11];
    char fecha[15]; // 2023/01/12
    char lugar[50];
    char hora[5]; // horas loacal 15:40:00
} registros_accident;

void imprimir_Accidentes(char *sql);

void imprimir_autos(char *sql);

void imprimir_registro(char *sql);

// funciones CRUD
void f_registrar(char *sql);
void f_listar(char *sql);
void f_eliminar(char *sql);
void f_actualizar(char *sql);

int i, j, num_cols, num_rows;

// variables Globales
char buffer_E[MAX_BUF];
char buffer_S[MAX_BUF];
char param_sql[200];
int tam_paramSql;
int fd_write;
int fd_read;
int opc_menuPrincipal;
int opc_submenu;
bool bandera = false;

/**
 * Colores en terminal
*/

void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[1;33m");
}
void reset()
{
    printf("\033[0m");
}
void blue()
{
    printf("\033[1;34m");
}
void green()
{
    printf("\033[1;32m");
}
void violet()
{
    printf("\033[35m");
}
void brown(){
    printf( "\x1B[31m");
}


// printf("\n\n");   scanf("%[^\n]",cad1);

int main(int argc, char const *argv[])
{
    char opc_inicio[MAX_BUF];
    bool bandera1 = false, bandera2 = false;
    printf("\n¡¡¡ Escribe [yes] para abrir el programa, [AnyKey] para Cancelar !!! \n\t\t\tEscribe [yes or anyKey] >> : ");
    scanf("%[^\n]", opc_inicio);
    if (0 == strcmp(opc_inicio, "yes"))
    {
        /*
                if (mkfifo(FIFO_CLIENTE,0666) < 0)
                {
                    perror("Error al crear el archivo FIFO_CLIENTE");
                }else{bandera1=true;}
                if (mkfifo(FIFO_SERVIDOR,0666) < 0)
                {
                    perror("Error al crear el archivo FIFO_SERVIDOR");
                }else{bandera2=true;}
        */
        // si esxisten los arcxhivos fifod se procede a contectarse
        if (bandera1 == false && bandera2 == false)
        {
            printf("\n\t\t==== Bienvenido Al sistema Aseguradora ====\n");
            MenuPrincipal();
        }
    }
    else
    {
        printf("Programa Canselado por el Cliente\n");
        // printf("\n\t\tAdios, Asta Pronto :] \n");
    }

    return 0;
}

// Menu Principal
void MenuPrincipal()
{

    do
    {   blue();
        printf("\n============================================");
        printf("\n\t**** Menu Principal ****\n");
        printf("===============================================");
        printf("\n1.- Administrar Clientes\n");
        printf("\n2.- Administra Autos\n");
        printf("\n3.- Administrar Accidentes\n");
        printf("\n4.- Salir del Systema\n");
       printf("\n>> Respuesta: ");
         reset();scanf("%d", &opc_menuPrincipal);
        printf("\n__________________________________________\n");
        system("clear");

        // eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S, "%d", opc_menuPrincipal);
        fd_write = open(FIFO_SERVIDOR, 0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_menuPrincipal)
        {
        case 1:
            submenu_Clientes();
            break;
        case 2:
            submenu_Autos();opc_menuPrincipal=0;
            break;
        case 3:
            submenu_Accidentes();
            break;
        case 4:
            break;

        default:
          red(); brown();  printf("\nOpcion Invalida Para Menu Principal :( \n"); 
            break; reset();
        }

    } while (opc_menuPrincipal != 4);
    printf("\nSesion Cerrada... \nPrograma Terminado\n");
}
// Sub_menu autos
void submenu_Autos()
{
   
    int opc_actualizar;
    char buscar[11];
    registro_autos autos;

    do
    {
         green();
        printf("\n__________________________________________");
        printf("\n\t*** Submenu Autos *** \n");
        printf("__________________________________________");
        printf("\n[1] Dar dec Alta\n");
        printf("\n[2] Dar de Baja\n");
        printf("\n[3] Consultar Catalogo de Autos\n");
        printf("\n[4] Actualizar Datos\n");
        printf("\n[5] Regresar al Menu\n");
        printf("\n<--- Respuesta: ");
        scanf("%d", &opc_submenu);
        printf("\n__________________________________________\n");

        // eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S, "%d", opc_submenu);
        fd_write = open(FIFO_SERVIDOR, 0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_submenu)
        {
        case 1:

            fflush(stdin);
            printf("\nRellena el Formulario para dar de alta Autos\n\t\t\t ++ NOTA ++\n\t\tEl encargado del Systema No se hace responsable de los espacios en blanco que deje iusted \n");
            printf("\nIngresa Placa: ");
            scanf("%s", autos.placa);
            printf("\nId_propietario: ");
            scanf("%s", autos.id_clie1);
            printf("\nColor: ");
            scanf("%s", autos.color);
            printf("\nMarca: ");
            scanf("%s", autos.marca);
            printf("\nModelo [mayor a 2015]: ");
            scanf("%s", autos.modelo);

            fflush(stdin);
            // enviamos sql a f_registrar
            sprintf(param_sql, "INSERT INTO t_auto VALUES ( %s, '%s', '%s', '%s', %s );", autos.id_clie1, autos.placa, autos.color, autos.marca, autos.modelo);
            f_registrar(param_sql);

            if (bandera == true)
            {
                printf("\nOcurrio un Eror Inesperado, o Ingresaste mal los Datos \n");
            }

            break;
        case 2:
            printf("\n¡Ojo!!!: La Ejecucion Tendra Exito Solo si Se Ingresa una Placa Valido\n");
            getchar();
            printf("\nIngresa la PLACA del Autos Para ELiminarlo\n\t---> PLACA: ");
            fflush(stdin);
            scanf("%s", autos.placa);
            sprintf(param_sql, "DELETE FROM t_auto WHERE placa = '%s' ;", autos.placa);
            // enviamos por parametro sentencia sql Eliminar copn el id
            f_eliminar(param_sql);
            break;
        case 3: // f_menuconsulta();
            printf("\nFuera de servicio\n");

            printf("\n\tConsultar Todos los Autos\n");
            // f_consult("SELECT*FROM t_auto;");
            imprimir_autos("select * from t_auto ORDER BY id_propietario1 asc;");
            break;
        case 4:
            fflush(stdin);
            getchar();
            printf("Ingresa la PLACA del Auto para verificar si existe: ");
            scanf("%s", buscar);
            // sprintf(param_sql, "SELECT * FROM t_propietario WHERE id_propietario = %s;", id_cli);
            // tam_paramSql = strlen(param_sql);
            fd_write = open(FIFO_SERVIDOR, O_WRONLY);
            write(fd_write, buscar, sizeof buscar);
            close(fd_write); // enviamos datos para la busqueda

            fd_read = open(FIFO_CLIENTE, O_RDONLY);
            read(fd_read, buffer_E, sizeof buffer_E);
            printf("\nmensaje (server)  Placa Auto Encontrado: %s\n", buffer_E);
            close(fd_read); // enviamos datos para la busqueda

            if (0 != strcmp(buffer_E, "No se Encontro Registro con el ID/placa PROPORCIONADO\n"))
            { // esperamos respuesta de que si esiste o no
                printf("Entramos al metod Actualizar\n");

                printf("\n=======================================================\n");

                // termino FuncionBuscarPor id

                // hacemos Switch par actualizar cada campo del cliente

                do
                {
                    printf("\n\t***** Si la Actulizacion no fallo , Sal del menu y vuelve a ingresar la Placa del auto *****\n");
                    printf("\nNo deje Campos Vacios \n");
                    printf("\n\t****** ¿Que deseas Actualizar? ****** \n");
                    printf("\n[1] PLACA\n");
                    printf("\n[2] id_Propietario\n");
                    printf("\n[3] Color\n");
                    printf("\n[4] Marca\n");
                    printf("\n[5] Modelo\n");
                    printf("\n[6] <<--- Regresar\n");
                    printf("\nRespuesta: ");
                    scanf("%d", &opc_actualizar);
                    /* Enviamos opc a se4rver */
                    char opc[5];
                    sprintf(opc, "%d", opc_actualizar);
                    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
                    write(fd_write, opc, sizeof opc);
                    close(fd_write);
                    fflush(stdin);

                    switch (opc_actualizar)
                    {
                    case 1:
                        getchar();
                        printf("\n[Edit] Placa : ");
                        fflush(stdin);
                        scanf("%s", autos.placa);
                        sprintf(param_sql, "UPDATE t_auto SET placa = '%s' WHERE placa = '%s';", autos.placa, buscar);
                        f_actualizar(param_sql);

                        break;
                    case 2:
                        getchar(); // esta fallando- al insertata caracteres de mas
                        printf("\n[Edit] Id_propietario : ");
                        fflush(stdin);
                        scanf("%s", autos.id_clie1);
                        sprintf(param_sql, "UPDATE t_auto SET id_propietario1 = %s WHERE placa = '%s'", autos.id_clie1, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 3:
                        getchar();
                        printf("\n[Edit] Color : ");
                        scanf("%s", autos.color);
                        sprintf(param_sql, "UPDATE t_auto SET color = '%s' WHERE placa = '%s';", autos.color, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 4:
                        getchar(); // caracteres de mas
                        printf("\n[Edit] Marca : ");
                        fflush(stdin);
                        scanf("%s", autos.marca);
                        sprintf(param_sql, "UPDATE t_auto SET marca = '%s' WHERE placa = '%s';", autos.marca, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 5:
                        getchar();
                        printf("\n[Edit] Modelo [Mayor a 2015] : ");
                        fflush(stdin);
                        scanf("%s", autos.modelo);

                        sprintf(param_sql, "UPDATE t_auto SET modelo = %s WHERE placa = '%s'", autos.modelo, buscar);
                        f_actualizar(param_sql);
                        break;

                    case 6:
                        printf("\nRegresando al subMenu Auto\n");
                        break;

                    default:
                        printf("\n---- OPCION INVALIDA PARA ACTUALIZAR LOS DATOS ----\n");

                        break;
                    }

                } while (opc_actualizar != 6);
            }

            break;
        case 5: printf("\nregresando al Menu Pricipal\n");
        break;

        default:
          brown();  printf("\nOpcion Invalida Para SubMenu autos :( \n");reset();
            break;
        }
    } while (opc_submenu != 5);

    printf("\n Finalizo Submenu Autos\n");
}
// Sub_menu Clientes
void submenu_Clientes()
{

    t_cliente persona;

    char cadenaSQL[MAX_BUF];
    // datos Personales
    int opc_submenu = 0;
    int opc_actualizar;
    char buscar[5];

    do
    {
        green();
        printf("\n____________________________________________\n");
        printf("\n\t*** Submenu Clientes: *** \n");
        printf("\n____________________________________________\n");
        printf("\n[1] Dar dec Alta\n");
        printf("\n[2] Dar de Baja\n");
        printf("\n[3] Consultar Catalogo de Clientes\n");
        printf("\n[4] Actualizar DAtos\n");
        printf("\n[5] Regresar al Menu del Systema\n");
        printf("\n<--- Respuesta: ");
      reset();  scanf("%d", &opc_submenu);
        printf("\n____________________________________________\n");

        // eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S, "%d", opc_submenu);
        fd_write = open(FIFO_SERVIDOR, 0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_submenu)
        {
        case 1:
            printf("\nRellena el Formulario para dar de alta el Cliente\n\t\t\t ++ NOTA ++\n\t\tEl encargado del Systema No se hace responsable de los espacios en blanco que deje iusted \n");
            fflush(stdin);
            printf("\nnew ID: ID generado automaticamente.");
            getchar();
            printf("\nName(s): ");
            scanf("%s", persona.name);
            printf("\nFirst name: ");
            scanf("%s", persona.fisrt_n);
            printf("\nLast name: ");
            scanf("%s", persona.last_n);
            printf("\nAge: ");
            scanf("%s", persona.age);
            printf("\nAddress: ");
            scanf("%s", persona.address);
            printf("\nRFC: ");
            scanf("%s", persona.rfc);
            printf("\nSexo [F/f] Femenino, [M/m] Masculino: ");
            scanf("%s", persona.sex);

            printf("\nEnter your Date phone's: Example --> '(501) 890-1237' \n");

            printf("\n1.- Phone home: ");
            scanf("%s", persona.telhome);
            printf("\n\t2.-   Cell phone: ");
            scanf("%s", persona.cellphone);
            printf("\n\t3.-Office Phone: ");
            scanf("%s", persona.teloffice);
            printf("\n++++> Press Enter para Finalizar Registro");
            getchar();
            fflush(stdin);

            sprintf(cadenaSQL, "WITH nueva_persona AS ( INSERT INTO t_propietario (nombre, ape_paterno, ape_materno, edad, sexo, direccion, rfc) VALUES('%s','%s','%s', %s, '%s', '%s', '%s') RETURNING id_propietario) ", persona.name, persona.fisrt_n, persona.last_n, persona.age, persona.sex, persona.address, persona.rfc);
            char cadenasql2[500];
            sprintf(cadenasql2, "INSERT INTO t_telefono (id_propietario1, tel_casa, tel_celular, tel_oficina) VALUES ( ( SELECT id_propietario FROM nueva_persona) , '%s', '%s', '%s');", persona.telhome, persona.cellphone, persona.teloffice);

            strcat(cadenaSQL, cadenasql2);

            printf("\nVariabole Direccion : %s\n", persona.cellphone);
            f_registrar(cadenaSQL);

            if (bandera == true)
            {
                printf("\nOcurrio un Eror Inesperado, o Ingresaste mal los Datos \n");
            }

            break;
        case 2:
            printf("\n¡Ojo!!: LA Eliminacion  de un Cliente Surtira Efecto Solo Si se Ingresa un Id Valido/Existente \n");
            getchar();
            printf("\nIngresa El ID del Cliente Para ELiminarlo\n\t---> ID Cliente: ");
            scanf("%[^\n]", persona.id_cli);
            sprintf(param_sql, "DELETE FROM t_propietario WHERE id_propietario = %s ;", persona.id_cli);
            // enviamos por parametro sentencia sql Eliminar copn el id
            f_eliminar(param_sql);
            break;
        case 3: //  f_menuconsulta();
                //    printf("\nFuera de servicio\n");

            printf("\n\tConsultar Todos los clientes\n");
            // f_consult("SELECT*FROM t_auto;");
            imprimir_registro("select * from vistaleft_propietariosTelefono ORDER BY id_propietario DESC");
            break;
        case 4:
            getchar();
            printf("Ingresa el id del cliente para verificar si existe: ");
            scanf("%[^\n]", buscar);
            // sprintf(param_sql, "SELECT * FROM t_propietario WHERE id_propietario = %s;", id_cli);
            // tam_paramSql = strlen(param_sql);
            fd_write = open(FIFO_SERVIDOR, O_WRONLY);
            write(fd_write, buscar, sizeof buscar);
            close(fd_write); // enviamos datos para la busqueda

            fd_read = open(FIFO_CLIENTE, O_RDONLY);
            read(fd_read, buffer_E, sizeof buffer_E);
            printf("\nmensaje (server)  Nombre_cliente: %s\n", buffer_E);
            close(fd_read); // enviamos datos para la busqueda

            if (0 != strcmp(buffer_E, "No se Encontro Registro con el ID/placa PROPORCIONADO\n"))
            { // esperamos respuesta de que si esiste o no
                printf("Entramos al metod Actualizar\n");

                printf("\n=======================================================\n");

                // termino FuncionBuscarPor id

                // hacemos Switch par actualizar cada campo del cliente

                do
                {
                   
                    printf("\n\t*****Si la Actulizacion no fallo , Sal del menu y vuelve a ingresar el id dedl Cliente *****\n");
                    printf("\nNo deje Campos Vacios\n");
                   yellow();  printf("\n\t **** ¿Que deseas Actualizar? **** \n");
                    printf("\n[1] ID\n");
                    printf("\n[2] name\n");
                    printf("\n[3] first name\n");
                    printf("\n[4] last name\n");
                    printf("\n[5] Age\n");
                    printf("\n[6] Sexo\n");
                    printf("\n[7] Addres\n");
                    printf("\n[8] RFC\n");
                    printf("\n[9] Numero de Casa\n");
                    printf("\n[10] Numero de Celulkar\n");
                    printf("\n[11] numero de oficina\n");
                    printf("\n[12] <<--- Regresar\n");
                    printf("\nRespuesta: ");
                  reset();  scanf("%d", &opc_actualizar);
                    /* Enviamos opc a se4rver */
                    char opc[5];
                    sprintf(opc, "%d", opc_actualizar);
                    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
                    write(fd_write, opc, sizeof opc);
                    close(fd_write);
                    fflush(stdin);

                    switch (opc_actualizar)
                    {
                    case 1:
                        getchar();
                        printf("\nNEW ID : ");
                        scanf("%[^\n]", persona.id_cli);
                        sprintf(param_sql, "UPDATE t_propietario SET id_propietario = %s WHERE id_propietario = %s;", persona.id_cli, buscar);
                        f_actualizar(param_sql);

                        break;
                    case 2:
                        getchar();
                        printf("\nNEW NOMBRE : ");
                        scanf("%[^\n]", persona.name);
                        sprintf(param_sql, "UPDATE t_propietario SET nombre = '%s' WHERE id_propietario = %s;", persona.name, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 3:
                        getchar();
                        printf("\nNEW apellido paterno : ");
                        scanf("%[^\n]", persona.fisrt_n);
                        sprintf(param_sql, "UPDATE t_propietario SET ape_paterno = '%s' WHERE id_propietario = %s;", persona.fisrt_n, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 4:
                        getchar();
                        printf("\nNEW Apellido Materno : ");
                        scanf("%[^\n]", persona.last_n);
                        sprintf(param_sql, "UPDATE t_propietario SET ape_materno = '%s' WHERE id_propietario = %s;", persona.last_n, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 5:
                        getchar();
                        printf("\nNEW Edad : ");
                        scanf("%[^\n]", persona.age);
                        sprintf(param_sql, "UPDATE t_propietario SET edad = %s WHERE id_propietario = %s;", persona.age, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 6:
                        getchar();
                        printf("\nNEW Sexo : ");
                        scanf("%[^\n]", persona.sex);
                        sprintf(param_sql, "UPDATE t_propietario SET sexo = '%s' WHERE id_propietario = %s;", persona.sex, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 7:
                        getchar();
                        printf("\nNEW direccion : ");
                        scanf("%[^\n]", persona.address);
                        sprintf(param_sql, "UPDATE t_propietario SET direccion = '%s' WHERE id_propietario = %s;", persona.address, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 8:
                        getchar();
                        printf("\nNEW RFC : ");
                        scanf("%[^\n]", persona.rfc);
                        sprintf(param_sql, "UPDATE t_propietario SET rfc = '%s' WHERE id_propietario = %s;", persona.rfc, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 9:
                        getchar();
                        printf("\nNEW Telefono de casa : ");
                        scanf("%[^\n]", persona.telhome);
                        sprintf(param_sql, "UPDATE t_telefono SET tel__casa = '%s' WHERE id_propietario1 = %s;", persona.telhome, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 10:
                        getchar();
                        printf("\nNEW Telefono Celular : ");
                        scanf("%[^\n]", persona.cellphone);
                        sprintf(param_sql, "UPDATE t_telefono SET tel_celular = '%s' WHERE id_propietario1 = %s;", persona.cellphone, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 11:
                        getchar();
                        printf("\nNEW tel Oficica : ");
                        scanf("%[^\n]", persona.teloffice);
                        sprintf(param_sql, "UPDATE t_telefono SET tel_oficina = '%s' WHERE id_propietario1 = %s;", persona.teloffice, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 12:
                        printf("\nRegresando al menu cliente\n");
                        break;

                    default:
                        printf("\n---- OPCION INVALIDA PARA ACTUALÑIZAR LOS DATOS ----\n");

                        break;
                    }

                } while (opc_actualizar != 12);
            }

            break;

        default:
         brown();   printf("\nOpcion Invalida Para SubMenu Clientes :( \n");  reset();
            break;
        }
    } while (opc_submenu != 5);

    printf("\n Finalizo Submenu Clintes\n");
}
// sub_menu Accidentes
void submenu_Accidentes()
{

    int opc_actualizar;
    char buscar[5];
    registros_accident accident;

    do
    {
        green();
        opc_submenu=0;
        printf("\n__________________________________________");
        printf("\n\t*** Submenu Accidentes *** \n");
        printf("__________________________________________");
        printf("\n[1] Dar de Alta\n");
        printf("\n[2] Dar de Baja\n");
        printf("\n[3] Consultar Catalogo de Acidentes\n");
        printf("\n[4] Actualizar Datos\n");
        printf("\n[5] Regresar al Menu\n");
        printf("\n<--- Respuesta: ");
      reset();  scanf("%d", &opc_submenu);
        printf("\n__________________________________________\n");

        // eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S, "%d", opc_submenu);
        fd_write = open(FIFO_SERVIDOR, 0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_submenu)
        {
        case 1:

        /**
         * capturAMO HORA DEL SYSTREMA 
         * 
        */
           time_t t = time(NULL);
    struct tm tiempolocal = *localtime(&t);
    char fechaNow[12];
    char *formato = "%Y-%m-%d";
    int byteescritos = strftime(fechaNow, sizeof(fechaNow), formato, &tiempolocal);

    if (byteescritos != 0)
    {
        // sino hay errores , los byteescritor no son 0
        printf("fecha: %s", fechaNow);
    }
    else
    {
        printf("Error formateando fecha");
    }

            fflush(stdin);
            printf("\nRellena el Formulario para Registrar un Acciendete\n\t\t\t ++ NOTA ++\n\t\tEl encargado del Systema No se hace responsable de los espacios en blanco que deje iusted \n");
            printf("\nIngresa ID_Accidente: ID Generado Automaticamente");
            printf("\nPlaca del Auto: ");
            scanf("%s", accident.placa1);
            printf("\nFecha: %s", fechaNow);
           strcpy(accident.fecha, fechaNow);//scanf("%s", accident.fecha);
            printf("\nLugar: ");
            scanf("%s", accident.lugar);
            printf("\nIngrese Hora la del Accidente (hh:mm) : ");
           scanf("%s", accident.hora);
           strcat(accident.hora, ":00");

            fflush(stdin);
            // enviamos sql a f_registrar
               // sprintf(param_sql, "INSERT INTO t_accidente (placa1,lugar) VALUES ( '%s', '%s' );", accident.placa1, accident.lugar);
                sprintf(param_sql, "INSERT INTO t_accidente (placa1, fecha, lugar, hora_accidente) VALUES ( '%s', '%s', '%s', '%s' );", accident.placa1, accident.fecha, accident.lugar, accident.hora);
           
            f_registrar(param_sql);
            if (bandera == true)
            {
                printf("\nOcurrio un Eror Inesperado, Ingresaste mal los Datos \n");
            }

            break;
        case 2:
            printf("\n¡Ojo!!!: La Ejecucion Tendra Exito Solo si Se Ingresa una ID_accidente  Valido\n");
            getchar();
            printf("\nIngresa el ID del Aaccidente Para ELiminarlo\n\t---> ID_ACCIDENTE: ");
            fflush(stdin);
            scanf("%s", accident.id_acc);
            sprintf(param_sql, "DELETE FROM t_accidente WHERE id_accidente = %s ;", accident.id_acc);
            // enviamos por parametro sentencia sql Eliminar copn el id
            f_eliminar(param_sql);
            break;
        case 3: // f_menuconsulta();
            printf("\nFuera de servicio\n");

            printf("\n\tConsultar Todos los Accidentes\n");
            // f_consult("SELECT*FROM t_auto;");
            imprimir_Accidentes("select * from t_accidente ORDER BY id_accidente asc;");
            break;
        case 4:
            fflush(stdin);
            getchar();
            printf("Ingresa la ID del Accidente para verificar: ");
            scanf("%[^\n]", buscar);
            // sprintf(param_sql, "SELECT * FROM t_propietario WHERE id_propietario = %s;", id_cli);
            // tam_paramSql = strlen(param_sql);
            fd_write = open(FIFO_SERVIDOR, O_WRONLY);
            write(fd_write, buscar, sizeof buscar);
            close(fd_write); // enviamos datos para la busqueda

            fd_read = open(FIFO_CLIENTE, O_RDONLY);
            read(fd_read, buffer_E, sizeof buffer_E);
            printf("\nmensaje (server)  ID_Accidente Encontrado: %s\n", buffer_E);
            close(fd_read); // enviamos datos para la busqueda

            if (0 != strcmp(buffer_E, "No se Encontro Registro con el ID/placa PROPORCIONADO\n"))
            { // esperamos respuesta de que si esiste o no
                printf("Entramos al metodo Actualizar\n");

                printf("\n=======================================================\n");

                // termino FuncionBuscarPor id

                // hacemos Switch par actualizar cada campo del cliente

                do
                {
                    
                    printf("\n\t***** NOta: Si la Actulizacion falla , Sal del menu y vuelve a ingresar opc= 4  *****\n");
                    printf("\nNo deje Campos Vacios \n");
                   yellow(); printf("\n\t****** ¿Que deseas Actualizar? ****** \n");
                    printf("\n[1] IDdel Accidente\n");
                    printf("\n[2] Cambiar Placa del auto\n");
                    printf("\n[3] Fecha de Accidente\n");
                    printf("\n[4] Lugar del Accidente\n");
                    printf("\n[5] Hora \n");
                    printf("\n[6] <<--- Regresar\n");
                    printf("\nRespuesta: ");
                   reset(); scanf("%d", &opc_actualizar);
                    /* Enviamos opc a se4rver */
                    char opc[5];
                    sprintf(opc, "%d", opc_actualizar);
                    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
                    write(fd_write, opc, sizeof opc);
                    close(fd_write);
                    fflush(stdin);

                    switch (opc_actualizar)
                    {
                    case 1:
                        getchar();
                        printf("\n[Edit] ID_Accidente : ");
                        fflush(stdin);
                        scanf("%s", accident.id_acc);
                        sprintf(param_sql, "UPDATE t_accidente SET id_accidente = %s WHERE id_accidente = %s;", accident.id_acc, buscar);
                        f_actualizar(param_sql);

                        break;
                    case 2:
                        getchar();
                        printf("\n[Edit] Placa : ");
                        fflush(stdin);
                        scanf("%s", accident.placa1);
                        sprintf(param_sql, "UPDATE t_accidente SET placa1 = '%s' WHERE id_accidente = %s;", accident.placa1, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 3:
                        getchar();
                        printf("\n[Edit] Fecha (AAA-MM-DD): ");
                        scanf("%s", accident.fecha);
                        sprintf(param_sql, "UPDATE t_accidente SET fecha = '%s' WHERE id_accidente = %s;", accident.fecha, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 4:
                        getchar();
                        printf("\n[Edit] Lugar del Accidente : ");
                        fflush(stdin);
                        scanf("%s", accident.lugar);
                        sprintf(param_sql, "UPDATE t_accidente SET lugar = '%s' WHERE id_accidente = %s;", accident.lugar, buscar);
                        f_actualizar(param_sql);
                        break;
                    case 5:
                        getchar();
                        printf("\n[Edit]Hora del Accidente [24Hrs.] : ");
                        fflush(stdin);
                        scanf("%s", accident.hora);
                        sprintf(param_sql, "UPDATE t_accidente SET hoa_formato24 = %s WHERE id_accidente = %s;", accident.hora, buscar);
                        f_actualizar(param_sql);
                        break;

                    case 6:
                        printf("\nRegresando al subMenu Accidentes\n");
                        break;

                    default:
                        printf("\n---- OPCION INVALIDA PARA ACTUALIZAR LOS DATOS ----\n");

                        break;
                    }

                } while (opc_actualizar != 6);
            }

            break;

        default:
        brown();    printf("\nOpcion Invalida Para SubMenu Accdente :( \n"); reset();
            break;
        }
    } while (opc_submenu != 5);

    printf("\n Finalizo Submenu Accidentes\n");
}

// funciones CRUD
void f_registrar(char *sql)
{
    // escribimos sentencia SQL a fifo para el server
    tam_paramSql = strlen(sql);
    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, tam_paramSql); // escribimos el tamaño del sql
    close(fd_write);

    printf("Mi Solicitud:  %s\n", sql);
    // y esperamos respuesta de  Exito

    fd_read = open(FIFO_Model_autos, O_RDONLY);
    read(fd_read, buffer_E, sizeof buffer_E);
    close(fd_read);
    printf("\nMensaje (Server) : %s\n", buffer_E);
    printf("F_registrar Teminado\n");

    if (0 == strcmp(buffer_E, "\n\t\t Algo Salio Mal :[ ,Registro Fallido :[\n"))
    {
        // printf();
        bandera = true;
    }
    else
    {
        printf("\n\t\t--- Se registro con exito ---\n");
    }
}

void f_eliminar(char *sql)
{
    // escribimos Sentencia SQL en el FIFO para el server
    tam_paramSql = strlen(sql);
    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, tam_paramSql);
    close(fd_write);

    // y esperamos respuesta del servidsor acerca de la consutla
    fd_read = open(FIFO_CLIENTE, O_RDONLY);
    read(fd_read, buffer_E, sizeof buffer_E);
    close(fd_read);
    printf("\nMensaje (server) : %s\n", buffer_E);
    printf("F_Eliminar Terminado\n");
}
void f_actualizar(char *sql)
{

    tam_paramSql = strlen(sql);
    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, tam_paramSql);
    close(fd_write);

    // y esperamos respuesta del servidsor acerca de la actualñizacion
    fd_read = open(FIFO_CLIENTE, O_RDONLY);
    read(fd_read, buffer_E, sizeof buffer_E);
    close(fd_read);
    printf("\nMensaje (server) : %s\n", buffer_E);
    printf("F_Actualizar Terminado\n");
}

void imprimir_registro(char *sql)
{

    int fd;
    int N = 30;
    char rows[5];

    // enviamos la sql select*from clñientes
    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, strlen(sql) + 1);
    close(fd_write); // select*from t_susuarios

    // eperamos los datos o el mensaje
    // char **data;
    fd = open(FIFO_CLIENTE, O_RDONLY);
    read(fd, rows, sizeof(rows));
    close(fd);

    N = atoi(rows);
    t_cliente array_datos[N];

    if (N > 0)
    {

        fd = open(FIFO_CLIENTE, O_RDONLY);
        read(fd, array_datos, N * sizeof(t_cliente));
        close(fd);

        // Imprimir los datos recibidos
        printf("\n----------------------------------------------------------------------------------------------------------------------------------");
        printf("\nFila| id| Nombre |1erApellido|2doApellido|Edad|sexo|       Direccion     |  RFC    |  Casa  | celular| Oficina\n");
        printf("-----------------------------------------------------------------------------------------------------------------------------------\n");

        for (int i = 0; i < N; i++)
        {
            printf("[%i]  %s  ", (i + 1), array_datos[i].id_cli);
            printf("  %s  ", array_datos[i].name);
            printf("  %s  ", array_datos[i].fisrt_n);
            printf("  %s  ", array_datos[i].last_n);
            printf("  %s  ", array_datos[i].age);
            printf("   %s  ", array_datos[i].sex);
            printf("     %s  ", array_datos[i].address);
            printf("  %s  ", array_datos[i].rfc);
            printf("  %s  ", array_datos[i].telhome);
            printf("  %s  ", array_datos[i].cellphone);
            printf("  %s  ", array_datos[i].teloffice);
            printf("\n");
        }

        printf("\n-----------------------------------------------------------------------------------------------------------------------------------\n");
    }
    else
    {
        // Imprimir los datos recibidos
        printf("\n------------------------------------------------------------------------------------------------------------------------------------");
        printf("\nFila | id | Nombre | 1erApellido | 2doApellido | Edad |sexo| Direccion | RFC | telCasa |  celular  | telOficina\n");
        printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("\t\t\t\t\t\t\t\t\t\t\t---- LA TABLA ESTA VACIA ----");

        printf("\n----------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

void imprimir_autos(char *sql)
{

    int fd;
    int N = 5;
    char rows[5];

    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, strlen(sql) + 1);
    close(fd_write); // select*from t_susuarios

    // eperamos los datos o el mensaje
    // char **data;
    fd = open(FIFO_CLIENTE, O_RDONLY);
    read(fd, rows, sizeof(rows));
    close(fd);

    N = atoi(rows);

    registro_autos array_datos[N];

    fd = open(FIFO_CLIENTE, O_RDONLY);
    read(fd, array_datos, N * sizeof(registro_autos));
    close(fd);
    fflush(stdin);
    // Imprimir los datos recibidos
    printf("\n---------------------------------------------------------");
    printf("\n     |   Placa  |Id_client|   color  |   Marca  |Modelo|\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < N; i++)
    {
        printf("[%i]  | %s  ", (i + 1), array_datos[i].placa);
        printf("  %s   ", array_datos[i].id_clie1);
        printf(" %s  ", array_datos[i].color);
        printf(" %s  ", array_datos[i].marca);
        printf(" %s  \n", array_datos[i].modelo);
    }
    printf("----------------------------------------------------------\n");
}

void imprimir_Accidentes(char *sql)
{
    int fd;
    int N = 5;
    char rows[5];

    fd_write = open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, strlen(sql) + 1);
    close(fd_write); // select*from t_susuarios

    // eperamos los datos o el mensaje
    // char **data;
    fd = open(FIFO_CLIENTE, O_RDONLY);
    read(fd, rows, sizeof(rows));
    close(fd);

    N = atoi(rows);

    registros_accident array[N];

    if (N > 0)
    {
        /* code */
        fd = open(FIFO_CLIENTE, O_RDONLY);
        read(fd, array, N * sizeof(registros_accident));
        close(fd);
        fflush(stdin);
        // Imprimir los datos recibidos
        printf("\n---------------------------------------------------------");
        printf("\n     |id_Accidente |  Placa  | fecha |          Lugar          | Hora Accidente (24Hrs.)|\n");
        printf("---------------------------------------------------------\n");
        for (int i = 0; i < N; i++)
        {
            printf("[%i]  | %s  ", (i + 1), array[i].id_acc);
            printf("  %s   ", array[i].placa1);
            printf(" %s  ", array[i].fecha);
            printf(" %s ", array[i].lugar);
            printf("  %s  \n", array[i].hora);
        }
        printf("----------------------------------------------------------\n");
    }
    else
    {
        printf("\n---------------------------------------------------------");
        printf("\n     |id_Accidente |  Placa  | fecha |          Lugar          | Hora Accidente (24Hrs.)|\n");
        printf("---------------------------------------------------------\n");
        printf("\t\t\t ---- LA TABLA SE ENCUENTRA VACIA ----");
        printf("----------------------------------------------------------\n");
    }
}
