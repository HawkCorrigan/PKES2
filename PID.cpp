#include "Odometry.h"
float P=0.75;
float I=4.5;
float D=0.02;
float lastErrorRight=0;
float lastErrorLeft=0;
float diffError=0;
float error=0;
float sumOfErrorsRight=0;
float sumOfErrorsLeft=0;

VelocityData getVelocityValues(float targetVelocityL, float targetVelocityR){
  VelocityData velocityData = getVelocityData();
  VelocityData setValues;
  error=targetVelocityR-velocityData.right;
  sumOfErrorsRight+=error;
  diffError=error-lastErrorRight;
  lastErrorRight=error;
  setValues.right=P*error+I*sumOfErrorsRight*0.075+D*diffError/0.075;
  error=targetVelocityL-velocityData.left;
  sumOfErrorsLeft+=error;
  diffError=error-lastErrorLeft;
    setValues.left=P*error+I*sumOfErrorsLeft*0.075+D*diffError/0.075;
  
  return setValues;
  }
    