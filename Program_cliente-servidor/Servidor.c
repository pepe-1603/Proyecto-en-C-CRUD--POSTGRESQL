#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<postgresql/libpq-fe.h>
#include<errno.h>
#include<stdbool.h>
#include<ctype.h>

#define MAX_BUF 1024
#define FIFO_SERVIDOR "fifo_servidor"
#define FIFO_CLIENTE "fifo_cliente"
#define MAX_COLS 15
#define MAX_ROWS 100


char msgQuery[MAX_BUF];
PGconn *conn;
PGresult *res;
//char *conninfo = "dbname=demo_aseguradora user=postgres password=0623";//Eror en sockquet no deja ingresar a postgres

int tam_paramSQL;

//variables para f_consutl
int num_cols=0, num_rows=0, i,j;
char matriz[MAX_ROWS][MAX_COLS][256];
char *value=NULL;


//Menu Principal
//void MenuPrincipal();
//Sub_menu autos
void submenu_Autos();
//Sub_menu Clientes
void submenu_Clientes();
//sub_menu Accidentes
void submenu_Accidentes();

typedef struct registros
{
    char id_cli[5];
    char name[15];
    char fisrt_n[15];
    char last_n[15];
    char age[5];
    char sex[9];
    char address[50];
    char rfc[12];
    //datos para Telefonos del cliente
    char telhome[12];
    char cellphone[12];
    char teloffice[12];

};

typedef struct registro_autos{
    char placa[15];
    char id_clie1[5];
    char color[15];
    char marca[20];
    char modelo[5];


};

typedef struct registros_accident
{
    char id_acc[5];
    char placa1[15];
    char fecha[15];
    char lugar[50];
    char hora[5];
};


void getRegistroAutos(char* dql);
void getRegistrosAccident(char *sql);


void obtener_registros(char *sql);
//funciones CRUD
void f_registrar(char* sql);

void f_eliminar(char* sql);
void f_actualizar (char* sql);

void buscarById(char* id);


//variables Globales
    char buffer_E[MAX_BUF];
    char buffer_S[MAX_BUF];
    int fd_write;
    int fd_read;
    int num_bytes;
    int Hecho=0;
    int opc_principal, opc_Submenu;
    bool banderaBuscar=false;

int main(int argc, char const *argv[])
{
    
printf("\n\t*** Server en Espera...\n\n");


        //Crear elArchivo FIFO
    mkfifo(FIFO_SERVIDOR,0666);
    mkfifo(FIFO_CLIENTE,0666);

  /*  
    if(mkfifo(FIFO_SERVIDOR,0666) < 0){
        perror("Error al Crear el Archivo FIFO_SERVIDOR");
        exit(1);
    }
    if(mkfifo(FIFO_CLIENTE,0666) < 0){
        perror("Error al Crear el Archivo FIFO_CLIENTE");
        exit(1);
    }
    */
    
 ////Primero establecemos la conexion con la base de datos
                        //conn = PQconnectdb(conninfo);
                        conn =  PQsetdbLogin("localhost", "5432", NULL, NULL, "demo_aseguradora", "rol_usr1", "rol_usr1");
   if (PQstatus(conn)== CONNECTION_OK)
    {
      printf("\nSe establecio Conexion Con la Base de Datos\n");

     //---- ACCEDEMOS AL MENU PRINCIPAL SI LA CONEXION ESTA ESTABLEWCIDA, DE LO CONTRAIO NO SE PERMITE EL ACCESO AL MENU PRICIPAL
      printf("\nEsperando Mensaje del Cliente\n");                               
        while (!Hecho)
        {

            //Abrir el Archivo Fifo en modo Lectura
            fd_read = open(FIFO_SERVIDOR, O_RDONLY);


            //Leer el archivo fifo para iniciar el menu Principal
                num_bytes = read(fd_read, buffer_E, sizeof buffer_E);
                

                    if (num_bytes > 0)
                    {
                        //mensaje por consola 
                        printf("\nSe ha Conectado el cliente\n");
                        //capturamos la opcion del usuario dedl menu ṕrinciapkl
                        opc_principal = atoi(buffer_E);//leemos el mensaje de cliente que esta guardado en buffer
                        close(fd_read); //cerramos el Fifo en modo lectura
                        //Menu Principal
                        printf("\n=== Cliente Entru Al Menu Principal ===\n");     
                        printf("Opc del cliente Fue: [%d] \n",opc_principal);                 
                        
                         switch (opc_principal)
                        {
                        case 1:
                        printf("Cliente  Elijio Adminsitracion de Clientes\n");
                        submenu_Clientes();
                                                    
                            break;
                        case 2:
                        printf("Cliente  Elijio Adminsitracion de Clientes\n");
                        submenu_Autos();

                        case 3: 
                        printf("\nCliente Elijio Administracion de Accidentes\n");
                        submenu_Accidentes();
                            break;

                            break;
                         case 4:printf("\nSaliendo del Systema\n");
                            break;
                                
                        default:
                            break;
                        }//fin switch Principall

                            


                                //Si la Consulta es un "exit" , salir del bucle infinito y termina el programa
                        if (strcmp(buffer_E, "exit")==0){
                           break;
                        }//Fin MenuPricipal


                        
                    }//fin if_numbytes
                

        


        }//Fin Loop 
        printf("\nSALIMOS DE LOOP INFINITO, Y CERRAMOS LA CONEXION A LA BASE DED DATOS \nY ELIMINAMOS EL FIFO_SERVIDOR\n");
            //Cerramos la Conexioon con la Base de Datos
          PQfinish(conn);

            //Se elimna el Fifo en modo Escritura
         unlink(FIFO_SERVIDOR);
    }
    else{
        fprintf(stderr, "No se pudo conectar ala base de datos : %s", PQerrorMessage(conn));
    }


    
    return 0;
}


