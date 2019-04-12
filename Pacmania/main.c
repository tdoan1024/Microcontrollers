
// Tai Doan
// Assignment 1: Pacmania
// The Pacmania program shall display a “Pacman” image on the PSoC4 GLCD.
// The wedge-shaped mouth of the Pacman image shall be made to appear to open and close repeatedly 
// as long as the program is running.


#include <device.h>
#include <header01.h>
#include <math.h> 
 
void GLCD_Draw_Line_Polar(int centerX, int centerY, int angle, int length, int color) {
    //draw a line from a center point to a point defined using a polar coordinate system
    double pi = 3.14;
    int x,y;
    x=length*cos(angle*pi/180);
    y=length*sin(angle*pi/180);
    int x2 = centerX+x;
    int y2 = centerY+y;
    GLCD_DrawLine(centerX,centerY,y2,x2, color);
}
int main()
{
    int16 i;       // counter variable -- int16 will take up less room than int32
    int16 x = CENTER;  // x and y coordinates for center of circle
    int16 y = CENTER;
	
	// Initialize LCD
    GLCD_Start();  
    Backlight_Write(1);       // turn on backlight
    
    LED_Blue_Write(1); LED_Green_Write(1); LED_Red_Write(1); // turn off all LEDs (1 means off)
 
    
    // clear LCD to black
    GLCD_Clear(GLCD_BLACK);
        
    // Draw a filled circle in yellow by drawing a series of circles of increasing radius 
    for (i = 0; i<50; i++) {
        GLCD_DrawCircle (x,y,i, GLCD_YELLOW);
    }
    //Loop for animation
    while(1) {
        //for loop to open the mouth
        for (int i =0;i<35;i++) {   
            GLCD_Draw_Line_Polar(x,y,i,50,GLCD_BLACK);                  
            GLCD_Draw_Line_Polar(x,y,-i,50,GLCD_BLACK);
        }
        //for loop to make a little moving dot
        for (int j = 121;j>y+20;j--) {
            for (int i = 0; i<5; i++) {
                GLCD_DrawCircle(x,j,i,GLCD_PINK);
            }
            for (int i = 0; i<5; i++) {
                GLCD_DrawCircle(x,j,i,GLCD_BLACK);
            }
        }
        //for loop to close the mouth
        for (int i =325;i<360;i++) {   
            GLCD_Draw_Line_Polar(x,y,i,50,GLCD_YELLOW);                  
            GLCD_Draw_Line_Polar(x,y,-i,50,GLCD_YELLOW);
        }
    }	
}

