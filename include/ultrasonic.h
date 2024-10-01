extern unsigned int distance;
extern int dist;
extern float Vin;

void ultras_baudrate();
void zero_ohm_read();
void trig_read();
void lp47k_serialMode();
void pinSetup();
void lp300k_echoMode();
float read_voltage();

/*struct DistanceData {
    char imei [16];
    unsigned int distance;
    float batteryVoltage;
};*/
