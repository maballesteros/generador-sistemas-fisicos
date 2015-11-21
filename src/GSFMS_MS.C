/*-------------------------------------------------------------------------
   GSFMS_MS - Movimiento del sistema
  -------------------------------------------------------------------------
  -------------------------------------------------------------------------*/

#include "gsfmsinc.h"
#include <math.h>

int    GSFMSf_movsistema( void );
int    GSFMSf_out_entorno( void );
int    GSFMSf_out_datos( void );
void   GSFMSf_moverbodys( void );

/*-------------------------------------------------------------------------
   VARIABLES - Usadas en este modulo
  -------------------------------------------------------------------------
   --- int   GSFMSv_ciclos;
       Guardamos el numero de movimientos que se van a calcular
   --- float GSFMSv_dist;
       Contiene la distancia que separa la particula i y la j
   --- float GSFMSv_dist_x;
       Contiene la distancia sobre x que separa la particula i y la j
   --- float GSFMSv_dist_y;
       Contiene la distancia sobre y que separa la particula i y la j
   --- float GSFMSv_dist_z;
       Contiene la distancia sobre z que separa la particula i y la j
   --- float aceleracion;
       Guarda la aceleracion que produce la particula GSFMSv_part_j
       a la particula GSFMSv_part_i, debido a uno de sus campos.
   --- float var_param;
       Variable auxiliar usada para calculos.
   --- int    cont_dir, cont_body;
       Son contadores de direcion y de particula usados en GSFMSf_outdatos

  -------------------------------------------------------------------------*/
int    GSFMSv_ciclos;
float  GSFMSv_dist;
float  GSFMSv_dist_x;
float  GSFMSv_dist_y;
float  GSFMSv_dist_z;
float  aceleracion;
float  var_param;
int    cont_dir, cont_body;




/*-------------------------------------------------------------------------
   ACELERACIONES - En este bloque estan las funciones aceleracion
  -------------------------------------------------------------------------
   17-7-1994, Aqui estan las funciones que retornan las aceleraciones que
   produce la particula j sobre la particula i. De momento he incluido dos
   tipos de 'campos'; el campo gravitatorio y uno que he venido a llamar
   nuclear. Ademas defino una matriz de funciones, donde guardaremos las
   funciones que definamos. Esto es muy util, como se vera.
  -------------------------------------------------------------------------
   28-7-1994, La mayor parte de las funciones aceleracion requieren de
   la masa de la particula i. La buscan SIEMPRE, y esto es IMPORTANTE,
   en la primera posicion (lease GSFMSm_bodys[].msc[0]) porque
   GRAVITATORIO vale 0. Por tanto, TODAS las definiciones de un sistema
   de particulas deben incluir como primer campo el gravitatorio y de
   no ponerlo, almenos poner SIEMPRE la masa como primera msc y Null
   como primer campo.
  -------------------------------------------------------------------------*/

   #define cte_G        6.67e-11
   #define cte_K        8.99e9

   float campo_gravitatorio()
	 {
	  return (-( cte_G )*GSFMSm_bodys[GSFMSv_part_j].msc[GRAVITATORIO]
		       /(GSFMSv_dist*GSFMSv_dist));
	 }
   float campo_unit_atr()  /* Campo tipo 1/d*d de cte unidad atractivo */
	 {
	  return (   -GSFMSm_bodys[GSFMSv_part_j].msc[GSFMSv_campo_j]*
		      GSFMSm_bodys[GSFMSv_part_i].msc[GSFMSv_campo_j]
		       /(GSFMSv_dist*
			 GSFMSv_dist*
			 GSFMSm_bodys[GSFMSv_part_i].msc[GRAVITATORIO]
			)
		 );
	 }
   float campo_unit_rep()  /* Campo tipo 1/d*d de cte unidad repulsivo */
	 {
	  return (    GSFMSm_bodys[GSFMSv_part_j].msc[GSFMSv_campo_j]*
		      GSFMSm_bodys[GSFMSv_part_i].msc[GSFMSv_campo_j]
		       /(GSFMSv_dist*
			 GSFMSv_dist*
			 GSFMSm_bodys[GSFMSv_part_i].msc[GRAVITATORIO]
			)
		 );
	 }
   float campo_coulomb()
	 {
	  return ( ( cte_K )*GSFMSm_bodys[GSFMSv_part_j].msc[GSFMSv_campo_j]
			    *GSFMSm_bodys[GSFMSv_part_i].msc[GSFMSv_campo_j]
			    /(GSFMSv_dist*
			      GSFMSv_dist*
			      GSFMSm_bodys[GSFMSv_part_i].msc[GRAVITATORIO]
			     )
		 );
	 }
   float fuerza_elastica()
	 {
	  unsigned position=(unsigned)
			   GSFMSm_bodys[GSFMSv_part_i].msc[GSFMSv_campo_j];
	  while(GSFMSm_msc_atr[position]!=GSFMSv_bodys){
	     if( GSFMSm_msc_atr[position]==GSFMSv_part_j )
		 return( -GSFMSm_msc_atr[position+1]*
			  (GSFMSv_dist-GSFMSm_msc_atr[position+2])/
			  GSFMSm_bodys[GSFMSv_part_i].msc[GRAVITATORIO]
		       );
	     position+=3;
	    };
	  return 0;
	 }



