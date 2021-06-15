# TPE Criptografía y Seguridad 2020C1 - Grupo 16

En este proyecto se realiza una implementación de un [algoritmo de secreto compartido con optimización de carga](https://www.scielo.cl/scielo.php?script=sci_arttext&pid=S0718-07642014000400021), basado en el esquema de secreto compartido de Shamir.

## Autores

- Luciano Boccardi - Github: [lboccardi](https://github.com/lboccardi)
- Tamara Puig - Github: [tpuig99](https://github.com/tpuig99)
- Ximena Zuberbuhler - Github: [ximezuber](https://github.com/ximezuber)

## Compilación del proyecto 

Desde el directorio root correspondiente al proyecto ejecutar el comando.

```bash
make
```

Se generará el programa correspondiente con el nombre ```main```.

En caso de desear compilarlo sin utilizar Makefile, tener en cuenta la inclusión de las librerías correspondientes.

## Parámetros

El programa se puede correr en dos modos, el primero correspondiente a la distribución de un secreto bajo un esquema (K,N), y la segunda correspondiente a la recuperación del secreto.

#### Primer parámetro:

- ```"d"``` - Indica que se va a distribuir una imagen en otras.
- ```"r"``` - Indica que se va a recuperar una imagen a partir de otras.

#### Segundo parámetro:

- ```"secret.bmp"``` - Si se seleccionó el modo de distribución, corresponde al path relativo al archivo secreto que se desea ocultar. Si el modo elegido fue de recuperación, es el nombre que se le va a dar al secreto recuperado.

#### Tercer parámetro:

- ```"k"``` - Debe ser un número entero en el rango [4,6]. Indica el mínimo de imágenes requeridas para recuperar el secreto, si se elige distribución. Si se elige recuperación, debe corresponder al esquema utilizado en la distribución.

#### Cuarto parámetro:

- ```"dir/"``` - Directorio donde se encuentran las imágenes. Si se elige distribución, debe ser un directorio con al menos k imágenes de formato BMP y que coincidan con el tamaño de la imagen secreta. Si se elige recuperación, debe contener como mínimo k imágenes que porten un secreto distribuido previamente.

#### Quinto parámetro:

- ```"--verbose"``` - Imprime información adicional durante la ejecución del programa. Brinda datos sobre las imágenes leídas así como los formatos.

## Ejemplo de ejecución

#### Distribución

```bash
./main d Albert.bmp 4 images/
```

Se distribuye el archivo ```Albert.bmp``` es un esquema (4, n), donde n es la cantidad de imágenes BMP contenidas en el directorio ```images/```.

#### Recuperación

```bash
./main r Secreto.bmp 4 images/
```

Se recupera el secreto y se guarda en un archivo ```Secreto.bmp``` mediante un esquema (4, n), tomando 4 imágenes del directorio ```images/```.