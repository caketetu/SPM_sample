#include "SPM_Modbus.h"

//UART Serial2(A4, A5);

int16_t ro_value[16];
int16_t rw_value[16];
uint8_t dio = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial2.begin(115200);
  delay(2000);

  SPM_ModbusSetAddress(0xff);

  ro_value[0] = 48;    //モデルナンバー
  ro_value[1] = 0002;  //バージョン
  ro_value[2] = 0000;  //サブバージョン

  rw_value[0] = 0xffff;  //デバイスID

  //各レジスタ初期化
  //コイル---rwレジスタと同じ
  p_coils[0] = &dio;  //&rw_value[0];
  p_coils[1] = &dio;  //&rw_value[1];

  for (int i = 0; i < 16; i++) {
    p_holding_regs[i] = &rw_value[i];
    p_input_regs[i] = &ro_value[i];
  }
  for (int i = 16; i < 32; i++) {
    p_input_regs[i] = &rw_value[i - 16];
  }

  //Cyclic Function0登録
  cycfunc0.rx_len = 1;
  cycfunc0.rx_adr[0] = &rw_value[0];
  cycfunc0.tx_len = 1;
  cycfunc0.tx_adr[0] = &rw_value[0];

  //Cyclic Function1登録
  cycfunc1.rx_len = 2;
  cycfunc1.rx_adr[0] = &rw_value[0];
  cycfunc1.rx_adr[1] = &rw_value[1];
  cycfunc1.tx_len = 2;
  cycfunc1.tx_adr[0] = &rw_value[0];
  cycfunc1.tx_adr[1] = &rw_value[1];

  //Cyclic Function2登録
  cycfunc2.rx_len = 3;
  cycfunc2.rx_adr[0] = &rw_value[0];
  cycfunc2.rx_adr[1] = &rw_value[1];
  cycfunc2.rx_adr[2] = &rw_value[2];
  cycfunc2.tx_len = 3;
  cycfunc2.tx_adr[0] = &rw_value[0];
  cycfunc2.tx_adr[1] = &rw_value[1];
  cycfunc2.tx_adr[2] = &rw_value[2];

  //Cyclic Function3登録
  cycfunc3.rx_len = 1;
  cycfunc3.rx_adr[0] = &rw_value[0];
  cycfunc3.tx_len = 2;
  cycfunc3.tx_adr[0] = &rw_value[0];
  cycfunc3.tx_adr[1] = &rw_value[1];

  //Cyclic Function4登録
  cycfunc4.rx_len = 8;
  cycfunc4.rx_adr[0] = &rw_value[0];
  cycfunc4.rx_adr[1] = &rw_value[1];
  cycfunc4.rx_adr[2] = &rw_value[2];
  cycfunc4.rx_adr[3] = &rw_value[3];
  cycfunc4.rx_adr[4] = &rw_value[4];
  cycfunc4.rx_adr[5] = &rw_value[5];
  cycfunc4.rx_adr[6] = &rw_value[6];
  cycfunc4.rx_adr[7] = &rw_value[7];
  cycfunc4.tx_len = 8;
  cycfunc4.tx_adr[0] = &rw_value[0];
  cycfunc4.tx_adr[1] = &rw_value[1];
  cycfunc4.tx_adr[2] = &rw_value[2];
  cycfunc4.tx_adr[3] = &rw_value[3];
  cycfunc4.tx_adr[4] = &rw_value[4];
  cycfunc4.tx_adr[5] = &rw_value[5];
  cycfunc4.tx_adr[6] = &rw_value[6];
  cycfunc4.tx_adr[7] = &rw_value[7];

  int t = SPM_ModbusParamLoad();
  SPM_ModbusSetAddress((uint8_t)rw_value[0]);
  //Serial2.println("Start...");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    delay(10);
    uint8_t rbuf[50];
    int len = 0;
    while (Serial.available()) {
      rbuf[len] = Serial.read();
      len++;
    }
    //----デバック用----
    /*
    Serial2.print("Rx ");
    for (int i = 0; i < len; i++) {
      Serial2.print(rbuf[i], HEX);
      Serial2.print(' ');
    }
    Serial2.print("\r\n");
    */

    //modbus_taskでデータ処理
    uint8_t sbuf[100];
    int sl = SPM_ModbusTask(rbuf, len, sbuf);
    Serial.write(sbuf, sl);

    //----デバック用----
    /*
    Serial2.print("Tx ");
    for (int i = 0; i < sl; i++) {
      Serial2.print(sbuf[i], HEX);
      Serial2.print(' ');
    }
    Serial2.print("\r\n");
    //------------------

    Serial2.print("coils ");
    for (int i = 0; i < 8; i++) {
      Serial2.print(*p_coils[i], HEX);
      Serial2.print(' ');
    }
    Serial2.println(' ');
    Serial2.print("regs ");
    for (int i = 0; i < 32; i++) {
      Serial2.print(*p_holding_regs[i], DEC);
      Serial2.print(' ');
    }
    Serial2.print("\r\n");
    */
  }
}