//Menu Principal
//void MenuPrincipal()


//Sub_menu autos
void submenu_Autos(){
    do
    {
        printf("\n*** Submenu Autos: *** \n");
        fd_read=open(FIFO_SERVIDOR,O_RDONLY);
        read(fd_read, buffer_E, sizeof buffer_E);
        close(fd_read);
        opc_Submenu = atoi(buffer_E);

        printf("\n=== Cliente Entro Al SubMenu Autos ===\n");     
        printf("Opc del cliente Fue: [%d] \n",opc_Submenu);  

        switch (opc_Submenu)
        {
        case 1:printf("\nCargando Datos para el Registro Auto....\n");
           // recibimos los datos del cliente 
           fd_read=open(FIFO_SERVIDOR, O_RDONLY);
           read(fd_read, buffer_E, sizeof buffer_E);
           close(fd_read);
           printf("\nMensaje (cliente) : %s\n",buffer_E);
           f_registrar(buffer_E);


            break;
        case 2:printf("\nCArgando Datos para Eliminar Auto...\n");
               //recibimos datos del clietne para eliminar
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, buffer_E, sizeof buffer_E);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", buffer_E);
                f_eliminar(buffer_E);

           
            break;
        case 3:
        printf("\nCArgando DAtos para Listar Los Autos...\n");
               /*recibimos datos del clietne para Consultar*/
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, buffer_E, sizeof buffer_E);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", buffer_E);
               // f_listar(buffer_E);
              // listarAl(buffer_E);
              //  f_consult(buffer_E);
              getRegistroAutos(buffer_E);
               printf("\nFuera de servicio \n");
               
            break;
        case 4:
                printf("\nCArgando DAtos para Actualizar Auto...\n");
                char placa_buscar[200];
               //recibimos datos del clietne para Buscar cliente asntres de actucalizarf
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, placa_buscar, sizeof placa_buscar);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", placa_buscar);
                char Sql[500];
                snprintf(Sql, sizeof(Sql) ,"SELECT * FROM t_auto WHERE placa = '%s' ;",placa_buscar);
                buscarById(Sql);
                
                if (banderaBuscar == true)
                {
                    do
                    {
                        printf("Switch Actuaslñizar datos Auto \n");
                        //recibimos la opc de switc de clietne
                        fflush(stdin);
                    fd_read=open(FIFO_SERVIDOR, O_RDONLY);
                    read(fd_read, buffer_E, sizeof buffer_E);
                    close(fd_read);
                    printf("El cliente escribio: [%s] \n",buffer_E);

                        switch (atoi(buffer_E))
                        {
                        case 1:case 2: case 3: case 4:
                        case 5://recibimos sql y lo pasamos a f_Actualizar
                            printf("\nActualiacion de Auto\n ");
                            sprintf(buffer_E, "hola");
                            fd_read=open(FIFO_SERVIDOR, O_RDONLY);
                            read(fd_read, buffer_E, sizeof buffer_E);fflush(stdin);
                            close(fd_read);

                            f_actualizar(buffer_E);

                            break;
                        
                        
                        default:printf("\nOpc invalida para menu Actualizar datos");
                            break;
                        }
                    } while (atoi(buffer_E) != 6);
                    
                    
                }
                
                


            break;
        case 5: printf("regresando al menu principal\n");
        break;
        
        default:printf("\nOpcion Invalida Para SubMenu Clientes :( \n");
            break;
        }  
    } while (opc_Submenu!=5);

    printf("\n Finalizo Submenu Clintes\n");

}
//Sub_menu Clientes
void submenu_Clientes(){
    
    do
    {
        printf("\n*** Submenu clientes: *** \n");
        fd_read=open(FIFO_SERVIDOR,O_RDONLY);
        read(fd_read, buffer_E, sizeof buffer_E);
        close(fd_read);
        opc_Submenu = atoi(buffer_E);

        printf("\n=== Cliente Entru Al SubMenu Clientes ===\n");     
        printf("Opc del cliente Fue: [%d] \n",opc_Submenu);  

        switch (opc_Submenu)
        {
        case 1:printf("\nCargando Datos para el Registro..\n");
           // recibimos los datos del cliente 
           fd_read=open(FIFO_SERVIDOR, O_RDONLY);
           read(fd_read, buffer_E, sizeof buffer_E);
           close(fd_read);
           printf("\nMensaje (cliente) : %s\n",buffer_E);
           f_registrar(buffer_E);

           //eperamos el otro REgistro
           // recibimos los datos telefonicos del cliente 
           fd_read=open(FIFO_SERVIDOR, O_RDONLY);
           read(fd_read, buffer_E, sizeof buffer_E);
           close(fd_read);
           printf("\nMensaje (cliente) : %s\n",buffer_E);
           f_registrar(buffer_E);


            break;
        case 2:printf("\nCArgando Datos para Eliminar...\n");
               //recibimos datos del clietne para eliminar
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, buffer_E, sizeof buffer_E);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", buffer_E);
                f_eliminar(buffer_E);

           
            break;
        case 3:
        printf("\nCArgando DAtos para Listar...\n");
               /*recibimos datos del clietne para Consultar*/
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, buffer_E, sizeof buffer_E);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", buffer_E);
               // f_listar(buffer_E);
              // listarAl(buffer_E);
              //  f_consult(buffer_E);
             obtener_registros(buffer_E);
              printf("\nFuera de servicio \n");
               
            break;
        case 4:
                printf("\nCArgando DAtos para Actualizar...\n");
                char id_buscar[200];
               //recibimos datos del clietne para Buscar cliente asntres de actucalizarf
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, id_buscar, sizeof id_buscar);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", id_buscar);
                char Sql[500];
                sprintf(Sql, "SELECT * FROM t_propietario WHERE id_propietario = %s ;", id_buscar);
                buscarById(Sql);
                
                if (banderaBuscar == true)
                {
                    do
                    {
                        printf("Switch Actuaslñizar datos\n");
                        //recibimos la opc de switc de clietne
                        fflush(stdin);
                    fd_read=open(FIFO_SERVIDOR, O_RDONLY);
                    read(fd_read, buffer_E, sizeof buffer_E);
                    close(fd_read);
                    printf("El clietne escribio: [%s] \n",buffer_E);

                        switch (atoi(buffer_E))
                        {
                        case 1:case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
                        case 11://recibimos sql y lo pasamos a f_Actualizar
                            printf("\nActualiacion de clietnee\n ");
                            sprintf(buffer_E, "hola");
                            fd_read=open(FIFO_SERVIDOR, O_RDONLY);
                            read(fd_read, buffer_E, sizeof buffer_E);fflush(stdin);
                            close(fd_read);

                            f_actualizar(buffer_E);

                            break;
                        
                        
                        default:printf("\nOpc invalida parta menu Actualizar datos");
                            break;
                        }
                    } while (atoi(buffer_E) != 12);
                    
                    
                }
                
                


            break;
        case 5: printf("regresando al menu principal\n");
        break;
        
        default:printf("\nOpcion Invalida Para SubMenu Clientes :( \n");
            break;
        }  
    } while (opc_Submenu!=5);

    printf("\n Finalizo Submenu Clintes\n");
}
//sub_menu Accidentes
void submenu_Accidentes(){
     do
    {
        printf("\n*** Submenu Accidnetes: *** \n");
        fd_read=open(FIFO_SERVIDOR,O_RDONLY);
        read(fd_read, buffer_E, sizeof buffer_E);
        close(fd_read);
        opc_Submenu = atoi(buffer_E);

        printf("\n=== Cliente Entro Al SubMenu Accidente ===\n");     
        printf("Opc del cliente Fue: [%d] \n",opc_Submenu);  

        switch (opc_Submenu)
        {
        case 1:printf("\nCargando Datos para el Registro Accidnete....\n");
           // recibimos los datos del cliente 
           fd_read=open(FIFO_SERVIDOR, O_RDONLY);
           read(fd_read, buffer_E, sizeof buffer_E);
           close(fd_read);
           printf("\nMensaje (cliente) : %s\n",buffer_E);
           f_registrar(buffer_E);


            break;
        case 2:printf("\nCArgando Datos para Eliminar Accidente...\n");
               //recibimos datos del clietne para eliminar
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, buffer_E, sizeof buffer_E);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", buffer_E);
                f_eliminar(buffer_E);

           
            break;
        case 3:
        printf("\nCArgando DAtos para Listar Los Accidentes...\n");
               /*recibimos datos del clietne para Consultar*/
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, buffer_E, sizeof buffer_E);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", buffer_E);
             getRegistrosAccident(buffer_E);
               printf("\nFuera de servicio \n");
               
            break;
        case 4:
                printf("\nCArgando DAtos para Actualizar Accidente...\n");
                char idAccidente_buscar[200];
               //recibimos datos del clietne para Buscar cliente asntres de actucalizarf
               fd_read=open(FIFO_SERVIDOR, O_RDONLY);
               read(fd_read, idAccidente_buscar, sizeof idAccidente_buscar);
               close(fd_read);
                printf("\nMensaje (cliente) : %s\n", idAccidente_buscar);
                char Sql[500];
                snprintf(Sql, sizeof(Sql) ,"SELECT * FROM t_accidente WHERE id_accidente = %s ;",idAccidente_buscar);
                buscarById(Sql);
                
                if (banderaBuscar == true)
                {
                    do
                    {
                        printf("Switch Actuaslizar datos Accidente \n");
                        //recibimos la opc de switc de clietne
                        fflush(stdin);
                    fd_read=open(FIFO_SERVIDOR, O_RDONLY);
                    read(fd_read, buffer_E, sizeof buffer_E);
                    close(fd_read);
                    printf("El cliente escribio: [%s] \n",buffer_E);

                        switch (atoi(buffer_E))
                        {
                        case 1:case 2: case 3: case 4:
                        case 5://recibimos sql y lo pasamos a f_Actualizar
                            printf("\nActualiacion de Accidente\n ");
                            sprintf(buffer_E, "hola");
                            fd_read=open(FIFO_SERVIDOR, O_RDONLY);
                            read(fd_read, buffer_E, sizeof buffer_E);fflush(stdin);
                            close(fd_read);

                            f_actualizar(buffer_E);

                            break;
                        
                        
                        default:printf("\nOpc invalida para menu Actualizar datos");
                            break;
                        }
                    } while (atoi(buffer_E) != 6);
                    
                    
                }
                
                


            break;
        case 5: printf("regresando al menu principal\n");
        break;
        
        default:printf("\nOpcion Invalida Para SubMenu Accidentes :( \n");
            break;
        }  
    } while (opc_Submenu!=5);

    printf("\n Finalizo Submenu Accidentes\n");

}

