//This message transports the information from the transceiver to the battery in which state the transceiver is and how much energy it need for that state.

message power_update{
    int power_consum;
    char current_activity;
}