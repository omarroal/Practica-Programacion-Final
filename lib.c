#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lib.h"

void datos_alumno(){

    printf("\n OMAR RODRIGUEZ ALVAREZ\n");
    printf(" 48679912K\n");
    printf(" omar.rodriguez@goumh.umh.es\n\n");

}

int validar_caracteres(char* nombre_bd){

    char cadena[100]="";
    int j = 0, errores = 0;

    strcpy(cadena, nombre_bd); 

    while(cadena[j] != '\0'){ 

        if(cadena[j] == '_'){
            j++;
            continue;
        }
        if(cadena[j]>='0' && cadena[j]<='9'){
            j++;
            continue;
        }
        if(cadena[j]>='a' && cadena[j]<='z'){
            j++;
            continue;
        }
        if(cadena[j]>='A' && cadena[j]<='Z'){
            j++;
            continue;
        }

        printf("ERROR de caracteres\n");
        errores++;
        break;
    }
    return errores;
}


void crear_bd(char *nombre_bd){
    char aux1[15]="\\meta.db";
    char aux2[50]="";

    strcpy(aux2, nombre_bd);
    strcat(aux2, aux1);
    printf("%s", aux2);

    if(access(nombre_bd, F_OK) == 0){
        printf("\n Carpeta ya creada\n");
    }
    else{
        mkdir(nombre_bd); 
        crear_meta(nombre_bd, aux2); 
    }
}


void crear_meta(char *nombre_bd, char *aux2){

    DIR *dir;

    if(dir = opendir(nombre_bd)){

        FILE *fichero;
        fichero = fopen(aux2, "w");

        if(fichero != NULL){
            fprintf(fichero, "TABLAS: 0\n");
            fprintf(fichero, "--------------------\n");

            fclose(fichero);
        }
        else{
            printf("ERROR");
        }
    }
}


int entrar_dir(char *nombre_bd){
    
    DIR *dir;
    if(dir = opendir(nombre_bd)){
        return 0;
    }
    else{ 
        printf("La base de Datos %s no fue encontrada\n", nombre_bd);
        return 1;
    }
}


TYPE validar_tipo(char *valor){

    if(strcmp(valor, "NUM") == 0){
        return NUM;
    }
    else if(strcmp(valor, "DATE") == 0){
        return DATE;
    }
    else if(strcmp(valor, "EMAIL") == 0){
        return EMAIL;
    }
    else if(strcmp(valor, "TEXT") == 0){
        return TEXT;
    }
    else{
        return ERR;
    }

}


TABLE* inicializar_tablas(int num_campos, char *nombre_tabla, char **campos, TYPE *tipos){
    
    TABLE *tabla = malloc(sizeof(TABLE)); 

    if(tabla == NULL){
        return NULL;
    }

    strcpy(tabla->nombre, nombre_tabla); 
    tabla->numCampos = num_campos;
    tabla->campos = campos;
    tabla->tipos = tipos;
    tabla->ant = NULL;
    tabla->sig = NULL;

    return tabla;
    
}


void crear_tabla_tab(TABLE *tabla, char *nombre_bd, char *nombre_tabla){

    char aux1[5] = ".tab";
    char aux2[5] = "\\";
    char aux3[100] = "";

    strcpy(aux3, nombre_bd);
    strcat(aux3, aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3, aux1);

    if(access(aux3, F_OK) == 0){

        printf("La tabla %s ya fue creada\n", nombre_tabla);

    }
    else{
        DIR *dir;
        if(dir = opendir(nombre_bd)){

            FILE *fichero;
            fichero = fopen(aux3, "w");

            if(fichero != NULL){
                editar_meta(nombre_bd, tabla);

            }
            fclose(fichero);
        }
        else{
            printf("algo salio mal\n");
        }  

    }    
}

void editar_meta(char *nombre_bd, TABLE *tabla){
    
    DIR *dir;

    char aux1[11] = "\\meta.db";
    char aux2[100]="";
    char aux_tipos[50]="";
    int numero_tablas = 0, i;

    strcpy(aux2, nombre_bd);
    strcat(aux2, aux1);
    printf("%s", aux2);

    numero_tablas = contar_tablas(nombre_bd); 

    if(dir = opendir(nombre_bd)){

        FILE *fichero;
        fichero = fopen(aux2,"w");

        if(fichero != NULL){

            fprintf(fichero, "TABLAS: %d\n", numero_tablas);
            fprintf(fichero, "---------------------\n");

            while(tabla != NULL){

                fprintf(fichero, "%s: %d\n", tabla->nombre, tabla->numCampos);

                for(i=0; i<tabla->numCampos; i++){
                    
                    if(tabla->tipos[i] == 0){
                        strcpy(aux_tipos, "NUM");
                    }
                    else if(tabla->tipos[i] == 1){
                        strcpy(aux_tipos, "DATE");
                    }
                    else if(tabla->tipos[i] == 2){
                        strcpy(aux_tipos, "EMAIL");
                    }
                    else if(tabla->tipos[i] == 3){
                        strcpy(aux_tipos, "TEXT");
                    }

                    fprintf(fichero, "%s: %s\n", tabla->campos[i], aux_tipos);

                }
                
                if(tabla->sig == NULL){
                    break;
                }

                fprintf(fichero, "---------------------\n");
                tabla = tabla->sig;

            }
        }

        fclose(fichero);
    }
}