//funciones CRUD
void f_registrar(char* sql){
    printf("\nEspere un poco mas y estra lsito la consultta Registro..\n");
    char msg1[]="\n\t\t Algo Salio Mal :[ ,Consulta Fallida\n";
    char msg2[]="\n\t\t Todo Salio Bien :] ,Consulta Exitosa \n";
    printf("\nEjecutando Consulta [INSERT]....\n");
    res = PQexec(conn, sql);
    if (PQresultStatus(res)==PGRES_COMMAND_OK)
    {
        printf("\nConsulta INSERT Exitosa\n");
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        //tam_paramSQL=strlen(sql);//escribimos el tamaño del sql
        write(fd_write, msg2, sizeof msg2);
        close(fd_write);
    }
    else{
        fprintf(stderr , "\nConsulta INSERT Fallida\n Error: %s\n",PQresultErrorMessage(res));
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        write(fd_write, msg1, sizeof msg1);
        close(fd_write);
    }
    

PQclear(res);
printf("F_Registrart Terminado\n");

}

void f_eliminar(char* sql){
    printf("\nEspere un poco, su consulta Eliminar estara sera completada...\n");
    char msg1[]="\n\t\t Algo Salio Mal :[ ,Eliminacion Fallida\n";
    char msg2[]="\n\t\t Todo Salio Bien :] ,Eliminacion Exitosa \n";
    printf("\nEjecutando Consulta [DELETE]....\n");

    res = PQexec(conn, sql);
    if (PQresultStatus(res)==PGRES_COMMAND_OK)
    {
        printf("\nConsulta DELETE Exitosa\n");
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        tam_paramSQL=strlen(sql);//escribimos el tamaño del sql
    write(fd_write, msg2, tam_paramSQL);
        close(fd_write);
    }
    else{
        fprintf(stderr , "\nConsulta DELETE Fallida\n Error: %s\n",PQresultErrorMessage(res));
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        write(fd_write, msg1, sizeof msg1);
        close(fd_write);
    }
    PQclear(res);
    printf("\nF_Eliminar Terminado\n");

}
void f_actualizar (char* sql){
    printf("\nEspere un poco, su consulta Actualizar estara sera completada...\n");
    char msg1[]="\n\t\t Algo Salio Mal :[ ,Actualizacion Fallida\n";
    char msg2[]="\n\t\t Todo Salio Bien :] ,Actualizacion Exitosa \n";
    printf("\nEjecutando Consulta [UPDATE]....\n");

    res = PQexec(conn, sql);
    if (PQresultStatus(res)==PGRES_COMMAND_OK)
    {
        printf("\nConsulta DELETE Exitosa\n");
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        tam_paramSQL=strlen(sql);//escribimos el tamaño del sql
        write(fd_write, msg2, tam_paramSQL);
        close(fd_write);
    }
    else{
        fprintf(stderr , "\nConsulta DELETE Fallida\n Error: %s\n",PQresultErrorMessage(res));
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        write(fd_write, msg1, sizeof msg1);
        close(fd_write);
    }
    PQclear(res);
    printf("\nF_Actualizar Terminado\n");

}

