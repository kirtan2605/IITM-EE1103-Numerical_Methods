/***********************************
Purpose : To find out the locations within bit array where there is a match or the locations of the minimum Hamming distance
Authors : Kirtan Patel(AE19B038)
Date    : 30/08/2020
Input   : Size of Bit Array and the Target Number
Output  : Location with Zero or Minimum Hamming Distance
*************************************/

/***********************************
arc : number of input on command line
argv[] : Array of characters to save command line input
target_num : stores the number from whom we calculate the hamming distance
target_bitsize : interger bitsize of the target number
target_string : stores the target number in binary as a string
c_bits : stores the size of the bit array as given by user
power : stores the power used to find binary of a number
n : takes the value of target number in int_to_binary(int) function
bin,binary : stores the binary value of the target number
add_bit : temporarily stores the random bit to be added to the collection
collection : stores the array of c_bitsize random bits
i,j : Loop variables
int_to_binary() : function to convert target number to binary
min_count : stores numbers of positions with minimum hamming distance
positions[] : array to store positions with minimum hamming distance
min_ham : stores the value of minimum hamming system
ham :  stores hamming distance of current set of bits
min_count : stores the number of positions with the minimum hamming distance
*************************************/

/*********Usage : Compile using 'gcc Hamming1.c -lm -o hamming'*************/
/*****enter "./hamming ~size of random bit array~ ~target number~" to get positions of minimum Hamming distance*****/

//Storing Data in one byte

//Storing Data in one byte
//Header files and initial declarations

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
int target_bitsize = 0;
// C Program for the binary representation of a given number 
int int_to_binary(int n) 
{   int power=0;
    
    long long int bin=0;
    while (n!=0)
    {   target_bitsize++;
        bin=bin+((n%2)*pow(10,power));
        n=(int)n/2;
        power++;
    }   
    return bin;   
} 

int main(int argc, char* argv[])     //Accepting command line for input
{   
    srand(time(NULL));
    int c_bits = atoi(argv[1]);
    char collection[c_bits];    //creating char array to store random bit collection
    int target_num = atoi(argv[2]);  //converting target number to integer
    long long int binary=int_to_binary(target_num);  //finding the binary of the target number
    char target_string[target_bitsize];   //initializing string to save binary of target number
    sprintf(target_string, "%lli", binary);   //alloting binary number to string
    collection[0]='\0';      //clearing the dump value assigned to it
 
    if(c_bits<target_bitsize)   //checking if the desired calculations can be made or not
    {   printf("Not Possible\n");
        exit(1);}

   for(int i=0;i<c_bits;i++)
   {
    char add_bit = rand()%2+'0';   //generating random bit and converting it to char
    strncat(collection,&add_bit,1);   //adding random bit to collection of bits
   }

    int min_count=0;  
    int positions[c_bits];   
    int min_ham=target_bitsize;   //setting minimum hamming distance to maximum possible hamming distance
  
    for(int i=0;i<(c_bits-target_bitsize);i++)   //parsing throufh the collection of random bits
    {   
        int ham=0;  //for every set of bits,initializing hamming distance to zero
        for(int j=0;j<target_bitsize;j++)   //loop to parse through a set of bits = number of bits or target number
        {  if(collection[i+j]!=target_string[j])  //checking corresponding bits
                ham++;
        }
        if(ham==min_ham)   //if hamming distance is minimum
        {
            positions[min_count]=i;   //add that index to positions[] 
            min_count++;
        }
        if(ham<min_ham && ham!=min_ham)   //if hamming distance is less than the minimum
        {   min_count=0;     //reset minimum hamming distance counter to zero
            memset(positions,0,c_bits*sizeof(int));    //clear the positions[] array since the initial positions are invalid now
            min_ham=ham;  //setting minimum hamming distance to current hamming distance
            positions[min_count]=i;  //addind the current index to the new position[] array
            min_count++; 
        }
    }
    printf("%d is the minimum Hamming Distance found in %d groups of %d bits strating from index: ",min_ham,min_count,target_bitsize);
    for(int i=0;i<min_count;i++)
        printf("%d ",positions[i]);
    printf("\n");
    return 0;
}
