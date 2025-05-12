# Análisis y resolución de conflictos de congestión y flujo en redes usando simulaciones discretas.

### Laboratorio 3 - Transporte - Redes y Sistemas Distribuidos - FaMAF - UNC

---
Abordamos problemas de congestión y flujo en tráfico de red bajo tasas de datos acotadas y tamaño de buffers limitados. Para ello trabajamos analizando dos casos particulares que son simulados y modelados en Omnet++, usando un modelo de colas que varía su configuración a medida que avanzamos con los análisis, llegando así a resolver estos problemas mediante algoritmos

### AGREGAR ALGORITMO FINAL

que se encargan de controlarlos

---
## Introducción

El control de flujo y congestión en redes de sistemas es una de las problemáticas centrales de la materia. Tanto el **control de congestión** como el **control de flujo** se encargan de moderar el tráfico entre nodos transmisores, la red y nodos receptores para evitar saturaciones en alguno de los actores y así evitar pérdidas de datos durante la transferencia de información. La diferencia entre uno y otro radica en *qué* actores de la red están involucrados y por lo tanto *qué* actores serán los encargados de gestionar los recursos de forma que se garantice un tráfico confiable y eficiente entre ellos.

La **congestión** se da en la red. Un nodo transmisor envía información a un nodo receptor a través de ella, si esta red no es lo suficientemente rápida y eficiente entonces un transmisor muy rápido podría saturarla, disminuyendo no solo la velocidad de viaje hasta el receptor de esta información sino, ocasionalmente, la cantidad de información enviada en un principio (la red podría descartar información si no fuese capaz de almacenarla hasta poder enviarla). Dadas estas improntas, conlcuimos en que el **control de congestión** involucra al *nodo transmisor* y a *la red*. Como la red es un actor "independiente", el actor encargado de gestionar el tráfico para que esta no se sature es el *nodo transmisor*.

El **control de flujo** es un proceso de extremo a extremo, es decir entre nodo transmisor y nodo receptor. Un transmisor muy rápido podría saturar un receptor muy lento, generando los mismos resultado explicados en el párrafo anterior. Así, concluimos que los actores involucrados son el *nodo transmisor* y el *nodo receptor*, la diferencia es que aquí es el *nodo receptor* quién debe gestionar en qué forma recibe la información del *transmisor*.

Para modelizar estos problemas de control de flujo y congestión utilizamos lo que se llama **simulación discreta**. Las simulaciones discretas buscan representar sistemas cuyas variables de estado cambian únicamente en un conjunto discreto de instantes en el tiempo, por eso usamos Omnet++, con un tiempo limitado de simulación y acumuladores estadísticos que reflejan el cambio de estas variables que se modifican una cierta cantidad de veces *dentro* de ese tiempo. Este tipo de simulaciones es muy útil cuando experimentar con el sistema real es muy complicado y también cuando tenemos la necesidad de analizar ese sistema en tiempo real, ambas utilidades atraviesan las problemáticas tratadas. Unas de las ventajas que más nos benefician es que, una vez que el modelo está construido, se puede utilizar repetidamente para analizar el mismo problema en distintos conextos y que esos contextos pueden ser controlados por el usuario, que es la forma en que abordamos este análisis y luego resolución de los problemas de flujo y congestión en una red.

El sistema simulado a analizar consiste en un modelo de colas conformado por:

1. **Nodo transmisor (NodeTx):** tendrá el generador de paquetes que se enviarán a través de la red y una cola donde esperarán dichos paquetes antes de ser enviados a dicha red. El generador presenta dos parámetros, un *generationInterval* que es el intervalo de generación entre paquetes y un *packetByteSize*, que especifica el tamaño de los paquetes generados (12500 bytes).
   La cola de este nodo tiene un *bufferSize*, que son la cantidad de paquetes que pueden quedarse esperando a ser enviados luego de ser generados, en este caso, 2.000.000; y un *serviceTime*, que es el tiempo que la cola tardará en "procesar" el paquete, es decir, enviarlo.
2. **Cola "intermedia" (Queue):** tiene los mismos parámetros que la cola del nodo transmisor, funciona como un buffer entre el nodo transmisor y el nodo receptor, guardando los paquetes antes de ser enviados al receptor.  Esta cola simula *la red*, mencionada en los párrafos anteriores, por lo tanto, el control que hace de los paquetes que debe transportar **no** está en "nuestras manos". Su *bufferSize* es de 200 paquetes, mucho más limitado que el del nodo transmisor.
3. **Nodo receptor (NodeRx):** tendrá una cola donde esperarán los paquetes antes de ser finalmente enviados al receptor y el receptor en sí mismo. Esta cola tiene los mismos parámetros que las anteriores, su *bufferSize* es igual de limitado que el de *la red* (Queue)

