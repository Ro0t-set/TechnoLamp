#define LOG_OUT 1  // use the log output function
#define FHT_N 256  // set to 256 point fht
#define MIN_BASS_RANGE 2
#define MAX_BASS_RANGE 3

#define MIN_MED_RANGE 8
#define MAX_MED_RANGE 11


#include <Arduino.h>
#include <math.h>
#include <RotaryEncoder.h>
#include <FHT.h>  // include the library
#include "switchController.h"


int encoder_clk_s1 = 7;
int encoder_dt_s2 = 5;
int encoder_btn = 6;

RotaryEncoder encoder(encoder_clk_s1, encoder_dt_s2, RotaryEncoder::LatchMode::TWO03);

int prevClk;
int prevDt;

int last_volume_bass = 200;
int last_volume_med = 100;

int bass_lower_bound = 70;
int med_lower_bound = 50;

void setup() {
  Serial.begin(115200);  // use the serial port
  TIMSK0 = 0;            // turn off timer0 for lower jitter
  ADCSRA = 0xe5;         // set the adc to free running mode
  ADMUX = 0x40;          // use adc0
  DIDR0 = 0x01;          // turn off the digital input for adc0
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);

}

  
void encoder_handler3() {
  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (newPos > 200) {
    newPos = 200;
    encoder.setPosition(newPos);
  }
  if (newPos < 50) {
    newPos = 50;
    encoder.setPosition(newPos);
  }
  if (pos != newPos) {
    pos = newPos;
  }  // if

  Serial.print("pos:");
  Serial.print(pos);
  Serial.print(" dir:");
  Serial.println((int)(encoder.getDirection()));

} 



// loop ()
void loop() {
  while (1) {                          // reduces jitter
    cli();                             // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0; i < FHT_N; i++) {  // save 256 samples
      while (!(ADCSRA & 0x10))
        ;             // wait for adc to be ready
      ADCSRA = 0xf5;  // restart adc
      byte m = ADCL;  // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m;  // form into an int
      k -= 0x0200;           // form into a signed int
      k <<= 6;               // form into a 16b signed int
      fht_input[i] = k;      // put real data into bins
    }
    fht_window();   // window the data for better frequency response
    fht_reorder();  // reorder the data before doing the fht
    fht_run();      // process the data in the fht
    fht_mag_log();  // take the output of the fht
    sei();



    encoder_handler3();

    last_volume_bass = music_led_controller(MIN_BASS_RANGE, MAX_BASS_RANGE, bass_lower_bound, fht_log_out, last_volume_bass, 13, 1300);
    last_volume_med = music_led_controller(MIN_BASS_RANGE, MAX_MED_RANGE, med_lower_bound, fht_log_out, last_volume_med, 12, 1500);



    /*
         Serial.write(255); // send a start byte
         fht_log_out[FHT_N / 2 - 2] = bass_lower_bound;

         fht_log_out[FHT_N / 2 - 1] = last_volume_bass;
         Serial.write(fht_log_out, (FHT_N/2)); // send out the data
      
      */

  }
}
