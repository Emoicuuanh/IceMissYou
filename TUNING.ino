void tuning()
{
  K1=map(analogRead(32),0,4095,0,100);
  K2=map(analogRead(35),0,4095,0,100);
  K3=map(analogRead(34),0,4095,0,100);
}
void dc()
{
  if(pwm<0)
  {
    digitalWrite(17,1);
    digitalWrite(16,0);
    analogWriteFrequency(2,20000);
    analogWrite(2,abs(pwm));
  }
  else if(pwm>0)
  {
    digitalWrite(17,0);
    digitalWrite(16,1);
    analogWriteFrequency(2,20000);
    analogWrite(2,abs(pwm));
  }
  else if(pwm==0)
  {
    digitalWrite(17,0);
    digitalWrite(16,0);
  }
}