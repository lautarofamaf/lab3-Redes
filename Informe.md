# Laboratorio 3: Transporte - Redes y Sistemas Distribuidos
---
En este laboratorio abordamos problemas de tráfico de red bajo tasas de datos acotadas y tamaño de buffers limitados, trabajando con simulaciones y modelos generados en Omnet++.
Se divide en dos partes:
+ **Parte de análisis:** se analiza el tráfico de una red particular, variando dichas tasas de datos y tamaños de buffers.
+ **Parte de diseño:** dado el análisis anterior se busca mejorar el tráfico de red, aumentando su eficiencia.
---
## Parte de análisis

Arrancamos trabajando con un Network basado en un modelo de colas que tiene tres actores:
1. **Nodo transmisor (NodeTx):** tendrá el generador de paquetes y una cola donde esperarán los paquetes antes de ser enviados. El generador presenta dos parámetros, un *generationInterval* que es el intervalo de generación entre paquetes y un *packetByteSize*, que especifica el tamaño de los paquetes generados (12500 bytes).
La cola de este nodo tiene un *bufferSize*, que son la cantidad de paquetes que pueden quedarse esperando a ser enviados luego de ser generados y antes de ser descartados, y un *serviceTime*, que es el tiempo que la cola tardará en procesar el paquete, es decir, enviarlo.
2. **Cola "intermedia" (Queue):** tiene los mismos parámetros que la cola del nodo transmisor, funciona como un buffer entre el nodo transmisor y el nodo receptor, guardando los paquetes antes de ser enviados. El *bufferSize* es de 200 paquetes.
3. **Nodo receptor (NodeRx):** tendrá una cola donde esperarán los paquetes antes de ser finalmente enviados al receptor y el receptor en sí mismo. Esta cola tiene los mismos parámetros que las anteriores.

Analizamos este Network y su funcionamiento para dos casos de estudio:
+ **Caso de estudio 1:** 
    - NodeTx a Queue: *datarate* = 1 Mbps, *delay* = 100 us
    - Queue a NodeRx: *datarate* = 1 Mbps, *delay* = 100 us
    - Queue a Sink: *datarate* = 0.5 Mbps
+ **Caso de estudio 2:**
    - NodeTx a Queue: *datarate* = 1 Mbps, *delay* = 100 us
    - Queue a NodeRx: *datarate* = 0.5 Mbps, *delay* = 100 us
    - Queue a Sink: *datarate* = 1 Mbps

Para cada uno de los casos corrimos simulaciones paramétricas variando el intervalo de generación de paquetes (*generationInterval*) entre 0.1 y 1, particularmente analizamos qué sucedía con 
- *generationInterval* = 0.1
- *generationInterval* = 0.3
- *generationInterval* = 0.5
- *generationInterval* = 0.7
- *generationInterval* = 1


exp1
| generationInterval | Carga ofrecida (pkt/s) | Carga útil (pkt/s) | % de pérdida | *Paquetes Recibidos* | *Delay Promedio (s)* |
| ------------------ | ---------------------- | ------------------ | ------------ |----------------------|----------------------|
| 0.1                | 10.00                  | 4.99               | 50.1%        | 200                  | 0.4247               |
| 0.5                | 2.00                   | 1.95               | 2.5%         | 285                  | 0.4311               |
| 0.7                | 1.43                   | 1.43               | 0.0%         | 389                  | 0.4568               |
| 1.0                | 1.00                   | 1.00               | 0.0%         | 655                  | 0.5757               |
| 0.3                | 3.33                   | 2.99               | 10.21%       | 1998                 | 2.8795               |

exp2
| generationInterval | Carga ofrecida (pkt/s) | Carga útil (pkt/s) | % de pérdida |
| ------------------ | ---------------------- | ------------------ | ------------ |
| 0.1                | 10.00                  | 4.99               | 50.45%       |
| 0.3                | 3.33                   | 3.28               | 0.61%        |
| 0.5                | 2.00                   | 1.95               | 0.26%        |
| 0.7                | 1.43                   | 1.43               | 0.00%        |
| 1.0                | 1.00                   | 1.00               | 0.00%        |
