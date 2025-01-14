void checkgoc()
{
  if(abs(mpu6050.getAngleX())<0.4) {dungthang=1;}
  else if(abs(mpu6050.getAngleX())>10){dungthang=0;}
}
void getdata()
{
  angleX=mpu6050.getAngleX();
  gyroX=mpu6050.getGyroX();
}
void ble()
{
    c = SerialBT.read();
  if(c=='t'){myServo.write(180);  }
  else if(c=='l'){myServo.write(0);  }
    else if(c=='L'){myServo.write(90);  }
      else if(c=='T'){myServo.write(90);  }
  else if(c=='a'){goc=goc+5;myServo1.write(goc);  }
  else if(c=='p'){goc=goc-5;myServo1.write(goc);   }
}
void saveOffsets() {
    EEPROM.put(0, offsets);  // Lưu offsets vào địa chỉ 0
    EEPROM.commit();         // Lưu thay đổi vào EEPROM
}
void loadOffsets() {
    EEPROM.get(0, offsets);  // Đọc offsets từ địa chỉ 0
    if (offsets.ID != 35) {  // Kiểm tra ID để xác nhận giá trị hợp lệ
        // Nếu không hợp lệ, bạn có thể thiết lập giá trị mặc định hoặc yêu cầu cali
        offsets.AcX = 0;
        offsets.AcY = 0;  
        offsets.AcZ = 0;  
        offsets.ID = 35;   // Đặt ID để xác nhận rằng giá trị đã được lưu
        saveOffsets();     // Lưu lại giá trị mới
    }
}