/*-------------------------------------------------------------------------
   GSFMSf_movsistema()  - Mueve el sistema de particulas y genera la
			  salida en formato .fms
  -------------------------------------------------------------------------
   28-7-1994,
  -------------------------------------------------------------------------*/
int    GSFMSf_movsistema( void )
	 {
	   long contador;

	   GSFMSf_campo[GRAVITATORIO] = campo_gravitatorio;
	   GSFMSf_campo[COULOMB] = campo_coulomb;
	   GSFMSf_campo[ELASTICO] = fuerza_elastica;
	   GSFMSf_campo[UNITA] = campo_unit_atr;
	   GSFMSf_campo[UNITR] = campo_unit_rep;

	   GSFMSv_ciclos=(int)( GSFMSv_movtime/GSFMSv_inc_t );

	   if( GSFMSv_tech )
	     {
	      printf(" TEC: Procesando movimientos en %d dimensiones\n",
		    GSFMSv_spcdim );
	      printf(" TEC: Numero de ciclos: %d\n",GSFMSv_ciclos+1);
	     }
	   GSFMSf_out_entorno();
	   GSFMSv_nuevo=1;
	   GSFMSv_MB_init=1;

	   fprintf(GSFMSv_fdestino,"\r\nMovSistema = {\r\n");
	   GSFMSf_out_datos();

	   for( contador=0; contador< GSFMSv_ciclos; contador++)
	      {
		printf("  Generando movimientos. Hecho: %d %c \r",
		       (int)(contador*100/GSFMSv_ciclos),'%' );
		GSFMSf_moverbodys();
		fprintf(GSFMSv_fdestino,",\r\n");
		GSFMSf_out_datos();
		GSFMSv_MB_init = 0;
	      }
	   fprintf(GSFMSv_fdestino,"\r\n             }");

	   printf(" Proceso terminado. Los datos han sido grabados en %s",
		  GSFMSv_cdestino );
	   return 0;
	 }



/*-------------------------------------------------------------------------
   GSFMSf_out_entorno() - Pasa al fichero destino el entorno en el que
			  se dan los movimientos de salida
  -------------------------------------------------------------------------
   28-7-1994, Se pasan al fichero destino las variables necesarias para
   conocer el formato de los movimientos que acompa¤an. Asi, cuando tratemos
   de leer los movimientos, sabremos cuantos ciclos vamos a leer, cuantos
   cuerpos intervienen, etc.
  -------------------------------------------------------------------------*/
int    GSFMSf_out_entorno( void )
	 {
	  fprintf( GSFMSv_fdestino, "DefEntorno =\r\n{\r\n" );
	  fprintf( GSFMSv_fdestino, " IncTemporal    = %g;\r\n",GSFMSv_inc_t);
	  fprintf( GSFMSv_fdestino, " Ciclos         = %d;\r\n",GSFMSv_ciclos+1);
	  fprintf( GSFMSv_fdestino, " NumBodys       = %d;\r\n",GSFMSv_bodys);
	  fprintf( GSFMSv_fdestino, " SpaceDimension = %d;\r\n",GSFMSv_spcdim);
	  fprintf( GSFMSv_fdestino, "}\r\n\r\n");
	 }




/*-------------------------------------------------------------------------
   GSFMSf_out_datos() - Pasa al fichero destino los datos de cada movim.
  -------------------------------------------------------------------------
   28-7-1994, v1.2 Paso las variables locales a globales. Asi se evita
		   tener que guardar espacio en cada llamada
  -------------------------------------------------------------------------*/
