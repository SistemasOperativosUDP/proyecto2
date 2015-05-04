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
#define TAM    20
unsigned char colorDraw = 0x0F;
unsigned char colorErase = 0x00;
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

void printString(const char* pStr) {
     while(*pStr) {
          __asm__ __volatile__ (
               "int $0x10" : : "a"(0x0e00 | *pStr), "b"(0x0007)
          );
          ++pStr;
     }
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
 * function to print the rectangle border                                                 
 * I follow the below sequence                                          
 * (left, top)     to (left, bottom)                                    
 * (left, bottom)  to (right, bottom)                                   
 * (right, bottom) to (right, top)                                      
 * (right, top)    to (left, top)                                       
 */
void initGraphics() {
     int i = 0, j = 0, m = 0;
     /* (left, top) to (left, bottom)                              */
     j = 0;
     for(i = m; i < MAX_ROWS - m; ++i) {
          drawPixel(colorDraw, j+m, i);
     }
     /* (left, bottom) to (right, bottom)                          */
     for(j = m; j < MAX_COLS - m; ++j) {
          drawPixel(colorDraw, j, i);
     }

     /* (right, bottom) to (right, top)                            */
     for(i = MAX_ROWS - m - 1 ; i >= m; --i) {
          drawPixel(colorDraw, MAX_COLS - m - 1, i);
     }
     /* (right, top)   to (left, top)                              */
     for(j = MAX_COLS - m - 1; j >= m; --j) {
          drawPixel(colorDraw, j, m);
     }   
}

/**
* Funcion que permite que la serpiente se 
* mueva de forma mas lentas gastanto tiempo
* en el procesardor
*/
void mySleep(int n){
     volatile int i;
     for(i = 0; i<n*10000; ++i){
     }
}

/**
* Funcion de hash que me lanzara de forma aleatorea la 
* ubicacion en la fila aleatoria para poder ubicar la comida
*/
/*int hashi(int x,int cont){
     int aleatorio;
     aleatorio= ((x*cont+13)*23209)%319;
     if(aleatorio <= 1 || aleatorio == 319){
         aleatorio=hashi(x+1,cont+7);
         return aleatorio;
    }
    else{
     return aleatorio;
    }
}*/
/**
* Funcion de hash que me lanzara de forma aleatorea 
* la ubicacion aleatoria en la columna asociada
*/
/*int hashj(int x,int cont){
     int aleatorio;
     aleatorio= ((x*cont+71)*23209)%199;
     if(aleatorio <= 1 || aleatorio == 199 || aleatorio >=200){
         aleatorio=hashj(x+3,cont+13);
         return aleatorio;
    }
    else{
     return aleatorio;
    }
}*/
/*void left(unsigned char colorErase, unsigned char colorDraw, int j){
     int i = 0, cont = 1;
     for( i = j+TAM; i == i;i--) {
           if(i == 0) i = 320;
           drawPixel(colorDraw, i-1, (MAX_ROWS/2));
           cont ++;
           mySleep(5);
           //drawPixel(colorDraw,hashi(i,cont),(MAX_ROWS/2));
           drawPixel(colorErase, i+(TAM*2)+1, (MAX_ROWS/2));
      }
}*/
/*void right(unsigned char colorErase, unsigned char colorDraw, int j){
     int i = 0, cont = 1;
     for( i = j-TAM; i == i;i++) {
           if(i == 319) i = 1;
           drawPixel(colorDraw, i-1, (MAX_ROWS/2));
           cont ++;
           mySleep(5);
           //drawPixel(colorDraw,hashi(i,cont),(MAX_ROWS/2));
           drawPixel(colorErase, i-(TAM*2)+1, (MAX_ROWS/2));
     }
}*/
void up(int x, int y){
     int i = 0;
     for( i = x+TAM; ;i--) {
       if(i == 0) {
          for(i = i+TAM;i>=0; i--){
            drawPixel(colorErase, y, i+1);
          }
        i = 200;
      }
     drawPixel(colorDraw, y,i);
     mySleep(10);
     if(i <= 200-TAM) drawPixel(colorErase, y, i+TAM);
     }
}
/**
 * function is boot code and it calls the below functions              
 * print a message to the screen to make the user hit the              
 * key to proceed further and then once the user hits then             
 * it displays rectangles in the descending order                      
 */
void main() {
     printString("Now in snake mode...hit a key to start\n\r");
     getch();
     initEnvironment();
     initGraphics();
     up(100, 200);
}