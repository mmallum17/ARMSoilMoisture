/*Exported Types*/
typedef struct
{
  uint8_t startDelimiter;
  uint16_t length;
  uint8_t frameType;
  uint8_t frameId;
  uint64_t sourceAddress64;
  uint16_t sourceAddress16;
  uint16_t remoteCommand;
  uint8_t status;
  uint8_t samples;
  uint8_t analogMask;
  uint8_t digitalMask;
  uint16_t digitalSample;
  uint16_t moistureSample;
  uint16_t temperatureSample;
  uint8_t checksum;
}APIResponseFrame;

/*Public Function Prototypes*/
void initXbee();
void sensorTransmit();
void sensorReceive(APIResponseFrame* samplesFrame);
float toCelsius(uint16_t tempSample);
float toFahrenheit(float celsius);
void floatToString(float value, char* floatString, int afterpoint);
