#ifndef _lib_H_
#define _lib_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>

typedef enum{
    NUM, 
    DATE,
    EMAIL, 
    TEXT, 
    ERR
}TYPE;

typedef struct tab{

    char nombre[100];
    int numCampos;
    char **campos;
    TYPE *tipos;
    struct tab *sig, *ant;

}TABLE;

typedef struct line{
    char **valores;
    struct line *sig, *ant;
}ROW;


void datos_alumno();
int validar_caracteres(char* nombre_bd);
void crear_bd(char *nombre_bd);
void crear_meta(char *nombre_bd, char *aux2);
int entrar_dir(char *nombre_bd);
TYPE validar_tipo(char *valor);
TABLE* inicializar_tablas(int num_campos, char *nombre_tabla, char **campos, TYPE *tipos);
void crear_tabla_tab(TABLE *tabla, char *nombre_bd, char *nombre_tabla);
void editar_meta(char *nombre_bd, TABLE *tabla);
int contar_tablas(char *nombre_bd);
void insertar_ultima(TABLE *LaGranTabla, TABLE *tabla_auxliar);
int ver_duplicados(char **cadena, int num_arg);
int entrar_tabla(char *nombre_db, char *nombre_tabla);
char *MiStrTok(char *chorizo, char *sep, int comillas);
TABLE *buscar_tabla(char *nombre_tabla, TABLE *tabla);
int validar_decimal(char *cadena);
int FechaValidarDMA(char *cadena);
ROW *inicializar_linea(char **valores);
void insertarUltimaLinea(ROW *LaGranLinea, ROW *linea_auxiliar);
void actualizar_lineas(char *nombre_db, ROW *linea, TABLE *tabla);
TABLE *cargarTablas(char *nombre_bd);
char *ObtenerCampo(char *lin, char *sep, int id, char *val);
ROW *cargar_lineas(char *nombre_db, char *nombre_tabla ,int num_campos,  TYPE* tipos);
void liberar_memoria_lineas(ROW *LaGranLinea, int num_campos);
void liberar_memoria_tablas(TABLE *LaGranTabla);
void impr_cabeceras(TABLE *tabla_auxiliar);
void imprimir_todo(ROW *LaGranLinea, int numcampos);
void imprimir_todo_ACS(ROW *LaGranLinea, int numcampos, TABLE *tabla_aux);
void ordenar_asc(ROW *LaGranLinea, TABLE *tabla_aux, int orden_campo);
void intercambiar_valores(ROW *i, ROW *j);
int FechatoNumeroDMA(char *cad);
void Trim(char *cad);
void imprimir_todo_DES(ROW *LaGranLinea, int numcampos, TABLE *tabla_aux);
void ordenar_des(ROW *LaGranLinea, TABLE *tabla_aux, int orden_campo);
void imprimir_valor_campo(int numcampos, ROW *LaGranLinea, TYPE tipo, char *valor, char *simbolo, int posicion);
void mostrar_tablas(TABLE *LaGranTabla);
void mostrar_bd();
int validar_caracteres_bd(char *nombre_bd);
int validar_email(char *email);
ROW *borrarLineaValorDeseado(int numcampos, ROW *linea, TYPE tipo, char *valor, char *simbolo, int posicion);
TABLE *borrar_tabla(TABLE *LaGranTabla, char *tabla_borradora);
TABLE *seleccionarTablaOtraBD(char *nombre_tabla, char *nombre_bd_origen, char *nombre_bd_destino, TABLE *LaGranTabla, char *nombreNuevaTabla);


#endif


