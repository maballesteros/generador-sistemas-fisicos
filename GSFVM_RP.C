/*-------------------------------------------------------------------------
   GSFVM_RP  - En este modulo reside el objetivo real de este programa
  -------------------------------------------------------------------------
   Incluye las funciones:
		int   GSFVMf_runprograma( void );
		int   GSFVMf_visciclo(int ciclo);
		int   GSFVMf_vismove( int steep, long retardo );
		void  GSFVMf_command();
		int   GSFVMf_rpmatch( int token_to_match );
		int   GSFVMf_rpseek_tok( char *str_token );
		int   GSFVMf_rplexan( void );
		void  GSFVMf_drawpant(void);
		void  GSFVMf_set3Dpixel(float x, float y, float z);
		void  GSFVMf_calcnorm(void);
  -------------------------------------------------------------------------*/
#include <graph.h>
#include <math.h>
#include <io.h>
#include <ctype.h>
#include <time.h>
#include "GSFVMinc.h"
/*-------------------------------------------------------------------------*/
#define QUIT        1
#define MOVSIS      2
#define ESCALA      3
#define CENTROX     4
#define CENTROY     5
#define RETARDO     6
#define STEEP       7
#define CLS         8
#define MARCA       9
#define PRINT       10
#define LOCATE      11
#define WAIT        12
#define COLOR       13
#define VIEWX       14
#define VIEWY       15
#define VIEWZ       16
#define EJES        17
#define NUM         250
/*-------------------------------------------------------------------------*/
int   GSFVMf_runprograma( void );
int   GSFVMf_visciclo(int ciclo);
int   GSFVMf_vismove( int steep, long retardo );
void  GSFVMf_command();
int   GSFVMf_rpmatch( int token_to_match );
int   GSFVMf_rpseek_tok( char *str_token );
int   GSFVMf_rplexan( void );
void  GSFVMf_drawpant(void);
void  GSFVMf_set3Dpixel(float x, float y, float z);
void  GSFVMf_calcnorm(void);
/*-------------------------------------------------------------------------*/
int     GSFVMv_token_type;
float   GSFVMv_token_val,n;
char    GSFVMv_comline[60];
short   GSFVMv_xmax, GSFVMv_ymax, GSFVMv_gcolor,GSFVMv_yline,
	GSFVMv_cpos, GSFVMv_marca, GSFVMv_textcx, GSFVMv_textcy,
	GSFVMv_textcol;
long    GSFVMv_retardo;
float   GSFVMv_xnorm,GSFVMv_ynorm,px,py,pz,GSFVMv_le;
clock_t goal,wait;
struct GSFVMs_token_struct {
	     char *key;
	     int   tok;
	    };
struct GSFVMs_token_struct GSFVMm_rptoken_mat[] =
	    {
	     "Quit",QUIT,
	     "MovSis",MOVSIS,
	     "Escala",ESCALA,
	     "Centrox",CENTROX,
	     "Centroy",CENTROY,
	     "Retardo",RETARDO,
	     "Steep",STEEP,
	     "Cls",CLS,
	     "Marca",MARCA,
	     "Print",PRINT,
	     "Locate",LOCATE,
	     "Wait",WAIT,
	     "Color",COLOR,
	     "OjoX",VIEWX,
	     "OjoY",VIEWY,
	     "OjoZ",VIEWZ,
	     "Ejes",EJES,
	     "",0
	    };
/*-------------------------------------------------------------------------*/




/*-------------------------------------------------------------------------
   GSFVMf_runprograma() - Lee y ejecuta el programa que gestionara la
			  visualizacion de los movimientos
  -------------------------------------------------------------------------
   28-7-1994, descripcion:
	      - Inicializamos variables. Ajustamos aquellas que dependan
		del modo de video seleccionado.
	      - Inicializamos el video
	      - Dibujamos la pantalla de trabajo
	      - Ejecutamos el bucle de comandos
	      - Todo ha ido bien. Restablecemos el video por defecto.
  -------------------------------------------------------------------------*/
