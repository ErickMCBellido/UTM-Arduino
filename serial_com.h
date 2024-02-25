#ifndef SERIAL_COM_H
#define SERIAL_COM_H

#include "header_lib.h"

void setupSerial(){
  Serial.begin(9600);
  while (!Serial.available()) delay(100);
  //Serial.println("Connection success");
  delay(100);
  Serial.setTimeout(5);
} 

void getTokens(String& input, int numTokens){
  int index, index1;
  index1 = input.indexOf(',');
    for(int i=0; i<numTokens; i++){
      index = index1;
      index1 = input.indexOf(',',index+1);
      tokens[i] = input.substring(index+1,index1);
      intTokens[i] = tokens[i].toInt();
  }
}

void getFTokens(String& input, int numTokens){
  int index, index1;
  index1 = input.indexOf(',');
    for(int i=0; i<numTokens; i++){
      index = index1;
      index1 = input.indexOf(',',index+1);
      tokens[i] = input.substring(index+1,index1);
      floatTokens[i] = tokens[i].toFloat();
  }
}

void getCalibValues(String& input){
  int index, index1;
  index = input.indexOf(',');
  index1 = input.indexOf(',',index+1);
  tokens[0] = input.substring(index+1,index1);
  readedOffset = tokens[0].toInt();

  index = index1;
  index1 = input.indexOf(',',index+1);
  tokens[1] = input.substring(index+1,index1);
  readedScale = tokens[1].toFloat();
}

Operation stopC(){
  String inputData;
  
  while(Serial.available()) {
    inputData = Serial.readStringUntil(';');
    if (!inputData.startsWith("#")) continue;
    
    if(inputData.startsWith(c_stop)){
      return STOP;
    }
    else if (inputData.startsWith(c_e_stop)){
      return E_STOP;
    }
    else {
      return NO_OP;
    }
  }
  return NO_OP;
}

Operation readCommand(){
  String inputData;
  
  while(Serial.available()) {
    inputData = Serial.readStringUntil(';');

    // Commands with #
    if(inputData.startsWith("#")){
      if (inputData.startsWith(c_stop)){
        return STOP;
      }
      else if (inputData.startsWith(c_e_stop)){
        return E_STOP;
      }
      else if (inputData.startsWith(c_clr_sc)){
        return CLR_SC;
      }
      else if (inputData.startsWith(c_calibrate)){
        return CALIBRATE;
      }
      else if (inputData.startsWith(c_t_test)){
        getTokens(inputData,1);
        return T_TEST;
      }
    }

    // Commands with $
    else if(inputData.startsWith("$")){
      if (inputData.startsWith(c_en_motor)){
        return EN_MOTOR;
      }
      else if (inputData.startsWith(c_dis_motor)){
        return DIS_MOTOR;
      }
      else if (inputData.startsWith(c_inv_rot)){
        return INV_ROT;
      }
      else if (inputData.startsWith(c_no_inv_rot)){
        return NO_INV_ROT;
      }
    }

    // Commands with %
    else if(inputData.startsWith("%")){
      if (inputData.startsWith(c_f_up)){
        return F_UP;
      }
      else if (inputData.startsWith(c_f_down)){
        return F_DOWN;
      }
      else if (inputData.startsWith(c_s_up)){
        getTokens(inputData,1);
        return S_UP;
      }
      else if (inputData.startsWith(c_s_down)){
        getTokens(inputData,1);
        return S_DOWN;
      }
      else if (inputData.startsWith(c_c_up)){
        getTokens(inputData,1);
        return C_UP;
      }
      else if (inputData.startsWith(c_c_down)){
        getTokens(inputData,1);
        return C_DOWN;
      }
      else if (inputData.startsWith(c_sc_up)){
        getTokens(inputData,2);
        return SC_UP;
      }
      else if (inputData.startsWith(c_sc_down)){
        getTokens(inputData,2);
        return SC_DOWN;
      }
    }
    
    // Commands with &
    else if(inputData.startsWith("&")){
      if (inputData.startsWith(c_gsc)){
        return G_SC;
      }
      else if (inputData.startsWith(c_g_lcr)){
        return G_LCR;
      }
      else if (inputData.startsWith(c_g_thr)){
        return G_THR;
      }
      else if (inputData.startsWith(c_g_calib)){
        return G_CALIBRATION;
      }
      else if (inputData.startsWith(c_g_dz)){
        return G_DZ;
      }
      else if (inputData.startsWith(c_g_status)){
        return G_STATUS;
      }
    }
        
    // Commands with &
    else if(inputData.startsWith("@")){
      if (inputData.startsWith(c_s_thr)){
        getFTokens(inputData,1);
        return S_THR;
      }
      else if (inputData.startsWith(c_s_crit)){
        getTokens(inputData,1);
        return S_CRITERIA;
      }
      else if (inputData.startsWith(c_s_calib)){
        //getFTokens(inputData,2);
        getCalibValues(inputData);
        return S_CALIBRATION;
      }
      else if (inputData.startsWith(c_s_dz)){
        getFTokens(inputData,1);
        return S_DZ;
      }
    }

    continue;

  }
  return NO_OP;
}


#endif //SERIAL_COM_H
