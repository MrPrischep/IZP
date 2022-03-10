//  Dmitrii Kozhevnikov
//  xkozhe00
 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
 
#define strSize 1000 //  size of string
#define left 1
#define right 2
#define upDown 4
 
//  struct for map
typedef struct {
    int rows;   //  how many rows we have
    int cols;   // how many cols we have
    unsigned char *cells;   //  value in cells
} Map;
 
//  struct for step
struct prirustek {
    int dr; //  for step on row
    int ds; //  for step on col
};
 
//  steps
enum smery { vychod, sever, zapad, jih, POCET_SMERU };
 
    struct prirustek d[POCET_SMERU] = {
        {0, +1}, // vychod (0); step right
        {-1, 0}, // sever (1);  step up
        {0, -1}, // zapad (2);  step left
        {+1, 0}, // jih (3) step down
    };
 
//  is char in string number or not
bool isCharNum(char *argument) {
    int len = strlen(argument);
    bool result = true;
    //  go in length of argument
    for (int i = 0; i < len; i++) {
        if (!isdigit(argument[i])) {    //  if symbol of argument not integer
            result = false; //  symbol not integer
        }
    }
    return result;
}
 
//  how much words in string without spaces
int countWordsInStr(char *str) {
    int i;
    int words = 0;  //  parametr for words
    int len = strlen(str) - 1;
    //  if first char is space and second char isn't space
    for (i = 0; i < len; i++) {
        if (str[i] == ' ' && str[i+1] != ' ') {
            words++;
        }
 
        if (str[0] != ' ' && i == 0) {
            words++;
        }
    }
 
    return words;
}
 
//  is file format TXT or not
bool isFileTxt(char *fileName) {
    //  control four last chars
    if (
        (fileName[strlen(fileName) - 4] == '.') &&
        (fileName[strlen(fileName) - 3] == 't') &&
        (fileName[strlen(fileName) - 2] == 'x') &&
        (fileName[strlen(fileName) - 1] == 't')
    )
    {
        return true;    //  file TXT
    }
    else
    {
        fprintf(stderr, "Error: file is not '.txt'");   //  file not TXT
        return false;
    }
}
 
//  takes cells value from struct Map
char getCell(Map *map, int row, int col) {
    return map->cells[ row * map->cols + col ];
}
 
//  control, have we up-border and down-border or not
bool hasBorderTopOrBottom(char cell) {
    return cell == '4' || cell == '5' || cell == '6' || cell == '7';
}
 
//  control, have we left-border or not
bool hasBorderLeft(char cell) {
    return cell == '1' || cell == '3' || cell == '5' || cell == '7';
}
 
//  control, have we right-border or not
bool hasBorderRight(char cell) {
    return cell == '2' || cell == '3' || cell == '6' || cell == '7';
}
 
//  control, has horizontal numbers conflict or not
bool hasHorizontalConflicts(char leftCell, char rightCell) {
    switch (rightCell){
        case '1' :
        case '3' :
        case '5' :
        case '7' : return leftCell == '1' || leftCell == '4' || leftCell == '5' || leftCell == '0';
        case '0' :
        case '2' :
        case '4' :
        case '6' : return leftCell == '2' || leftCell == '3' || leftCell == '6' || leftCell == '7';
    }
    return 0;
}
 
//  control, has vertical numbers conflict or not
bool hasVerticalConflicts(char downCell, char upCell) {
    switch (downCell){
        case '1' :
        case '2' :
        case '3' :
        case '0' : return upCell == '4' || upCell == '5' || upCell == '6' || upCell == '7';
        case '4' :
        case '5' :
        case '6' :
        case '7' : return upCell == '1' || upCell == '2' || upCell == '3' || upCell == '0';
    }
    return 0;
}
 
//  has two elements conflict or not
bool hasConflictCells(Map *map, int i, int j){
    bool error = false;
    //  go from first col + 1
    if (j > 0){
        error = hasHorizontalConflicts(getCell(map, i, j-1), getCell(map, i, j));   //  two horizontal elements has conflict
   
    }
    //  go from first row + 1 and even cells
    if (i > 0 && (i + j) % 2 == 0 && !error){
        error = hasVerticalConflicts(getCell(map, i - 1, j), getCell(map, i, j));   //  two certical elements has conflict
    }
    return error;
}
 
 
 