int   GSFVMf_runprograma( void )
	{
	 GSFVMv_escala = 10;
	 GSFVMv_marca  =  0;
	 GSFVMv_steep =   1;
	 GSFVMv_textcx=2; GSFVMv_textcy=2;
	 GSFVMv_retardo=  0;
	 px=py=pz=10;

	 GSFVMf_calcnorm();
	 GSFVMv_xmax= 639;
	 switch(GSFVMv_vmode){
	    case 1:     GSFVMv_vmode=_HRESBW; GSFVMv_yline=24;
			GSFVMv_x=300;GSFVMv_y=100;
			GSFVMv_ymax=199; GSFVMv_gcolor=1; break;
	    case 2:     GSFVMv_vmode=_HRES16COLOR; GSFVMv_yline=24;
			GSFVMv_x=300;GSFVMv_y=100;
			GSFVMv_ymax=199; GSFVMv_gcolor=15; break;
	    case 3:     GSFVMv_vmode=_VRES16COLOR; GSFVMv_yline=29;
			GSFVMv_x=300;GSFVMv_y=250;
			GSFVMv_ymax=479; GSFVMv_gcolor=15; break; }

	 GSFVMv_textcol=GSFVMv_gcolor;

	 if(!_setvideomode(GSFVMv_vmode)){
	    puts("\n\nERROR: no puedo inicializar la pantalla de video.");
	    return(1);}

	 GSFVMf_drawpant();
	 GSFVMf_command();

	 _setvideomode(_DEFAULTMODE);
	}

/*-------------------------------------------------------------------------
   GSFVMf_drawpant() - Dibuja la pantalla de trabajo
  -------------------------------------------------------------------------
   23-7-1994
  -------------------------------------------------------------------------*/
void  GSFVMf_drawpant(void)
       {
	 _setcolor(GSFVMv_gcolor);
	 _rectangle(_GBORDER, 0,0, GSFVMv_xmax,GSFVMv_ymax);
	 _rectangle(_GBORDER, 2,2, GSFVMv_xmax-2,GSFVMv_ymax-2);
	 _moveto(GSFVMv_xmax-(11*8+2),2);
	 _lineto(GSFVMv_xmax-(11*8+2),GSFVMv_ymax-2);
	 _moveto(2,GSFVMv_ymax-36);
	 _lineto(GSFVMv_xmax-(11*8+2),GSFVMv_ymax-36);

	 _setviewport(3,3,GSFVMv_xmax-(11*8+3),GSFVMv_ymax-37);

	 _settextposition(2,71);
	 puts("OPCIONES");
	 _settextposition(4,71);
	 puts("Cls");
	 _settextposition(5,71);
	 puts("MovSis");
	 _settextposition(6,71);
	 puts("Steep");
	 _settextposition(7,71);
	 puts("Retardo");
	 _settextposition(8,71);
	 puts("Marca");
	 _settextposition(9,71);
	 puts("CentroX");
	 _settextposition(10,71);
	 puts("CentroY");
	 _settextposition(11,71);
	 puts("Locate");
	 _settextposition(12,71);
	 puts("Print");
	 _settextposition(13,71);
	 puts("Wait");
	 _settextposition(13,71);
	 puts("Color");
	 _settextposition(14,71);
	 puts("OjoX");
	 _settextposition(15,71);
	 puts("OjoY");
	 _settextposition(16,71);
	 puts("OjoZ");
	 _settextposition(17,71);
	 puts("Ejes");
       }



/*-------------------------------------------------------------------------
   GSFVMf_command() - Lee una linea de mandatos desde el canal de entrada
  -------------------------------------------------------------------------
   23-7-1994, ademas la ejecuta.
  -------------------------------------------------------------------------*/
