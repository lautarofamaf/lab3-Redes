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

  En este caso de estudio notamos que el *cuello de botella* está en el nodo receptor en la cola que guarda los paquetes a recibir antes de ser enviados al receptor/consumidor final.

+ **Caso de estudio 2:**

  - NodeTx a Queue: *datarate* = 1 Mbps, *delay* = 100 us
  - Queue a NodeRx: *datarate* = 0.5 Mbps, *delay* = 100 us
  - Queue a Sink: *datarate* = 1 Mbps

  En este caso de estudio el *cuello de botella* está en la cola intermedia, en *la red*, entre nodo emisor y nodo receptor.

Para cada uno de los casos corrimos simulaciones paramétricas con:

- *generationInterval* = 0.1
- *generationInterval* = 0.3
- *generationInterval* = 0.5
- *generationInterval* = 0.7
- *generationInterval* = 1


## Caso 1

Experiment	Measurement	Replication	Module	Name	Count	Mean	StdDev	Variance
General		#0	Network.nodeTx.gen	packetGen	1979	1	0	0
General		#0	Network.nodeTx.queue	BufferSize	1979	13.072764	8.249200	68.049303
General		#0	Network.nodeTx.queue	PacketEnqueuedCount	1979	990	571.432411	326,535
General		#0	Network.queue	BufferSize	1970	1	0	0
General		#0	Network.queue	PacketEnqueuedCount	1970	985.500000	568.834334	323,572.500000
General		#0	Network.nodeRx.queue	BufferSize	1199	165.121768	59.178913	3,502.143758
General		#0	Network.nodeRx.queue	PacketEnqueuedCount	1199	600	346.265794	119,900
General		#0	Network.nodeRx.sink	PacketUsed	998	1	0	0
General		#0	Network.nodeRx.sink	Delay	998	32.879469	12.874907	165.763227
General		#0	Network.nodeRx.queue	PacketDropCount	770	385.500000	222.424144	49,472.500000

## Caso 2

Experiment	Measurement	Replication	Module	Name	Count	Mean	StdDev	Variance
General		#0	Network.nodeTx.gen	packetGen	1979	1	0	0
General		#0	Network.nodeTx.queue	BufferSize	1979	13.072764	8.249200	68.049303
General		#0	Network.nodeTx.queue	PacketEnqueuedCount	1979	990	571.432411	326,535
General		#0	Network.queue	BufferSize	1199	165.121768	59.178913	3,502.143758
General		#0	Network.queue	PacketEnqueuedCount	1199	600	346.265794	119,900
General		#0	Network.nodeRx.queue	BufferSize	999	1	0	0
General		#0	Network.nodeRx.queue	PacketEnqueuedCount	999	500	288.530761	83,250
General		#0	Network.nodeRx.sink	PacketUsed	998	1	0	0
General		#0	Network.nodeRx.sink	Delay	998	32.879469	12.874907	165.763227
General		#0	Network.queue	PacketDropCount	771	386	222.712820	49,601

Estos son resultados que nos dio la ejecucion en omnet. Asi tal cual como esta es dificil entender y mas aun sacar una conclusion, por lo que haremos algunas operaciones con los datos para poder observar mas de cerca que esta sucediendo.

1. **% Paquetes Perdidos:**
    PacketDropCount (paquetes descartados) vs PacketEnqueuedCount (paquetes encolados).

    Fórmula:
        %Perdidas=(PacketDropCount/PacketEnqueuedCount en nodeTx) x 100.

    Caso 1: (385.5/990)×100 ≈ 50.1%.

    Caso 2: (386/990)×100 ≈ 50.45%.

2. **Throughput Útil (pkt/s):**
    PacketUsed en el sink (paquetes recibidos) y tiempo de simulación (200s).

    Fórmula:
        Throughput=Total paquetes recibidos/Tiempo de simulacion.

        Caso 1: 998 paquetes/200s ≈ 4.99 pkt/s.

        Caso 2: Mismo cálculo (998/200 ≈ 4.99 pkt/s).

3. **Delay Promedio (s):**
    Columna Delay en Network.nodeRx.sink.

    Valor directo:

        Caso 1: 0.4247s (promedio).

        Caso 2: 32.8795s (promedio).

4. **Ocupación Queue Central:**
    PacketEnqueuedCount en Network.queue.

    Observacion:

        Caso 1: El buffer reporta un promedio de 1 paquete.

        Caso 2: 165.12 paquetes (promedio), lo que indica saturación (el buffer real solo soporta ~1 paquete de 12,500 bytes).


Con un panorama más general de los datos podemos sacar conclusiones de lo que realmente está sucediendo en cada uno de los casos.
Primero observemos que en ambos casos se pierden paquetes, y bastantes para nuestro gusto, pero lo más llamativo es que en cada caso es una parte diferente de nuestra "red" la cual es la causante de estas pérdidas.
En el caso 1 notamos que el promedio de paquetes en el buffer es de 1, es decir, que siempre que entró un paquete a este buffer salió de inmediato, ya que sabemos que este buffer soporta solamente un paquete, por lo que podemos concluir que en la cola del medio no está el problema. Es decir, el nodo Tx manda paquetes, aproximadamente unos 10 pkt/s, también sabemos que en la cola central no hay delay por el punto 3, lo único que queda es fijarnos en el nodo receptor. Este nodo tiene una menor tasa de procesamiento y un buffer bastante más pequeño que el nodo emisor, por lo cual esto hace que el nodo emisor mande paquetes, estos lleguen casi sin delay, y al tener poca capacidad de procesarlos a todos, el nodo receptor los empieza a descartar. A este problema se le llama "problema de flujo".

Muy bien, ahora miremos qué ocurre en el caso 2. A diferencia del caso anterior, podemos ver por 3 y 4 que hay un delay muchísimo mayor, es decir que tardan más en llegar los paquetes al nodo receptor, y que aparte la cola central tiene en promedio muchísimos más paquetes, pero este buffer solo tenía capacidad para un solo paquete... Podemos decir que encontramos el problema. La cola, al no tener un buffer más grande ni una capacidad mayor de procesamiento, no le queda más que empezar a descartar paquetes. A este problema de aumento de tráfico y limitada capacidad de procesamiento entre el receptor y emisor se le llama "problema de congestión".

**Conclusión:** Con estos dos casos logramos visualizar dos problemas muy importantes en la red, como es el problema de flujo y congestión. Lo que haremos a continuación (o por lo menos intentaremos) es crear o proponer algoritmos que resuelvan estas problemáticas, pero teniendo en cuenta que deben ser protocolos entre el receptor y el emisor, es decir, tendremos que abstraernos de todo lo relacionado con la cola central.

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
