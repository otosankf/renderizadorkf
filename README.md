# renderizadorkf
## Renderizador sencillo
Este renderizador cumple la función básica de un renderizador, generando una imagen de salida basado en una entrada de información 3D, basándose únicamente en la función de un raytracer y el color asignado a cada triangulo.

No contiene ningún tipo de comprovación de datos, ya que la finalidad de este programa es mostrar como funciona un raytracer.
## Uso
 main.exe input_file.txt output_file.bmp
## Estructura de datos entrada
La entra es un archive de texto (.txt), estructurado en secciones de la siguiente forma:

~~~
v n
X1 Y1 Z1
X2 Y2 Z2
…
Xn Yn Zn
f n
A1 B1 C1 K1
A2 B2 C2 K2
…
An Bn Cn Kn
k n
R1 G1 B1
R2 G2 B2
…
Rn Gn Bn
c 1
Cx Cy Cz Dx Dy Dz Ox Oy Oz W H Wpx Hpx
~~~

__v n__: indica el inicio del archivo y la sección de vértices con n vértices.

__Xn Yn Zn__ indica las cordenadas de los n vértices (Xn, Yn, Zn).

__f n__ indica el inicio de la sección de caras (triángulos, para ser exactos) con n triángulos.

__An Bn Cn__: indica los vértices A B C de cada tríangulo con un índice (inicia en 0) que hace referencia a los vértices de la sección de vértices.

__Kn__: hace referencia al índice (inicia en 0) de los colores de la siguiente sección.

__k n__: indica el inicio de la sección de colores con n colores.

__Rn Gn Bn__: indica el color RGB con valor 0-255 para cada uno de los n colores. El primer color de esta lista será el color de fondo de la imagen.

__c 1__ indica el inicio de la sección de cámara. solo admite 1.

__Cx Cy Cz__: indican la posicion de la cámara en el espacio en el punto (Cx, Cy, Cz).

__Dx Dy Dz__: indican la dirección donde apunta la camara en el espacio en el punto (Dx, Dy, Dz).

__Ox Oy Oz__: indican la dirección donde apunta la parte superior de la  camara en el espacio en el punto (Ox, Oy, Oz). Debe formar un ángulo de 90° con el vector CD (camara-dirección).

__W H__: indica el ancho (W) y el alto (H) del plano de visión en el espacio.

__Wpx Hpx__: indica el ancho (Wpx) y el alto (Hpx) de la imagen de visión. El ratio W:H debería ser igual al ratio Wpx:Hpx.
