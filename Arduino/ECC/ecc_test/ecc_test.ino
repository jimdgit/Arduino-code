
#include <types.h>
#include <uECC.h>
#include <uECC_vli.h>

#include <uECC.h>

extern "C" {

static int RNG(uint8_t *dest, unsigned size) {
  // Use the least-significant bits from the ADC for an unconnected pin (or connected to a source of 
  // random noise). This can take a long time to generate random data if the result of analogRead(0) 
  // doesn't change very frequently.
  while (size) {
    uint8_t val = 0;
    for (unsigned i = 0; i < 8; ++i) {
      int init = analogRead(0);
      int count = 0;
      while (analogRead(0) == init) {
        ++count;
      }
      
      if (count == 0) {
         val = (val << 1) | (init & 0x01);
      } else {
         val = (val << 1) | (count & 0x01);
      }
    }
    *dest = val;
    ++dest;
    --size;
  }
  // NOTE: it would be a good idea to hash the resulting random data using SHA-256 or similar.
  return 1;
}

}  // extern "C"

void setup() {
  Serial.begin(115200);
  Serial.print("Testing ecc\n");
  uECC_set_rng(&RNG);
}

void loop() {
  const struct uECC_Curve_t * curve = uECC_secp256r1();
  uint8_t private1[32];
  uint8_t private2[32];
  
  uint8_t public1[64];
  uint8_t public2[64];
  
  uint8_t secret1[32];
  uint8_t secret2[32];
  uint8_t sig[64];
  uint8_t hash[8]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7};
  uint8_t signature[64];
  
  unsigned long a = millis();
  Serial.print("Private key size :");Serial.println(uECC_curve_private_key_size( curve));
  Serial.print("Public  key size :");Serial.println(uECC_curve_public_key_size( curve));
  
  uECC_make_key(public1, private1, curve);
  unsigned long b = millis();
    Serial.print("Got here 2 int :");Serial.println(sizeof(int));
    //return;
  Serial.print("Made key 1 in "); Serial.println(b-a);

  a = millis();
  uECC_make_key(public2, private2, curve);
  b = millis();
  Serial.print("Made key 2 in "); Serial.println(b-a);

  a = millis();
  int r = uECC_shared_secret(public2, private1, secret1, curve);
  b = millis();
  Serial.print("Shared secret 1 in "); Serial.println(b-a);
  if (!r) {
    Serial.print("shared_secret() failed (1)\n");
    return;
  }

  a = millis();
  r = uECC_shared_secret(public1, private2, secret2, curve);
  b = millis();
  Serial.print("Shared secret 2 in "); Serial.println(b-a);
  if (!r) {
    Serial.print("shared_secret() failed (2)\n");
    return;
  }
    
  if (memcmp(secret1, secret2, 20) != 0) {
    Serial.print("Shared secrets are not identical!\n");
  } else {
    Serial.print("Shared secrets are identical\n");
  }
  uECC_sign(private1,
             hash,
              sizeof(hash),
              signature,
              curve);
 if ( uECC_verify(public1,
               hash,
                sizeof(hash),
                signature,
                curve) )
  Serial.print("Key 1 Verified!\n");  
  else
  Serial.print("Key 1 FAILED Not Verified!\n");  
               
   uECC_sign(private2,
             hash,
              sizeof(hash),
              signature,
              curve);
 if ( uECC_verify(public2,
               hash,
                sizeof(hash),
                signature,
                curve) )
  Serial.print("Key 2 Verified!\n");  
  else
  Serial.print("Key 2 FAILED Not Verified!\n");                                
}