//  testing and reading map from file
bool readAndTestMap(Map *map, char *fileName) {
    FILE *fp;
    fp = fopen(fileName, "r");  //  open TXT file with labirinth
 
    //  control, that we have this file
    if (fp == NULL) {
        fprintf(stderr, "Can't open file '%s'\n", fileName);    //  can't open file
        return false;   //  we can't open file
    } else {
        char str[strSize] = "";
        //  read first string from file
        fgets(str, strSize, fp);
        if (strlen(str) == 0 || strlen(str) ==1) {
            return false;
        }
        //  control, how many words we have in this string
        if (countWordsInStr(str) == 2) {
            char *firstword = strtok(str, " ");
            char *secondword = strtok(NULL, "\n");
 
            //  control, are that numbers or not
            if (isCharNum(firstword) && isCharNum(secondword)) {
                map->rows = atoi(firstword);    //  this is rows
                map->cols = atoi(secondword);   //  this is cols
               
                map->cells = malloc(map->rows * map->cols * sizeof(unsigned char) + 1); //  allocation
 
                //  reading strings from file
                for (int i = 0; i < map->rows; i++) {
                    fgets(str, strSize, fp);
                    //  if strings in file < rows
                    if (str == NULL) {
                        fprintf(stderr, "Invalid\n");   //  file is bad
                        return false;
                    }
 
                    //  control, how many words in string we have
                    if (countWordsInStr(str) != map->cols) {
                        fprintf(stderr, "Invalid\n");   //  file is bad
                        return false;
                    }
 
                    //  swap symbol new-string to symbol end-of-string
                    if (str[strlen(str)-1] == '\n') {
                        str[strlen(str)-1] = '\0';
                    }
 
                    firstword = strtok(str, " ");
 
                    for (int j = 0; j < map->cols || firstword != NULL; j++) {
                       
                        //  control have number one char or not
                        if (strlen(firstword) > 1) {
                            fprintf(stderr, "Invalid\n");   //  bad file
                            return false;
                        } else {
 
                            //  is number from 0 to 7
                            if (firstword[0] >= '0' && firstword[0] <= '7') {
                                char number = firstword[0];
                                map->cells[i * map->cols + j] = number;
                                firstword = strtok(NULL, " ");  //  go to new word
                            } else {
                                fprintf(stderr, "Invalid\n");   //  bad file
                                return false;
                            }
                           
                        }
                       
                    }
                }
                //  control, how many rows we have in file
                if (fgets(str, strSize, fp)) {
                    fprintf(stderr, "Invalid\n");   //  bad file
                    return false;
                }
               
                bool isClose = true;    //  control, are first row and last row opened
               
                //  control uneven pozitions
                for (int j = 0; j < map->cols; j = j + 2) {
                    //  have we border on top and bottom
                    if (
                        !hasBorderTopOrBottom(getCell(map, 0, j)) ||
                        !hasBorderTopOrBottom(getCell(map, map->rows - 1, j))
                    ) {
                        isClose = false;    //  top or bottom are open
                       
                    }
                }
 
                //  control, are first col and last col opened
                for (int j = 0; j < map->rows; j = j + 1) {
                    //  have we border in left and right
                    if (
                        !hasBorderLeft(getCell(map, j, 0)) ||
                        !hasBorderRight(getCell(map, j, map->cols - 1))
                    ) {
                        isClose = false;    //  left or right are open
                       
                    }
                }
 
                //  if labirinth is closed
                if (isClose) {
                    fprintf(stderr, "Invalid\n");   //  bad file
                    return false;
                }
               
                //  control numbers for conflict
                for (int i = 0; i < map->rows; i++) {
                    for (int j = 0; j < map->cols; j++) {
                        //  if numbers are in conflict
                        if (hasConflictCells(map, i, j)) {
                            fprintf(stderr, "Invalid\n");   //  bad file
                            return false;
                        }
                       
                    }
                }
               
            } else {
                fprintf(stderr, "Invalid\n");   //  bad file
                return false;
            }
 
       
        }
    }
   
    return true;
}
 