void  GSFVMf_command()
      {
       short color, err;

       for(;;){
	_settextposition(GSFVMv_yline,2);
	puts(">                                                            ");
	_settextposition(GSFVMv_yline,3);
	gets(GSFVMv_comline); GSFVMv_cpos=0; err=0;

	GSFVMv_token_type=GSFVMf_rplexan();

	switch(GSFVMv_token_type){

	  case QUIT:     return;

	  case MOVSIS:   if(GSFVMv_spcdim<3){
			 GSFVMf_vismove(GSFVMv_steep,
			       GSFVMv_retardo);break;}
			 else if(GSFVMv_spcdim==3){
			 GSFVMf_vismove3D(GSFVMv_steep,
			       GSFVMv_retardo);break;}

	  case STEEP:    GSFVMf_rpmatch(STEEP);
			 GSFVMv_steep=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case MARCA:    GSFVMf_rpmatch(MARCA);
			 GSFVMv_marca=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case RETARDO:  GSFVMf_rpmatch(RETARDO);
			 GSFVMv_retardo=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case ESCALA:   GSFVMf_rpmatch(ESCALA);
			 GSFVMv_escala=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case CENTROX:  GSFVMf_rpmatch(CENTROX);
			 GSFVMv_x=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case CENTROY:  GSFVMf_rpmatch(CENTROY);
			 GSFVMv_y=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case LOCATE:   GSFVMf_rpmatch(LOCATE);
			 GSFVMv_textcx=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 if(GSFVMf_rpmatch(',')) {err++;break;}
			 GSFVMv_textcy=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case PRINT:    _settextposition(GSFVMv_textcx,GSFVMv_textcy);
			 _outtext(GSFVMv_comline+GSFVMv_cpos+1);
			 break;

	  case COLOR:    GSFVMf_rpmatch(COLOR);
			 GSFVMv_textcol=GSFVMv_token_val;
			 _settextcolor(GSFVMv_textcol);
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 break;

	  case WAIT:     GSFVMf_rpmatch(WAIT);
			 goal=(clock_t)GSFVMv_token_val+clock();
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 while( goal > clock() );
			 break;

	  case EJES:     GSFVMf_rpmatch(EJES);
			 GSFVMv_le=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 if(GSFVMv_marca!=0){
			  _setcolor(GSFVMv_textcol);
			  for(n=-GSFVMv_le;n<GSFVMv_le;n+=(float)1/GSFVMv_marca)
			    {GSFVMf_set3Dpixel(n,0,0);}
			  for(n=-GSFVMv_le;n<GSFVMv_le;n+=(float)1/GSFVMv_marca)
			    {GSFVMf_set3Dpixel(0,n,0);}
			  for(n=-GSFVMv_le;n<GSFVMv_le;n+=(float)1/GSFVMv_marca)
			    {GSFVMf_set3Dpixel(0,0,n);}
			  _setcolor(GSFVMv_gcolor);
			 }
			 break;

	  case VIEWX:    GSFVMf_rpmatch(VIEWX);
			 px=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 GSFVMf_calcnorm();
			 break;

	  case VIEWY:    GSFVMf_rpmatch(VIEWY);
			 py=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 GSFVMf_calcnorm();
			 break;

	  case VIEWZ:    GSFVMf_rpmatch(VIEWZ);
			 pz=GSFVMv_token_val;
			 if(GSFVMf_rpmatch(NUM)) {err++;break;}
			 GSFVMf_calcnorm();
			 break;

	  case CLS:      color=_getcolor();_setcolor(0);
			 _rectangle(_GFILLINTERIOR,3,3,
				    GSFVMv_xmax-(11*8+3),GSFVMv_ymax-37);
			 _setcolor(color);
			 break;

	  default:       _settextposition(GSFVMv_yline,2);
			 puts(">                                                            ");
			 _settextposition(GSFVMv_yline,3);
			 puts(" error de sintaxis"); getch();

	}/*switch*/
	if(err){
	  _settextposition(GSFVMv_yline,2);
	  puts(">                                                            ");
	  _settextposition(GSFVMv_yline,3);
	  puts(" error de sintaxis"); getch();
	 }


       }/*for(;;)*/
      }



/*-------------------------------------------------------------------------
   GSFVMf_rpmatch() - Chequea el token. Si es correcto, coge el siguiente
  -------------------------------------------------------------------------
   23-7-1994, El esquema es el siguiente:
	      - Se comprueba si coinciden los token
		- Si coinciden buscar el siguiente token. Retornar un 0.
		- No coinciden, continuar
	      - Retornar un 1
  -------------------------------------------------------------------------*/
int    GSFVMf_rpmatch( int token_to_match )
	 {
	  if( token_to_match==GSFVMv_token_type )
	      {GSFVMv_token_type=GSFVMf_rplexan(); return 0;}
	  return 1;
	 }



/*-------------------------------------------------------------------------
   GSFVMf_rpseek_tok() - Esta funcion busca en la matriz de keywords una
		    cadena que coincida con su parametro. Retorna el
		    token asociado al keyword.
  -------------------------------------------------------------------------
   23-7-1994,- Llega al final de la matriz. Esto lo sabe porque el ultimo
	       token es 0
	     - Encuentra la cadena coincidente. En este caso retorna el
	       valor numerico del token.
  -------------------------------------------------------------------------*/
