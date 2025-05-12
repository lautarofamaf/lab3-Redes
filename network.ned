// Generator ->
simple Generator
{
    parameters:
        volatile double generationInterval; // sec
        volatile int packetByteSize;
        @display("i=block/source");
    gates:
        output out;
}
// (-> Queue ->)
simple Queue
{
    parameters:
        volatile double serviceTime; // sec
        volatile int bufferSize;
        @display("i=block/queue;q=buffer");
    gates:
        input in;
        output out;
}
// (-> Sink )
simple Sink
{
    parameters:
        @display("i=block/sink");
    gates:
        input in;
}

// NODOS MODIFICADOS ABAJO
//Tarea Diseno

simple TransportTx
{
    parameters:
        int bufferSize; // Tamaño del buffer
        //double sendInterval; // Intervalo de transmisión
        @display("i=block/queue;q=buffer");
    gates:
        input toApp;
        inout toOut;
}
simple TransportRx
{
    parameters:
        int bufferSize; // Tamaño del buffer del receptor
        @display("i=block/queue;q=buffer");
    gates:
        output toApp;
        inout toOut;
}


// (Generattor -> Queue) ->
module NodeTx
{
    parameters:
        @display("i=block/routing");

    gates:
        output out; //to packet queue
        input in; // from feedback queue
    submodules:
        gen: Generator {
            parameters:
                @display("p=75,50");
        }
        traTx: TransportTx {
            parameters:
                @display("p=75,190");
        }

    connections:

        gen.out --> traTx.toApp;   // gen alimenta a traTx
        in --> traTx.toOut$i;     // El feedback que ENTRA a NodeTx (por gate 'in') va a traTx
        traTx.toOut$o --> out;   // Los datos que SALEN de traTx van a la salida de NodeTx (gate 'out')

}
// -> (Queue -> Sink) ->
module NodeRx
{
    parameters:
        @display("i=block/routing");

    gates:
        input in;
        output out;
    submodules:
        sink: Sink {
            parameters:
                @display("p=75,50");
        }
        traRx: TransportRx {
            parameters:
                @display("p=75,190");
        }
    connections:

        in --> traRx.toOut$i;     // Los datos que ENTRAN a NodeRx (por gate 'in') van a traRx
        traRx.toOut$o --> out;   // El feedback que SALE de traRx va a la salida de NodeRx (gate 'out')

        //Case Study 1:
        traRx.toApp --> {  datarate = 0.5Mbps; } --> sink.in;
        //Case Study 2:
        //traRx.toApp --> {  datarate = 1Mbps; } --> sink.in;
}
// (Generattor -> Queue) -> Queue -> (Queue -> Sink)
network Network
{
    @display("bgl=2");
    submodules:
        nodeTx: NodeTx {
            @display("p=30,30");
        }
        queueData: Queue {
            @display("p=130,30");
        }
        queueFeedback: Queue {
            @display("p=130,60");
        }
        nodeRx: NodeRx {
            @display("p=230,30");
        }
    connections:
        nodeTx.out --> {  datarate = 1Mbps; delay = 100us; } --> queueData.in;

        queueFeedback.out --> nodeTx.in;

        nodeRx.out --> queueFeedback.in;
        //Case Study 1:
        queueData.out --> {  datarate = 1Mbps; delay = 100us; } --> nodeRx.in;

        //Case Study 2:
        //queueData.out --> {  datarate = 0.5Mbps; delay = 100us; } --> nodeRx.in;

}

