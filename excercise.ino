// -----------------------------------------------------------------
// Exercise 1
// -----------------------------------------------------------------

#include <FrameStream.h>
#include <Frameiterator.h>
#include <avr/io.h>
#include "Display.h"
bool checkbox=false;

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
endrunnerlist();

// second level
// SL the slider and Ft the checkbox are registerd here
beginrunnerlist(GUI);
fwdrunner(SL, CallbackSL);
fwdrunner(CB,CallbackCB);
endrunnerlist();

/*
 * this is the callback of the Slider SL
 * slider callback str will contain
 * a number ranging from -200 to 1000 in a string
 */
void CallbackCB(char* str, size_t length){
	checkbox=!checkbox;
}
void CallbackSL(char* str, size_t length)
{ bool isFloat=false;
  bool isNegative=false;
  int zahl=0;
  int diviser=0;
    for(unsigned int i=0; str[i]!='\0' && i<3+(isFloat?1:0)+(isNegative?1:0);i++){
      if(str[i]=='-'){
        isNegative=true;
        i++;
      }
      if(str[i]=='.'){
        i++;
        isFloat=true;
      }
      if(isFloat)
        diviser++;
      zahl*=10;
      zahl+=str[i]-'0';
    }
 float gleitZahl=zahl;
 for(unsigned int i=0;i<diviser;i++)
   gleitZahl/=10;
 if(isNegative){
   zahl*=-1;
   gleitZahl*=-1;
 }
 if(checkbox){
   isFloat=false;
   zahl=(int)gleitZahl;
 }
 if(isFloat){
    writeValueToDisplay(gleitZahl);
 }
 else{
    writeValueToDisplay(zahl);
 }
    // TODO interprete string str as integer
    // TODO map integer to
    // show current number string in Arduinoview
    frm.print("!jdocument.getElementById(\"info_val\").innerText=");
    frm.print(str);
    frm.end();
}


/*
 * @brief initialises the GUI of ArduinoView
 *
 * In this function, the GUI, is configured. For this, Arduinoview shorthand,
 * HTML as well as embedded JavaScript can be used.
 */
void InitGUI()
{
    frm.print(F("!h<h1>PKeS Exercise 1</h1>"));
    frm.end();

    // Generating the Slider SL
    frm.print("!SsSL");
    frm.end();
    frm.print("!ScCB");
    frm.end();

    // modify the Slider using JavaScript
    frm.print("!jvar x=document.getElementById(\"_SD_SL\");");
    frm.print("x.max= 1000;");
    frm.print("x.min=-200;");
    frm.print("x.step=.01;");
    frm.print("x.style.width=\"100%\";");
    frm.end();

    // generate some Space to display Information
    frm.print(F("!H<div>Slider value: <span id=info_val></span></div>"));
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

    //request reset of GUI
    frm.print("!!");
    frm.end();

    initDisplay();
    delay(500);
}
/*
 *  @brief Main loop
 *
 *  This function will be called repeatedly and shall therefore implement the
 *  main behavior of the program.
 */
void loop()
{
    // read & run ArduinoView Frames
    while(frm.run());
}