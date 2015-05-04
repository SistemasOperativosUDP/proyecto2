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

#define MAX_COLS     320 // maximum columns of the screen
#define MAX_ROWS     200 // maximum rows of the screen

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

short getchar() {
     short word;

     __asm__ __volatile__(
          "int $0x16" : : "a"(0x1000)
     );

     __asm__ __volatile__(
          "movw %%ax, %0" : "=r"(word)
     );

     return word;
}

void putchar(short ch) {
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


void main() {
     printString("Now in keyboard mode, hit a key to start, then hit W A S or D");
         while(1){
          short key = getchar();
          if((char)key == 'w'){
               putchar('^'); 
               putchar('\n'); 
               putchar('\b'); 
          }          
          if((char)key == 'a'){
               putchar('<'); 
               putchar('\n'); 
               putchar('\b'); 
          }          
          if((char)key == 's'){
               putchar('v'); 
               putchar('\n'); 
               putchar('\b'); 
          }          
          if((char)key == 'd'){
               putchar('>'); 
               putchar('\n'); 
               putchar('\b'); 
          }          
          else {
               continue;
          }
     }
     //initEnvironment();
     //initGraphics();
}
