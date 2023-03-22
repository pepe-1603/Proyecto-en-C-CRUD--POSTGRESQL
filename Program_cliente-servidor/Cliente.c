#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdbool.h>
#define MAX_BUF 1024
#define FIFO_SERVIDOR "fifo_servidor"
#define FIFO_CLIENTE "fifo_cliente"

//Menu Principal
void MenuPrincipal();
//Sub_menu autos
void submenu_Autos();
//Sub_menu Clientes
void submenu_Clientes();
//sub_menu Accidentes
void submenu_Accidentes();

struct Cliente
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

void imprimir_Accidentes(char *sql);

void imprimir_autos(char* sql);

void imprimir_registro(char* sql);

//funciones CRUD
void f_registrar(char* sql);
void f_listar(char* sql);
void f_eliminar(char* sql);
void f_actualizar (char* sql);


int i, j, num_cols, num_rows;

//variables Globales
char buffer_E[MAX_BUF];
    char buffer_S[MAX_BUF];
    char param_sql[200];
    int tam_paramSql;
    int fd_write;
    int fd_read;
    int opc_menuPrincipal;int opc_submenu;
    bool bandera=false;

//printf("\n\n");   scanf("%[^\n]",cad1);



int main(int argc, char const *argv[])
{
    char opc_inicio[MAX_BUF];
    bool bandera1=false, bandera2=false;
    printf("\n¡¡¡ Escribe [yes] para abrir el programa, [AnyKey] para Cancelar !!! \n\t\t\tEscribe [yes or anyKey] >> : ");
    scanf("%[^\n]",opc_inicio);
    if (0==strcmp(opc_inicio,"yes"))
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
        //si esxisten los arcxhivos fifod se procede a contectarse
        if (bandera1 == false && bandera2==false)
        {
            printf("\n\t\t==== Bienvenido Al sistema Aseguradora ====\n");
            MenuPrincipal();
        }
        
        
        
    }
    else{
        printf("Programa Canselado por el Cliente\n");
        //printf("\n\t\tAdios, Asta Pronto :] \n");
    }


    return 0;
}

