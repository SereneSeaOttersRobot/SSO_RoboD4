#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHSD.h>

// Declarations for analog optosensors
AnalogInputPin right_opto(FEHIO::P1_0);
AnalogInputPin middle_opto(FEHIO::P1_3);
AnalogInputPin left_opto(FEHIO::P1_6);

int main(void)
{
    float x, y; //for touch screen

    // Open output file and prepare for writing values to it
    // <ADD CODE HERE>
    FEHFile * file = SD.FOpen("linefil.txt","w");

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Analog Optosensor Testing");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    // Record values for optosensors on and off of the straight line
    // Left Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place left optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    float val = left_opto.Value();
    SD.FPrintf(file,"Left opto sensor on line : %f",&val);
    // Left Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place left optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    val = left_opto.Value();
    SD.FPrintf(file,"Left opto sensor off line : %f",&val);
    // Repeat process for remaining optosensors, and repeat all three for the curved line values
    // <ADD CODE HERE>

    // Record values for optosensors on and off of the straight line
    // Right Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Right optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    val = right_opto.Value();
    SD.FPrintf(file,"Right opto sensor on line : %f",&val);
    // Right Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Right optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    val = right_opto.Value();
    SD.FPrintf(file,"Right opto sensor off line : %f",&val);

    // Record values for optosensors on and off of the straight line
    // Middle Optosensor on straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Middle optosensor on straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    val = middle_opto.Value();
    SD.FPrintf(file,"Middle opto sensor on line : %f",&val);
    // middle Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Middle optosensor off straight line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    val = middle_opto.Value();
    SD.FPrintf(file,"Middle opto sensor off line : %f",&val);

    //CURVED CODE

    // Record values for optosensors on and off of the curved line
    // Middle Optosensor on curved line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Middle optosensor on curved line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    val = middle_opto.Value();
    SD.FPrintf(file,"Middle opto sensor on curv line : %f",&val);
    // middle Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Middle optosensor off curved line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    val = middle_opto.Value();
    SD.FPrintf(file,"Middle opto sensor off curv line : %f",&val);

    // Record values for optosensors on and off of the curved line
    // Left Optosensor on curved line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Left optosensor on curved line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    val = left_opto.Value();
    SD.FPrintf(file,"Left opto sensor on curv line : %f",&val);
    // middle Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Left optosensor off curved line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    val = left_opto.Value();
    SD.FPrintf(file,"Left opto sensor off curv line : %f",&val);

    // Record values for optosensors on and off of the curved line
    // Middle Optosensor on curved line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Right optosensor on curved line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (1/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    val = right_opto.Value();
    SD.FPrintf(file,"Right opto sensor on curv line : %f",&val);
    // middle Optosensor off straight line
    LCD.Clear(BLACK);
    LCD.WriteLine("Place Right optosensor off curved line");
    Sleep(0.25); // Wait to avoid double input
    LCD.WriteLine("Touch screen to record value (2/12)");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed
    // Write the value returned by the optosensor to your output file
    // <ADD CODE HERE>
    val = right_opto.Value();
    SD.FPrintf(file,"Right opto sensor off curv line : %f",&val);

    // Close output file
    // <ADD CODE HERE> 
    SD.FCloseAll();
    // Print end message to screen
    LCD.Clear(BLACK);
    LCD.WriteLine("Test Finished");

    return 0;
}