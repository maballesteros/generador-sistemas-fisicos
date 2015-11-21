/*-------------------------------------------------------------------------
   GSFMSINC.H  - Fichero include para GSFMOVSIS
  -------------------------------------------------------------------------
  -------------------------------------------------------------------------*/
#include <stdio.h>


#define DIM     3       /* Dimension maxima del espacio */
#define NCAMPOS 5       /* Numero maximo de campos que puede generar
			   una particula. La matriz de bodys, guarda
			   NCAMPOS posiciones para posibles campos */

#define FUNCAMPOS    3  /* Numero de campos definidos por funciones */

#define NOCAMPO      255  /* Numeros asociados a campos */
#define ALLCAMPOS    254
#define GRAVITATORIO 0
#define COULOMB      1
#define ELASTICO     2
#define UNITA       3
#define UNITR       4

#define MSCATR       500

/*-------------------------------------------------------------------------
   VARIABLES usadas en GSFMOVSIS
  -------------------------------------------------------------------------
   --- float GSFMSm_msc_atr[MSCATR];
       Es una matriz de enteros. La utilizo para guardar posibles
       atributos o complementos a las msc. Un ejemplo de atributos es
       el uso que le voy a dar. Para una fuerza que siga la ley de Hook,
       la magnitud podria ser la constante elastica y los atributos,
       las particulas a las que esta asociada. La matriz consta de
       un numero de elementos igual a MSCATR, que es una constante
       definida mas arriba.

   --- short  GSFMSv_nuevo;
       Esta variable es utilizada por GSFMS_moverbodys() como conmutador
       de datos nuevos y viejos. Contiene los valores 0 o 1, indicando
       cuales son los datos nuevos. Asi, si queremos saber cual es la
       coordenada x antigua pondremos: pos[!GSFMSv_nuevo][0]

   --- struct GSFMSs_body { ... };
       Esta estructura define los datos que se necesitan de toda particula

   --- int GSFMSv_part_i;
       En esta variable guardamos el numero indicador de la particula para
       la que se esta calculando la aceleracion

   --- int GSFMSv_part_j;
       En esta variable guardamos el numero indicador de la particula que
       interacciona con la particula dada por GSFMSv_part_i

   --- int GSFMSv_campo_j;
       Esta variable contiene una posicion en la matriz de fuerzas. Se
       utiliza para determinar que fuerza esta actuando.

   --- int GSFMSv_dir;
       Se utiliza para indicar la direccion espacial que se esta
       considerando. 0-eje x,  1-eje y,  2-eje z

   --- float GSFMSv_dist_ij;
       Aqui se guarda la distancia entre la particula i y la j

   --- int GSFMSv_bodys;
       Esta variable contiene el numero de particulas que forman el
       sistema.

   --- int GSFMSv_campos;
       Esta variable contiene el numero de campos de fuerzas que seran
       analizados para calcular las aceleraciones. Es decir, siendo n el
       valor de esta variable, solo los n primeros campos seran analizados

   --- int GSFMSv_spcdim;
       Aqui guardamos la dimension del espacio. Los valores mas tipicos
       son 2 y 3

   --- short GSFMSv_MB_init;
       Se utiliza como indicador. Si GSFMSf_moverbodys() no ha sido
       llamada nunca, GSFMSv_MB_init valdra 1. Despues de la llamada, su
       valor sera permanentemente 0.

   --- float GSFMSv_inc_t;
       Contiene el valor incremento de tiempo.

   --- struct GSFMSs_body *GSFMSm_bodys;
       Esta es un puntero a GSFMSs_body. La usaremos como una matriz
       de GSFMs_body dinamica. Apunta al comienzo de los datos.

   --- float (*GSFMSf_campo[FUNCAMPOS])();
       Esta es una matriz de funciones contiene punteros a las funciones
       con las que se inicializa.
       Supongamos que tenemos una funcion llamada campo_gravitatorio() de
       tipo float, y que retorna la aceleracion de una particula debida al
       campo gravitatorio. Si queremos incluirla en nuestra matriz de
       campos solo tenemos que hacer:
		  GSFMSf_campo[ 0 ] = campo_gravitatorio;
       La habremos situado en la posicion 0. Ahora, si queremos acceder
       a la funcion, lo podemos hacer de dos modos:
		  campo_gravitatorio()
		  (GSFMSf_campo[0])()
       Esto es muy util, ya que podemos seleccionar el campo que queremos
       con tan solo indicar su numero asociado.

   --- FILE   *GSFMSv_ffuente;
       Es la variable tipo FILE para el fichero de entrada (.fsf)
   --- char   GSFMSv_cfuente[12];
       Es una cadena donde guardamos el nombre del fichero de entrada.
   --- FILE   *GSFMSv_fdestino;
       Es la variable tipo FILE para el fichero de salida (.fms)
   --- char   GSFMSv_cdestino[12];
       Cadena donde guardamos el nombre del fichero destino.

   --- unsigned GSFMSv_num_lines;
       Guarda el numero de lineas del fichero fuente
   --- int    GSFMSv_tech;
       0-Sin 1-Con informacion tecnica
   --- int    GSFMSv_errors;
       Numero de errores encontrados
   --- int    GSFMSv_no_interact;
       Si esta variable vale 1, podemos evitar que una particula sea
       sensible a un campo determinado de otra particula

  -------------------------------------------------------------------------*/

   float GSFMSm_msc_atr[MSCATR];
   unsigned GSFMSv_msc_atr_p;
   short  GSFMSv_nuevo;
  #define BODYLENGHT 92
   struct GSFMSs_body {             /* Estructura que define una particula */
		       float pos[2][DIM];      /* 4*2*3 24 bytes */
		       float vel[2][DIM];      /* 4*2*3 24 bytes */
		       float ace[DIM];         /* 4*3   12 bytes */
		       float msc[NCAMPOS];     /* 4*5   20 bytes */
		       int   campo[NCAMPOS];   /* 2*5   10 bytes */
		       int   nopart;           /* 2      2 bytes */
		      };                       /* TOTAL: 92 bytes */
   int    GSFMSv_part_i;
   int    GSFMSv_part_j;
   int    GSFMSv_campo_j;
   int    GSFMSv_dir;
   float  GSFMSv_dist_ij;
   int    GSFMSv_bodys;
   int    GSFMSv_movtime;
   int    GSFMSv_campos;
   int    GSFMSv_spcdim;
   short  GSFMSv_MB_init;
   float  GSFMSv_inc_t;
   struct GSFMSs_body *GSFMSm_bodys;
   float  (*GSFMSf_campo[FUNCAMPOS])();

   FILE   *GSFMSv_ffuente;
   char   GSFMSv_cfuente[12];
   FILE   *GSFMSv_fdestino;
   char   GSFMSv_cdestino[12];

   unsigned GSFMSv_num_lines;
   int    GSFMSv_tech;
   int    GSFMSv_errors;
   int    GSFMSv_no_interact;

/*-------------------------------------------------------------------------
   CABECERAS DE FUNCIONES
  -------------------------------------------------------------------------*/

/* Estas cabeceras son requeridas por el modulo GSFMS_MA (main) */
int    GSFMSf_getargs( int argc, char *argv[] );
int    GSFMSf_openfiles();
int    GSFMSf_getentorno();
int    GSFMSf_getmemoria();
int    GSFMSf_getsistema();
int    GSFMSf_movsistema();
int    GSFMSf_putmemoria();
int    GSFMSf_closefiles();
