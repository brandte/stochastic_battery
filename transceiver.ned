package stochastic_battery;

simple transceiver{
    parameters:	//description Text
        int message_length = default(500);
        int sending_interval=default(30);
        string transceiver_type=default("CC2530");

    gates:
        inout battery_connection; 
        inout transmission;
}
