package stochastic_battery;

simple transceiver{
    parameters:	//description Text
        int message_length = default(500);
        int sending_interval=default(30);
        string transceiver_type=default("CC2530");
        int transceiver_energy_sending=default(33500);
        int transceiver_energy_receiving=default(24300);
        int transceiver_energy_sleeping=default(200);
        int transceiver_datarate=default(250);
        double receiver_offtime=default(0.95);

    gates:
        inout battery_connection; 
        inout transmission;
}