//Menu Principal
void MenuPrincipal(){

    do
        {
        printf("\n============================================");
        printf("\n\t**** Menu Principal ****\n");
        printf("===============================================");
        printf("\n1.- Administrar Clientes\n");
        printf("\n2.- Administra Autos\n");
        printf("\n3.- Administrar Accidentes\n");
        printf("\n4.- Salir del Systema\n");
        printf("\n>> Respuesta: ");scanf("%d",&opc_menuPrincipal);
        printf("\n__________________________________________\n");
        system("clear");

        //eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S,"%d", opc_menuPrincipal);
        fd_write=open(FIFO_SERVIDOR,0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_menuPrincipal)
        {
        case 1:    
            submenu_Clientes(); 
            break;
        case 2:
        submenu_Autos();
            break;
        case 3:
        submenu_Accidentes();
            break;
        case 4: 
            break;
        
        
        default: printf("\nOpcion Invalida Para Menu Principal :( \n");
            break;
        }
        
    } while (opc_menuPrincipal!=4);

}
//Sub_menu autos
void submenu_Autos(){
    int opc_actualizar;
    char buscar[20];
struct registro_autos autos;

     do
    {
        printf("\n__________________________________________");
      printf("\n\t*** Submenu Autos *** \n");
      printf("__________________________________________");
        printf("\n[1] Dar dec Alta\n");
        printf("\n[2] Dar de Baja\n");
        printf("\n[3] Consultar Catalogo de Autos\n");
        printf("\n[4] Actualizar Datos\n");
        printf("\n[5] Regresar al Menu\n");
        printf("\n<--- Respuesta: ");scanf("%d",&opc_submenu);
        printf("\n__________________________________________\n");

        //eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S,"%d", opc_submenu);
        fd_write=open(FIFO_SERVIDOR,0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_submenu)
        {
        case 1:  
            
            fflush(stdin);
          printf("\nRellena el Formulario para dar de alta el Cliente\n\t\t\t ++ NOTA ++\n\t\tEl encargado del Systema No se hace responsable de los espacios en blanco que deje iusted \n");
                    printf("\nIngresa Placa: ");   scanf("%s", autos.placa);
                    printf("\nId_propietario: ");           scanf("%s",autos.id_clie1);
                    printf("\nColor: ");        scanf("%s",autos.color);
                    printf("\nMarca: ");        scanf("%s",autos.marca);
                    printf("\nModelo [mayor a 2015]: ");               scanf("%s",autos.modelo);
                    
                    fflush(stdin);
                    //enviamos sql a f_registrar 
                    sprintf(param_sql, "INSERT INTO t_auto VALUES ( '%s', %s, '%s', '%s', %s );", autos.placa, autos.id_clie1, autos.color, autos.marca, autos.modelo);
                    f_registrar(param_sql);
                    if (bandera==true)
                    {
                        printf("\nOcurrio un Eror Inesperado, Ingresaste mal los Datos \n");
                    }
                    

            break;
        case 2:     printf("\n¡Ojo!!!: La Ejecucion Tendra Exito Solo si Se Ingresa una Placa Valido\n");
                    getchar(); printf("\nIngresa la PLACA del Autos Para ELiminarlo\n\t---> PLACA: "); fflush(stdin);  scanf("%s",autos.placa);
                    sprintf(param_sql, "DELETE FROM t_auto WHERE placa = '%s' ;",autos.placa);
                    //enviamos por parametro sentencia sql Eliminar copn el id 
                    f_eliminar(param_sql);
            break;
        case 3:    // f_menuconsulta();
                        printf("\nFuera de servicio\n");
                       
        printf("\n\tConsultar Todos los Autos\n");
            //f_consult("SELECT*FROM t_auto;");
            imprimir_autos("select * from t_auto ORDER BY id_propietario1 asc;");
            break;
        case 4:
        fflush(stdin);getchar();
        printf("Ingresa la PLACA del Auto para verificar si existe: ");scanf("%[^\n]",buscar);
       // sprintf(param_sql, "SELECT * FROM t_propietario WHERE id_propietario = %s;", id_cli);
       // tam_paramSql = strlen(param_sql);
        fd_write=open(FIFO_SERVIDOR, O_WRONLY);
        write(fd_write, buscar, sizeof buscar);
        close(fd_write);//enviamos datos para la busqueda


        fd_read=open(FIFO_CLIENTE, O_RDONLY);
        read(fd_read, buffer_E, sizeof buffer_E);
        printf("\nmensaje (server)  Placa Auto Encontrado: %s\n", buffer_E);
        close(fd_read);//enviamos datos para la busqueda
       

        if (0 != strcmp(buffer_E,"No se Encontro Registro con el ID/placa PROPORCIONADO\n"))
        {        //esperamos respuesta de que si esiste o no
        printf("Entramos al metod Actualizar\n");


        printf("\n=======================================================\n");

     //termino FuncionBuscarPor id

        //hacemos Switch par actualizar cada campo del cliente

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
                    printf("\nRespuesta: ");scanf("%d",&opc_actualizar);
            /* Enviamos opc a se4rver */
            char opc[5];
            sprintf(opc, "%d",opc_actualizar);
            fd_write=open(FIFO_SERVIDOR,O_WRONLY);
            write(fd_write, opc, sizeof opc);
            close(fd_write);
            fflush(stdin);

        switch (opc_actualizar)
                {
                case 1:
                getchar();
                        printf("\n[Edit] Placa : ");fflush(stdin);   scanf("%s",autos.placa);
                       sprintf(param_sql, "UPDATE t_auto SET placa = '%s' WHERE placa = '%s';", autos.placa, buscar);
                        f_actualizar(param_sql);

                    break;
                case 2:getchar();
                        printf("\n[Edit] Id_propietario : "); fflush(stdin); scanf("%s",autos.id_clie1);
                       sprintf(param_sql, "UPDATE t_auto SET id_propietario1 = %s WHERE placa = '%s';", autos.id_clie1, buscar);
                        f_actualizar(param_sql);
                    break;
                case 3:getchar();
                        printf("\n[Edit] Color : ");   scanf("%s",autos.color);
                       sprintf(param_sql, "UPDATE t_auto SET color = '%s' WHERE placa = '%s';", autos.color, buscar);
                        f_actualizar(param_sql);
                    break;
                case 4:getchar();
                        printf("\n[Edit] Marca : "); fflush(stdin);  scanf("%s",autos.marca);
                       sprintf(param_sql, "UPDATE t_auto SET marca = '%s' WHERE placa = '%s';", autos.marca, buscar);
                        f_actualizar(param_sql);
                    break;
                case 5:getchar();
                        printf("\n[Edit] Modelo [Mayor a 2015] : ");fflush(stdin);   scanf("%s",autos.modelo);
                      sprintf(param_sql, "UPDATE t_auto SET modelo = %s WHERE placa = '%s';", autos.modelo, buscar);
                        f_actualizar(param_sql);
                    break;

                case 6:printf("\nRegresando al subMenu Auto\n");
                    break;
                
                default:
                        printf("\n---- OPCION INVALIDA PARA ACTUALIZAR LOS DATOS ----\n");
                      
                    break;
                }

        } while (opc_actualizar != 6);
        
        }

            break;
        
        default: printf("\nOpcion Invalida Para SubMenu autos :( \n");
            break;
        }  
    } while (opc_submenu!=5);
    
    
    printf("\n Finalizo Submenu Clintes\n");


}
//Sub_menu Clientes
void submenu_Clientes(){
    char cadenaSQL[200];
    //datos Personales
   int opc_submenu=0;
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
    int opc_actualizar;
    char buscar[5];

    do
    {
        printf("\n____________________________________________\n");
      printf("\n\t*** Submenu Clientes: *** \n");
      printf("\n____________________________________________\n");
        printf("\n[1] Dar dec Alta\n");
        printf("\n[2] Dar de Baja\n");
        printf("\n[3] Consultar Catalogo de Clientes\n");
        printf("\n[4] Actualizar DAtos\n");
        printf("\n[5] Regresar al Menu del Systema\n");
        printf("\n<--- Respuesta: ");scanf("%d",&opc_submenu);
        printf("\n____________________________________________\n");

        //eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S,"%d", opc_submenu);
        fd_write=open(FIFO_SERVIDOR,0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_submenu)
        {
        case 1:    printf("\nRellena el Formulario para dar de alta el Cliente\n\t\t\t ++ NOTA ++\n\t\tEl encargado del Systema No se hace responsable de los espacios en blanco que deje iusted \n");
                   fflush(stdin); getchar(); printf("\nIngresa el ID: ");   scanf("%[^\n]",id_cli);
                    getchar(); printf("\nName(s): ");           scanf("%[^\n]",name);
                    getchar(); printf("\nFirst name: ");        scanf("%[^\n]",fisrt_n);
                    getchar(); printf("\nLast name: ");        scanf("%[^\n]",last_n);
                    getchar(); printf("\nAge: ");               scanf("%[^\n]",age);
                    getchar(); printf("\nAddress: ");           scanf("%[^\n]",address);
                    getchar(); printf("\nRFC: ");               scanf("%[^\n]",rfc);
                    printf("\nSexo: ");              scanf("%s",sex);
                    
                    fflush(stdin);
                    //enviamos sql a f_registrar 
                    sprintf(cadenaSQL, "INSERT INTO t_propietario VALUES ( %s, '%s', '%s', '%s', %s, '%s', '%s', '%s');", id_cli, name, fisrt_n, last_n, age, sex, address, rfc);
                    printf("\n la VAriablker SExo: %s\n",sex);
                    printf("\nVariabole Direccion : %s\n", address);
                    printf("\nmI SOLICITUD ES: %s \n\tPress Enter....\n",cadenaSQL);
                    f_registrar(cadenaSQL);
                    if (bandera!=true)
                    {                                                                                    
                        //enviamos para registrar primeroe l producto en nuestra base de datos y despues insertamos slos telefonos con id cliente references
                    printf("\nEnter your Date phone's:  \n");
                    getchar();  printf("\n1.- Phone home: ");   scanf("%[^\n]",telhome);
                    getchar(); printf("\n2.- Cell phone: ");   scanf("%[^\n]",cellphone);
                    getchar(); printf("\n3.- Office Phone: ");   scanf("%[^\n]",teloffice);
                    sprintf(param_sql, "INSERT INTO  t_telefono (id_propietario1, tel_casa, tel_celular, tel_oficina) VALUES (%s, '%s', '%s', '%s');",id_cli, telhome, cellphone,teloffice);
                    f_registrar(param_sql);

                    }else printf("\t\tLo Sentimos :[ ,No se Puede Continuar con el Registro de Telefonos...\n");

            break;
        case 2:     printf("\n¡Ojo!!: LA Eliminacion  de un Cliente Surtira Efecto Solo Si se Ingresa un Id Valido/Existente \n");
                    getchar(); printf("\nIngresa El ID del Cliente Para ELiminarlo\n\t---> ID Cliente: ");   scanf("%[^\n]",id_cli);
                    sprintf(param_sql, "DELETE FROM t_propietario WHERE id_propietario = %s ;",id_cli);
                    //enviamos por parametro sentencia sql Eliminar copn el id 
                    f_eliminar(param_sql);
            break;
        case 3:   //  f_menuconsulta();
                        printf("\nFuera de servicio\n");
                       
        printf("\n\tConsultar Todos los clietnes\n");
            //f_consult("SELECT*FROM t_auto;");
            imprimir_registro("select * from vistaleft_propietariosTelefono ORDER BY id_propietario asc");
            break;
        case 4:
        getchar();
        printf("Ingresa el id del cliente para verificar si existe: ");scanf("%[^\n]",buscar);
       // sprintf(param_sql, "SELECT * FROM t_propietario WHERE id_propietario = %s;", id_cli);
       // tam_paramSql = strlen(param_sql);
        fd_write=open(FIFO_SERVIDOR, O_WRONLY);
        write(fd_write, buscar, sizeof buscar);
        close(fd_write);//enviamos datos para la busqueda


        fd_read=open(FIFO_CLIENTE, O_RDONLY);
        read(fd_read, buffer_E, sizeof buffer_E);
        printf("\nmensaje (server)  Nombre_cliente: %s\n", buffer_E);
        close(fd_read);//enviamos datos para la busqueda
       

        if (0 != strcmp(buffer_E,"No se Encontro Registro con el ID/placa PROPORCIONADO\n"))
        {        //esperamos respuesta de que si esiste o no
        printf("Entramos al metod Actualizar\n");


        printf("\n=======================================================\n");

     //termino FuncionBuscarPor id

        //hacemos Switch par actualizar cada campo del cliente

        do
        {
            printf("\n\t*****Si la Actulizacion no fallo , Sal del menu y vuelve a ingresar el id dedl Cliente *****\n");
                    printf("\nNo deje Campos Vacios\n");
                    printf("\n\t **** ¿Que deseas Actualizar? **** \n");
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
                    printf("\nRespuesta: ");scanf("%d",&opc_actualizar);
            /* Enviamos opc a se4rver */
            char opc[5];
            sprintf(opc, "%d",opc_actualizar);
            fd_write=open(FIFO_SERVIDOR,O_WRONLY);
            write(fd_write, opc, sizeof opc);
            close(fd_write);
            fflush(stdin);

        switch (opc_actualizar)
                {
                case 1:
                getchar();
                        printf("\nNEW ID : ");   scanf("%[^\n]",id_cli);
                       sprintf(param_sql, "UPDATE t_propietario SET id_propietario = %s WHERE id_propietario = %s;", id_cli, buscar);
                        f_actualizar(param_sql);

                    break;
                case 2:getchar();
                        printf("\nNEW NOMBRE : ");   scanf("%[^\n]",name);
                       sprintf(param_sql, "UPDATE t_propietario SET nombre = '%s' WHERE id_propietario = %s;", name, buscar);
                        f_actualizar(param_sql);
                    break;
                case 3:getchar();
                        printf("\nNEW apellido paterno : ");   scanf("%[^\n]",fisrt_n);
                       sprintf(param_sql, "UPDATE t_propietario SET ape_paterno = '%s' WHERE id_propietario = %s;", fisrt_n, buscar);
                        f_actualizar(param_sql);
                    break;
                case 4:getchar();
                        printf("\nNEW Apellido Materno : ");   scanf("%[^\n]",last_n);
                       sprintf(param_sql, "UPDATE t_propietario SET ape_materno = '%s' WHERE id_propietario = %s;", last_n, buscar);
                        f_actualizar(param_sql);
                    break;
                case 5:getchar();
                        printf("\nNEW Edad : ");   scanf("%[^\n]",age);
                       sprintf(param_sql, "UPDATE t_propietario SET edad = %s WHERE id_propietario = %s;", age, buscar);
                        f_actualizar(param_sql);
                    break;
                case 6:getchar();
                        printf("\nNEW Sexo : ");   scanf("%[^\n]",sex);
                       sprintf(param_sql, "UPDATE t_propietario SET sexo = '%s' WHERE id_propietario = %s;", sex, buscar);
                        f_actualizar(param_sql);
                    break;
                case 7:getchar();
                        printf("\nNEW direccion : ");   scanf("%[^\n]",address);
                       sprintf(param_sql, "UPDATE t_propietario SET direccion = '%s' WHERE id_propietario = %s;", address, buscar);
                        f_actualizar(param_sql);
                    break;
                case 8:getchar();
                        printf("\nNEW RFC : ");   scanf("%[^\n]",rfc);
                       sprintf(param_sql, "UPDATE t_propietario SET rfc = '%s' WHERE id_propietario = %s;", rfc, buscar);
                        f_actualizar(param_sql);
                    break;
                case 9:getchar();
                        printf("\nNEW Telefono de casa : ");   scanf("%[^\n]",telhome);
                       sprintf(param_sql, "UPDATE t_telefono SET te_casa = '%s' WHERE id_propietario1 = %s;", telhome, buscar);
                        f_actualizar(param_sql);
                    break;
                case 10:getchar();
                        printf("\nNEW Telefono Celular : ");   scanf("%[^\n]",cellphone);
                       sprintf(param_sql, "UPDATE t_telefono SET tel_celular = '%s' WHERE id_propietario1 = %s;", cellphone, buscar);
                        f_actualizar(param_sql);
                    break;
                case 11:getchar();
                        printf("\nNEW tel Oficica : ");   scanf("%[^\n]",teloffice);
                       sprintf(param_sql, "UPDATE t_telefono SET tel_oficina = '%s' WHERE id_propietario1 = %s;", teloffice, buscar);
                        f_actualizar(param_sql);
                    break;
                case 12:printf("\nRegresando al menu cliente\n");
                    break;
                
                default:
                        printf("\n---- OPCION INVALIDA PARA ACTUALÑIZAR LOS DATOS ----\n");
                      
                    break;
                }

        } while (opc_actualizar != 12);
        
            
        }

            break;
        
        default: printf("\nOpcion Invalida Para SubMenu Clientes :( \n");
            break;
        }  
    } while (opc_submenu!=5);
    
    
    printf("\n Finalizo Submenu Clintes\n");
}
//sub_menu Accidentes
void submenu_Accidentes(){
    
      int opc_actualizar;
    char buscar[20];
struct registros_accident accident;

     do
    {
        printf("\n__________________________________________");
      printf("\n\t*** Submenu Accidentes *** \n");
      printf("__________________________________________");
        printf("\n[1] Dar de Alta\n");
        printf("\n[2] Dar de Baja\n");
        printf("\n[3] Consultar Catalogo de Acidentes\n");
        printf("\n[4] Actualizar Datos\n");
        printf("\n[5] Regresar al Menu\n");
        printf("\n<--- Respuesta: ");scanf("%d",&opc_submenu);
        printf("\n__________________________________________\n");

        //eviamos la opcion aServer y esperamos respuesta
        sprintf(buffer_S,"%d", opc_submenu);
        fd_write=open(FIFO_SERVIDOR,0666);
        write(fd_write, buffer_S, sizeof buffer_S);
        close(fd_write);

        switch (opc_submenu)
        {
        case 1:  
            
            fflush(stdin);
          printf("\nRellena el Formulario para Registrar un Acciendete\n\t\t\t ++ NOTA ++\n\t\tEl encargado del Systema No se hace responsable de los espacios en blanco que deje iusted \n");
                    printf("\nIngresa ID_Accidente: ");   scanf("%s", accident.id_acc);
                    printf("\nPlaca del Auto: ");           scanf("%s",accident.placa1);
                    printf("\nFecha: ");        scanf("%s",accident.fecha);
                    printf("\nLugar: ");        scanf("%s",accident.lugar);
                    printf("\nHora Accidente (4Hrs.): ");               scanf("%s",accident.hora);
                    
                    fflush(stdin);
                    //enviamos sql a f_registrar 
                    sprintf(param_sql, "INSERT INTO t_accidente VALUES ( %s , %s, '%s', '%s', %s );", accident.id_acc, accident.placa1, accident.fecha, accident.lugar, accident.lugar, accident.hora);
                    f_registrar(param_sql);
                    if (bandera==true)
                    {
                        printf("\nOcurrio un Eror Inesperado, Ingresaste mal los Datos \n");
                    }
                    

            break;
        case 2:     printf("\n¡Ojo!!!: La Ejecucion Tendra Exito Solo si Se Ingresa una ID_accidente  Valido\n");
                    getchar(); printf("\nIngresa el ID del Aaccidente Para ELiminarlo\n\t---> ID_ACCIDENTE: "); fflush(stdin);  scanf("%s",accident.id_acc);
                    sprintf(param_sql, "DELETE FROM t_accidente WHERE id_accidente = %s ;",accident.id_acc);
                    //enviamos por parametro sentencia sql Eliminar copn el id 
                    f_eliminar(param_sql);
            break;
        case 3:    // f_menuconsulta();
                        printf("\nFuera de servicio\n");
                       
        printf("\n\tConsultar Todos los Accidentes\n");
            //f_consult("SELECT*FROM t_auto;");
            imprimir_autos("select * from t_accidente ORDER BY id_accidente asc;");
            break;
        case 4:
        fflush(stdin);getchar();
        printf("Ingresa la ID del Accidente para verificar: ");scanf("%[^\n]",buscar);
       // sprintf(param_sql, "SELECT * FROM t_propietario WHERE id_propietario = %s;", id_cli);
       // tam_paramSql = strlen(param_sql);
        fd_write=open(FIFO_SERVIDOR, O_WRONLY);
        write(fd_write, buscar, sizeof buscar);
        close(fd_write);//enviamos datos para la busqueda


        fd_read=open(FIFO_CLIENTE, O_RDONLY);
        read(fd_read, buffer_E, sizeof buffer_E);
        printf("\nmensaje (server)  ID_Accidente Encontrado: %s\n", buffer_E);
        close(fd_read);//enviamos datos para la busqueda
       

        if (0 != strcmp(buffer_E,"No se Encontro Registro con el ID/placa PROPORCIONADO\n"))
        {        //esperamos respuesta de que si esiste o no
        printf("Entramos al metodo Actualizar\n");


        printf("\n=======================================================\n");

     //termino FuncionBuscarPor id

        //hacemos Switch par actualizar cada campo del cliente

        do
        {
            printf("\n\t***** NOta: Si la Actulizacion falla , Sal del menu y vuelve a ingresar opc= 4  *****\n");
                    printf("\nNo deje Campos Vacios \n");
                    printf("\n\t****** ¿Que deseas Actualizar? ****** \n");
                    printf("\n[1] IDdel Accidente\n");
                    printf("\n[2] Cambiar Placa del auto\n");
                    printf("\n[3] Fecha de Accidente\n");
                    printf("\n[4] Lugar del Accidente\n");
                    printf("\n[5] Hora \n");
                    printf("\n[6] <<--- Regresar\n");
                    printf("\nRespuesta: ");scanf("%d",&opc_actualizar);
            /* Enviamos opc a se4rver */
            char opc[5];
            sprintf(opc, "%d",opc_actualizar);
            fd_write=open(FIFO_SERVIDOR,O_WRONLY);
            write(fd_write, opc, sizeof opc);
            close(fd_write);
            fflush(stdin);

        switch (opc_actualizar)
                {
                case 1:
                getchar();
                        printf("\n[Edit] ID_Accidente : ");fflush(stdin);   scanf("%s",accident.id_acc);
                       sprintf(param_sql, "UPDATE t_accidente SET id_accidente = %s WHERE id_accidente = %s;", accident.id_acc, buscar);
                        f_actualizar(param_sql);

                    break;
                case 2:getchar();
                        printf("\n[Edit] Placa : "); fflush(stdin); scanf("%s",accident.placa1);
                       sprintf(param_sql, "UPDATE t_accidente SET placa1 = '%s' WHERE id_accidente = %s;", accident.placa1, buscar);
                        f_actualizar(param_sql);
                    break;
                case 3:getchar();
                        printf("\n[Edit] Fecha (AAA-MM-DD): ");   scanf("%s", accident.fecha);
                       sprintf(param_sql, "UPDATE t_accidente SET fecha = '%s' WHERE id_accidente = %s;", accident.fecha, buscar);
                        f_actualizar(param_sql);
                    break;
                case 4:getchar();
                        printf("\n[Edit] Lugar del Accidente : "); fflush(stdin);  scanf("%s",accident.lugar);
                       sprintf(param_sql, "UPDATE t_accidente SET lugar = '%s' WHERE id_accidente = %s;", accident.lugar, buscar);
                        f_actualizar(param_sql);
                    break;
                case 5:getchar();
                        printf("\n[Edit]Hora del Accidente [24Hrs.] : ");fflush(stdin);   scanf("%s",accident.hora);
                      sprintf(param_sql, "UPDATE t_accidente SET hoa_formato24 = %s WHERE id_accidente = %s;", accident.hora, buscar);
                        f_actualizar(param_sql);
                    break;

                case 6:printf("\nRegresando al subMenu Accidentes\n");
                    break;
                
                default:
                        printf("\n---- OPCION INVALIDA PARA ACTUALIZAR LOS DATOS ----\n");
                      
                    break;
                }

        } while (opc_actualizar != 6);
        
        }

            break;
        
        default: printf("\nOpcion Invalida Para SubMenu Accdente :( \n");
            break;
        }  
    } while (opc_submenu!=5);
    
    
    printf("\n Finalizo Submenu Accidentes\n");
}


