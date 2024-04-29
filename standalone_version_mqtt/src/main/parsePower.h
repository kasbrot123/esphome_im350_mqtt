
// void parse_message(byte array[]) {
void parsePower(byte array[]) {
      counter_reading_p_in = byteToUInt32(array, 57);
      counter_reading_p_out = byteToUInt32(array, 62);
      counter_reading_q_in = byteToUInt32(array, 67);
      counter_reading_q_out = byteToUInt32(array, 72);
      current_power_usage_in = byteToUInt32(array, 77);
      current_power_usage_out = byteToUInt32(array, 82);
      
      char reading_p_in[10];
      char reading_p_out[10];
      char reading_q_in[10];
      char reading_q_out[10];
      char power_in[10];
      char power_out[10];

    //syntax double to string: dtostrf(double __val, char __width, char __prec, char *__s)
    
      // Serial.println(result, DEC);
      Serial.printf("counter_reading_p_in: %d\n", counter_reading_p_in);
      TelnetStream.printf("counter_reading_p_in: %d\n", counter_reading_p_in);
      dtostrf(counter_reading_p_in, 10, 0, reading_p_in);
      client.publish("ingmarsretro/SM_Kelag/Zaehler_Wirk_in", reading_p_in);

      Serial.printf("counter_reading_p_out: %d\n", counter_reading_p_out);
      TelnetStream.printf("counter_reading_p_out: %d\n", counter_reading_p_out);
      dtostrf(counter_reading_p_out, 10, 0, reading_p_out);
      client.publish("ingmarsretro/SM_Kelag/Zaehler_Wirk_out", reading_p_out);

      Serial.printf("counter_reading_q_in: %d\n", counter_reading_q_in);
      TelnetStream.printf("counter_reading_q_in: %d\n", counter_reading_q_in);
      dtostrf(counter_reading_q_in, 10, 0, reading_q_in);
      client.publish("ingmarsretro/SM_Kelag/Zaehler_Blind_in", reading_q_in);

      Serial.printf("counter_reading_q_out: %d\n", counter_reading_q_out);
      TelnetStream.printf("counter_reading_q_out: %d\n", counter_reading_q_out);
      dtostrf(counter_reading_q_out, 10, 0, reading_q_out);
      client.publish("ingmarsretro/SM_Kelag/Zaehler_Blind_out", reading_q_out);

      Serial.printf("current_power_usage_in: %d\n", current_power_usage_in);
      TelnetStream.printf("current_power_usage_in: %d\n", current_power_usage_in);
      dtostrf(current_power_usage_in, 10, 0, power_in);
      client.publish("ingmarsretro/SM_Kelag/WirkLeistung_in", power_in);

      Serial.printf("current_power_usage_out: %d\n", current_power_usage_out);
      TelnetStream.printf("current_power_usage_out: %d\n", current_power_usage_out);
      dtostrf(current_power_usage_out, 10, 0, power_out);
      client.publish("ingmarsretro/SM_Kelag/WirkLeistung_out", power_out);
}
