#include "MyModbus.h"
#define DEV_ID 0x30

UART Serial2(A4, A5);
MyModbus modbus(DEV_ID);

uint8_t coils[8];
int16_t regs[32];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200);
  delay(2000);

  //各レジスタ初期化
  for(int i=0; i<modbus.COILS_MAX; i++){
    coils[i] = 0;
    modbus.p_coils[i] = &coils[i];
  }
  for(int i=0; i<modbus.REGS_MAX; i++){
    regs[i] = i;
    modbus.p_holding_regs[i] = &regs[i];
    modbus.p_input_regs[i] = &regs[i];
  }
  regs[0] = 48;   //モデルナンバー
  regs[1] = 0001; //バージョン
  regs[2] = 0000; //サブバージョン

  Serial.println("Start...");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial2.available()) {
    delay(10);
    uint8_t rbuf[50];
    int len = 0;
    while (Serial2.available()) {
      rbuf[len] = Serial2.read();
      len++;
    }
    //----デバック用----
    Serial.print("Rx ");
    for (int i = 0; i < len; i++) {
      Serial.print(rbuf[i], HEX);
      Serial.print(' ');
    }
    Serial.print("\r\n");

    //modbus_taskでデータ処理
    uint8_t sbuf[50];
    int sl = modbus.modbus_task(rbuf, len, sbuf);
    Serial2.write(sbuf, sl);

    //----デバック用----
    Serial.print("Tx ");
    for (int i = 0; i < sl; i++) {
      Serial.print(sbuf[i], HEX);
      Serial.print(' ');
    }
    Serial.print("\r\n");
    //------------------

    Serial.print("coils ");
    for (int i = 0; i < 8; i++) {
      Serial.print(*modbus.p_coils[i], HEX);
      Serial.print(' ');
    }
    Serial.println(' ');
    Serial.print("regs ");
    for (int i = 0; i < 32; i++) {
      Serial.print(*modbus.p_holding_regs[i], DEC);
      Serial.print(' ');
    }
    Serial.print("\r\n");
  }
}