void buscarById( char* id){
  

    //variable de numero Columnas
    int nfields;
    //numero columnas de tabla
    int nrows;

    //varible apara el valor de la co,lmnas
    char *value;
    //creamos variabole pasra indice de columans
    int col;

    //ejecutamos la copnsulta con el identificador proporcionado
    res = PQexec(conn, id);
    //verificamos si la consulta fue exoit0sa
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {

        fprintf(stderr,"Erorr al ejecutrar la consulta: %s\n",PQerrorMessage(conn));
        fd_write=open(FIFO_CLIENTE,O_WRONLY);
        write(fd_write,"Error al ejecutar la consulta", 30);
        close(fd_write);
        PQclear(res);
        return;

    }

//    obbtenemos el numer ode columnas y filas;
nfields=PQnfields(res);
nrows=PQntuples(res);

//si no se encontor ningun registro , mostrar mensaje y salir de la funcioni
if (nrows == 0)
{
    char *cad="No se Encontro Registro con el ID/placa PROPORCIONADO\n";
    /* code */
    printf("No se encontro con ningun registro con el ID/Placa PROPORCIONADO\n");
    fd_write=open(FIFO_CLIENTE,O_WRONLY);
        write(fd_write, cad,strlen( cad)+1);
        close(fd_write);

        PQclear(res);
        return;
    
}
if(nrows > 0){
            fd_write = open(FIFO_CLIENTE, O_WRONLY);
            if (fd_write == -1)
            {
                perror("open");
                exit(1);
            }

            //--------------

            fd_write = open (FIFO_CLIENTE, O_WRONLY);
            sprintf(buffer_S, "%s\n", PQgetvalue(res, 0, 1));
            //enviamos valores de la ytabla
            write(fd_write, buffer_S, strlen(buffer_S));
            close(fd_write);
            printf("EL valor 2(nombre)es: %s\n", buffer_S);

            banderaBuscar=true;


    }

    for (int i = 0; i < nrows; i++) {
            for (int j = 0; j < nfields; j++) {
                value = PQgetvalue(res, i, j);
                col = PQftype(res, j);
                if (value != NULL) {
                    printf("%s\t", value);
                //  write(fd_write, value, strlen (value)+1);
                    
                } else {
                    printf("NULL\t");
                }
            }
            printf("\n");
        }

    //  close(fd_write);

    
        PQclear(res);

    printf("\nF_Buscar Finalizado\n");
    

}

