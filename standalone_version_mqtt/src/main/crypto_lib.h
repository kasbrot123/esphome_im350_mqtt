// encryption / decryption tools

// struct Vector_GCM {
//     const char *name;
//     uint8_t key[16];
//     uint8_t ciphertext[90];
//     uint8_t authdata[16];
//     uint8_t iv[12];
//     uint8_t tag[12];
//     size_t authsize;
//     size_t datasize;
//     size_t tagsize;
//     size_t ivsize;
// };


// void init_vector(Vector_GCM *vect, const char *Vect_name, byte *key_SM) {
//   vect->name = Vect_name;  // vector name
//
//   // asign key
//   for (unsigned int i = 0; i < 16; i++) {
//     vect->key[i] = key_SM[i];
//   }
//
//   // asign message to decrypt
//   for (unsigned int i = 0; i < 90; i++) {
//     vect->ciphertext[i] = message[i+30];
//   }
//
//   // what is authdata?
//   byte AuthData[] = {0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf}; // fixed value, i got it from the gurus director software
//   for (int i = 0; i < 16; i++) {
//      vect->authdata[i] = AuthData[i];
//   }
//
//   for (int i = 0; i < 8; i++) {
//      vect->iv[i] = message[16+i]; // manufacturer + serialnumber 8 bytes
//   }
//   for (int i = 8; i < 12; i++) {
//     vect->iv[i] = message[18+i]; // frame counter
//   }
//
//   byte tag[12]; // 12x zero
//   for (int i = 0; i < 12; i++) {
//     vect->tag[i] = tag[i];
//   }
//
//   vect->authsize = 16;
//   vect->datasize = 90;
//   vect->tagsize = 12;
//   vect->ivsize  = 12;
// }

// void decrypt_text(Vector_GCM *vect) {
//     
//
//     gcmaes128 = new GCM<AES128>();
//     gcmaes128->setKey(vect->key, gcmaes128->keySize());
//     gcmaes128->setIV(vect->iv, vect->ivsize);
//     gcmaes128->decrypt((byte*)buffer, vect->ciphertext, vect->datasize);
//
//     // this does not work...
//     // bool decryption_failed = false;
//     // if (!gcmaes128->checkTag(vect->tag, vect->tagsize)) {
//     //   decryption_failed = true;
//     //   Serial.println("Decryption Failed!");
//     // }
//     // else {
//     //   Serial.println("Decryption OK!");
//     // }
//     delete gcmaes128;
// }




int decryptBuffer(byte *key, byte *auth_key) {
    GCM<AES128> *gcmaes128 = 0;
    int offset = 2 + buffer[1] + 8; // sollte passen, type conversion
    Serial.println(offset);
    int cyphertext_length = sizeof(buffer) - offset - 12; // oder über buffer[11:12]; laenge buffer nicht vergessen
    Serial.println(cyphertext_length);

    // byte plaintext[cyphertext_length];
    byte cyphertext[cyphertext_length];
    byte init_vector[buffer[1]+4];
    byte tag[12];
    Serial.println('init_vector length');
    Serial.println(sizeof(init_vector));
    for (int i = 0; i < cyphertext_length; i++) {
        cyphertext[i] = buffer[offset+i];
    }
    for (int i = 0; i < buffer[1]; i++) { // Int type conversion passt nicht
        init_vector[i] = buffer[2+i];
    }
    for (int i = 0; i < 4; i++) {
        init_vector[buffer[1]+i] = buffer[ 2+buffer[1]+4 + i ];
    }
    for (int i = 0; i < 12; i++) {
        tag[i] = buffer[cyphertext_length+i];
        Serial.print(buffer[cyphertext_length+i]);
    }
    Serial.println();

    for (int i = 0; i < 12; i++) {
        if (init_vector[i] < 16) {Serial.print("0");}
        Serial.print(init_vector[i], HEX);
    }
    Serial.println();


    gcmaes128 = new GCM<AES128>();
    gcmaes128->setKey(key, KEY_LENGTH);
    gcmaes128->setIV(init_vector, buffer[1]+4);
    gcmaes128->addAuthData(auth_key, sizeof(auth_key));
    gcmaes128->decrypt((byte*)plaintext, cyphertext, cyphertext_length);

    // this does not work...
    // bool decryption_failed = false;
    if (!gcmaes128->checkTag(tag, 12)) {
      // decryption_failed = true;
      Serial.println("Decryption Failed!");
    }
    else {
      Serial.println("Decryption OK!");
    }
    delete gcmaes128;
    return cyphertext_length;
}



