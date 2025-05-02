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
// (Generattor -> Queue) ->
module NodeTx
{
    parameters:
        @display("i=block/routing");

    gates:
        output out;
    submodules:
        gen: Generator {
            parameters:

                @display("p=75,50");
        }
        queue: Queue {
            parameters:

                @display("p=75,190");


        }
    connections:
        gen.out --> queue.in;
        queue.out --> out;
}
// -> (Queue -> Sink) ->
module NodeRx
{
    parameters:
        @display("i=block/routing");

    gates:
        input in;
    submodules:
        sink: Sink {
            parameters:
                @display("p=75,50");
        }
        queue: Queue {
            parameters:
                @display("p=75,190");

        }
    connections:
        in --> queue.in;
        //Case Study 1:
        //queue.out --> {  datarate = 0.5Mbps;  } --> sink.in;
        //Case Study 2:
        queue.out --> {  datarate = 1Mbps;  } --> sink.in;
}
// (Generattor -> Queue) -> Queue -> (Queue -> Sink)
network Network
{
    @display("bgl=2");
    submodules:
        nodeTx: NodeTx {
            @display("p=30,30");
        }
        queue: Queue {
            @display("p=130,30");
        }
        nodeRx: NodeRx {
            @display("p=230,30");
        }
    connections:
        nodeTx.out --> {  datarate = 1Mbps; delay = 100us;  } --> queue.in;
        //Case Study 1:
        //queue.out --> {  datarate = 1Mbps; delay = 100us;  } --> nodeRx.in;
        //Case Study 2:
        queue.out --> {  datarate = 0.5Mbps; delay = 100us;  } --> nodeRx.in;
}