/*Notas de mis codigo, 
Recordar que para el uso de uns Array de caracterres  se inicializa 
char Array[]="algo en la cadena"; y para medir el tamaño ded la cadena que contiene el array ,mas no el tamaño del Arra de caracteres se utilizad Array[]; se utilizad corchetes
para indicar un array de caracteres mas un de un solo caracter, y asi se puiede usar la funcion SIzeof 
o de otra MANERA :
char *cadena ="algo en la cadena"; 
Este es otra manera de mandar como parametro un array de carachetres y para obetener el tamaño se usaa la funcion STRL(nom_cadena char);
asi se hara referencia a un puntero char 
*/


void obtener_registros(char* sql){

    int fd;
    int N= 25;
    char rows[5];

    

    res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        printf("Error al ejecutar la consulta");
    }


num_rows=PQntuples(res);
N = num_rows;
  
struct registros array_data[N+1];//CAPTURAR nUM_FILAS EN pqNtUPLES
    if (num_rows >0)
    {
        sprintf(rows, "%d",num_rows);
        fd_write = open(FIFO_CLIENTE, O_WRONLY);
        write(fd_write,rows,sizeof rows);
        close(fd_write);
    }else{
        sprintf(rows, "%d",num_rows);
        fd_write = open(FIFO_CLIENTE, O_WRONLY);
        write(fd_write,rows,sizeof rows);
        close(fd_write);
    }

    
    for (i = 0; i < N; i++)
    {
        strcpy(array_data[i].id_cli, PQgetvalue(res, i, 0));
        strcpy(array_data[i].name, PQgetvalue(res, i, 1));
        strcpy(array_data[i].fisrt_n, PQgetvalue(res, i, 2));
        strcpy(array_data[i].last_n, PQgetvalue(res, i, 3));
        strcpy(array_data[i].age, PQgetvalue(res, i, 4));
        strcpy(array_data[i].sex, PQgetvalue(res, i, 5));
        strcpy(array_data[i].address, PQgetvalue(res, i, 6));
        strcpy(array_data[i].rfc, PQgetvalue(res, i, 7));
        strcpy(array_data[i].telhome, PQgetvalue(res, i, 8));
        strcpy(array_data[i].cellphone, PQgetvalue(res, i, 9));
        strcpy(array_data[i].teloffice, PQgetvalue(res, i, 10));
        


        
    }
    
    fd = open(FIFO_CLIENTE, O_WRONLY);
    write(fd,array_data, N * sizeof(struct registros));
    close(fd);
    PQclear(res);
    printf("\n id | Nombre  |  Ape Paterno | Ape MAterno \n");
     for (int i = 0; i < N; i++) {
        printf(" %s  ", array_data[i].id_cli);
        printf(" %s  ", array_data[i].name);
        printf(" %s  ", array_data[i].fisrt_n);
        printf(" %s  \n", array_data[i].last_n);
    }

}