//  control, have we border or not
bool isborder(Map *map, int r, int c, int border) {
    bool levo, pravo, verchNiz;
    levo = (getCell(map, r, c) - '0') & left;
    pravo = (getCell(map, r, c) - '0') & right;
    verchNiz = (getCell(map, r, c) - '0') & upDown;
    switch(border) {
        case sever :
        case jih : return verchNiz; //  up or down
        case vychod : return pravo; //  right
        case zapad : return levo;   //  left
    }
    return 0;
}
 
//  control border in start pozition and how we go to labirinth
int start_border(Map *map, int r, int c, int leftright) {
    int smer;
    // right hand
    if (leftright == 0) {
        //  if we are in even cell
        if ((r + c) % 2 == 0) {
            //  go from left
            if (c == 0 && !isborder(map, r, c, zapad)) {
                smer = vychod;  //  to right
            }
            //  go from right
            else if (c == map->cols - 1 && !isborder(map, r, c, vychod)) {
                smer = sever;   //  to up
            }
            //  go from up
            else if (r == 0 && !isborder(map, r, c, sever)) {
                smer = zapad;   //  to left
            }
            //  we can't go from down
            else {
                fprintf(stderr, "Bad vstup\n");
                return -1;
            }
        }
        //  if we are in uneven cell
        else {
            //  go from left
            if (c == 0 && !isborder(map, r, c, zapad)) {
                smer = jih; //  to down
            }
            //  go from right
            else if (c == map->cols - 1 && !isborder(map, r, c, vychod)) {
                smer = zapad;   //  to left
            }
            //  go from down
            else if (r == map->rows -1 && !isborder(map, r, c, jih)) {
                smer = vychod;  //  to right
            }
            //  we can't go from up
            else {
                fprintf(stderr, "Bad vstup\n");
                return -1;
            }
        }
    }
    //  left hand
    else {
        //  if we are in even pozition
        if ((r + c) % 2 == 0) {
            //  go from left
            if (c == 0 && !isborder(map, r, c, zapad)) {
                smer = sever;   //  to up
            }
            //  go from right
            else if (c == map->cols - 1 && !isborder(map, r, c, vychod)) {
                smer = zapad;   //  to left
            }
            //  go from up
            else if (r == 0 && !isborder(map, r, c, sever)) {
                smer = vychod;  //  to right
            }
            //  we can't go from down
            else {
                fprintf(stderr, "Bad vstup\n");
                return -1;
            }
        }
        //  if we in uneven pozition
        else {
            //  go from left
            if (c == 0 && !isborder(map, r, c, zapad)) {
                smer = vychod;  //  to right
            }
            //  go from right
            else if (c == map->cols - 1 && !isborder(map, r, c, vychod)) {
                smer = jih; //  to down
            }
            //  go from down
            else if (r == map->rows - 1 && !isborder(map, r, c, jih)) {
                smer = zapad;   //  to left
            }
            //  we can't go from up
            else {
                fprintf(stderr, "Bad vstup\n");
                return -1;
            }
        }
 
    }
    return smer;    //  where we go
}
 
//  go to labirinth; right hand
void walkInLabirinthRight(Map *map, int row, int col, int smer) {
    //  go, while we are in labirinth
    while ((row < map->rows && col < map->cols) && (row >= 0 && col >= 0)) {
        //  if we have border
        while (isborder(map, row, col, smer)) {
            smer = (smer + 1) % POCET_SMERU;    //  turn to the right    
            //  if we see at corner in even pozition
            if ((row + col) % 2 == 0 && smer == jih) {
                smer = (smer + 1) % POCET_SMERU;    //  turn to the right
            }
            //  if we see at corner in uneven pozition
            else if ((row + col) % 2 != 0 && smer == sever) {
                smer = (smer + 1) % POCET_SMERU;    //  turn to the right
            }
        }
        printf("%d,%d\n", row + 1, col + 1);    //  where we stay
        row = row + d[smer].dr; //  step in row
        col = col + d[smer].ds; //  step in col
        //  if we don't see at corner after step
        if (((row + col) % 2 == 0 && smer == vychod) || ((row + col) % 2 != 0 && smer == zapad)){
        }
        //  if we see at corner after step
        else {
            smer = (smer + 3) % POCET_SMERU;    //  turn to the left
        }
   
    }
}
 