int    GSFMSf_out_datos( void )
	 {

	  fputc( (int)'{', GSFMSv_fdestino);
	  for( cont_body=0; cont_body<GSFMSv_bodys; cont_body++)
	   {
	    fputc( (int)'{', GSFMSv_fdestino);
	    for( cont_dir=0; cont_dir<GSFMSv_spcdim; cont_dir++ )
	      {
	       fprintf( GSFMSv_fdestino, "%g",
		      GSFMSm_bodys[cont_body].pos[!GSFMSv_nuevo][cont_dir] );
	       if( cont_dir < (GSFMSv_spcdim-1) )
		   fputc( (int)',', GSFMSv_fdestino);
	      }
	    fputc( (int)'}', GSFMSv_fdestino);
	    if( cont_body < (GSFMSv_bodys-1) )
		fputc( (int)',', GSFMSv_fdestino);

	   }
	  fputs( "}", GSFMSv_fdestino);
	 }




/*-------------------------------------------------------------------------
   GSFMSf_moverbodys()   - Calcula las nuevas posiciones de las
			   particulas tras un tiempo dt, con la
			   posibilidad de excluir interacciones
			   especificas con otras particulas.
			   Movimientos en 1D y 2D
  -------------------------------------------------------------------------
   28-7-1994, version 1.2
  -------------------------------------------------------------------------*/
