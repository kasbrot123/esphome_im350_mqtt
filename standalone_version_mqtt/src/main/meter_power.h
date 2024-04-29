
    // if (message[0] == start_byte and message[sizeof(message)-1] == stop_byte) {
    //   Serial.println("Got message from meter, try to decrypt.");
    //   TelnetStream.println("Got message from meter, try to decrypt.");
    //   Serial.print("ReceivedMessage: ");
    //   TelnetStream.print("ReceivedMessage: ");
    //   printBytesToHex(message, (sizeof(message)/sizeof(message[0])));
    //
    //   // todo: why initialize the same vector in the loop? nothing changes ?
    //   init_vector(&Vector_SM, "Vector_SM", sm_decryption_key); 
    //
    //   // print decryption details
    //   Serial.print("IV: ");
    //   TelnetStream.print("IV: ");
    //   printBytesToHex(Vector_SM.iv, (sizeof(Vector_SM.iv)/sizeof(Vector_SM.iv[0])));
    //   Serial.print("Key: ");
    //   TelnetStream.print("Key: ");
    //   printBytesToHex(Vector_SM.key, (sizeof(Vector_SM.key)/sizeof(Vector_SM.key[0])));
    //   Serial.print("Authdata: ");
    //   TelnetStream.print("Authdata: ");
    //   printBytesToHex(Vector_SM.authdata, (sizeof(Vector_SM.authdata)/sizeof(Vector_SM.authdata[0])));
    //   Serial.print("Tag: ");
    //   TelnetStream.print("Tag: ");
    //   printBytesToHex(Vector_SM.tag, (sizeof(Vector_SM.tag)/sizeof(Vector_SM.tag[0])));
    //   Serial.print("Encrypted Data (Ciphertext): ");
    //   TelnetStream.print("Encrypted Data (Ciphertext): ");
    //   printBytesToHex(Vector_SM.ciphertext, (sizeof(Vector_SM.ciphertext)/sizeof(Vector_SM.ciphertext[0])));
    //
    //   decrypt_text(&Vector_SM);
    //   Serial.print("Decrypted Data: ");
    //   TelnetStream.print("Decrypted Data: ");
    //   printBytesToHex(buffer, (sizeof(buffer)/sizeof(buffer[0])));
    //
    //
    //   Serial.print("======Decrypted Parsed Data======\n");
    //   TelnetStream.print("======Decrypted Parsed Data======\n");
    //   parse_message(buffer);
    //   Serial.print("======Decrypted Parsed Data======\n");
    //   TelnetStream.print("======Decrypted Parsed Data======\n");
    //
    //   parse_timestamp(buffer);
    //
    //   // No Validation
    //   // I mean it is already encrypted why should it not be the real data??
    //   //
    //   // if (validate_message_date()) {
    //   //   Serial.println("Do something.");
    //   //   // TelnetStream.println("Do something.");
    //   // }
    //   // else {
    //   //   Serial.println("Do nothing.");
    //   //   // TelnetStream.println("Do nothing.");
    //   // }
    //
    // }
    // else {
    //   Serial.println("Message not starting/ending with 0xE7, skip this message!");
    //   TelnetStream.println("Message not starting/ending with 0xE7, skip this message!");
    //   Serial.print("Received Message: ");
    //   TelnetStream.print("Received Message: ");
    //   printBytesToHex(message, (sizeof(message)/sizeof(message[0])));
    // }
