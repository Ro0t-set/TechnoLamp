#define DROP_RATE 2


int music_led_controller(int lower_bound, int upper_bound, int sensibility, uint8_t fht_log_out[], int previus_value, int pin, int jump_diff) {

  int medium_value = 0;

  for (int i = lower_bound; i < upper_bound; i++) {
    medium_value = medium_value + fht_log_out[i];
  }
  medium_value = medium_value / (upper_bound - lower_bound);

  digitalWrite(pin, LOW);
  //Serial.println("------------");
  //Serial.println(medium_value);
  //Serial.println(previus_value);
  //Serial.println((medium_value - (jump_diff / previus_value)));

  if (medium_value > sensibility && (medium_value - (jump_diff / previus_value)) > previus_value) {
    digitalWrite(pin, HIGH);
    previus_value = medium_value;
  }

  if (previus_value > sensibility) {
    previus_value -= DROP_RATE;
  }


  return previus_value;
}