//  go to labirinth; left hand
void walkInLabirinthLeft(Map *map, int row, int col, int smer) {
    //  go, while we in labirinth
    while ((row < map->rows && col < map->cols) && (row >= 0 && col >= 0)) {
        //  if we have border
        while (isborder(map, row, col, smer)) {
            smer = (smer + 3) % POCET_SMERU;    //  turn to the left
            //  if we see at corner in even pozition
            if ((row + col) % 2 == 0 && smer == jih) {
                smer = (smer + 3) % POCET_SMERU;    //  turn to the left
            }
            //  if we see at corner in uneven pozition
            else if ((row + col) % 2 != 0 && smer == sever) {
                smer = (smer + 3) % POCET_SMERU;    //  turn to the left
            }
        }
        printf("%d,%d\n", row + 1, col + 1);    //  where we stay
        row = row + d[smer].dr; //  step in row
        col = col + d[smer].ds; //  step in col
        //  if we don't see at corner after step
        if (((row + col) % 2 == 0 && smer == zapad) || ((row + col) % 2 != 0 && smer == vychod)){
        }
        //  if we see at corner after step
        else {
            smer = (smer + 1) % POCET_SMERU;    //  turn to the right
        }
   
    }
}
 
int main(int argc, char *argv[]) {
 
    Map map;
    //  if we have two arguments
    if (argc == 2 && (strcmp(argv[1], "--help") == 0)) {
        printf("Napoveda!\nZadejte vstup, cislo radku a cislo sloupci!\nSpecialni komandy:\n");
        printf("-- help - vypisuje napovedu\n");
        printf("--test nazev_souboru.txt - zkontroluje, ze soubor dany druhym argumentem programu obsahuje radnou definici mapy bludiste\n");
        printf("--rpath R C nazev_souboru.txt - hleda pruchod bludistem, zadanym z souboru txt, na vstupu na rádku R a sloupci C. Pruchod hleda pomoci pravidla prave ruky (prava ruka vzdy na zdi)\n");
        printf("--lpath R C nazev_souboru.txt - hleda pruchod bludistem, zadanym z souboru txt, na vstupu na rádku R a sloupci C. Pruchod hleda pomoci pravidla leve ruky (leva ruka vzdy na zdi)\n");
        printf("--shortest R C nazev_souboru.txt - hleda nejkratsi cestu z bludiste, zadanym z souboru txt, pri vstupu na radku R a sloupci C\n");
        return 0;
    }
    //  if we have three arguments    
    else if (argc == 3 && strcmp(argv[1], "--test") == 0 && isFileTxt(argv[2])) {
        if (readAndTestMap(&map, argv[2])) {
            printf("Valid\n");
        } else {
            free(map.cells);
            return 0;
        }
    }
       
    //  if we have five arguments
    else if (argc == 5 && isFileTxt(argv[4]))
    {
        readAndTestMap(&map, argv[4]);
        int leftright;
        //  for right hand
        if (isCharNum(argv[2]) && isCharNum(argv[3]))
        {
            int startRows = atoi(argv[2]) - 1;  //  start row
            int startCols = atoi(argv[3]) - 1;  //  start col
           
            if ((strcmp(argv[1], "--rpath") == 0) && (startRows >= 0 && startRows < map.rows) && (startCols >= 0 && startCols < map.cols))
            {
                leftright = 0;  //  right hand
                int smer = start_border(&map, startRows, startCols, leftright);
                walkInLabirinthRight(&map, startRows, startCols, smer);
                return 0;
            }
            //  правило левой руки
            else if ((strcmp(argv[1], "--lpath") == 0) && (startRows >= 0 && startRows < map.rows) && (startCols >= 0 && startCols < map.cols))
            {
                leftright = 1;  //  левая рука
                int smer = start_border(&map, startRows, startCols, leftright);
                walkInLabirinthLeft(&map, startRows, startCols, smer);
                return 0;
            }
            //  we print bad arguments
            else
            {
            fprintf(stderr, "Error: chybna komanda\nPro napovedu napiste --help\n");
            return -1;            
            }
        }
        else
        {
            fprintf(stderr, "Error: chybna komanda\nPro napovedu napiste --help\n");
            return -1;            
        }  
    }
    else
    {
        fprintf(stderr, "Error: chybna komanda\nPro napovedu napiste --help\n");
        return -1;
    }
 
    free(map.cells);
    return 0;
}