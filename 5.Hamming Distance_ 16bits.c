/***********************************
Purpose : To find out the locations within bit array where there is a match or the locations of the minimum Hamming distance
Authors : Kirtan Patel(AE19B038)
Date    : 3/09/2020
Input   : Size of Bit Array and the Target Number
Output  : Location with Zero or Minimum Hamming Distance
*************************************/
/***********************************
arc : number of input on command line
argv[] : Array of characters to save command line input
target : stores the number from whom we calculate the hamming distance
collection_size : stores the size of the bit array as given by user
randombit : temporarily stores the random bit to be added to the collection
i : Loop variables
hammingdistance(int,int) : function to find out the hamming distance for the given input pair
num1,num2 : store the value of numbers to be used in hammingdistance()
min_count : stores numbers of positions with minimum hamming distance
positions[] : integer pointer to store positions with minimum hamming distance
min_Hamming : stores the value of minimum hamming system
Ham :  stores hamming distance of current set of bits
min_count : stores the number of positions with the minimum hamming distance
size : stores the size of positions[]
num : stores the initially generated random number 
*************************************/

/*********Usage : Compile using 'gcc Hamming.c -lm -o hamming'*************/
/*****enter "./hamming ~size of random bit array~ ~target number~" to get positions of minimum Hamming distance*****/

//IMPORTANT NOTE: Since we're using only 16 bits the Target Number should be from 0 to 65,535

//Header files and initial declarations
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>


int hammingdistance(int num1,int num2)       
{   int to_count=0;
    int count=0;
    to_count=(num1^num2);       //XOR gives 1 for positions with difference in bits for 2 numbers
    while(to_count!=0)          //We then count the number of 1s in the XOR of the 2 numbers
    {                           //Number of 1s is the Hamming Distance for the pair of numbers
        if((to_count%2)==1)
            count++;
        to_count=floor(to_count/2);
    }
    return count;       //returns the number of 1s in the XOR i.e the Hamming Distance
}

int main(int argc, char* argv[])        //Accepting command line for input
{   srand(time(NULL));      //seeding Random bit generator with time
    int target = atoi(argv[2]);     //Storing Target number
    int collection_size=atoi(argv[1]);      //Storing size of bit array
    int min_Hamming=16;         //initializing minimum Hamming Distance to the worst case scenario
    int size = 1;       //initializing array size to store the indices of minimum hamming distance
    int min_count=0;   
    int* positions = (int*) calloc(size,sizeof(int));       //integer array to store indices  of min hamming distance

     int num=rand()%65536;      //assign a random  starting number for 8 bit //turns out int is not 8 bit its 4 byte
    for(int i=0;i<collection_size-15;i++)
    {  
        int Ham=hammingdistance(num,target); 

        if(Ham==min_Hamming)
        {   size++;
            positions=realloc(positions,(size)*sizeof(int));        //increasing size of to store more positions
            positions[min_count]=i;
            min_count++;
        } 
        if(Ham<min_Hamming && Ham!=min_Hamming)     //if hamming distance is less than the minimum
        {   min_Hamming=Ham;
            size=1;         //resetting the variables back for new minimum Hamming Distance
            min_count=0;                                 
            positions = realloc(positions,1*sizeof(int));       //resetting size of positions to 1
            positions[min_count]=i;
            min_count++;
        }
        num<<=17;       //removes leftmost bit of 16bit integer
        num>>=17;       //restores the position of the other 15 bits
        num<<=1;        //adds a 0 by default  as the right most bit 
        int randombit = rand()%2;       //randomly decides whether the next bit to be added is 0 or 1
        if(randombit==1)        //if true , then we make the random bit 1
            num=num+1;      //since it is rightmost bit,changing it from 0 to 1 means increasing it by 1
        num=abs(num);       //to avoid negative numbers if they arise in case
    }
    printf("Minimum Hamming distance is %d and is found at %d indices: ",min_Hamming,size);
    for(int i=0;i<min_count;i++)
        printf("%d, ",positions[i]);
    printf("\n");

    free(positions);    //freeing the pointer 
    return 0;
}