int    GSFVMf_rpseek_tok( char *str_token )
	 {
	  int GSFVMv_mat_pos;
	  for( GSFVMv_mat_pos=0;
	       GSFVMm_rptoken_mat[GSFVMv_mat_pos].tok!=0;
	       GSFVMv_mat_pos++ )
	    {
	     if(strcmpi(str_token,GSFVMm_rptoken_mat[GSFVMv_mat_pos].key)==0)
	       return GSFVMm_rptoken_mat[GSFVMv_mat_pos].tok;
	    }
	  return 0;
	 }




/*-------------------------------------------------------------------------
   GSFVMf_rplexan() - Lee el siguiente token presente en el fich. fuente
  -------------------------------------------------------------------------
   23-7-1994, El esquema es el siguiente:
	      - Lee el caracter en la entrada
	      - Si es un numero o '-',leerlo y guardarlo en GSFVMv_token_val
		Retornar NUM
	      - Si es una letra, buscar en la matriz de tokens, de que
		tipo se trata. Retornar el tipo de token
	      - Si es un caracter de separacion, desecharlo
	      - Si es un caracter aceptable, retornar el caracter
  -------------------------------------------------------------------------*/
int    GSFVMf_rplexan( void )
	 {
	  int    GSFVMv_char;
	  int    GSFVMv_casepos=0;
	  int    GSFVMv_buff_pos=0;
	  char   GSFVMv_buffer[30];
	  while(1)
	  {
	   if( (GSFVMv_char=GSFVMv_comline[GSFVMv_cpos++] )==EOF ) return EOF;
	   if( GSFVMv_char == 26 ) return EOF;
	   switch( GSFVMv_casepos )
	   {
	    case 0:  if( (GSFVMv_char=='=')||(GSFVMv_char=='{')||
			 (GSFVMv_char=='}')||(GSFVMv_char==';')||
			 (GSFVMv_char=='(')||(GSFVMv_char==')')||
			 (GSFVMv_char=='"')||(GSFVMv_char==',') )
			    {GSFVMv_casepos=1;--GSFVMv_cpos;break;}
		     else if(isalpha(GSFVMv_char))
			    {GSFVMv_casepos=2;--GSFVMv_cpos;break;}
		     else if( isdigit(GSFVMv_char) ||
			      (GSFVMv_char=='-') )
			    {
			     --GSFVMv_cpos;
			     sscanf(GSFVMv_comline+GSFVMv_cpos,
				    "%f", &GSFVMv_token_val);
			     while(isdigit(*(GSFVMv_comline+(GSFVMv_cpos++)) ));
			     --GSFVMv_cpos;
			     return NUM;
			    }
		     else if( (GSFVMv_char=='\n')||(GSFVMv_char=='\r')||
			      (GSFVMv_char==' ')||(GSFVMv_char=='\t') )
			      break;
		     else {return 0;}

	    case 1:  return GSFVMv_char;

	    case 2:  while( isalnum(GSFVMv_char) )
		      {
		       GSFVMv_buffer[GSFVMv_buff_pos++]=GSFVMv_char;
		       GSFVMv_char = GSFVMv_comline[GSFVMv_cpos++];
		      }
		     --GSFVMv_cpos;
		     GSFVMv_buffer[GSFVMv_buff_pos]='\0';
		     return GSFVMf_rpseek_tok( GSFVMv_buffer );
	   }
	  }
	 }





/*-------------------------------------------------------------------------
   GSFVMf_visciclo() - Visualiza las posiciones de las particulas
  -------------------------------------------------------------------------
   28-7-1994, de momento esta funcion no se usa
  -------------------------------------------------------------------------*/
int   GSFVMf_visciclo(int ciclo)
	{
	   int n;
	   for(n=0;n<GSFVMv_bodys;n++)
	    {
	     _setpixel( *(GSFVMm_pos+ciclo*GSFVMv_bodys*GSFVMv_spcdim
			+n*GSFVMv_spcdim)*GSFVMv_escala+GSFVMv_x,
			- *(GSFVMm_pos+ciclo*GSFVMv_bodys*GSFVMv_spcdim
			+n*GSFVMv_spcdim+1)*GSFVMv_escala+GSFVMv_y );
	    }
	}





