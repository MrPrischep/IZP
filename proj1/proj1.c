#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define strSize 100 //  string size

//  function for control, have we got symbol of argument in string or not 
bool isCharInString(char c, char* str) 
{
    int len = strlen(str);
    for (int i = 0; i < len; i++) 
    {
        if (str[i] == c) 
        {
            return true;
        }
    }
    return false;
}

//  transformation of symbol of argument from numbers to letters
char* getValidChars(char telButton) 
{
    switch (telButton) 
    {
        case '1': return "1";
        case '2': return "2abc";
        case '3': return "3def";
        case '4': return "4ghi";
        case '5': return "5jkl";
        case '6': return "6mno";
        case '7': return "7pqrs";
        case '8': return "8tuv";
        case '9': return "9wxyz";
        case '0': return "0";
        case '+': return "+";
        default: return "";
    }
}

//  function for control, have we got symbols of argument in string or not
bool startsSimilar(char* str, char* filterStr) 
{
    int len = strlen(filterStr);
    for (int j = 0; j < len; j++) 
    {
        char* validChars = getValidChars(filterStr[j]); //  take value for symbol of argument
        char curChar = tolower(str[j]);
        //  control, have we got argument in string or not
        if (!isCharInString(curChar, validChars)) 
        {
            return false;
        }
    }
return true;
}

// control, have we got symbols of argument in string and taken positions, if we have
int findInString(char* str, char* filterStr) 
{
    int len = strlen(str) - (filterStr ? strlen(filterStr):0);  //  length of string - length of argument
    if (filterStr) 
    {
        for (int i = 0; i <= len; i++) 
        {
            if (startsSimilar(str + i, filterStr))  //  control argument in string; cut start of string by i
            { 
                return i;   // if we have argument in string, get back position
            }
        }
    }
    return -1; 
}

//  function for reading strings from file
char *readline(char *str) 
{
    char *result = fgets(str, strSize, stdin);  //  read string
    //  swap '\n' to '\0'
    if (str[strlen(str)-1] == '\n')
    {
        str[strlen(str)-1] = '\0';
    }
    return result;
}

int main(int argc, char *argv[])
{
    char *filterStr = argv[1];  //  variable for argument
	char nameStr[strSize];  //  variable for name
	char telStr[strSize];   //  variable for number
    bool searchSuccesseful = false; //  variable for searching
    //  control, how many arguments we have
    if(argc > 2) 
    {
        printf("%s\n", "Add only one argument!"); 
        return 1;
    }
    else if(argc <= 2)
    {
        //  working until the files end
	    while (readline(nameStr) != NULL && readline(telStr) != NULL)
	    {
            int nameSearch = findInString(nameStr, filterStr);  //  variable for searching in name (get back position or -1)
            int telSearch = findInString(telStr, filterStr);    //  variable for searching in phone (get back position or -1)
            char printNewName[strSize]; //  variable for new string
            if (nameSearch != -1 || telSearch != -1 || filterStr == NULL)
		    {
                int len = strlen(nameStr) + 1;
                for (int i = 0; i < len; i++)
                {
                    int lenlen = filterStr ? strlen(filterStr):0;
                    //  if we find argument in name, then transform letters on positions from small to high
                    if (i >= nameSearch && i < nameSearch + lenlen && nameSearch != -1)
                    {
                        printNewName[i] = toupper(nameStr[i]);
                    }
                    //  if we find argument in phone or, if we haven't got argument, then transform letters from high to small
                    else                    
                    {
                        printNewName[i] = tolower(nameStr[i]);
                    }
                    
                }
                printf("%s, %s\n", printNewName, telStr);   //  writing names and phones 
			    searchSuccesseful = true;   //  finding was successful
		    }
        } 
        //  if finding wasn't successful, print information about that  
	    if (!searchSuccesseful)
	    {
		    printf("Not found\n");
	    }
	return 0;
    }
}