void getRegistroAutos(char* sql){
    int fd;
    int N=3;
    char rows[5];
    

  

    res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        printf("Error al ejecutar la consulta");
    }
num_rows=PQntuples(res);
N = num_rows;
 struct registro_autos data[N+1];//CAPTURAR nUM_FILAS EN pqNtUPLES    

    if (num_rows >0)
    {
        sprintf(rows, "%d",num_rows);
        fd_write = open(FIFO_CLIENTE, O_WRONLY);
        write(fd_write,rows,sizeof rows);
        close(fd_write);
    }else{
        sprintf(rows, "%d",num_rows);
        fd_write = open(FIFO_CLIENTE, O_WRONLY);
        write(fd_write,rows,sizeof rows);
        close(fd_write);
    }
    
     

printf("\nColumnas: %d  --- Rows: %d\n",PQnfields(res), PQnfields(res));

    
    

    for (i = 0; i < N; i++)//[N]array debe ser menor a numFilas
    {
        strcpy(data[i].placa, PQgetvalue(res, i, 0));
     strcpy(data[i].id_clie1, PQgetvalue(res, i, 1));
        strcpy(data[i].color, PQgetvalue(res, i, 2));
        strcpy(data[i].marca, PQgetvalue(res, i, 3));
        strcpy(data[i].modelo, PQgetvalue(res, i, 4));      
        
    }
    printf("\nListando de Autos completaod...\n");
    
    fd = open(FIFO_CLIENTE, O_WRONLY);
    write(fd,data, N * sizeof(struct registro_autos ));
    close(fd);
    PQclear(res);
    
    printf("\n   PLACA  | ID propietario | Color \n");
     for (int i = 0; i < N; i++) {
        printf(" %s  ", data[i].placa);
        printf(" %s  ", data[i].id_clie1);
        printf(" %s  \n", data[i].color);

    }
    
}