int contar_tablas(char *nombre_bd){

    DIR *dir;
    struct dirent *f; 
    int num = 0;

    if(dir = opendir(nombre_bd)){
        while(f = readdir(dir)){
            num++;
        }
    }

    num = num - 3;
    return num;
}


void insertar_ultima(TABLE *LaGranTabla, TABLE *tabla_auxliar){

    TABLE *LaGranTabla2;

    LaGranTabla2 = LaGranTabla;
    while(LaGranTabla2->sig != NULL){
        LaGranTabla2 = LaGranTabla2->sig;
    }

    LaGranTabla2->sig = tabla_auxliar;
    tabla_auxliar->ant = LaGranTabla2;

}


int ver_duplicados(char **cadena, int num_arg){

    int i, j;

    for(i=0; i<num_arg; i++){

        for(j = i+1; j< num_arg; j++){

            if(strcmp(cadena[i], cadena[j]) == 0){
                printf("No se pueden repetir los campos\n");
                return 1;
            }
        }
    }

    return 0;

}

int entrar_tabla(char *nombre_db, char *nombre_tabla){

    DIR *dir;
    char aux1[10] = ".tab";
    char aux2[5] = "\\";
    char aux3[100] = "";

    strcpy(aux3, nombre_db);
    strcat(aux3, aux2);
    strcat(aux3, nombre_tabla);
    strcat(aux3, aux1);

    printf("%s", aux3);

    if(access(aux3, F_OK) == 0){
        if(dir = opendir(nombre_db)){
            
            return 1;
        }
    }
    else{
        printf("La tabla %s no estaba creada, por lo tanto todo OK\n", nombre_tabla);
        return 0;
    }
}


char *MiStrTok(char *chorizo, char *sep, int comillas){ //off com = 0, on com = 1

    static char *pt1 = NULL;
    static char *pt2 = NULL;
    static char *ptFinal;

    if(chorizo != NULL){
        pt1 = chorizo;
        ptFinal = pt1 + strlen(chorizo);
    }
    else{
        pt1 = pt2+1;
    }

    if(comillas == 1){

        while(pt1[0] != '\0' && pt1 < ptFinal && strchr(sep, pt1[0]) != NULL && pt1[0] != '"'){
            pt1++;
        }

        if(pt1[0] == '"'){
            pt1++;
            pt2 = pt1+1;
            while(pt2[0] != '\0' && pt2[0] != '"'){
                
                pt2++;
            }

            if(pt2[0] == '\0'){
                pt1 = pt2 = ptFinal = NULL;
                return NULL;

            }
            else{
                pt2[0]='\0';
                return pt1;
            }
        }
    }
    else{

        while(pt1[0] != '\0' && pt1<ptFinal && strchr(sep, pt1[0]) != NULL){
            pt1++;
        }
    }

    if(pt1>=ptFinal){
        pt1 = pt2 = ptFinal = NULL;
        return NULL;
    }

    pt2 = pt1 +1;

    while(pt2[0] != '\0' && strchr(sep, pt2[0]) == NULL){
        pt2++;
    }

    pt2[0]='\0';

    return pt1;

}


TABLE *buscar_tabla(char *nombre_tabla, TABLE *tabla){

    TABLE *tabla2 = tabla;
    
    if(tabla2 != NULL){

        while(tabla2 != NULL){

            if(strcmp(tabla2->nombre, nombre_tabla) == 0){
                return tabla2;
            }

            tabla2 = tabla2->sig;
        }

    }
    else{
        printf("%d", tabla2->numCampos);
        return tabla2;
    }

}


int validar_decimal(char *cadena){
    
    int i, p=0;

    if(cadena[0] == '+' || cadena[0] == '-'){
        i = 1;
    }
    else{
        i=0;
    }

    if(strlen(cadena)==1 && i == 1){ 
        return 0;
    }

    if(strlen(cadena) == 0){
        return 0;
    }

    if(i == 1 && strlen(cadena) == 2 && cadena[1] == '.'){ 
        return 0;
    }
    if(strlen(cadena) == 1 && i == 0 && cadena[0] == '.'){ 
        return 0;
    }

    for(i; i<strlen(cadena); i++){
        
        if(cadena[i]<'0' || cadena[i]>'9'){ 
            
            if(cadena[i] == '.'){
                p++; 
            }
            else{
                return 0;
            }
        }
        if(p==2){ 
            return 0;
        }
    }

    return 1; 

}


int FechaValidarDMA(char *cadena){ 

    int i, d, m, a;
    
    if(strlen(cadena) != 10){
        return 0;
    }

    for(i = 0; i < 10; i++){

        if(i != 2 && i != 5){

            if(cadena[i]<'0' || cadena[i] > '9'){
                return 0;
            }
        }
    }

    if(cadena[2] != '/' && cadena[5] != '/'){
      
        return 0;
    }

    d=(cadena[0]-'0')*10 +(cadena[1]-'0');
    m=(cadena[3]-'0')*10 +(cadena[4]-'0');
    a=(cadena[6]-'0')*1000 +(cadena[7]-'0')*100+(cadena[8]-'0')*10+ (cadena[9]-'0'); 

    if(d<1 || m<1 || a<1000 || d>31 || m>12 || a>9999){
     
        return 0;
    }
    else if((m == 4 || m == 6 || m == 9 || m == 11) && d>30){
  
        return 0;
    }
    //bisiesto
    else if( m == 2 && a%4 == 0 && d>29){ 
        return 0;
    }
    else if(m == 2 && a%4!= 0 && d>28){ 
        return 0;
    }

    return 1;

}