void GSFMSf_moverbodys( void )
     {
      unsigned ptr_part_no_int,part_no_int;

  /* En primer lugar calculamos las aceleraciones que experimentan
     las particulas del sistema ----------------------------------*/
      for(GSFMSv_part_i=0;GSFMSv_part_i<GSFMSv_bodys;GSFMSv_part_i++){


	/* Ponemos a cero las componentes de la aceleracion de cada
	   particula. Supongo que la maxima dimension es 3 -------*/
	    GSFMSm_bodys[GSFMSv_part_i].ace[0]=0;
	    if( GSFMSv_spcdim > 1 ) GSFMSm_bodys[GSFMSv_part_i].ace[1]=0;
	    if( GSFMSv_spcdim > 2 ) GSFMSm_bodys[GSFMSv_part_i].ace[2]=0;


	/* Calculamos el puntero a particulas con las que no interacciona */
	    ptr_part_no_int = GSFMSm_bodys[GSFMSv_part_i].nopart;
	    if( ptr_part_no_int==0 )
	      {
	       part_no_int=GSFMSv_bodys;
	      }
	    else
	      {
	       part_no_int = GSFMSm_msc_atr[ ptr_part_no_int++ ];
	      }

	/* Calculamos las aceleraciones que le producen el resto de
	   particulas por interaccion ----------------------------*/
	for(GSFMSv_part_j=0;GSFMSv_part_j<GSFMSv_bodys;GSFMSv_part_j++){

	  /* Si la particula no es ella misma, y no es una con la
	     que no interacciona calcular aceleraciones */
	  if( (GSFMSv_part_j!=GSFMSv_part_i)&&
	      (GSFMSv_part_j!=part_no_int) )
	   {
	     /* Calculamos la distancia entre i y j */
	     GSFMSv_dist_x =
		  GSFMSm_bodys[GSFMSv_part_i].pos[!GSFMSv_nuevo][0]
		 -GSFMSm_bodys[GSFMSv_part_j].pos[!GSFMSv_nuevo][0];
	     if( GSFMSv_spcdim > 1)
	       {
		GSFMSv_dist_y =
		   GSFMSm_bodys[GSFMSv_part_i].pos[!GSFMSv_nuevo][1]
		  -GSFMSm_bodys[GSFMSv_part_j].pos[!GSFMSv_nuevo][1];
		if( GSFMSv_spcdim > 2)
		  {
		   GSFMSv_dist_z =
		      GSFMSm_bodys[GSFMSv_part_i].pos[!GSFMSv_nuevo][2]
		     -GSFMSm_bodys[GSFMSv_part_j].pos[!GSFMSv_nuevo][2];
		  }
		else GSFMSv_dist_z = 0.0;
	       }
	     else {GSFMSv_dist_y = 0.0; GSFMSv_dist_z = 0.0; }
	    GSFMSv_dist=sqrt( GSFMSv_dist_x*GSFMSv_dist_x +
			      GSFMSv_dist_y*GSFMSv_dist_y +
			      GSFMSv_dist_z*GSFMSv_dist_z );

	   for(GSFMSv_campo_j=0;GSFMSv_campo_j<GSFMSv_campos;GSFMSv_campo_j++){

	   /* ¨Genera la particula este campo? Si no lo genera, siguiente */
	     if(GSFMSm_bodys[GSFMSv_part_j].campo[GSFMSv_campo_j]==NOCAMPO)
	       continue;
	   /* La particula genera el campo. Bien, ahora calculamos la
	      aceleracion que produce. Esto es IMPORTANTE: se calculan
	      aceleraciones, por tanto, dado que (segun Newton Laws) la
	      aceleracion experimentada por una particula, es directamente
	      proporcional a la fuerza aplicada e inversamente proporcional
	      a la masa de la particula, las funciones GSFMSf_campo[]
	      deben calcula la fuerza y dividir el resultado por la masa
	      de la particula.
	      Llamamos a la funcion que nos calcula la aceleracion */
	      aceleracion= GSFMSf_campo[
			    GSFMSm_bodys[GSFMSv_part_j].campo[GSFMSv_campo_j]
			   ]();

	   /* Calculamos las componentes de la aceleracion y aplicamos el
	      principio de superposicion ------------------------------
	      Si la distancia es cero, tambien lo son las distancias
	      componentes. Con el if solucionamos el problema de la
	      division por cero */

	      if( GSFMSv_dist!=0 )
	      {
		  GSFMSm_bodys[GSFMSv_part_i].ace[0]+=
			 aceleracion*GSFMSv_dist_x/GSFMSv_dist;
		  if( GSFMSv_spcdim > 1 )
		    {
		      GSFMSm_bodys[GSFMSv_part_i].ace[1]+=
			 aceleracion*GSFMSv_dist_y/GSFMSv_dist;
		    }
		 if( GSFMSv_spcdim > 2 )
		    {
		      GSFMSm_bodys[GSFMSv_part_i].ace[2]+=
			 aceleracion*GSFMSv_dist_z/GSFMSv_dist;
		    }
	      }

	    } /* del FOR, bucle de campos que genera la particula j */
	   } /* del IF, la particula j SI interacciona con i */


	 /* La particula resulta ser una de las que NO interaccionan;
	    no se han calculado las aceleraciones que produce. Ahora
	    pasamos a la siguiente particula que no interacciona.
	    Si llega al final de la lista de particulas que no interacionan,
	    part_no_int valdra GSFMSv_bodys y se produciran todas las
	    demas interacciones */
	 if(GSFMSv_part_j==part_no_int)
	   {
	    part_no_int = GSFMSm_msc_atr[ ptr_part_no_int++ ];
	   }

	} /* del FOR, bucle de particulas j */
      }

      /* Aplicando tecnicas de calculo numerico, calculamos las nuevas
	 posiciones de las particulas --------------------------------
	 El metodo usado es el que se encuentra en el tema de 'las leyes
	 de Newton' de The Feynman Lectures on Phisics, volumen 1
	 (castellano) */
      if(GSFMSv_MB_init==1){
      for(GSFMSv_part_i=0;GSFMSv_part_i<GSFMSv_bodys;GSFMSv_part_i++){
	    for(GSFMSv_dir=0;GSFMSv_dir<GSFMSv_spcdim;GSFMSv_dir++){
	       GSFMSm_bodys[GSFMSv_part_i].vel[GSFMSv_nuevo][GSFMSv_dir]=
		     GSFMSm_bodys[GSFMSv_part_i].vel[!GSFMSv_nuevo][GSFMSv_dir]+
		     (GSFMSv_inc_t/2)*GSFMSm_bodys[GSFMSv_part_i].ace[GSFMSv_dir];
	       GSFMSm_bodys[GSFMSv_part_i].pos[GSFMSv_nuevo][GSFMSv_dir]=
		     GSFMSm_bodys[GSFMSv_part_i].pos[!GSFMSv_nuevo][GSFMSv_dir]+
		     GSFMSv_inc_t*GSFMSm_bodys[GSFMSv_part_i].vel[GSFMSv_nuevo][GSFMSv_dir];
	      }
	}
      }else{
      for(GSFMSv_part_i=0;GSFMSv_part_i<GSFMSv_bodys;GSFMSv_part_i++){
	    for(GSFMSv_dir=0;GSFMSv_dir<GSFMSv_spcdim;GSFMSv_dir++){
	       GSFMSm_bodys[GSFMSv_part_i].vel[GSFMSv_nuevo][GSFMSv_dir]=
		     GSFMSm_bodys[GSFMSv_part_i].vel[!GSFMSv_nuevo][GSFMSv_dir]+
		     GSFMSv_inc_t*GSFMSm_bodys[GSFMSv_part_i].ace[GSFMSv_dir];
	       GSFMSm_bodys[GSFMSv_part_i].pos[GSFMSv_nuevo][GSFMSv_dir]=
		     GSFMSm_bodys[GSFMSv_part_i].pos[!GSFMSv_nuevo][GSFMSv_dir]+
		     GSFMSv_inc_t*GSFMSm_bodys[GSFMSv_part_i].vel[GSFMSv_nuevo][GSFMSv_dir];
	      }
	}
      } /* if-else */

      GSFMSv_nuevo=!GSFMSv_nuevo; /* Conmutamos las matrices viejas por las nuevas */
}