void getRegistrosAccident(char *sql){
        int fd;
    int N=3;
    char rows[5];
    

  

    res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        printf("Error al ejecutar la consulta");
    }
num_rows=PQntuples(res);
N = num_rows;
 struct registros_accident datos[N+1];//CAPTURAR nUM_FILAS EN pqNtUPLES    

    if (num_rows >0)
    {
        sprintf(rows, "%d",num_rows);
        fd_write = open(FIFO_CLIENTE, O_WRONLY);
        write(fd_write,rows,sizeof rows);
        close(fd_write);
    }else{
        sprintf(rows, "%d",num_rows);
        fd_write = open(FIFO_CLIENTE, O_WRONLY);
        write(fd_write,rows,sizeof rows);
        close(fd_write);
    }
    
     

printf("\nColumnas: %d  --- Rows: %d\n",PQnfields(res), PQnfields(res));

    
    

    for (i = 0; i < N; i++)//[N]array debe ser menor a numFilas
    {
        strcpy(datos[i].id_acc, PQgetvalue(res, i, 0));
     strcpy(datos[i].placa1, PQgetvalue(res, i, 1));
        strcpy(datos[i].fecha, PQgetvalue(res, i, 2));
        strcpy(datos[i].lugar, PQgetvalue(res, i, 3));
        strcpy(datos[i].hora, PQgetvalue(res, i, 4));      
        
    }
    printf("\nListando de Accidentes completaodo...\n");
    
    fd = open(FIFO_CLIENTE, O_WRONLY);
    write(fd,datos, N * sizeof(struct registros_accident ));
    close(fd);
    PQclear(res);
    
    printf("\nid_propietario|   PLACA  |  Fecha  |          Lugar         | hora_Accidente(24Hrs.) \n");
     for (int i = 0; i < N; i++) {
        printf(" %s  ", datos[i].id_acc);
        printf(" %s  ", datos[i].placa1);
        printf(" %s  \n", datos[i].fecha);

    }
}