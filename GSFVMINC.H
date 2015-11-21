/*-------------------------------------------------------------------------
   GSFVMINC.H  - Fichero include para GSFVISMOV
  -------------------------------------------------------------------------
  -------------------------------------------------------------------------*/
#include <stdio.h>


/*-------------------------------------------------------------------------
   VARIABLES usadas en GSFMOVSIS
  -------------------------------------------------------------------------

   --- int GSFVMv_dir;
       Se utiliza para indicar la direccion espacial que se esta
       considerando. 0-eje x,  1-eje y,  2-eje z

   --- int GSFVMv_bodys;
       Esta variable contiene el numero de particulas que forman el
       sistema.

   --- int GSFVMv_campos;
       Esta variable contiene el numero de campos de fuerzas que seran
       analizados para calcular las aceleraciones. Es decir, siendo n el
       valor de esta variable, solo los n primeros campos seran analizados

   --- int GSFVMv_spcdim;
       Aqui guardamos la dimension del espacio. Los valores mas tipicos
       son 2 y 3

   --- float GSFVMv_inc_t;
       Contiene el valor incremento de tiempo.

   --- int   GSFVMv_fprograma;
       Es la variable tipo int para el fichero de programa (.fps)
   --- char   GSFVMv_cprograma[12];
       Es una cadena donde guardamos el nombre del fichero de entrada.
   --- FILE   *GSFVMv_fmovimientos;
       Es la variable tipo FILE para el fichero de movimientos (.fms)
   --- char   GSFVMv_cmovimientos[12];
       Cadena donde guardamos el nombre del fichero de movimientos.

   --- long   GSFVMv_ciclos;
       Numero de ciclos de los que consta el movimiento.
   --- int    GSFVMv_tech;
       Variable: 0 - Sin,  1 - Con informacion tecnica
   --- long   GSFVMv_fprograma_len;
       Guarda el tamanyo en bytes del fichero de programa.
   --- char   *GSFVMm_codigo;
       Matriz donde guardamos el programa
   --- int    GSFVMv_steep;
       Esta variable contiene el incremento en el numero de ciclo.
       Se inicializa en GSFVMf_getentorno().

   --- int    GSFVMv_errors
   --- int    GSFVMv_lines
   --- int    GSFVMv_PC

  -------------------------------------------------------------------------*/

   float huge *GSFVMm_pos;
   int    GSFVMv_bodys;
   int    GSFVMv_movtime;
   int    GSFVMv_campos;
   int    GSFVMv_spcdim;
   float  GSFVMv_inc_t;
   int    GSFVMv_fprograma;
   char   GSFVMv_cprograma[12];
   FILE   *GSFVMv_fmovimientos;
   char   GSFVMv_cmovimientos[12];

   long   GSFVMv_ciclos;
   int    GSFVMv_tech;
   long   GSFVMv_fprograma_len;

   char   GSFVMm_codigo[500];
   int    GSFVMv_steep;
   int    GSFVMv_escala;
   int    GSFVMv_x;
   int    GSFVMv_y;

   int    GSFVMv_errors;
   int    GSFVMv_lines;
   short  GSFVMv_vmode;
/*-------------------------------------------------------------------------
   CABECERAS DE FUNCIONES
  -------------------------------------------------------------------------*/

/* Estas cabeceras son requeridas por el modulo GSFVM_MA (main) */

int GSFVMf_getargs( int nargs, char *args[] );
int GSFVMf_openfiles( void );
int GSFVMf_getentorno( void );
int GSFVMf_getmemoria( void );
int GSFVMf_getsistema( void );
int GSFVMf_runprograma( void );
int GSFVMf_putmemoria( void );
int GSFVMf_closefiles( void );
