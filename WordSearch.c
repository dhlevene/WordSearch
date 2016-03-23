//Daniel Levene da935603

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "WordSearch.h"

int main (void)
{
    Dictionary *dict;
    WordSearchPuzzle *puzzle;
    int i;

    dict = malloc(sizeof(Dictionary));
    puzzle = malloc(sizeof(WordSearchPuzzle));

    readDictionary(dict); //calls method readDictionary
    readPuzzle(puzzle); //calls method readDictionary
    possibleStrings(dict, puzzle); //calls method possibleStrings
        for(i=0; i<dict->size; i++)
        {
            if(dict->counts[i] >= 1)
                //prints out matches between puzzle and dictionary
                printf("%s (%d)\n", dict->words[i], dict->counts[i]);
        }
    destroyDictionary(dict); //calls method destroyDictionary
    destroyPuzzle(puzzle); //calls method destroyPuzzle
    free(dict);
    free(puzzle);

return 0;
}

//fuction to read in dictionary from file
int readDictionary(Dictionary *dict)
{
    int i, wordlength, readcheck;
    char readin [MAX_WORD_LENGTH + 1];
    FILE *ifp = fopen("dictionary.txt", "r");

    fscanf(ifp, "%d", &dict->size);

    dict->words = NULL;
    dict->words = malloc(sizeof(char*) * dict->size+1);
    if(dict->words == NULL)
        return 0;
    dict->counts = NULL;
    dict->counts = malloc(sizeof(int) * dict->size+1);
    if(dict->counts == NULL)
        return 0;

    for(i=0; i < dict->size; i++)
    {
        dict->counts[i] = 0; //set all spots in counts array to 0
        fscanf(ifp, "%s", readin); //puts dictionary in static array readin
        wordlength = strlen(readin);
        //allocates space for each array of characters in words array
        dict->words[i] = malloc(sizeof(char) * (wordlength+1));
        strcpy(dict->words[i], readin);
    }
    fclose(ifp);
    return 1;
}

//function to destroy dictionary to make sure there are no memory leaks
void destroyDictionary(Dictionary *dict){
    free(dict->words);
    free(dict->counts);
    dict->size = 0;
    dict->counts = NULL;
    dict->words = NULL;
}

//function to read in wordsearch puzzle
int readPuzzle(WordSearchPuzzle *puzzle)
{
    int i;
    scanf("%d", &puzzle->height);
    scanf("%d", &puzzle->width);

    puzzle->grid = NULL;
    puzzle->grid = malloc(sizeof(char*) * puzzle->height);
    if(puzzle->grid == NULL)
        return 0;

    for(i=0; i < puzzle->height; i++)
    {
        puzzle->grid[i] = malloc(sizeof(char) * (puzzle->width+1));
        scanf("%s", puzzle->grid[i]);
    }
    return 1;
}

//function to destroy puzzle to make sure there are no memory leaks
void destroyPuzzle(WordSearchPuzzle *puzzle){
    puzzle->width=0;
    puzzle->height=0;
    free(puzzle->grid);
    puzzle->grid = NULL;
}