Estos tres actores estan conectados en una red Network. Analizamos su funcionamiento para dos casos de estudio:

+ **Caso de estudio 1:**

  - NodeTx a Queue: *datarate* = 1 Mbps, *delay* = 100 us
  - Queue a NodeRx: *datarate* = 1 Mbps, *delay* = 100 us
  - Queue a Sink: *datarate* = 0.5 Mbps (Dentro del nodo receptor)

+ **Caso de estudio 2:**

  - NodeTx a Queue: *datarate* = 1 Mbps, *delay* = 100 us
  - Queue a NodeRx: *datarate* = 0.5 Mbps, *delay* = 100 us
  - Queue a Sink: *datarate* = 1 Mbps


Para cada uno de los casos corrimos simulaciones paramétricas con:

- *generationInterval* = 0.1s
- *generationInterval* = 0.3s
- *generationInterval* = 0.5s
- *generationInterval* = 0.7s
- *generationInterval* = 1s

Para analizar ambos casos realizamos gráficos en donde se enfrentan la carga ofrecida por el nodo transmisor y la carga recibida por el nodo receptor para cada una de las simulaciones con distintos intervalos de generación de paquetes. Se realizaron dos gráficos en los que se observa que los valores de carga ofrecida y carga recibida para ambos casos (1 y 2) son similares, por lo que decidimos resumirlo en un solo gráfico

![Gráfico carga ofrecida vs carga recibida, se juntan todas las simulaciones en un solo gráfico.](/redes25lab3g07/graphs/C1y2--offChVSRecCh.png)

En un sistema sin pérdidas ni congestión, la carga recibida debería crecer directamente proporcional a la carga ofrecida. Por lo que vemos en el gráfico, en nuestra simulación esto no ocurre así, dejando en evidencia que a partir de que el nodo transmisor comienza a enviar más de 3 paquetes por segundo hay algun buffer (cola/queue) que comienza a saturarse y a perder paquetes. Realizamos otro gráfico en donde enfrentamos la carga ofrecida con el retardo promedio de los paquetes en llegar al nodo receptor.

![Carga ofrecida vs retardo promedio](/redes25lab3g07/graphs/C1y2-offChVSavDel.png)

Aquí también es claro que, a partir de la generación de 3 paquetes por segundo, los paquetes tardan cada vez más en llegar. Esto refleja, nuevamente, la saturación de algún buffer, haciendo que el/los último/s paquete/s almacenado/s en dicho buffer deban esperar a que este envíe todos los paquetes que hay delante de el/ellos para poder ser enviado/s.

Dados estos gráficos, decidimos, para cada uno de los casos, graficar el tamaño al momento de cada uno de los buffers en la red para ver cuál es el que se está saturando. Usamos un intervalo de generación de paquetes de 0.1s ya que implica la mayor carga ofrecida en nuestras simulaciones y por lo tanto será el caso en donde los buffers estén más exigidos y posiblemente saturados. Será claro en dónde (en qué cola/buffer) está ocurriendo el problema.

### Caso 1

![Tamaño de buffer al momento](/redes25lab3g07/graphs/C1-bufferSize.png)

Vemos que el buffer del nodo receptor, luego de aproximadamente 40 segundos, se satura alcanzando su límite de 200 paquetes y se mantiene saturado hasta el final de la simulación. Queda claro que el *cuello de botella* en este caso se encuentra en la cola de el nodo receptor. Vemos un gráfico de los paquetes perdidos a lo largo de toda la simulación en este buffer

![Paquetes droppeados por la cola del nodo receptor](/redes25lab3g07/graphs/C1-droppedPacks.png)

A medida que pasa el tiempo, luego de los 40 segundos, el buffer del nodo receptor continúa perdiendo paquetes de forma casi lineal, es decir, podría asumirse que todos los paquetes que llegan a dicha cola son descartados por falta de espacio.

Dados estos gráficos asumimos que el problema para este caso está en el buffer del nodo receptor, sugiriendo un problema de **control de flujo**.

### Caso 2

![Tamaño de buffer al momento](/redes25lab3g07/graphs/C2-bufferSize.png)

