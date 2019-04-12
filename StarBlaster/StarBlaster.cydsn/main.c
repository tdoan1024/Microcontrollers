/* ========================================
 *  CPE 3280
 *  Assignment 2: StarBlaster
 *  Author: Tai Doan
 * ========================================
*/

#include <project.h>
#include <stdlib.h>

//Make a struct to store coordinates of a star
struct Star {
    int16 x_Coor;
    int16 y_Coor;
};

//Function to draw the crosshair, takes 4 integers as its parameters:
//x and y are the coordinates of the crosshair's center, radius of the circle and the color of it.
void crosshair(int x, int y, int radius, int color) {
    GLCD_DrawCircle(x,y,radius,color);
    GLCD_DrawLine(x-10,y,x+10,y,color);
    GLCD_DrawLine(x,y-10,x,y+10,color);
}

//Blaster in the charged state with Green LED
//Takes 2 integers as the center of the explosion
void fire1(int x,int y) {
    for (int i=0;i<25;i++) {
        GLCD_DrawCircle(x,y,i,GLCD_PINK);
    }
    for (int i=0;i<25;i++) {
        GLCD_DrawCircle(x,y,i,GLCD_SKYBLUE);
        GLCD_DrawCircle(x,y,i,GLCD_BLACK);
    }
}

//Big blaster in the hypecharged state with Blue LED
//Takes 2 integers as the center of the explosion
void fire2(int x, int y) {
    for (int i=0;i<45;i++) {
        GLCD_DrawCircle(x,y,i,GLCD_WHITE);
    }
    for (int i=0;i<45;i++) {
        GLCD_DrawCircle(x,y,i,GLCD_BLACK);
    }
}
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    int16 i;
    int16 x_Center = 62; //x-coordinate of center point
    int16 y_Center = 62; //y-coordinate of center point
    int16 xCur=x_Center,yCur=y_Center; //current coordinates of the crosshair
    int16 count = 100; //Counter for LED states

    
    struct Star stars[150]; //Array of 150 stars
    
    //Randomly put stars into the2 array
    for (i=0;i<150;i++) {
        stars[i].x_Coor = rand() % 132;
        stars[i].y_Coor = rand() % 132;
    }
    
    //Initialize the LCD
    GLCD_Start();  
    Backlight_Write(1);       // turn on backlight
    GLCD_Clear(GLCD_BLACK);   //turn background to black
    LED_Red_Write(1); LED_Green_Write(1); LED_Blue_Write(0);  // Blue LED is first ON, Green and Red LED are OFF
    
    for(;;) {
        //Draw stars
        for (i=0;i<150;i++) {
            GLCD_Pixel(stars[i].x_Coor, stars[i].y_Coor,GLCD_WHITE);
        }
        
        //Draw the crosshair
        crosshair(xCur,yCur,8,GLCD_YELLOW);
        
        //Make the starts twinking
        int16 randomS = rand() % 150;
        GLCD_Pixel(stars[randomS].x_Coor, stars[randomS].y_Coor,GLCD_BLACK);
        CyDelay(50);
        GLCD_Pixel(stars[randomS].x_Coor, stars[randomS].y_Coor,GLCD_WHITE);
        
        
       //Movement of the crosshair with 4 control buttons
        //D to move left
        if (D_Read()==0 && yCur>=15){
            crosshair(xCur,yCur,8,GLCD_BLACK);
            crosshair(xCur,yCur-4,8,GLCD_YELLOW);
            yCur = yCur - 4;
        }
        //A to move up
        if (A_Read()==0 && xCur>=15){
            crosshair(xCur-4,yCur,8,GLCD_YELLOW);
            crosshair(xCur,yCur,8,GLCD_BLACK);
            xCur=xCur-4;
        }
        //B to move right
        if (B_Read()==0 && yCur <=116){
            crosshair(xCur,yCur+4,8,GLCD_YELLOW);
            crosshair(xCur,yCur,8,GLCD_BLACK);
            yCur=yCur+4;
        }
        //C to move down
        if (C_Read()==0 && xCur <=116){
            crosshair(xCur+4,yCur,8,GLCD_YELLOW);
            crosshair(xCur,yCur,8,GLCD_BLACK);
            xCur=xCur+4;
        }
        //Empty state       
        if (count == 0) {
            LED_Red_Write(0);
        }
        //No explosion when joystick is pushed in empyty state
        if ((LED_Red_Read() == 0)&& (Joy_Center_Read()==0))
            continue;
        //Charged state
        if (count == 50) {       
            LED_Red_Write(1);
            LED_Green_Write(0);
        }
        //Small explosion when joystick is pushed in charged state
        if ((count >=50) && (count <100) && (Joy_Center_Read()==0)){
            fire1(xCur,yCur);
            LED_Green_Write(1);
            LED_Red_Write(0);
            count = 0;
        }
        //Hypercharged state
        if (count == 100) {      
            LED_Green_Write(1);
            LED_Blue_Write(0);
        }
        //Large explosion when joystick is pushed in hypercharged state
        if ((count >=100) && (Joy_Center_Read()==0)) {
            fire2(xCur,yCur);
            LED_Blue_Write(1);
            LED_Red_Write(0);
            count = 0;
        }
        //Increment of counter
        count++;
    }
}

/* [] END OF FILE */