/*-------------------------------------------------------------------------
   GSFVMf_vismove() - Visualiza los movimientos de todos los ciclos
  -------------------------------------------------------------------------
   28-7-1994, se usa cuando los movimientos son en 2D o 1D
  -------------------------------------------------------------------------*/
int   GSFVMf_vismove( int steep, long retardo )
	{
	  int n1,n2,n3,color;

	  color=_getcolor();
	  goal=(clock_t)retardo;

	  if(steep==0) steep=1;
	  for(n1=0;(n1+1)<GSFVMv_ciclos;n1+=steep)
	     {
	       _setcolor(color);
	       for(n2=0;n2<GSFVMv_bodys;n2++)
		  {
		   _setpixel( *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim)*GSFVMv_escala+GSFVMv_x,
			      - *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim+1)*GSFVMv_escala+GSFVMv_y );
		  }

	       goal = retardo + clock();
	       while( goal > clock() );


	       if( (GSFVMv_marca==0)||(n1%GSFVMv_marca)!=0 )
		 {
		  _setcolor(0);
		  for(n2=0;n2<GSFVMv_bodys;n2++)
		     {
		      _setpixel( *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
				 +n2*GSFVMv_spcdim)*GSFVMv_escala+GSFVMv_x,
				 - *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
				 +n2*GSFVMv_spcdim+1)*GSFVMv_escala+GSFVMv_y );
		     }
		 }

	     } /* Fin del for(n1) */
	  _setcolor(color);
      } /* Fin de GSFVMf_vismove() */


/*-------------------------------------------------------------------------
   GSFVMf_vismove3D() - Visualiza los movimientos cuando estamos en 3D
  -------------------------------------------------------------------------
   26-7-1994
  -------------------------------------------------------------------------*/
int   GSFVMf_vismove3D( int steep, long retardo )
	{
	  int n1,n2,n3,color;

	  color=_getcolor();
	  goal=(clock_t)retardo;

	  if(steep==0) steep=1;
	  for(n1=0;(n1+1)<GSFVMv_ciclos;n1+=steep)
	     {
	       _setcolor(color);
	       for(n2=0;n2<GSFVMv_bodys;n2++)
		  {
		   GSFVMf_set3Dpixel(
			      *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim),
			      *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim+1),
			      *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim+2)
			      );
		  }

	       goal = retardo + clock();
	       while( goal > clock() );


	       if( (GSFVMv_marca==0)||(n1%GSFVMv_marca)!=0 )
		 {
		  _setcolor(0);
		  for(n2=0;n2<GSFVMv_bodys;n2++)
		     {
		       GSFVMf_set3Dpixel(
			      *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim),
			      *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim+1),
			      *(GSFVMm_pos+n1*GSFVMv_bodys*GSFVMv_spcdim
			      +n2*GSFVMv_spcdim+2)
			      );
		     }
		 }

	     } /* Fin del for(n1) */
	  _setcolor(color);
      } /* Fin de GSFVMf_vismove() */




/*-------------------------------------------------------------------------
   GSFVMf_set3Dpixel() - Halla la proyeccion del punto 3D sobre la pantalla
  -------------------------------------------------------------------------
   28-7-1994
  -------------------------------------------------------------------------*/
void  GSFVMf_set3Dpixel(float x, float y, float z)
{
 float x2D,y2D;
 if( (px!=0)&&(py!=0) ){
 y2D= (x*px*pz+y*py*pz-z*(px*px+py*py))*GSFVMv_ynorm;
 x2D= (py*x-px*y)*GSFVMv_xnorm;}
 else{y2D=y;x2D=x;}
 _setpixel( GSFVMv_x+GSFVMv_escala*x2D, GSFVMv_y-GSFVMv_escala*y2D );
}




/*-------------------------------------------------------------------------
   GSFVMf_calcnorm() - Calcula los coeficientes de normalizacion para la
		       proyeccion sobre la pantalla 2D
  -------------------------------------------------------------------------
   28-7-1994
  -------------------------------------------------------------------------*/
void  GSFVMf_calcnorm(void)
{
 if( (px!=0)&&(py!=0) ){
 GSFVMv_ynorm=1/sqrt( (px*px+py*py)*(px*px+py*py)+pz*pz*(px*px+py*py) );
 GSFVMv_xnorm=1/sqrt(px*px+py*py);}
 else {GSFVMv_ynorm=1;GSFVMv_xnorm=1;}
}
