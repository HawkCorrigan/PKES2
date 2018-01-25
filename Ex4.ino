
// -----------------------------------------------------------------
// Exercise 4
// -----------------------------------------------------------------

#include <FrameStream.h>
#include <Frameiterator.h>
#include <avr/io.h>
#include "Distance.h"
#include "Odometry.h"
#include "PID.h"
#include "everytime.h"
#include "MotorControl.h"
#include <math.h>







#define OUTPUT__BAUD_RATE 57600
FrameStream frm(Serial1);

// Forward declarations
void InitGUI();

// hierarchical runnerlist, that connects the gui elements with
// callback methods
declarerunnerlist(GUI);

// First level will be called by frm.run (if a frame is recived)
beginrunnerlist();
fwdrunner(!g, GUIrunnerlist); //forward !g to the second level (GUI)
callrunner(!!, InitGUI);
fwdrunner(ST, stickdata);
endrunnerlist();

// GUI level
beginrunnerlist(GUI);
callrunner(es, CallbackSTOP);
callrunner(ms, CallbackSTART);
endrunnerlist();


void stickdata(char* str, size_t length)
{
  int left = (int) atof(str);
    int i; int right=0;
    for(i=0; i<length; i++){
      if (*(str+i)==','){
        right=(int) atof(str+i+1);
        break;
      }
    }
  setMotors(left,right);
}

void CallbackSTOP()
{
    deactivateMotors();
}


void CallbackSTART()
{
    activateMotors();
}




/*
 * @brief initialises the GUI of ArduinoView
 *
 * In this function, the GUI, is configured. For this, Arduinoview shorthand,
 * HTML as well as embedded JavaScript can be used.
 */
void InitGUI()
{
    delay(500);

    frm.print(F("!SbesvSTOP"));
    frm.end();

    frm.print(F("!SbmsvSTART"));
    frm.end();


    //this implements a joystick field using HTML SVG and JS
    //and some info divs next to it
    frm.print(F("!H"
                "<div><style>svg * { pointer-events: none; }</style>\n"
                "<div style='display:inline-block'> <div id='state'> </div>\n"
                "<svg id='stick' width='300' height='300' viewBox='-150 -150 300 300' style='background:rgb(200,200,255)' >\n"
                "<line id='pxy' x1='0' y1='0' x2='100' y2='100' style='stroke:rgb(255,0,0);stroke-width:3' />\n"
                "<circle id='cc' cx='0' cy='0' r='3'  style='stroke:rgb(0,0,0);stroke-width:3' />\n"
                "</svg></div>"
                "<div style='display:inline-block'>"
                "<div id='info0'></div>"
                "<div id='info1'></div>"
                "<div id='info2'></div>"
                "</div></div>\n"
                "<script>\n"
                "var getEl=function(x){return document.getElementById(x)};\n"
                "var setEl=function(el,attr,val){(el).setAttributeNS(null,attr,val)};\n"
                "var stick=getEl('stick');\n"
                "function sticktransform(x,y){\n"
                "x = x-150;\n"
                "y = -(y-150);\n"
                "setstick(x,y);\n"
                "}\n"
                "function setstick(x,y){\n"
                "setpointer(x,y);\n"
                "l = Math.floor(Math.min(127,Math.max(-127,y + x/2)));\n"
                "r = Math.floor(Math.min(127,Math.max(-127,y - x/2)));\n"
                "setStateDisplay(x,y,l,r);\n"
                "sendframe(\"ST\"+l +\",\"+r);\n"
                "}\n"
                "function setStateDisplay(x,y,l,r){\n"
                "msg=getEl('state');\n"
                "msg.innerHTML= 'x= '+ x +' y= '+ y +' l= '+ l +' r= '+ r ;\n"
                "}\n"
                "function setpointer(x,y){\n"
                "pxy=getEl('pxy');\n"
                "setEl(pxy,'x2',x);\n"
                "setEl(pxy,'y2',-y);\n"
                "}\n"
                "stick.onmousemove=function(e){\n"
                "if( e.buttons == 1 ){\n"
                "sticktransform(e.offsetX,e.offsetY);\n"
                "}};\n"
                "stick.onmousedown=function(e){\n"
                "if( e.buttons == 1 ){\n"
                "sticktransform(e.offsetX,e.offsetY);\n"
                "}};\n"
                "stick.onmouseup=function(e){\n"
                "setstick(0,0);\n"
                "};\n"
                "stick.onmouseleave=function(e){\n"
                "setstick(0,0);\n"
                "};\n"
                "function sendframe(msg){\n"
                "ArduinoView.sendMessage(ArduinoView._createSerialFrame(msg))\n"
                "}\n"
                "setstick(0,0);\n"
                "</script>\n"));
    frm.end();






}

/*
 * @brief Initialisation
 *
 * Implement basic initialisation of the program.
 */


void setup()
{
    //prepare Serial interfaces
    Serial.begin(OUTPUT__BAUD_RATE);
    Serial1.begin(OUTPUT__BAUD_RATE);

    Serial.println(F("Willkommen zur PKeS Übung"));

    Serial.println(F("PKes4"));

    //request reset of GUI
    frm.print("!!");
    frm.end();

    delay(500);

    // TODO initialize Odometry, Sensors, Motors, etc.
  initOdom();
  configADC();
  //initSensors();
  initMotors();
  deactivateMotors();
}

/*
 *  @brief Main loop
 *
 *  This function will be called repeatedly and shall therefore implement the
 *  main behavior of the program.
 */

void loop()
{

  every(50){
    motorMain();
      
  }
  every(75){
    //VelocityData vD = getVelocityValues(0.5,0.5);
    //setVelocityMotors(vD.left,vD.right);
    //activateMotors();
  }
  
    // read & run ArduinoView Frames
    while(frm.run());
}