//funciones CRUD
void f_registrar(char* sql){
    //escribimos sentencia SQL a fifo para el server
    tam_paramSql= strlen(sql);
    fd_write= open(FIFO_SERVIDOR,O_WRONLY);
    write(fd_write, sql, tam_paramSql);//escribimos el tamaño del sql
    close(fd_write);

    printf("Mi Solicitud:  %s\n",sql);
    //y esperamos respuesta de  Exito
    
    fd_read=open(FIFO_CLIENTE, O_RDONLY);
    read(fd_read, buffer_E,sizeof buffer_E);
    close(fd_read);
    printf("\nMensaje (Server) : %s\n", buffer_E);
    printf("F_registrar Teminado\n");

    if (0==strcmp(buffer_E, "\n\t\t Algo Salio Mal :[ ,Consulta Fallida\n"))
    {
        //printf();
        bandera=true;
    }

}

void f_eliminar(char* sql){
    //escribimos Sentencia SQL en el FIFO para el server
    tam_paramSql=strlen(sql);
    fd_write=open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, tam_paramSql);
    close(fd_write);

    //y esperamos respuesta del servidsor acerca de la consutla
    fd_read=open(FIFO_CLIENTE, O_RDONLY);
    read(fd_read, buffer_E, sizeof buffer_E);
    close(fd_read);
    printf("\nMensaje (server) : %s\n", buffer_E);
    printf("F_Eliminar Terminado\n");

}
void f_actualizar (char* sql){

    tam_paramSql = strlen(sql);
    fd_write=open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, tam_paramSql);
    close(fd_write);

    //y esperamos respuesta del servidsor acerca de la actualñizacion
    fd_read=open(FIFO_CLIENTE, O_RDONLY);
    read(fd_read, buffer_E, sizeof buffer_E);
    close(fd_read);
    printf("\nMensaje (server) : %s\n", buffer_E);
    printf("F_Actualizar Terminado\n");


}


