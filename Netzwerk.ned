package stochastic_battery;

network Netzwerk
{
    submodules:
        Sender: Node {
            @display("p=205,30");
        }
        Receiver: Node {

            @display("p=39,30");
        }
    connections:
        Sender.port <--> Receiver.port;
}
