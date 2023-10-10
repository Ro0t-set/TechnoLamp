#define DROP_RATE 2


int music_led_controller(int lower_bount, int upper_bound, int lower_bound, uint8_t fht_log_out[], int previus_value, int pin, int jump_diff) {

  int medium_value = 0;

  for (int i = lower_bount; i < upper_bound; i++) {
    medium_value = medium_value + fht_log_out[i];
  }
  medium_value = medium_value / (upper_bound - lower_bount);

  digitalWrite(pin, LOW);

  if (medium_value > medium_value && (medium_value - (jump_diff / previus_value)) > previus_value) {
    digitalWrite(13, HIGH);
    previus_value = medium_value;
  }

  if (previus_value > lower_bound) {
    previus_value -= DROP_RATE;
  }


  return previus_value;
}
