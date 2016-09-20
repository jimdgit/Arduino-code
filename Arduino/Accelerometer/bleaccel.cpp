#include <CurieBLE.h>
#include "bleaccel.h"
#include "string.h"

#define ACCEL_BYTES  2
#define LED_PIN     13   

void blePeripheralConnectHandler(BLECentral& central) ;
void blePeripheralDisconnectHandler(BLECentral& central); 
void bleCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic);
void accelCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic);

int period = 1000;
int config_value = 0;
bool isConnectedFlag = false;

BLEPeripheral blePeripheral;

BLEService accelService("AA10");     // ACCEL service

// The Accel data is 3 signed 16 bit ints.
BLECharacteristic accelDataChar("AA11",  BLERead | BLENotify, ACCEL_BYTES * 3);    
BLEDescriptor accelDataCharUserDesc("2901","Accel Data");

// Enables converts and sets the "Gs"
BLEUnsignedCharCharacteristic accelConfigChar("AA12",  BLERead | BLEWrite);  
BLEDescriptor accelConfigCharUserDesc("2901","Accel Conf");

// The sample period, in milliseconds defaluts to 1000
BLECharacteristic accelPeriodChar("AA13",  BLERead | BLEWrite, 2);    // This set the sample perion
BLEDescriptor accelPeriodCharUserDesc("2901","Acc Period");

// Default bogus data for testing
unsigned char accel_data[ACCEL_BYTES * 3]={0x01,0x01,
                             0x02,0x01,
                             0x03,0x01,
                             };
                          
void initAccelBLE()
{
  pinMode(LED_PIN, OUTPUT);   // initialize the LED on pin 13 to indicate when a central is connected

  // set the local name peripheral advertises
  blePeripheral.setLocalName("ACCEL");
  // set the UUID for the service this peripheral advertises
  blePeripheral.setAdvertisedServiceUuid(accelService.uuid());  // Add the service ID.
  blePeripheral.addAttribute(accelService); 
  
  // Add characteristics and descriptions
  blePeripheral.addAttribute(accelDataChar);
  blePeripheral.addAttribute(accelDataCharUserDesc);  
  accelDataChar.setValue(&accel_data[0], ACCEL_BYTES * 3);

  blePeripheral.addAttribute(accelConfigChar); 
  blePeripheral.addAttribute(accelConfigCharUserDesc);  
  accelConfigChar.setEventHandler(BLEWritten, accelCharacteristicWritten);
  
  blePeripheral.addAttribute(accelPeriodChar);   
  blePeripheral.addAttribute(accelPeriodCharUserDesc);   
  accelPeriodChar.setEventHandler(BLEWritten, accelCharacteristicWritten);
  
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);  
  
  blePeripheral.begin();
  Serial.println("BLE Set up complete");
}

bool isConnected()
{
  return isConnectedFlag;
}

void blePeripheralConnectHandler(BLECentral& central) 
{
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  digitalWrite(LED_PIN, HIGH);
  isConnectedFlag = true;
}

void blePeripheralDisconnectHandler(BLECentral& central) 
{
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  digitalWrite(LED_PIN, LOW);
  isConnectedFlag = false;
  period = 1000;
  config_value = 0;
}

void accelCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) 
{
  const char* uuid_notify = characteristic.uuid();
  
  Serial.print("Characteristic event, written uuid = ");
  Serial.print(uuid_notify);
  
  // Test which characteristic was written to.
  if( !strcmp(uuid_notify,accelConfigChar.uuid()) )
  {
      config_value = accelConfigChar.value();
      Serial.print(" accelConfigChar was written value = ");
      Serial.println(accelConfigChar.value() );
      return;
  }
  
  if( !strcmp(uuid_notify,accelPeriodChar.uuid()) )
  {
      Serial.print(" accelPeriodChar was written value = ");
      const unsigned char *v  =  accelPeriodChar.value();
      period = (v[0] * 256) + v[1];
      Serial.println(period );
      return;
  }
  
  Serial.println(" Unknown UUID");
  
}
void convertToBytes(  unsigned char *b, unsigned int v)
{
  int m = (ACCEL_BYTES - 1) * 8;
  
  for( int i = 0 ; i < ACCEL_BYTES ; ++i )
  {
    b[i] =  v >> m;;
    m -= 8;
  }  
}
void setAccelData(int x, int y, int z)
{
   convertToBytes( &accel_data[0], x);
   convertToBytes( &accel_data[ACCEL_BYTES], y);
   convertToBytes( &accel_data[ACCEL_BYTES * 2], z);
   accelDataChar.setValue(&accel_data[0], ACCEL_BYTES * 3);
}
int getCongfig()
{
  return config_value;
}
int getPeriod()
{
  return period;
}