//function to generate all possible strings from the puzzle
int possibleStrings(Dictionary *dict,WordSearchPuzzle *puzzle)
{
    int i,j,k,l,m;
    char* tempString;
    char* invertedString;
    char* columnString;
    char* diagString;

    //Rows
    tempString = NULL;
    tempString = malloc(sizeof(char) * (puzzle->width+1));
    if(tempString == NULL)
        return 0;
        for(j=0; j<puzzle->height; j++)
            //gets all the rows and passes them to makeStrings function
            makeStrings(dict,tempString, puzzle->grid[j], puzzle->width);
        free(tempString);

    //Columns
    tempString = NULL;
    tempString = malloc(sizeof(char) * (puzzle->height+1));
    if(tempString == NULL)
        return 0;
    columnString = NULL;
    columnString = malloc(sizeof(char) * (puzzle->height +1));
    if(columnString == NULL)
        return 0;
        for(j=0; j<puzzle->width; j++)
        {
            for(k=0, l=0; l<=puzzle->height-1; l++, k++)
                //transfers all columns into temporary array to work with
                columnString[k] = puzzle->grid[l][j];
                //passes along all columns to makeStrings function
            makeStrings(dict,tempString, columnString, puzzle->height);
        }
        free(tempString);
        free(columnString);

    //Diagonal left to right
    tempString = NULL;
    tempString = malloc(sizeof(char) * puzzle->width+1);
    if(tempString == NULL)
        return 0;
    diagString = NULL;
    diagString = malloc(sizeof(char) * puzzle->width+1);
    if(diagString == NULL)
        return 0;
        for(j=0; j<puzzle->width-1; j++)
        {
            m=0; //variable to move diagString along
            for(k=0, l=j; l<puzzle->width && k<puzzle->height; k++, l++)
            {
                //transfers all diags into temporary array to work with
                diagString[m] = puzzle->grid[k][l];
                diagString[m+1] = '\0'; //manual null terminator for each string
                m++;
            }
            //passes all possible diagonals (in one direction) to makeStrings
            makeStrings(dict, tempString, diagString, m);
        }
        free(tempString);
        free(diagString);

    //Diagonal right to left (other side)
    tempString = NULL;
    tempString = malloc(sizeof(char) * puzzle->width+1);
    if(tempString == NULL)
        return 0;
    diagString = NULL;
    diagString = malloc(sizeof(char) * puzzle->width+1);
    if(diagString == NULL)
        return 0;
        for(j=1; j<puzzle->height-1; j++)
        {
            m=0;
            for(k=j, l=0; k<puzzle->height; k++, l++)
            {
                diagString[m] = puzzle->grid[k][l];
                diagString[m+1] = '\0';
                m++;
            }
        makeStrings(dict,tempString, diagString,m);
        }
        free(tempString);
        free(diagString);

    //Diagonal left to right
    tempString = NULL;
    tempString = malloc(sizeof(char) * (puzzle->width+1));
    if(tempString == NULL)
        return 0;
    diagString = NULL;
    diagString = malloc(sizeof(char) * (puzzle->width+1));
    if(diagString == NULL)
        return 0;
        for(j=1; j<puzzle->width; j++)
        {
            m=0;
            for(k=0, l=j; l>=0 && k<puzzle->height; k++, l--)
            {
                diagString[m] = puzzle->grid[k][l];
                diagString[m+1] = '\0';
                m++;
            }
            makeStrings(dict,tempString, diagString,m);
        }
        free(tempString);
        free(diagString);

    //Diagonal left to right (other)
    tempString = NULL;
    tempString = malloc(sizeof(char) * puzzle->width+1);
    if(tempString == NULL)
        return 0;
    diagString = NULL;
    diagString = malloc(sizeof(char) * puzzle->width+1);
    if(diagString == NULL)
        return 0;
        for(j=1; j<puzzle->height-1; j++)
        {
            m=0;
            for(k=j, l=puzzle->width-1; k<puzzle->height && l>=0; k++, l--)
            {
                if(k == puzzle->height && l == puzzle->width-1)
                    break;
                diagString[m] = puzzle->grid[k][l];
                diagString[m+1] = '\0';
                m++;
            }
            makeStrings(dict,tempString, diagString,m);
        }
        free(tempString);
        free(diagString);

    //seperate check for single characters to eliminate chance for
    //unintentional duplicates when doing other string generations
    char* singleChar;
    tempString = NULL;
    tempString = malloc(sizeof(char) * puzzle->width+1);
    if(tempString == NULL)
        return 0;
    singleChar = NULL;
    singleChar = malloc(sizeof(char) * 1);
    if(singleChar == NULL)
        return 0;
        for(i=0; i<puzzle->height; i++)
        {
            for(j=0; j<puzzle->width; j++)
            {
                //goes through every character in grid and stores them in singleChar
                singleChar[0] = puzzle->grid[i][j];
                //passes each character to makeStrings for checking
                makeStrings(dict,tempString, singleChar, 1);
            }
        }
        free(tempString);
        free(singleChar);
}

//function that takes all substrings and checks them in dictionary
int checkString(Dictionary *dict, char *str){
    //binary search credit to Sean Szumlanski
    int lo = 0, hi = dict->size - 1, mid, i;

    while (hi >= lo)
    {
        mid = lo + (hi - lo) / 2;
        // if stringcompare result is less than 0, only check the left half of the dictionary
        if (strcmp(str,dict->words[mid])<0)
            hi = mid - 1;
        // if stringcompare result is more than 0,only check the right half of the dictionary
        else if (strcmp(str,dict->words[mid])>0)
            lo = mid + 1;
        // otherwise, we found the string in the dictionary yeeey!!
        else if(strcmp(str,dict->words[mid])==0)
        {
            dict->counts[mid]++;
            return 1;
        }
    }
    return 0;
}

//function to tak all strings from possibleStrings and to make every possible
//substring and inverse substring
int makeStrings(Dictionary *dict,char *str1, char*str2, int length)
{
    int i, j, k;
    char* invertedArray;

    //for single characters only to check in dictionary
    if(length == 1)
    {
        for(i=1; i<length+1; i++)
        {
            for(j=0; j<length-(i-1); j++)
            {
                strncpy(str1, str2 + j, i);
                str1[i] = '\0';
                //calls function checkString to check single characters in dictionary
                checkString(dict, str1);
            }
        }
    }

    //for all other strings
    for(i=2; i<length+1; i++)
    {
        for(j=0; j<length-(i-1); j++)
        {
            //makes all possible substrings of array str2 and puts them in str1
            strncpy(str1, str2 + j, i);
            str1[i] = '\0';
            //calls function checkStrings to check substings in dictionary
            checkString(dict, str1);
        }
    }

    invertedArray=NULL;
    invertedArray = malloc(sizeof(char) * length);
    if(invertedArray == NULL)
        return 0;
    //inverts strings passed into function
    for(i=length-1, j=0; i>=0; j++,i--)
    {
        invertedArray[j] = str2[i];
    }
    for(i=2; i<length+1; i++)
    {
        for(j=0; j<length-(i-1); j++)
        {
            //makes all possible substrings of inverted str2 and puts them in str1
            strncpy(str1, invertedArray + j, i);
            str1[i] = '\0';
            //calls function checkStrings to check iverted substrings in dictionary
            checkString(dict, str1);
        }
    }
    free(invertedArray);
}