Lo que sucede en este caso es muy similar a lo ocurrido en el caso 1, pero el problema se observa en la **cola intermedia**, es decir, **la red**. Luego de los 40 segundos el buffer de la red se satura y parece mantenerse saturado hasta el final de la simulación, sugiriendo que cada paquete que llegue a él a partir de ese momento será descartado. Graficamos dicha pérdida para corroborar nuestra suposición

![Paquetes droppeados por la cola del nodo receptor](/redes25lab3g07/graphs/C2-droppedPacks.png)

El gráfico es prácticamente igual que el gráfico de paquetes perdidos para el caso 1, pero aquí, como el problema viene de la red, se alude un problema de **control de congestión**

## Algoritmos de **Control de Congestion y Flujo**

Durante las simulaciones realizadas, identificamos dos problemas principales en la red: **pérdida de paquetes** y un **nodo receptor completamente colapsado**. Estos problemas nos enontramos  en los casos tratados1. Para abordar estas problemáticas, diseñamos e implementamos algoritmos que buscan eliminar la congestión y el flujo descontrolado, para un mejor rendimiento.

El **problema de flujo** ocurre cuando el nodo transmisor envía paquetes a una velocidad mayor de la que el nodo receptor puede procesar. Esto genera una acumulación de paquetes en el buffer del receptor, que eventualmente se desborda, resultando en la pérdida de datos. Para resolver este problema, es necesario implementar un mecanismo de control de flujo que permita al receptor regular la velocidad de transmisión del emisor, asegurando que no se envíen más paquetes de los que el receptor puede manejar.

Por otro lado, el **problema de congestión** se presenta cuando la capacidad de la red intermedia (representada por la cola central en nuestras simulaciones) es insuficiente para manejar el tráfico generado por el nodo transmisor. Esto provoca un aumento en el tiempo de espera de los paquetes y, eventualmente, el descarte de los mismos debido a la saturación del buffer. Se requiere un algoritmo de control de congestión que permita al transmisor ajustar dinámicamente su tasa de envío en función de las condiciones de la red.

Con nuestro equipo de trabajo fuimos un poco mas a la hora de implementar un algoritmos que solucione ambos problemas en nuestra pequeña simulacion. Pero esto requeria prevenciòn entoces decidimos tener un algorimto secillo de respaldo (este es solo un esquema bastante bueno de resolucion, que iba  a ser terminado y detallado, si solo si, No lograbamos implementar el algoritmo )



## Algoritmo PLAN-B

Este algorimo consta de agragarle un delay a la cola del nodo transmisor

y pasar de  :
```cpp
scheduleAt(simTime() + 0, endServiceEvent);
```
a capturar y darle cierto tiempo de delay
```cpp
   scheduleAt(simTime() + this->delay, endServiceEvent);
```

Aqui el nodo (transporTx) tiene cierto tiempo para largar los datos, esto ayuda a que la queue real tenga tiempo para procesar paquetes y enviarlos hacia transporRx, y tambien esa espera del inicio hace que la ventana del receptor no se desborde. Se solucionan ambos problemas.

### Como se calcula el delay


TransporRx calcula constantemente el delay del ultimo paquete y le envia ese resultado a transporTx.
```cpp
    feedbackPkt->delay = simTime().dbl() - msg->getCreationTime().dbl();
    send(feedbackPkt, "toOut$o");
```
Con esa informaciòn claculamos que delay tendra la cola de este nodo

```cpp
this->is_bussy = (feedbackPkt->delay > this->last_delay + 0.05);
this->last_delay = feedbackPkt->delay;
```
**Comparación del Delay**: Se compara el delay recibido **(feedbackPkt->delay)** con el último delay registrado (this->last_delay) más un margen de tolerancia (0.05 segundos). Si el delay actual es mayor, se considera que la cola del receptor está saturada (is_bussy = true).
**Actualización del Delay**: El valor del delay recibido se almacena como el nuevo last_delay.
```cpp
if (is_bussy) {
    this->delay += 40;
} else {
    if (this->delay > 40) {
        this->delay = 40;
    } else {
        this->delay = 0;
    }
}
scheduleAt(simTime() + this->delay, endServiceEvent);
```
**Incremento del Delay**: Si el receptor está ocupado (is_bussy = true), se incrementa el delay en 40 unidades.
**Reducción del Delay**: Si el receptor no está ocupado, el delay se reduce gradualmente hasta un máximo de 40 unidades o se elimina por completo (this->delay = 0).
Programación del Evento: El transmisor programa el envío del siguiente paquete con el delay ajustado.
