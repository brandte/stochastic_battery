package stochastic_battery;

network Netzwerk
{
    submodules:
        Sender: Node;
        Receiver: Node {

            @display("p=91,30");
        }
    connections:
        Sender.port <--> {  delay = 100ms; } <--> Receiver.port;
}
