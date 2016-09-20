
#ifndef _BLE_ACCEL_H
#define _BLE_ACCEL_H
extern void initAccelBLE();
extern void convertToBytes( unsigned char *b, unsigned int v);
extern int getCongfig();
extern int getPeriod();
extern void setAccelData(int x, int y, int z);
extern bool isConnected();
#endif // header stop.