void imprimir_registro(char* sql){

int fd;
int N =30;
char rows[5];




     fd_write=open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, strlen(sql)+1);
    close(fd_write);//select*from t_susuarios

//eperamos los datos o el mensaje
//char **data;
fd=open(FIFO_CLIENTE, O_RDONLY);
read(fd, rows,sizeof(rows) );
close(fd);

N = atoi(rows) ;
struct Cliente array_datos[N];

fd=open(FIFO_CLIENTE, O_RDONLY);
read(fd, array_datos, N * sizeof(struct Cliente) );
close(fd);

// Imprimir los datos recibidos
printf("\n--------------------------------------------------------------------------------------------------------------------------------------------");
printf("\nFila | id | Nombre | 1erApellido | 2doApellido | Edad |    sexo    |       Direccion     |     RFC    |  telCasa  |  celular  | telOficina\n");
printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < N; i++) {
        printf("[%i]  %s  ",(i+1) ,array_datos[i].id_cli);
        printf("  %s  ", array_datos[i].name);
        printf("  %s  ", array_datos[i].fisrt_n);
        printf("  %s  ", array_datos[i].last_n);
        printf("  %s  ", array_datos[i].age);
        printf("   %s  ", array_datos[i].sex);
        printf("     %s  ", array_datos[i].address);
        printf("  %s  ", array_datos[i].rfc);
        printf("  %s  ", array_datos[i].telhome);
        printf("  %s  ", array_datos[i].cellphone);
        printf("  %s  \n", array_datos[i].teloffice);

    }
printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void imprimir_autos(char* sql){

int fd;
int N = 5;char rows[5];



     fd_write=open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, strlen(sql)+1);
    close(fd_write);//select*from t_susuarios

//eperamos los datos o el mensaje
//char **data;
fd=open(FIFO_CLIENTE, O_RDONLY);
read(fd, rows,sizeof(rows) );
close(fd);

N = atoi(rows) ;

struct registro_autos array_datos[N];

fd=open(FIFO_CLIENTE, O_RDONLY);
read(fd, array_datos, N * sizeof(struct registro_autos) );
close(fd);
fflush(stdin);
// Imprimir los datos recibidos
printf("\n---------------------------------------------------------");
printf("\n     |   Placa  |Id_client|   color  |   Marca  |Modelo|\n");
printf("---------------------------------------------------------\n");
    for (int i = 0; i < N; i++) {
        printf("[%i]  | %s  ",(i+1) ,array_datos[i].placa);
        printf("  %s   ", array_datos[i].id_clie1);
        printf(" %s  ", array_datos[i].color);
        printf(" %s  ", array_datos[i].marca);
        printf(" %s  \n", array_datos[i].modelo);

    }
printf("----------------------------------------------------------\n");
}


void imprimir_Accidentes(char *sql){
    int fd;
int N = 5;char rows[5];



     fd_write=open(FIFO_SERVIDOR, O_WRONLY);
    write(fd_write, sql, strlen(sql)+1);
    close(fd_write);//select*from t_susuarios

//eperamos los datos o el mensaje
//char **data;
fd=open(FIFO_CLIENTE, O_RDONLY);
read(fd, rows,sizeof(rows) );
close(fd);

N = atoi(rows) ;

struct registros_accident array[N];

fd=open(FIFO_CLIENTE, O_RDONLY);
read(fd, array, N * sizeof(struct registros_accident) );
close(fd);
fflush(stdin);
// Imprimir los datos recibidos
printf("\n---------------------------------------------------------");
printf("\n     |id_Accidente |  Placa  | fecha |          Lugar          | Hora Accidente (24Hrs.)|\n");
printf("---------------------------------------------------------\n");
    for (int i = 0; i < N; i++) {
        printf("[%i]  | %s  ",(i+1) ,array[i].id_acc);
        printf("  %s   ", array[i].placa1);
        printf(" %s  ", array[i].fecha);
        printf(" %s ", array[i].lugar);
        printf("  %s  \n", array[i].hora);

    }
printf("----------------------------------------------------------\n");
}


