/***********************************
Purpose : To find out the Factorial of a given positive integer
Authors : Kirtan Patel(AE19B038)
Date    : 11/08/2020
Input   : Number whose Factorial is to be calculated
Output  : Factorial of the Number if it is a positive integer
*************************************/

/***********************************
arc : number of input on command line
argv[] : Array of characters to save command line input
num : stores number input for factorial calculation by user
fon : Acronym for 'Floor of Num'. Stores Floot value of Num
fact : stores the value of factorial
*************************************/

//Usage : Compile using 'gcc factorial.c -lm -o factorial'*/

//Header files and initial declarations

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

long fact(int m)                      //declaring factorial method
   {
      if(m==1)
         return(1);
      else
         return(m*fact(m-1));        // using recursion to calculate factorial
   }   
   
int main(int argc, char* argv[])     //Accepting command line for input
{
   float num = atof(argv[1]);
   int fon =floor(num);
   long fact = 1;                   // initialization of factorial variable
    if (argc == 2)                 // checking for valid input syntax
   	{ 
         if(num>=0 && num-fon==0)           // checking if the input number is an integer
         {
            long factorial= fact(num1);
            printf("The factorial of %d is %li \n",num1,factorial);     
         } 

         else
            {
               printf("INVALID INPUT : Factorial exists for positive integers\n");
            }
      }
   else
   { 
   	   printf("Write command as './factorial *number*' to find out the factorial of *number*\n");
   }

   return 0;
}
