# WLANDECRYPTER

## Descripción
Generador de diccionario de claves por defecto para los routers de Timofonik Zyxel, Xavy, Comtrend, Thomson y Huawei.

## USO

uso: 
```
wlandecrypter [-l] <bssid> <essid> [output_file opcion]
	-l : lista en pantalla todos los routers conocidos.
```
opcion:
```
	-e : genera diccionario experimental para routers desconocidos.
	-a : genera diccionario para todos los routers conocidos y una WLAN_XX.
```
<essid> = WLAN_??, para crear diccionario con todas las posibilidades.
## OBSERVACIONES
* Las opciones -e y -a no se ejecutan a la vez, wlandecrypter sólo llevará a cabo la primera que encuentre en la línea de comandos.

* Opción -e experimental: Ya que en algunos de los últimos routers las claves siguen el patrón X112233xxxxAB para por ejemplo un router Xavi con mac 11:22:33:xx:xx:xx y WLAN_AB, e igual en algunos Comtrend, Zyxel, etc, porque no probar y ver que pasa con otros que puedan aparecer y no estén aun en la lista.

* Opción -a: Genera las claves de un WLAN_XX para todos los routers conocidos sin tener en cuenta la bssid introducida. Por ejemplo puede servir para un router no conocido, distinto a los entregados por el operador, pero en el que se haya mantenido la configuración del original.

* Un diccionario con la opción -a y wlan_?? ocupará más de 4 GB de disco duro,¿realmente necesitas eso Y PUEDES MANEJARLO?.

* Con la opción -e y wlan_?? el diccionario será de unos 670 MB.


# EJEMPLOS

* Ejemplo1: 

```
wlandecrypter 00:11:22:33:44:55 wlan_aa prueba -e
```
Genera Diccionario 'prueba'con C001122xxxxAA, X001122xxxxAA, Z001122xxxxAA, T001122xxxxAA y H001122xxxxAA


* Ejemplo2: 

```
wlandecrypter 00:11:22:33:44:55 wlan_aa prueba2 -a 
```

Genera el diccionario 'prueba2' para cada BSSID, es decir, para cada router conocido y WLAN_AA, no importa que pongamos como <bssid> no se usará. El diccionario ocupará 19,2 MB en disco duro.


## Descarga

 https://codeload.github.com/glarfs/wlandecryper/tar.gz/1.2.0

 Incluye código fuente.


## INSTALACION

### Compilación

```
make clean && make
```

### Instalación

```
sudo make install
```