ROW *inicializar_linea(char **valores){

    ROW *linea = malloc(sizeof(ROW));

    if(linea == NULL){
        return NULL;
    }

    linea->valores = valores;
    linea->sig = NULL;
    linea->ant = NULL;

    return linea;

}


void insertarUltimaLinea(ROW *LaGranLinea, ROW *linea_auxiliar){

    ROW *LaGranLinea2 = LaGranLinea;

    while(LaGranLinea2->sig != NULL){
        LaGranLinea2 = LaGranLinea2->sig;
    }

    LaGranLinea2->sig = linea_auxiliar;
    linea_auxiliar->ant = LaGranLinea2;
    linea_auxiliar->sig = NULL;
}


void actualizar_lineas(char *nombre_db, ROW *linea, TABLE *tabla){

    DIR *dir;

    char aux1[11] = ".tab";
    char aux2[50] = "\\";
    char aux3[50] = "";

    strcpy(aux3, nombre_db);
    strcat(aux3, aux2);
    strcat(aux3, tabla->nombre);
    strcat(aux3, aux1);


    if(dir = opendir(nombre_db)){

        FILE *fichero;
        fichero = fopen(aux3, "w");

        if(fichero != NULL){
            while(linea != NULL){

                for(int i = 0; i<tabla->numCampos ; i++){ 

                    if(tabla->tipos[i] == TEXT && i == tabla->numCampos-1){ 
                        fprintf(fichero, "\"%s\"\n", linea->valores[i]);
                    }
                    else if( tabla->tipos[i] == TEXT){
                        fprintf(fichero, "\"%s\", ", linea->valores[i]);
                    }
                    else if(tabla->numCampos-1 == i){
                        fprintf(fichero, "%s\n", linea->valores[i]);
                    }
                    else{
                        fprintf(fichero, "%s, ", linea->valores[i]);
                    }
                }

                if(linea->sig == NULL){
                    break;
                }

                linea = linea->sig;
            }
            
            fclose(fichero);
        }
        else{
            printf("algo salio mal\n");
        }

    }
    else{
        printf("Extraño caso no nos deja entrar a la carpeta\n");
    }

}

TABLE *cargarTablas(char *nombre_bd){

    TABLE *tabla=NULL, *tablanueva;
    FILE *meta;
    char aux1[11]="\\meta.db";
    char aux2[50]="";
    char aux3[50]="";
    char linea[1000],val[100];
    int num_tabla,num_campos, i, j;
    char *nombre_tabla;
    char **nombre_campo;
    TYPE *tipos; 
    

    strcpy(aux2,nombre_bd);
    strcat(aux2,aux1);

    meta = fopen(aux2, "r");
    if(meta == NULL){
        printf("ERROR");
        return 0;
    }
    fgets(linea,1000,meta); 
    ObtenerCampo(linea,":",1,val); 
    Trim(val);                  
    num_tabla = atoi(val);     

    for(i=0; i<num_tabla;i++){ 

        fgets(linea,1000,meta);
        fgets(linea,1000,meta);
        ObtenerCampo(linea, ":", 0,val); 
        Trim(val);                  
        nombre_tabla=strdup(val);   
        ObtenerCampo(linea,":",1,val); 
        Trim(val);                  
        num_campos=atoi(val);       

        nombre_campo = malloc(sizeof(char*)*num_campos); 
        tipos = malloc(sizeof(TYPE*)*num_campos); 
        
        for(j=0;j<num_campos;j++){ 
            fgets(linea,1000,meta); 
            ObtenerCampo(linea,":",0,val);  
            Trim(val);
            nombre_campo[j]=strdup(val); 
            ObtenerCampo(linea,":",1,val);
            Trim(val);
            tipos[j]=validar_tipo(val); 
            printf("------------\n");
            printf("%d\n", tipos[j]);
            printf("------------\n");
        }

        tablanueva = inicializar_tablas(num_campos,nombre_tabla, nombre_campo, tipos); 

        if(tabla==NULL){ 
            tabla=tablanueva;
                    
        }
        else{ 
            insertar_ultima(tabla, tablanueva);          
        }

    }
    fclose(meta); 
    return tabla;
}

