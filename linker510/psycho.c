/**
 * @source http://www.codeproject.com/Articles/664165/Writing-a-boot-loader-in-Assembly-and-C-Part
 * @file psycho.c
 *
 * @author AshakiranBhatter
 * @modified_by UDP
 */

// generate 16 bit code
__asm__(".code16gcc\n");
// jump to main function or program code
__asm__("jmpl $0x0000, $main\n");

#define MAX_COLS    320 // maximum columns of the screen
#define MAX_ROWS    200 // maximum rows of the screen
#define TAM    10
//#include <unistd.h>
/**
 * function to print string onto the screen                             
 * input ah = 0x0e                                                      
 * input al = <character to print>                                      
 * interrupt: 0x10                                                      
 * we use interrupt 0x10 with function code 0x0e to print               
 * a byte in al onto the screen                                         
 * this function takes string as an argument and then                   
 * prints character by character until it founds null                   
 * character                                                            
 */

/*void printString(const char* pStr) {
     while(*pStr) {
          __asm__ __volatile__ (
               "int $0x10" : : "a"(0x0e00 | *pStr), "b"(0x0007)
          );
          ++pStr;
     }
}*/


short getcharAS() {
     short word;

     __asm__ __volatile__(
          "int $0x16" : : "a"(0x1000)
     );

     __asm__ __volatile__(
          "movw %%ax, %0" : "=r"(word)
     );

     return word;
}

void putcharAS(short ch) {
     __asm__ __volatile__(
          "int $0x10" : : "a"(0x0e00 | (char)ch)
     );
}

/**
 * function to get a keystroke from the keyboard                        
 * input ah = 0x00                                                      
 * input al = 0x00                                                      
 * interrupt: 0x16                                                      
 * we use this function to hit a key to continue by the                 
 * user                                                                 
 */
void getch() {
     __asm__ __volatile__ (
          "xorw %ax, %ax\n"
          "int $0x16\n"
     );
}


/** 
 * function to print a colored pixel onto the screen                    
 * at a given column and at a given row                                 
 * input ah = 0x0c                                                      
 * input al = desired color                                             
 * input cx = desired column                                            
 * input dx = desired row                                               
 * interrupt: 0x10                                                      
 */
void drawPixel(unsigned char color, int col, int row) {
     __asm__ __volatile__ (
          "int $0x10" : : "a"(0x0c00 | color), "c"(col), "d"(row)
     );
}

/**
 * function to clear the screen and set the video mode to               
 * 320x200 pixel format                                                 
 * function to clear the screen as below                                
 * input ah = 0x00                                                      
 * input al = 0x03                                                      
 * interrupt = 0x10                                                     
 * function to set the video mode as below                              
 * input ah = 0x00                                                      
 * input al = 0x13                                                      
 * interrupt = 0x10                                                     
 */
void initEnvironment() {
     /* clear screen                                                    */
     __asm__ __volatile__ (
          "int $0x10" : : "a"(0x03)
     );
     __asm__ __volatile__ (
          "int $0x10" : : "a"(0x0013)
     );
}

/**
 * function to print rectangles in descending order of                  
 * their sizes                                                          
 * I follow the below sequence                                          
 * (left, top)     to (left, bottom)                                    
 * (left, bottom)  to (right, bottom)                                   
 * (right, bottom) to (right, top)                                      
 * (right, top)    to (left, top)                                       
 */
void initGraphics() {
     int i = 0, j = 0;
     int m = 0;
     //int cnt1 = 0, cnt2 =0;
     unsigned char color = 5;
     /* (left, top) to (left, bottom)                              */
     j = 0;
     for(i = m; i < MAX_ROWS - m; ++i) {
          drawPixel(color, j+m, i);
     }
     /* (left, bottom) to (right, bottom)                          */
     for(j = m; j < MAX_COLS - m; ++j) {
          drawPixel(color, j, i);
     }

     /* (right, bottom) to (right, top)                            */
     for(i = MAX_ROWS - m - 1 ; i >= m; --i) {
          drawPixel(color, MAX_COLS - m - 1, i);
     }
     /* (right, top)   to (left, top)                              */
     for(j = MAX_COLS - m - 1; j >= m; --j) {
          drawPixel(color, j, m);
     }
     m += 6;
     
     //if(++color > 255)  color = 0;

     color = 10;
     for(i = (MAX_COLS/2)-TAM; i<(MAX_COLS/2)+TAM;i++) {
          //drawPixel(color, i, (MAX_ROWS/2)-1);
          drawPixel(color, i, (MAX_ROWS/2));
          //drawPixel(color, i, (MAX_ROWS/2)+1);
     }
     
}

/*void sleep(){
     __asm__ __volatile__(
          "MOV     CX, 0FH\n"
          "MOV     DX, 4240H\n"
          "MOV     AH, 86H\n"
          "INT     15H\n"
     );
}*/

void mySleep(int n){
     volatile int i;
     for(i = 0; i<n*10000; ++i){
     }
}

int hash(int x,int cont){
     return ((x*cont+13)*23209)%320;
}

void move(){
     int i = 0, cont = 1;


     unsigned char colorErase = 255;
     unsigned char colorDraw = 10;
     //unsigned long pls;
     for( i = (MAX_COLS/2)-TAM; i == i;i++) {
          if(i == 300 ) i = 1;
          drawPixel(colorDraw, i-1, (MAX_ROWS/2));
          drawPixel(colorErase, i-(TAM*2)+1, (MAX_ROWS/2));
          cont ++;
          mySleep(10);
          drawPixel(colorDraw,hash(i,cont),(MAX_ROWS/2));
          //drawPixel(colorDraw, i-1, (MAX_ROWS/2)-1);
          //drawPixel(colorDraw, i-1, (MAX_ROWS/2)+1);
          //drawPixel(colorErase, i+(TAM*2)+1, (MAX_ROWS/2)-1);
          //drawPixel(colorErase, i-(TAM*2)+1, (MAX_ROWS/2)+1);
          //usleep(1000);
          //mySleep(1000000);

     }

}

/**
 * function is boot code and it calls the below functions              
 * print a message to the screen to make the user hit the              
 * key to proceed further and then once the user hits then             
 * it displays rectangles in the descending order                      
 */
void main() {
     //printString("Now in snake mode...hit a key to start\n\r");
     getch();
     initEnvironment();
     initGraphics();
     //getch();
     move();
}
