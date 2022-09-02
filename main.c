#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib.h"

// OMAR RODRIGUEZ ALVAREZ 
// 48679912-K 

// compilar con en windows con: --->   gcc lib.c main.c -o main.exe

int main(){

    char prompt[100] = "*";
    char com[100];
    char comaux[100];
    char confirmar[100];
    char nombre_bd[100] = "*";
    char nombre_tabla[100] = "*";

    int num_arg, i, j,  auxiliar, num_arg_aux, num_arg_comfirmar;
    int auxliar_enteros, auxiliar_comprobante;
    char s2[8] = " \n\t\r";

    char *comtok[100], *rem[100];
    char *comillas, *comillasaux;
    char **nombre, **valores;
    TYPE *tipos;
    TABLE *LaGranTabla = NULL, *tabla_auxiliar = NULL, *tabla_auxiliar2 = NULL;
    ROW *LaGranLinea=NULL, *linea_auxiliar;

    datos_alumno();

    while(1){

        printf("[%s]:>", prompt);
        gets(com);
        strcpy(comaux, com);
        num_arg = 0;
        comtok[num_arg] = strtok(com, s2);

        num_arg++;

        while((comtok[num_arg] = strtok(NULL, s2)) != NULL){
            num_arg++;
        }

        if(comtok[0] == NULL){ 
            continue;
        }

        if(num_arg == 1 && strcmp(comtok[0], "exit") != 0 && strcmp(comtok[0], "select") != 0){
            printf("error\n");
            continue;
        }

        if(strcmp(comtok[0], "exit") == 0){

            if(num_arg != 1){
                printf("Numero de argumentos incorrectos\n");
                continue;
            }

            if(LaGranTabla != NULL){
                if(LaGranLinea != NULL){
                    liberar_memoria_lineas(LaGranLinea, tabla_auxiliar->numCampos);
                    LaGranLinea = NULL;
                }

                liberar_memoria_tablas(LaGranTabla);

                LaGranTabla = NULL;
                return 0;

            }
            else if(LaGranTabla == NULL){

                return 0;

            }
            else{

                printf("Error algo salio mal\n");

            }

        }
        else if(strcmp(comtok[0], "new") == 0 && strcmp(comtok[1], "db") == 0){

            if(num_arg != 3){
                printf("ASI NO SE CREAN BD\n");
                continue;
            }

            auxliar_enteros = validar_caracteres(comtok[2]);

            if(auxliar_enteros == 0){
                printf("nombre de la carpeta bn escrito\n");

                crear_bd(comtok[2]);
            }
            else{
                printf("que el nombre de la carpeta esta mal escrito\n");
            }


        }
        else if(strcmp(comtok[0], "use") == 0 && strcmp(comtok[1], "db") == 0){

            if(num_arg != 3){
                printf("Error numero de argumentos incorrecto\n");
                continue;
            }

            if(strcmp(comtok[2], ".") == 0 || strcmp(comtok[2], "..") == 0){
                printf("NO\n");
                continue;
            }

            auxliar_enteros = entrar_dir(comtok[2]); //si = 0 y no = 1

            if(auxliar_enteros == 0){

                strcpy(prompt, comtok[2]);
                strcpy(nombre_bd, comtok[2]);
                strcpy(nombre_tabla, "*"); 

            }
            else{
                continue;
            }

            // LIBERAR MEMORIA
            if(LaGranTabla != NULL){
                if(LaGranLinea != NULL){
                    liberar_memoria_lineas(LaGranLinea, tabla_auxiliar->numCampos);
                    LaGranLinea = NULL;
                }

                liberar_memoria_tablas(LaGranTabla);
                LaGranTabla = NULL;

            }


            LaGranTabla = cargarTablas(nombre_bd);

            mostrar_tablas(LaGranTabla);
          
        }
        else if(strcmp(comtok[0], "new") == 0 && strcmp(comtok[1], "table") == 0){
            if(num_arg<5){
                printf("numero de comandos insuficientes\n");
                continue;
            }
            else if(num_arg % 2 == 0){

                printf("numero de comandos incorrecto, numero par\n");
                continue;
            }
            else if(strcmp(prompt, "*") == 0){

                printf("ERROR base de datos inactiva\n");
                continue;
            }

            for(i=2; i<num_arg; i++){
                auxliar_enteros = validar_caracteres(comtok[i]); 

                if(auxliar_enteros>0){
                    printf("Mala escritura\n");
                    break;
                }
            }

            if(auxliar_enteros>0){
                continue; 
            }

            int num_campos = (num_arg-3)/2; 

            nombre = malloc(sizeof(char*)*(num_campos)); 
            tipos = malloc(sizeof(TYPE*)*(num_campos)); 
            auxiliar = 0;
            auxiliar_comprobante = 0;

            for(i=3; i<num_arg; i++){

                if(i % 2 == 1){
                    nombre[auxiliar] = strdup(comtok[i]);
                }
                else{
                    auxliar_enteros = validar_tipo(comtok[i]);

                    if(auxliar_enteros == 4){
                        printf("ERROR DE TIPOS\n");
                        auxiliar_comprobante = 1;
                        break;
                    }

                    tipos[auxiliar] = validar_tipo(comtok[i]);
                    auxiliar++;
                }
            }

            i = ver_duplicados(nombre, auxiliar);

            j = entrar_tabla(nombre_bd, comtok[2]);

            if(j != 0){
                printf("error\n");
                continue;
            }

            if(auxiliar_comprobante == 0 && i == 0 && j == 0){
                tabla_auxiliar2 = inicializar_tablas(num_campos, comtok[2],nombre, tipos);
            

                if(tabla_auxiliar2 != NULL){

                    if(LaGranTabla == NULL){

                        LaGranTabla = tabla_auxiliar2;
                        crear_tabla_tab(LaGranTabla, nombre_bd, comtok[2]);

                    }
                    else{
                        insertar_ultima(LaGranTabla, tabla_auxiliar2);
                        crear_tabla_tab(LaGranTabla, nombre_bd, comtok[2]);

                    }

                }
            }

        }
        else if(strcmp(comtok[0], "use") == 0 && strcmp(comtok[1], "table") == 0){

            if(num_arg != 3){
                printf("Numero de argumentos incorrecto\n");
                continue;
            }
            if(strcmp(prompt, "*") == 0){

                printf("No hay una bases de datos activas\n");
                continue;
            }

            auxiliar_comprobante = entrar_tabla(nombre_bd, comtok[2]);

            if(auxiliar_comprobante == 1){
                
                strcpy(prompt, nombre_bd);
                strcpy(nombre_tabla, comtok[2]);
                strcat(prompt, "/");
                strcat(prompt, nombre_tabla);

                if(LaGranLinea != NULL){
                    liberar_memoria_lineas(LaGranLinea, tabla_auxiliar->numCampos);
                    
                }

                LaGranLinea = NULL;
  
            }
            else{
                
                continue;
            }

            printf("2\n");

            /*if(LaGranTabla->ant != NULL){

                while(LaGranTabla != NULL){
                    LaGranTabla=LaGranTabla->ant;
                    if(LaGranTabla->ant == NULL) break;

                    printf("%s\n" , LaGranTabla->nombre);
                }
            }*/
            
            tabla_auxiliar = buscar_tabla(nombre_tabla, LaGranTabla);


            
            LaGranLinea = cargar_lineas(nombre_bd, nombre_tabla, tabla_auxiliar->numCampos, tabla_auxiliar->tipos);

    
            //INICIALIZACION DE LINEAS
            /*linea_auxiliar = LaGranLinea;

            while(linea_auxiliar != NULL){
                for(i=0; i<tabla_auxiliar->numCampos; i++){
                    printf("[%s]", linea_auxiliar->valores[i]);
                }
                printf("\n");

                linea_auxiliar = linea_auxiliar->sig;
            }*/

        }
        else if(strcmp(comtok[0],"insert")==0){

            if(strcmp(prompt, "*") == 0){
                printf("ERROR no hay ninguna bd activa\n");
                continue;
            }

            if(strcmp(prompt, nombre_bd) == 0){
                printf("ERROR no hay ninguna tabla activa\n");
                continue;
            }

            auxiliar_comprobante = 0;
            comillas = strchr(comaux, '"');

            if( comillas != NULL){

                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL){
                    //ERROR
                    printf("ERROR DE COMILLAS\n");
                    auxiliar_comprobante++;
                    continue;
                }
                else{
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                    num_arg_aux++;
                    
                    while((comtok[num_arg_aux] = MiStrTok(NULL, s2, 1)) != NULL){ 
                        num_arg_aux++;
                    }
                }
            }
            else{
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = MiStrTok(NULL, s2, 0)) != NULL){ 
                    num_arg_aux++;
                }

            }

            if(num_arg_aux != (tabla_auxiliar->numCampos+1)){
                printf("error, numero de campos incorrecto\n");
                continue;
            }

            for(i=0; i<tabla_auxiliar->numCampos; i++){

                if(tabla_auxiliar->tipos[i] == NUM){
                    if(validar_decimal(comtok[i+1]) == 0){
                        printf("error al insertar un numero\n");
                        auxiliar_comprobante++;
                        break;
                    }

                }
                else if(tabla_auxiliar->tipos[i] == DATE){
                    if(FechaValidarDMA(comtok[i+1]) == 0){
                        printf("error al insertar la fecha\n");
                        auxiliar_comprobante++;
                        break;

                    }

                }
                else if(tabla_auxiliar->tipos[i] == EMAIL){

                    if(validar_email(comtok[i+1]) != 0){
                        printf("El correo esta mal escrito\n");
                        auxiliar_comprobante++;
                        break;
                    }
                }
                else if(tabla_auxiliar->tipos[i] == TEXT){

                    if(validar_email(comtok[i+1]) == 0 || FechaValidarDMA(comtok[i+1]) == 1 || validar_decimal(comtok[i+1]) == 1){

                        printf("error esto no se puede insertar en un campo tipo texto\n");
                        auxiliar_comprobante++;
                        break;

                    }
                }

            }


            if(auxiliar_comprobante != 0){
                continue;
            }

            //RESERVA MEMORIA
            valores = malloc(sizeof(char*)*tabla_auxiliar->numCampos); 

            for(i = 0; i < tabla_auxiliar->numCampos; i++){
                valores[i] = strdup(comtok[i+1]);
            }

            for(i=0; i< tabla_auxiliar->numCampos; i++){
                printf("%s\n", valores[i]);
            }

            //inicializacion
            linea_auxiliar = inicializar_linea(valores);

            if(linea_auxiliar != NULL){
                if(LaGranLinea == NULL){

                    LaGranLinea = linea_auxiliar;

                    actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                }
                else{
                    insertarUltimaLinea(LaGranLinea, linea_auxiliar);
                    actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);

                }
            }
        }
        else if(strcmp(comtok[0], "select") == 0){
            
            if(LaGranTabla == NULL){
                printf("Todavia no hay una bd cargada en memoria\n");
                continue;
            }
            if(strcmp(prompt, nombre_bd) == 0){
                printf("Todavia no hay una tabla activa\n");
                continue;
            }


            comillas = strchr(comaux, '"');

            if(comillas != NULL){

                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL){
                    printf("ERROR de comillas insuficientes");
                    break;
                }
                else{
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                    num_arg_aux++;

                    while((comtok[num_arg_aux] = MiStrTok(NULL, s2, 1)) != NULL){
                        //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                        num_arg_aux++;
                    }
                }
            }
            else{
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = MiStrTok(NULL, s2, 0)) != NULL){
                    //printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                    num_arg_aux++;
                }

            }

            // SELECT

            if(num_arg_aux == 1){ 
                impr_cabeceras(tabla_auxiliar);
                imprimir_todo(LaGranLinea, tabla_auxiliar->numCampos);
            }
            else if(num_arg_aux == 2){   

                if(strcmp(comtok[1], "ASC") == 0){  

                    impr_cabeceras(tabla_auxiliar);
                    imprimir_todo_ACS(LaGranLinea, tabla_auxiliar->numCampos, tabla_auxiliar);

                }
                else if(strcmp(comtok[1], "DES") == 0){

                    impr_cabeceras(tabla_auxiliar);
                    imprimir_todo_DES(LaGranLinea, tabla_auxiliar->numCampos, tabla_auxiliar);


                }
                else{
                    printf("Error segundo parametro mal introducido\n");
                }

            }
            else if(num_arg_aux == 4){ 
                i = 0;
                for(i ; i<tabla_auxiliar->numCampos; i++){

                    if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                        if(tabla_auxiliar->tipos[i] == NUM){

                            if(validar_decimal(comtok[3]) == 1){

                                impr_cabeceras(tabla_auxiliar);
                                imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                break;

                            }
                            else{
                                printf("Error en las columnas\n");
                                break;
                            }
                        }
                        else if(tabla_auxiliar->tipos[i] == DATE){

                            if(FechaValidarDMA(comtok[3]) == 1){

                                impr_cabeceras(tabla_auxiliar);
                                imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                break;

                            }
                            else{
                                printf("error de tipos con las columnas\n");
                                break;
                            }
                        }
                        else if(tabla_auxiliar->tipos[i] == EMAIL){

                            printf("%s -- %d\n", tabla_auxiliar->campos[i], tabla_auxiliar->tipos[i]);

                            if(validar_email(comtok[3]) == 0){

                                impr_cabeceras(tabla_auxiliar);
                                imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                break;

                            }
                            else{
                                printf("error de tipos con las columnas\n");
                                break;
                            }
                        }
                        else if(tabla_auxiliar->tipos[i] == TEXT){

                            impr_cabeceras(tabla_auxiliar);
                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                            break;

                        }
                        else{
                            printf("error de tipos\n");
                            i++;
                            continue;
                        }
                    }

                    if(i+1 == tabla_auxiliar->numCampos){
                        printf("Campo no encontrado");
                    }
                }

            }
            else if(num_arg_aux == 5){

                if(strcmp(comtok[4], "ASC") == 0){

                    i = 0;
                    for(i ; i<tabla_auxiliar->numCampos; i++){

                        if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                            if(tabla_auxiliar->tipos[i] == NUM){

                                if(validar_decimal(comtok[3]) == 1){

                                    impr_cabeceras(tabla_auxiliar);
                                    ordenar_asc(LaGranLinea, tabla_auxiliar, 0);
                                    imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                    break;

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == DATE){

                                if(FechaValidarDMA(comtok[3]) == 1){

                                    impr_cabeceras(tabla_auxiliar);
                                    ordenar_asc(LaGranLinea, tabla_auxiliar, 0);

                                    imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                    break;

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == EMAIL){

                                if(validar_email(comtok[3]) == 1){

                                    impr_cabeceras(tabla_auxiliar);
                                    ordenar_asc(LaGranLinea, tabla_auxiliar, 0);

                                    imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                    break;

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == TEXT){

                                impr_cabeceras(tabla_auxiliar);

                                ordenar_asc(LaGranLinea, tabla_auxiliar, 0);
                                imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                break;

                            }
                            else{
                                printf("error de tipos\n");
                                i++;
                                continue;
                            }
                        }

                        if(i+1 == tabla_auxiliar->numCampos){
                            printf("Campo no encontrado");
                        }
                    }
                }
                if(strcmp(comtok[4], "DES") == 0){

                    i = 0;
                    for(i ; i<tabla_auxiliar->numCampos; i++){

                        if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                            if(tabla_auxiliar->tipos[i] == NUM){

                                if(validar_decimal(comtok[3]) == 1){

                                    impr_cabeceras(tabla_auxiliar);
                                    ordenar_des(LaGranLinea, tabla_auxiliar, 0);
                                    imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                    break;

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == DATE){

                                if(FechaValidarDMA(comtok[3]) == 1){

                                    impr_cabeceras(tabla_auxiliar);
                                    ordenar_des(LaGranLinea, tabla_auxiliar, 0);
                                    imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                    break;

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == EMAIL){

                                if(validar_email(comtok[3]) == 1){

                                    impr_cabeceras(tabla_auxiliar);
                                    ordenar_des(LaGranLinea, tabla_auxiliar, 0);
                                    imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                    break;

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == TEXT){

                                impr_cabeceras(tabla_auxiliar);
                                ordenar_des(LaGranLinea, tabla_auxiliar, 0);
                                imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                break;

                            }
                            else{
                                printf("error de tipos\n");
                                i++;
                                continue;
                            }
                        }

                        if(i+1 == tabla_auxiliar->numCampos){
                            printf("Campo no encontrado");
                        }
                    }
                }
            }
            else if(num_arg_aux == 6){

                auxiliar_comprobante = 0;
                auxliar_enteros = 0;

                if(strcmp(comtok[4], "ASC") == 0){

                    i = 0;
                    for(i ; i<tabla_auxiliar->numCampos; i++){

                        if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                            if(tabla_auxiliar->tipos[i] == NUM){

                                if(validar_decimal(comtok[3]) == 1){

                                    for(j=0; j<tabla_auxiliar->numCampos; j++){

                                        if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                            impr_cabeceras(tabla_auxiliar);
                                            ordenar_asc(LaGranLinea, tabla_auxiliar, j);
                                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                            auxliar_enteros++;
                                            break;


                                        }

                                    }
                                    if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                        printf("Campo no encontrado");
                                        auxiliar_comprobante++;
                                        break;
                                    }

                                    

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == DATE){

                                if(FechaValidarDMA(comtok[3]) == 1){

                                    for(j=0; j<tabla_auxiliar->numCampos; j++){

                                        if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                            impr_cabeceras(tabla_auxiliar);
                                            ordenar_asc(LaGranLinea, tabla_auxiliar, j);
                                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                            auxliar_enteros++;
                                            break;


                                        }

                                    }
                                    if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                        printf("Campo no encontrado");
                                        auxiliar_comprobante++;
                                        break;
                                    }

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == EMAIL){

                                if(validar_email(comtok[3]) == 1){

                                    for(j=0; j<tabla_auxiliar->numCampos; j++){

                                        if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                            impr_cabeceras(tabla_auxiliar);
                                            ordenar_asc(LaGranLinea, tabla_auxiliar, j);
                                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                            auxliar_enteros++;
                                            break;


                                        }

                                    }
                                    if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                        printf("Campo no encontrado");
                                        auxiliar_comprobante++;
                                        break;
                                    }

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == TEXT){

                                for(j=0; j<tabla_auxiliar->numCampos; j++){

                                    if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                        impr_cabeceras(tabla_auxiliar);
                                        ordenar_asc(LaGranLinea, tabla_auxiliar, j);
                                        imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                        auxliar_enteros++;
                                        break;


                                    }

                                }
                                if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                    printf("Campo no encontrado");
                                    auxiliar_comprobante++;
                                    break;
                                }

                            }
                            else{
                                printf("error de tipos\n");
                                i++;
                                continue;
                            }
                        }

                        if(auxiliar_comprobante != 0){
                            continue;
                        }

                        if(i+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                            printf("Campo no encontrado1");
                        }
                    }
                }
                if(strcmp(comtok[4], "DES") == 0){

                    i = 0;
                    for(i ; i<tabla_auxiliar->numCampos; i++){

                        if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                            if(tabla_auxiliar->tipos[i] == NUM){

                                if(validar_decimal(comtok[3]) == 1){

                                    for(j=0; j<tabla_auxiliar->numCampos; j++){

                                        if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                            impr_cabeceras(tabla_auxiliar);
                                            ordenar_des(LaGranLinea, tabla_auxiliar, j);
                                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                            auxliar_enteros++;
                                            break;


                                        }

                                    }
                                    if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                        printf("Campo no encontrado");
                                        auxiliar_comprobante++;
                                        break;
                                    }

                                    

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == DATE){

                                if(FechaValidarDMA(comtok[3]) == 1){

                                    for(j=0; j<tabla_auxiliar->numCampos; j++){

                                        if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                            impr_cabeceras(tabla_auxiliar);
                                            ordenar_des(LaGranLinea, tabla_auxiliar, j);
                                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                            auxliar_enteros++;
                                            break;


                                        }

                                    }
                                    if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                        printf("Campo no encontrado");
                                        auxiliar_comprobante++;
                                        break;
                                    }

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == EMAIL){

                                if(validar_email(comtok[3]) == 1){

                                    for(j=0; j<tabla_auxiliar->numCampos; j++){

                                        if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                            impr_cabeceras(tabla_auxiliar);
                                            ordenar_des(LaGranLinea, tabla_auxiliar, j);
                                            imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                            auxliar_enteros++;
                                            break;


                                        }

                                    }
                                    if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                        printf("Campo no encontrado");
                                        auxiliar_comprobante++;
                                        break;
                                    }

                                }
                                else{
                                    printf("error de tipos con las columnas\n");
                                    break;
                                }
                            }
                            else if(tabla_auxiliar->tipos[i] == TEXT){

                                for(j=0; j<tabla_auxiliar->numCampos; j++){

                                    if(strcmp(tabla_auxiliar->campos[j], comtok[5]) == 0){

                                        impr_cabeceras(tabla_auxiliar);
                                        ordenar_des(LaGranLinea, tabla_auxiliar, j);
                                        imprimir_valor_campo(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                                        auxliar_enteros++;
                                        break;


                                    }

                                }
                                if(j+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                                    printf("Campo no encontrado");
                                    auxiliar_comprobante++;
                                    break;
                                }

                            }
                            else{
                                printf("error de tipos\n");
                                i++;
                                continue;
                            }
                        }

                        if(auxiliar_comprobante != 0){
                            continue;
                        }

                        if(i+1 == tabla_auxiliar->numCampos && auxliar_enteros == 0){
                            printf("Campo no encontrado1");
                        }
                    }
                }
            }
            else{
                printf("Estamos en un caso de numero de parametros incorrectos\n");
            }
        }
        else if(strcmp(comtok[0], "list")==0){

            if(num_arg != 2){
                printf("Numero de argumentos incorrecto\n");
                continue;
            }

            if(strcmp(comtok[1], "db") == 0){
                mostrar_bd();
            }
            else if(strcmp(comtok[1], "table") == 0){

                if(strcmp(prompt, "*") == 0){
                    printf("ERROR no hay una base de datos activa\n");
                    continue;
                }

                mostrar_tablas(LaGranTabla);

            }
            else{
                printf("Los parametros indicados son erroneos\n");
            }
        }
        else if(strcmp(comtok[0], "delete") == 0){
            if(LaGranTabla == NULL){
                printf("error no hay una bd activa\n");
                continue;
            }

            if(strcmp(nombre_bd, prompt) == 0){
                printf("error no hay una table activa\n");
                continue;
            }

            comillas = strchr(comaux, '"');

            if(comillas != NULL){

                comillasaux = strchr(comillas+1, '"');

                if(comillasaux == NULL){
                    printf("error de comillas insuficientes");
                    break;
                }
                else{
                    num_arg_aux = 0;
                    comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                    num_arg_aux++;

                    while((comtok[num_arg_aux] = MiStrTok(NULL, s2, 1)) != NULL){
                        printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                        num_arg_aux++;
                    }
                }
            }
            else{
                num_arg_aux = 0;
                comtok[num_arg_aux] = MiStrTok(comaux, s2, 0);
                num_arg_aux++;

                while((comtok[num_arg_aux] = MiStrTok(NULL, s2, 0)) != NULL){
                    printf("%d, %s\n", num_arg_aux, comtok[num_arg_aux]);
                    num_arg_aux++;
                }

            }


            if(num_arg_aux != 4){ 
                printf("error numero de argumentos\n");
                continue;
            }

            for(i=0; i<tabla_auxiliar->numCampos; i++){ 

                if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                    if(tabla_auxiliar->tipos[i] == NUM && validar_decimal(comtok[3]) == 1){

                        LaGranLinea = borrarLineaValorDeseado(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                        actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                        break;

                    }
                    else if(tabla_auxiliar->tipos[i] == DATE && FechaValidarDMA(comtok[3]) == 1){

                        LaGranLinea = borrarLineaValorDeseado(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                        actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                        break;

                    }else if(tabla_auxiliar->tipos[i] == EMAIL && validar_email(comtok[3]) == 0){

                        LaGranLinea = borrarLineaValorDeseado(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                        actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                        break;

                    }else if(tabla_auxiliar->tipos[i] == TEXT){

                        LaGranLinea = borrarLineaValorDeseado(tabla_auxiliar->numCampos, LaGranLinea, tabla_auxiliar->tipos[i], comtok[3], comtok[2], i);
                        actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                        break;

                    }
                    else{
                        printf("error de parametros, mira a ver que has escrito y ponlo bn anda\n");
                        continue;
                    }
                }
            }
        }
        else if(strcmp(comtok[0], "remove") == 0){
            
            if(num_arg != 2){
                printf("error numero de argumentos incorrecto\n");
                continue;
            }

            if(strcmp(prompt, "*") == 0){
                printf("error no hay una bd activa\n");
                continue;
            }

            printf("Introduzca s en minuscula para confirmar la orden\n");
            gets(confirmar);

            num_arg_comfirmar = 0;
            rem[num_arg_comfirmar] = strtok(confirmar, s2);

            num_arg_comfirmar++;

            while((rem[num_arg_comfirmar] = strtok(NULL, s2)) != NULL){
                num_arg_comfirmar++;
            }

            if(rem[0] == NULL){
                printf("Comando no valido para confirmar la orden\n");
                continue;
            }

            if(strcmp(rem[0], "s") != 0){
                printf("Comando no valido para confirmar la orden\n");
                continue;
            }


            auxiliar_comprobante = entrar_tabla(nombre_bd, comtok[1]);

            if(auxiliar_comprobante == 0){
                printf("No hemos encontrado la tabla indicada\n");
                continue;
            }

            if(strcmp(nombre_tabla, comtok[1]) == 0){
                strcpy(prompt, nombre_bd);
                strcpy(nombre_tabla, "*");
            }

            LaGranTabla = borrar_tabla(LaGranTabla, comtok[1]);

            char aux_ruta[100] = "";

            strcpy(aux_ruta, nombre_bd);
            strcat(aux_ruta, "\\");
            strcat(aux_ruta, comtok[1]);
            strcat(aux_ruta, ".tab");

            remove(aux_ruta);

            editar_meta(nombre_bd, LaGranTabla);
        }
        else if(strcmp(comtok[0], "order") == 0){

            if(num_arg != 3){
                printf("Error numero de argumentos incorercto\n");
                continue;
            }

            if(strcmp(prompt,"*") == 0){
                printf("error no hay una base de datos activa\n");
                continue;
            }

            if(strcmp(prompt, nombre_bd) == 0){
                printf("error no hay ninguna tabla activa\n");
                continue;
            }

            if(LaGranLinea == NULL){
                printf("error no hay datos\n");
            }

            if(strcmp(comtok[2], "ASC") == 0){

                for(i=0; i<tabla_auxiliar->numCampos; i++){

                    if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                        if(tabla_auxiliar->tipos[i] == NUM){

                            ordenar_asc(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;

                        }
                        else if(tabla_auxiliar->tipos[i] == DATE){
                            ordenar_asc(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;
                        }
                        else if(tabla_auxiliar->tipos[i] == EMAIL){
                            ordenar_asc(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;
                        }
                        else if(tabla_auxiliar->tipos[i] == TEXT){
                            ordenar_asc(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;

                        }
                        else{
                            printf("error tipos de campos\n");
                        }
                    }

                    if(i+1 == tabla_auxiliar->numCampos){
                        printf("Campo no encontrado");
                    }

                }

            }
            else if(strcmp(comtok[2], "DES") == 0){

                for(i=0; i<tabla_auxiliar->numCampos; i++){

                    if(strcmp(tabla_auxiliar->campos[i], comtok[1]) == 0){

                        if(tabla_auxiliar->tipos[i] == NUM){

                            ordenar_des(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;

                        }
                        else if(tabla_auxiliar->tipos[i] == DATE){
                            ordenar_des(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;
                        }
                        else if(tabla_auxiliar->tipos[i] == EMAIL){
                            ordenar_des(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;
                        }
                        else if(tabla_auxiliar->tipos[i] == TEXT){
                            ordenar_des(LaGranLinea, tabla_auxiliar, i);
                            actualizar_lineas(nombre_bd, LaGranLinea, tabla_auxiliar);
                            break;

                        }
                        else{
                            printf("error tipos de campos\n");
                        }

                    }

                    if(i+1 == tabla_auxiliar->numCampos){
                        printf("Campo no encontrado");
                    }

                }

            }
            else{ 
                printf("Error con el tercer argumento\n");
                continue;
            }



        }
        else if(strcmp(comtok[0], "import") == 0 && strcmp(comtok[1], "table") == 0 && strcmp(comtok[3], "from") == 0 && strcmp(comtok[5], "as") == 0){

            if(num_arg != 7){
                printf("Error numero de argumentos\n");
                continue;
            }

            if(strcmp(prompt, "*") == 0){
                printf("error no hay yna base de datos activa\n");
                continue;
            }

            if(strcmp(nombre_bd, comtok[4]) == 0){
                printf("error, no se puede usar la misma base de datos\n");
                continue;
            }

            if(entrar_dir(comtok[4]) == 1){ 
                continue;
            }

            if(entrar_tabla(comtok[4], comtok[2]) == 0){ 
                printf("La tabla a copiar no existe 1\n");
                continue;
            }

            // if(entrar_tabla(nombre_bd, comtok[2]) == 1){ 
            //     printf("La tabla a copiar existe 2\n");
            //     continue;
            // }

            if(entrar_tabla(nombre_bd, comtok[6]) == 1){ // aqui comprobamos que el nombre de la nueva tabla de destino coincida con el nombre de alguna de las tablas ya existentes en la base de datos activa
                printf("No puedes llamar la tabla asi por que ya existe una con el mismo nombre en la bd de destino\n");
                continue;
            }

            LaGranTabla = seleccionarTablaOtraBD(comtok[2], comtok[4], nombre_bd, LaGranTabla, comtok[6]);

            mostrar_tablas(LaGranTabla);
        }
        else{
            printf("No se reconoce el orden\n");
            continue;
        }
    }
}





