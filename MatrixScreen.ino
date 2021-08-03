
#include <u8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, D8, D1, D3);


char* getRandomCharacter();

//This is the letterobject that holds a char array for the letter and it's coordinates 
class LetterObj
{
public:
    int x;
    int y;
    char* letter;
    LetterObj() {
        int x = 1; 
        int y = 2; 
        
        letter = getRandomCharacter(); 
     //   Serial.println("Initial Letter: " + String(letter)); 
    };
   
private:

};
// An object that holds a character and it's position on the screen 
class CharacterNode {
public: 
    LetterObj *Node; 
    CharacterNode *next; 
    CharacterNode() {
        Node = new LetterObj(); 
        next = nullptr; 
    }

};

//creates a global variable to hold the character list 
CharacterNode* firstNode = nullptr;

// Generates a random character from a Dictionary 
char* getRandomCharacter() {
    char* charList[36] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
    char c;
    int r = random(0, 35);
    return charList[r];

}

//write a message to the screen
void writeMessage(String msg) {
    u8g2.firstPage();
    do {
        u8g2.setFont(u8g2_font_freedoomr10_mu);
        u8g2.drawStr(0, 30, msg.c_str());
    } while (u8g2.nextPage());
    delay(1000);
}
// initializes the list of characters
void InitializeMatrix() {
    firstNode = nullptr;
    //number of coloums of characters on screen.
    int numOfCol = 9; 
    //number of rows of characters generated 
    int numOfRows = 5; 
    CharacterNode* currentNode = nullptr;
    //number of column currently generating, should start at 1. 
    int colNum = 0;
    // number of row currently generating, should start at 0 
    int rowNum = 0;
    //generate initial Matrix 
    for (int x = 0; x < (numOfCol*numOfRows); x++) {
        //if we have reached the max number of columns, reset colnum and iterate rowNum
        if (x % numOfCol == 0) {
            colNum = 0;
            rowNum = rowNum + 1;
        }
        //initializes firstNode if null 
        if (firstNode == nullptr) {
            firstNode = new CharacterNode();
            currentNode = firstNode;
        }
        //moves currentPointer to next if FirstNode is not null and generates new node, then sets that node as currentNode
        else {
            currentNode->next = new CharacterNode();
            currentNode = currentNode->next;
        }
        
        //set x for the bottom left hand corner of the text 
        currentNode->Node->x = colNum * 15;
        //stagger every other column
        if (colNum % 2 == 1) {
            currentNode->Node->y = 10 + (rowNum * 15);
        }
        else {
            currentNode->Node->y = 1 + (rowNum * 15);
        }
        //iterate colNum
        colNum = colNum + 1;
       
        //Serial.println(x);
        //Serial.println(currentNode->Node->letter);


    }
}

void setup() {
    //Serial.begin(115200); 
    randomSeed(1522254);
   // start the display
    u8g2.begin();
    writeMessage("INITIALIZING...");
    InitializeMatrix(); 
    
    delay(3000); 
}

void MatrixScreenIterate() {
    
    //nt colNum = 1;
    //int rowNum = 0;
    //create Current  Node Pointer 
    CharacterNode* currentNode = nullptr;
    //set to first node 
    currentNode = firstNode;
    // start iteration counter 
    int iter = 0;
    //u8g2.setFont(u8g2_font_missingplanet_t_all);
    u8g2.setFont(u8g2_font_freedoomr10_mu);
    //u8g2.setFont(u8g2_font_glasstown_nbp_t_all);
    //if the iter is less than 80 (the number of vertical pixals plus a 15 pixel buffer to make surefirst line is completely off the screen) 
    while (iter < 80)
    {
        //iterate
        iter++;
        do {
            //clear the old char from the buffer
        
                u8g2.clearBuffer();
            
            //count number of characters
            int spaceCount = 0;
            //while not at end of list
            while (currentNode != nullptr) {
                //check to see if the character is off the screen
                if (currentNode->Node->y > 70) {
                    //if off the screen, reset to top of screen and generate new character
                    currentNode->Node->y = 1;
                    currentNode->Node->letter = getRandomCharacter();
                }
                //iterate spacecount
                spaceCount++;
                //every 7 characters create one that moves faster
                if (spaceCount % 7 == 0) {
                    //move character 4 pixels down 
                    currentNode->Node->y = currentNode->Node->y + 4;
                }
                //move character by 2 pixels down
                currentNode->Node->y = currentNode->Node->y + 2;
                //Serial.println(currentNode->Node->letter);
                //Serial.println(currentNode->Node->x);
                //Serial.println(currentNode->Node->y);
                int x = currentNode->Node->x;
                int y = currentNode->Node->y;
                char* lett = currentNode->Node->letter;
                //draw character to buffer
                u8g2.drawUTF8(currentNode->Node->x, currentNode->Node->y, currentNode->Node->letter);
                //move current node to next node
                currentNode = currentNode->next;
                
            }
            //after all characters have been printed, move current node to first node
            currentNode = firstNode;
            //print buffer to screen
        } while (u8g2.nextPage());
        delay(30);
    }
}

void loop() {
    //create next frame of animation
    MatrixScreenIterate(); 
	
    delay(30); 
    
    
}