void Trim(char *cad) 
{
	int c, i, j, n=strlen(cad);
	
	if (cad==NULL || n<1)
		return;
	n--;
	while(n>=0 && ((c=cad[n])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		n--;
	cad[n+1]='\0';
	i=0;
	while (i<=n && ((c=cad[i])==' ' || c=='\t' || c=='\n' || c=='\r' || c=='"'))
		i++;
	for (j=0 ; j<=n-i+1; j++)
		cad[j]=cad[j+i];
}


char *ObtenerCampo(char *lin, char *sep, int id, char *val){
    
    int i, tamSep = 1;
    char *p1, *p2;

    if(lin == NULL || lin[0] == '\0'){
        return NULL;
    }

    p1 = lin;

    p2 = strstr(lin, sep);

    for(i=0; i<id; i++){
        p1 = p2+tamSep;
        p2 = strstr(p1, sep);
    }

    if(p2 == NULL){
        strcpy(val, p1);
    }
    else{
        strncpy(val, p1, p2-p1);
        val[p2-p1] = '\0';
    }

    return val;
}


ROW *cargar_lineas(char *nombre_db, char *nombre_tabla ,int num_campos,  TYPE* tipos){

   char aux1[5]=".tab";
    char aux2[5]="\\";
    char aux3[100]="";
    FILE *fichero;
    char linea[1000];
    int i,comillas;
    long final;
    char *token, *lineaaux;
    char **valores;
    ROW *fila = NULL, *filanueva;

    strcpy(aux3,nombre_db);
    strcat(aux3,aux2);
    strcat(aux3,nombre_tabla);
    strcat(aux3,aux1);

    if(access(aux3,F_OK) == 0 ){ 

        fichero = fopen(aux3, "r"); 

        fseek( fichero, 0, SEEK_END ); 

        if (ftell( fichero ) == 0 ) 
        {
            fclose(fichero);
            return fila;
        }

        final=ftell(fichero); 
        fseek(fichero,0,SEEK_SET); 


        while(final != ftell(fichero)){ 

            fgets(linea,1000,fichero); 

            valores = malloc(sizeof(char*)*num_campos); 

            for(i=0;i<num_campos;i++){
             
                comillas = (tipos[i] == TEXT); 
            
                if(i == 0){
                    lineaaux = linea;
                }
                else{
                    lineaaux = NULL;
                }
                token = MiStrTok(lineaaux,",\n",comillas);
                valores[i]=strdup(token);
                Trim(valores[i]);
            }

            filanueva = inicializar_linea(valores); 

            if(fila==NULL){ 
                fila=filanueva;       
            }
            else{
                insertarUltimaLinea(fila, filanueva);   
            }
        }                 
    }
    else{
        printf("ERROR");
        return 0;
    }

    fclose(fichero);
    return fila;

}


void liberar_memoria_lineas(ROW *LaGranLinea, int num_campos){

    ROW *lineasiguiente;
    int i;

    if(LaGranLinea == NULL){ 
        return;
    }

    while (LaGranLinea != NULL) 
    {
        lineasiguiente = LaGranLinea->sig; 

        for(i=0; i<num_campos;i++){

            free(LaGranLinea->valores[i]); 
        }

        free(LaGranLinea->valores); 
        free(LaGranLinea);

        LaGranLinea = lineasiguiente;
    }
}

void liberar_memoria_tablas(TABLE *LaGranTabla){
    
    TABLE *tabla_sig;
    int i;

    if(LaGranTabla == NULL){
        return;
    }

    while(LaGranTabla != NULL){
        
        tabla_sig = LaGranTabla->sig;

        for(i=0; i<LaGranTabla->numCampos; i++){
            free(LaGranTabla->campos[i]);
        }

        free(LaGranTabla->campos);
        free(LaGranTabla->tipos);
        free(LaGranTabla);

        LaGranTabla = tabla_sig;

    }
}


void impr_cabeceras(TABLE *tabla_auxiliar){

    int i;

    for(i=0; i<tabla_auxiliar->numCampos; i++){
        printf("%s\t", tabla_auxiliar->campos[i]);
    }

    printf("\n");

}


void imprimir_todo(ROW *LaGranLinea, int numcampos){

    ROW *linea_auxiliar = LaGranLinea;
    int i, contador = 1;
    
    if(linea_auxiliar == NULL){
        printf("No hay datos en la tabla\n");
    }
    else{

        while(linea_auxiliar != NULL){

            printf("%d - ", contador);

            for(i=0; i<numcampos-1; i++){
                printf("%s, ", linea_auxiliar->valores[i]);
            }

            printf("%s \n", linea_auxiliar->valores[numcampos-1]);

            contador++;

            linea_auxiliar = linea_auxiliar->sig;

        }
    }
}


void imprimir_todo_ACS(ROW *LaGranLinea, int numcampos, TABLE *tabla_aux){

    int i, contador=1;
    ROW *linea_auxiliar = LaGranLinea;

    if(linea_auxiliar == NULL){
        printf("No hay datos en la tabla\n");
    }
    else{

       
        ordenar_asc(linea_auxiliar, tabla_aux, 0);

        while(linea_auxiliar != NULL){
            printf("%d - ", contador);

            for(i=0; i < numcampos-1; i++){
                printf("%s, ", linea_auxiliar->valores[i]);
            }

            printf("%s \n", linea_auxiliar->valores[numcampos-1]);

            contador++;

            linea_auxiliar = linea_auxiliar->sig;
        }
    }
}


void imprimir_todo_DES(ROW *LaGranLinea, int numcampos, TABLE *tabla_aux){

    int i, contador=1;
    ROW *linea_auxiliar = LaGranLinea;

    if(linea_auxiliar == NULL){
        printf("No hay datos en la tabla\n");
    }
    else{
        ordenar_des(LaGranLinea, tabla_aux, 0);

        while(linea_auxiliar != NULL){

            printf("%d - ", contador);
            for(i=0; i<numcampos-1; i++){
                printf("%s, ", linea_auxiliar->valores[i]);
            }

            printf("%s\n", linea_auxiliar->valores[numcampos-1]);

            contador++;

            linea_auxiliar = linea_auxiliar->sig;
        }
    }
}


void ordenar_asc(ROW *LaGranLinea, TABLE *tabla_aux, int orden_campo){

    ROW *i, *j;

    printf("2\n");
    printf(" holus %d\n",tabla_aux->tipos[orden_campo]);    

    if(tabla_aux->tipos[orden_campo] == NUM){

        printf("3\n");

        for(i=LaGranLinea; i != NULL; i = i->sig){

            printf("4\n");

            for(j=i; j != NULL; j = j->sig){

                printf("5\n");

                if(atof(i->valores[orden_campo]) > atof(j->valores[orden_campo])){

                    printf("6\n");

                    printf("%s  %s \n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambiar_valores(i, j);

                }

            }
        }

    }
    if(tabla_aux->tipos[orden_campo] == DATE){

        printf("3\n");

        for(i = LaGranLinea; i != NULL; i = i->sig){

            printf("4\n");

            for(j = i; j != NULL; j = j->sig){

                printf("5\n");

                if(FechatoNumeroDMA(i->valores[orden_campo]) > FechatoNumeroDMA(j->valores[orden_campo])){

                    printf("6\n");

                    printf("%s > %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambiar_valores(i, j);
                }    
            }
        }

    }
    else if(tabla_aux->tipos[orden_campo] == EMAIL){

        for(i = LaGranLinea; i != NULL; i = i->sig){

            for(j = i; j != NULL ; j = j->sig){

                if(strcmp(i->valores[orden_campo], j->valores[orden_campo])>0){

                    printf("%s > %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambiar_valores(i, j);

                }
            }
        }
    }
    else if(tabla_aux->tipos[orden_campo] == TEXT){

        for(i = LaGranLinea; i != NULL; i = i->sig){

            for(j = i; j != NULL ; j = j->sig){

                if(strcmp(i->valores[orden_campo], j->valores[orden_campo])>0){

                    printf("%s > %s\n", i->valores[orden_campo], j->valores[orden_campo]);

                    intercambiar_valores(i, j);

                }
            }
        }
    }
}

void ordenar_des(ROW *LaGranLinea, TABLE *tabla_aux, int orden_campo){

    ROW *i, *j;

    if(tabla_aux->tipos[orden_campo] == NUM){

        for(i=LaGranLinea; i != NULL; i = i->sig){

            for(j = i; j != NULL; j = j->sig){

                if(atof(i->valores[orden_campo]) < atof(j->valores[orden_campo])){

                    printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);
                    intercambiar_valores(i,j);

                }

            }
        }

    }
    else if(tabla_aux->tipos[orden_campo] == DATE){

        for(i = LaGranLinea; i != NULL; i = i->sig){

            for(j = i; j != NULL; j = j->sig){

                if(FechatoNumeroDMA(i->valores[orden_campo]) < FechatoNumeroDMA(j->valores[orden_campo])){

                    printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);
                    intercambiar_valores(i, j);
                }


            }

        }

    }
    else if(tabla_aux->tipos[orden_campo] == EMAIL){

        for(i = LaGranLinea; i != NULL; i = i->sig){

            for(j = i; j != NULL; j = j->sig){

                if(strcmp(i->valores[orden_campo], j->valores[orden_campo]) < 0){

                    printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);
                    intercambiar_valores(i, j);

                }
            }
        }
    }
    else if(tabla_aux->tipos[orden_campo] == TEXT){

        for(i = LaGranLinea; i != NULL; i = i->sig){

            for(j = i; j != NULL; j = j->sig){

                if(strcmp(i->valores[orden_campo], j->valores[orden_campo]) < 0){

                    printf("%s < %s\n", i->valores[orden_campo], j->valores[orden_campo]);
                    intercambiar_valores(i, j);

                }
            }
        }
    }
}


void intercambiar_valores(ROW *i, ROW *j){

    printf("7\n");

    char **lineaux;

    lineaux = i->valores;
    i->valores = j->valores;
    j->valores = lineaux;

}


int FechatoNumeroDMA(char* cad)
{
	int res, dia, mes, anyo;
	dia=(cad[0]-'0')*10 +(cad[1]-'0');
    mes=(cad[3]-'0')*10 +(cad[4]-'0');
    anyo=(cad[6]-'0')*1000 +(cad[7]-'0')*100+(cad[8]-'0')*10+ (cad[9]-'0');
	res=dia;

	if (mes==1)
		res=res;
	else if (mes==2)
		res=res+31;
	else if (mes==3 && (anyo%4)!=0)
		res=res+59;
	else if (mes==3 && (anyo%4)==0)
		res=res+60;
	else if (mes==4 && (anyo%4)!=0)
		res=res+90;
	else if (mes==4 && (anyo%4)==0)
		res=res+91;
	else if (mes==5 && (anyo%4)!=0)
		res=res+120;
	else if (mes==5 && (anyo%4)==0)
		res=res+121;
	else if (mes==6 && (anyo%4)!=0)
		res=res+151;
	else if (mes==6 && (anyo%4)==0)
		res=res+152;
	else if (mes==7 && (anyo%4)!=0)
		res=res+181;
	else if (mes==7 && (anyo%4)==0)
		res=res+182;
	else if (mes==8 && (anyo%4)!=0)
		res=res+212;
	else if (mes==8 && (anyo%4)==0)
		res=res+213;
	else if (mes==9 && (anyo%4)!=0)
		res=res+243;
	else if (mes==9 && (anyo%4)==0)
		res=res+244;
	else if (mes==10 && (anyo%4)!=0)
		res=res+273;
	else if (mes==10 && (anyo%4)==0)
		res=res+274;
	else if (mes==11 && (anyo%4)!=0)
		res=res+304;
	else if (mes==11 && (anyo%4)==0)
		res=res+305;
	else if (mes==12 && (anyo%4)!=0)
		res=res+334;
	else if (mes==12 && (anyo%4)==0)
		res=res+335;

	res=(res+(anyo-1)*365);
	res=(res+(anyo-1)/4);


	return res;
}

void imprimir_valor_campo(int numcampos, ROW *LaGranLinea, TYPE tipo, char *valor, char *simbolo, int posicion){

    int i, contador = 1;
    ROW *linea_auxiliar = LaGranLinea;
    


    if(linea_auxiliar == NULL){
        printf("Sin datos en la tabla\n");
    }
    else{

        if(tipo == NUM){

            if(strcmp(simbolo, "=") == 0){

                while(linea_auxiliar != NULL){

                    if(atof(valor) == atof(linea_auxiliar->valores[posicion])){

                        printf("%d - ", contador);

                        for(i = 0; i<numcampos-1; i++ ){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;

                }

                if(contador == 1){
                    printf("Sin datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, ">") == 0){

                while(linea_auxiliar != NULL){

                    if(atof(linea_auxiliar->valores[posicion]) > atof(valor)){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }

                if(contador == 1){
                    printf("Sin datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, "<") == 0){

                while(linea_auxiliar != NULL){

                    if(atof(linea_auxiliar->valores[posicion]) < atof(valor)){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;
                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }

                if(contador == 1){
                    printf("Sin datos para mostrar\n");
                }
            }
            else{
                printf("Error simbolos\n");
                return;
            }
        }
        else if(tipo == DATE){

            if(strcmp(simbolo, "=") == 0){

                while(linea_auxiliar != NULL){

                    if(FechatoNumeroDMA(valor) == FechatoNumeroDMA(linea_auxiliar->valores[posicion])){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;


                }

                if(contador == 1){
                    printf("Sin datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, ">") == 0){

                while(linea_auxiliar != NULL){

                    if(FechatoNumeroDMA(linea_auxiliar->valores[posicion]) > FechatoNumeroDMA(valor)){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;


                }

                if(contador == 1){
                    printf("Sin datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, "<") == 0){

                while(linea_auxiliar != NULL){

                    if(FechatoNumeroDMA(linea_auxiliar->valores[posicion]) < FechatoNumeroDMA(valor)){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }

                if(contador == 1){
                    printf("Sin datos para mostrar\n");
                }

            }
            else{
                printf("error simbolos\n");
                return;
            }



        }
        else if(tipo == EMAIL){

            if(strcmp(simbolo, "=") == 0){

                while(linea_auxiliar != NULL){

                    if(strcmp(valor, linea_auxiliar->valores[posicion]) == 0){ 

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;


                }

                if(contador == 1){
                    printf("No hay datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, ">") == 0){

                while(linea_auxiliar != NULL){

                    if(strcmp(linea_auxiliar->valores[posicion], valor) > 0){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;


                }

                if(contador == 1){
                    printf("No hay datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, "<") == 0){

                while(linea_auxiliar != NULL){

                    if(strcmp(linea_auxiliar->valores[posicion], valor) < 0){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }

                if(contador == 1){
                    printf("No hay datos para mostrar\n");
                }

            }
            else{
                printf("error de simbolos\n");
                return;
            }
        }
        else if(tipo == TEXT){

            if(strcmp(simbolo, "=") == 0){

                while(linea_auxiliar != NULL){

                    if(strcmp(valor, linea_auxiliar->valores[posicion]) == 0){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;


                }

                if(contador == 1){
                    printf("No hay datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, ">") == 0){

                while(linea_auxiliar != NULL){

                    if(strcmp(linea_auxiliar->valores[posicion], valor) > 0){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;


                }

                if(contador == 1){
                    printf("No hay datos para mostrar\n");
                }

            }
            else if(strcmp(simbolo, "<") == 0){

                while(linea_auxiliar != NULL){

                    if(strcmp(linea_auxiliar->valores[posicion], valor) < 0){

                        printf("%d - ", contador);

                        for(i = 0; i < numcampos-1; i++){
                            printf("%s, ", linea_auxiliar->valores[i]);
                        }

                        printf("%s\n", linea_auxiliar->valores[numcampos-1]);

                        contador++;

                    }

                    linea_auxiliar = linea_auxiliar->sig;
                }

                if(contador == 1){
                    printf("Sin datos para ver\n");
                }

            }
            else{
                printf("error de simbolos\n");
                return;
            }
        }
    }
}


// COMIENZO DICIEMBRE

void mostrar_tablas(TABLE *LaGranTabla){

    TABLE * tabla_temporal_ayuda = LaGranTabla;

    int numero_tablas = 0; 

    while(tabla_temporal_ayuda != NULL){
        
        numero_tablas++;

        printf("%s\n", tabla_temporal_ayuda->nombre);

        if(tabla_temporal_ayuda->sig == NULL){
            break;
        }

        tabla_temporal_ayuda = tabla_temporal_ayuda->sig;

       
    }

    if(numero_tablas == 0){
        printf("no hay tablas\n");
    }

}

void mostrar_bd(){

    DIR *d;
    struct dirent *f;
    int j=0;

    d = opendir(".");

    if(d == NULL){
        printf("No hay BD disponibles\n");
    }
    else{

        while((f = readdir(d)) != NULL){

            if(strcmp(f->d_name, ".") != 0 && strcmp(f->d_name, "..") != 0){

                if(validar_caracteres_bd(f->d_name) == 0){

                    printf("%s\n", f->d_name);
                    j++;

                }
            }
        }

        if(j==0){
            printf("No hay BD disponibles\n");
        }

    }

    closedir(d);

}

int validar_caracteres_bd(char *nombre_bd){

    char copia[100] = "";
    int j = 0, errores = 0;

    strcpy(copia, nombre_bd);

    while(copia[j] != '\0'){

        if(copia[j] != '.'){
            j++;
        }
        else{
            errores++;
            break;
        }
    }

    return errores;

}

/*
    1  error y 0  ok
*/

int validar_email(char *email){

    char copia[100] = "";
    int i = 0, errores = 0, punto = 0, arroba = 0;


    strcpy(copia, email);

    int g = strlen(copia);

    if( g < 5){

        printf("1\n");
        return 1;

    }

    if(copia[g-1] == '.'){
        printf("2\n");
        return 1;
    }

    if(copia[0] == '.' || copia[0] == '@'){
        printf("3\n");
        errores++;
        return errores;
    }

    while(copia[i] != '\0'){

        if(copia[i] >= 'a' && copia[i] <= 'z'){
            printf("4\n");
            i++;
        }
        else if(copia[i] >= 'A' && copia[i] <= 'Z'){
            printf("5\n");
            i++;
        }
        else if(copia[i] >= '0' && copia[i] <= '9'){
            printf("6\n");
            i++;
        }
        else if(copia[i] == '.'){
            printf("7\n");

            if(arroba == 0){
                printf("8\n");
                errores ++;
                return errores;
            }

            i++;
            punto++;

        }
        else if(copia[i] == '@'){

            printf("9\n");

            if(punto != 0){

                printf("10\n");
                errores++;
                return errores;
            }

            i++;
            arroba++;
        }
        else{
            printf("11\n");
            errores++;
            return errores;
        }
    }

    if(errores != 0 || arroba != 1 || punto != 1 ){
        printf("12\n");
        return 1;
    }

    printf("13\n");

    return errores;
}

ROW *borrarLineaValorDeseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion){

    int i, k;
    ROW *lineaux = linea, *lineasiguiente, *lineaanterior;

    if(linea == NULL){
        printf("Sin valores a borrar\n");
    }
    else{

        if(tipo == NUM){

            if(strcmp(simbolo, "=") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(atof(valor) == atof(lineaux->valores[posicion])){
                        //unico
                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        //primer
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        //ultimo
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        //en medio
                        else{ 

                            lineaanterior->sig = lineaux->sig; 
                            lineasiguiente->ant = lineaux->ant; 
                        }

                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;

                }

                return linea;

            }
            else if(strcmp(simbolo, ">") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(atof(lineaux->valores[posicion]) > atof(valor)){

                        if(lineaux == linea && lineaux->sig == NULL){
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{
                            lineaanterior->sig = lineaux->sig;
                            lineasiguiente->ant = lineaux->ant;
                        }

                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);


                    }

                    lineaux = lineasiguiente;

                }

                return linea;
                
            }
            else if(strcmp(simbolo, "<") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(atof(lineaux->valores[posicion]) < atof(valor)){

                        if(lineaux == linea && lineaux->sig == NULL){
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{
                            lineaanterior->sig = lineaux->sig;
                            lineasiguiente->ant = lineaux->ant;
                        }

                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);


                    }

                    lineaux = lineasiguiente;

                }

                return linea;
                
            }
            else{
                printf("error simbolo\n");
            }

        }
        else if(tipo == DATE){

            if(strcmp(simbolo, "=") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(FechatoNumeroDMA(valor) == FechatoNumeroDMA(lineaux->valores[posicion])){
                        
                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{ 

                            lineaanterior->sig = lineaux->sig; 
                            lineasiguiente->ant = lineaux->ant; 
                        }
                        
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;

                }

                return linea;

            }
            else if(strcmp(simbolo, ">") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(FechatoNumeroDMA(lineaux->valores[posicion])  >  FechatoNumeroDMA(valor)){

                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaux->sig; 
                            lineasiguiente->ant = lineaux->ant; 
                        }
                        
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);
                    }
                    lineaux = lineasiguiente;

                }
                return linea;

            }
            else if(strcmp(simbolo, "<") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(FechatoNumeroDMA(lineaux->valores[posicion])  <  FechatoNumeroDMA(valor)){

                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{ 

                            lineaanterior->sig = lineaux->sig; 
                            lineasiguiente->ant = lineaux->ant; 
                        }
                
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }
                    lineaux = lineasiguiente;

                }
                return linea;
            }
            else{
                printf("error simbolo\n");
            }


        }else if(tipo == EMAIL){

            if(strcmp(simbolo, "=") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(strcmp(valor, lineaux->valores[posicion]) == 0){

                        if(lineaux == linea && lineaux->sig == NULL){
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaux->sig;
                            lineasiguiente->ant = lineaux->ant;
                        }

                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;

                }

                return linea;

            }
            else if(strcmp(simbolo, ">") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(strcmp(lineaux->valores[posicion], valor) > 0){

                        if(lineaux == linea && lineaux->sig == NULL){
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{
                            lineaanterior->sig = lineaux->sig;
                            lineasiguiente->ant = lineaux->ant;
                        }               
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }
                        free(lineaux->valores);
                        free(lineaux);
                    }

                    lineaux = lineasiguiente;

                }
                return linea;

            }
            else if(strcmp(simbolo, "<") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(strcmp(lineaux->valores[posicion], valor) < 0){

                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{ 

                            lineaanterior->sig = lineaux->sig;
                            lineasiguiente->ant = lineaux->ant; 
                        }
                        
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;

                }

                return linea;

            }
            else{
                printf("error simbolo\n");
            }


        }else if(tipo == TEXT){

             if(strcmp(simbolo, "=") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(strcmp(valor, lineaux->valores[posicion]) == 0){

                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{ 

                            lineaanterior->sig = lineaux->sig; 
                            lineasiguiente->ant = lineaux->ant;
                        }

                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;
                }

                return linea;

            }
            else if(strcmp(simbolo, ">") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(strcmp(lineaux->valores[posicion], valor) > 0){

                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{ 

                            lineaanterior->sig = lineaux->sig; 
                            lineasiguiente->ant = lineaux->ant; 
                        }
                    
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;
                }
                return linea;

            }
            else if(strcmp(simbolo, "<") == 0){

                while(lineaux != NULL){

                    lineasiguiente = lineaux->sig;
                    lineaanterior = lineaux->ant;

                    if(strcmp(lineaux->valores[posicion], valor) < 0){

                        if(lineaux == linea && lineaux->sig == NULL){ 
                            linea = NULL;
                        }
                        else if(lineaux == linea && lineaux->ant == NULL){ 
                            linea = lineaux->sig;
                            lineasiguiente->ant = NULL;
                            lineaux->sig = NULL;
                        }
                        else if(lineaux->sig == NULL){ 
                            lineaanterior->sig = NULL;
                            lineaux->ant = NULL;
                        }
                        else{

                            lineaanterior->sig = lineaux->sig;
                            lineasiguiente->ant = lineaux->ant; 
                        }
                        
                        for(i=0; i<numcampos; i++){
                            free(lineaux->valores[i]);
                        }

                        free(lineaux->valores);
                        free(lineaux);

                    }

                    lineaux = lineasiguiente;

                }

                return linea;

            }
            else{
                printf("error simbolo\n");
            }

        }else{
            printf("ERROR\n");
        }





    }

}

// FIN DICIEMBRE

// COMIENZO ENERO 

TABLE *borrar_tabla(TABLE *LaGranTabla, char *tabla_borradora){

    int i;
    TABLE *tabla_auxiliar = NULL, *tablasiguiente = NULL, *tablaanterior = NULL;

    tabla_auxiliar = buscar_tabla(tabla_borradora, LaGranTabla);

    tablasiguiente = tabla_auxiliar->sig;
    tablaanterior = tabla_auxiliar->ant;


    if(tabla_auxiliar == LaGranTabla && tabla_auxiliar->sig == NULL){ 
        LaGranTabla = NULL;
    }
    else if(tabla_auxiliar == LaGranTabla && tabla_auxiliar->ant == NULL){ 
        LaGranTabla = LaGranTabla->sig;
        tablasiguiente->ant = NULL;
        tabla_auxiliar->sig = NULL;

    }
    else if(tabla_auxiliar->sig == NULL){
        tablaanterior->sig = NULL;
        tabla_auxiliar->ant = NULL;

    }else{ 
        tablaanterior->sig = tabla_auxiliar->sig; 
        tablasiguiente->ant = tabla_auxiliar->ant; 
    }

    for(i=0; i<tabla_auxiliar->numCampos; i++){
        free(tabla_auxiliar->campos[i]);
    }

    free(tabla_auxiliar->campos);
    free(tabla_auxiliar->tipos);
    free(tabla_auxiliar);

    return LaGranTabla;

}

TABLE *seleccionarTablaOtraBD(char *nombre_tabla, char *nombre_bd_origen, char *nombre_bd_destino, TABLE *LaGranTabla){

    TABLE *LaGranTablaAux = NULL, *tabla_aux = NULL, *tabla_aux2 = NULL;
    ROW *LaGranLinea = NULL;

    LaGranTablaAux = cargarTablas(nombre_bd_origen);
    mostrar_tablas(LaGranTablaAux);

    tabla_aux = buscar_tabla(nombre_tabla, LaGranTablaAux);
    tabla_aux2 = inicializar_tablas(tabla_aux->numCampos, tabla_aux->nombre, tabla_aux->campos, tabla_aux->tipos);

    LaGranLinea = cargar_lineas(nombre_bd_origen, nombre_tabla, tabla_aux->numCampos, tabla_aux->tipos);

    if(LaGranTabla == NULL){
        LaGranTabla = tabla_aux2;
    }
    else{
        insertar_ultima(LaGranTabla, tabla_aux2);
    }

    crear_tabla_tab(LaGranTabla, nombre_bd_destino, nombre_tabla);
    actualizar_lineas(nombre_bd_destino, LaGranLinea, tabla_aux);
    return LaGranTabla;

}





