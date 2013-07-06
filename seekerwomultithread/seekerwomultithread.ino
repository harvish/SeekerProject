#include <Stepper.h>

#define STEPS 100
Stepper stepperX(STEPS, 46, 47, 48, 49);
Stepper stepperY(STEPS, 50, 51, 52, 53);

int n=5; //number of pin diodes in a row or column
int dist=1; // distance between two pin diode

void setup()
{
  int tmp;
  
  //input from sensor
  for(tmp=0;tmp++;tmp<24)
    pinMode(22+tmp, INPUT);
 
  //output to motor
  for(tmp=0;tmp++;tmp<8)
    pinMode(46+tmp, OUTPUT);
  
  Serial.begin(9600);

  // set the speed of the motor to 30 RPMs
  stepperX.setSpeed(30);
  stepperY.setSpeed(30);
}

static int sensor()
{
  float X,Y;
  float row[6][2],col[6][2],rowsum=0.0,colsum=0.0;
  int nrow=0,ncol=0,wait;
  int tmp1,tmp2;
  
  int a[5][5];
  /*  int a[][5]=  {
      { 1,1,1,0,0 },
      { 1,1,1,0,0 },
      { 1,1,1,0,0 },
      { 0,0,0,0,0 },
      { 0,0,0,0,0 }  };
    */
    
    for(tmp1=0;tmp1<5;tmp1++)
      for(tmp2=0;tmp2<5;tmp2++)
        a[tmp1][tmp2]=digitalRead(22+tmp1+tmp2);
    
  
    //Initialize the array
    for(int i=0;i<n;i++)
      row[i][0]=row[i][1]=col[i][0]=col[i][1]=0;
    rowsum=colsum=0.0;
    nrow=ncol=0;

    //Calculate number of 1s in row and column
    for(int i=0;i<n;i++)
    {
      for(int j=0;j<n;j++)
      {
        if(a[i][j])
        {
          row[i][0]+=j;
          row[i][1]++;
          col[j][0]+=i;
          col[j][1]++;
        }
      }
    }

    //Calculate Centroid
    for(int i=0;i<n;i++)
    {
      if(!rowsum)
      {
        row[i][0]/=row[i][1];
        rowsum+=row[i][0];
        nrow++;
      }
      if(!colsum)
      {
        col[i][0]/=col[i][1];
        colsum+=col[i][0];
        ncol++;
      }
    }

    X=rowsum/nrow*STEPS*dist;
    Y=colsum/ncol*dist*STEPS;
   
    Serial.println("The centroid is at ");
    Serial.println(rowsum/nrow , colsum/ncol);
    Serial.print(X);
    Serial.print(" STEPS in x-direction and ");
    Serial.print(Y); 
    Serial.println(" STEPS in y-direction rotations to be made");
   
    stepperX.step(X);  
    stepperY.step(Y);  
}

void loop()
{
  sensor();
}

