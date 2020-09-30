/***********************************
Purpose : To find out the Mean and Standard Deviation of every column of a given dataset
Authors : Kirtan Patel(AE19B038)
Date    : 23/08/2020
Input   : Path of the file without any spaces (replace spaces in path file with underscore if needed) 
Output  : Mean and Standard Deviation of each field of the dataset
*************************************/
/***********************************
Purpose : To find out the Mean and Standard Deviation of every column of a given dataset
Authors : Kirtan Patel(AE19B038)
Date    : 23/08/2020
Input   : Path of the file without any spaces (replace spaces in path file with underscore if needed)
Output  : Mean and Standard Deviation of each field of the dataset
*************************************/

/*********Usage : Compile using 'gcc columnwise_mean_sd_new.c -lm -o run'*************/
/*****Usage : To get answer , enter "./run *filepath*" to get the columnwise Mean and Standard Deviation*****/

//Header files and initial declarations
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>

int main(int argc,char *argv[])  //taking filepath arguement
{	
	FILE* fptr;
	fptr=fopen(argv[1],"r");   //opening file 
	if(fptr==NULL)          //checking if file exist or filepath is correct
	{
 		printf("FILE NOT VALID");   //printing error message
  		exit(1);
  	}

	int skip_line=0,space =0,index=0,line_count=0;   //initializing variables
	char ch,*extra;
	char current_number[100];
	double numbers_sum[10];
	double numbers_square[10];
	for(int i=0;i<10;i++)     //initializing of sum and squared sum array
	{
		numbers_sum[i]=0;
		numbers_square[i]=0;
	}
	while((ch=fgetc(fptr))!=EOF)  //accessing the file character by character
	{
		if(ch=='#')              //checking if row starts with #
			skip_line=1;         //skip line if it starts with #

		if(skip_line==0)
		{
			if(isalpha(ch)||isdigit(ch)||ch=='-'||ch=='.')   //checking if char is a part of the field
					strncat(current_number,&ch,1);	         //if yes adding it to current number
			else 
			{   double digits = strtod(current_number,&extra);      //if field data ends, converting it to double
				numbers_sum[index]+=digits;                      //adding number to sum of corresponding column
				numbers_square[index]+=(digits*digits);		     //adding square of number to sum of squares of corresponding column
				index++;                                         // switching to next column
				current_number[0]='\0';						     //Resetting the number to be added to zero
			}
		}

		if(ch=='\n')        //checking for next line
		{
			line_count++;      //counter to count the lines
			skip_line=0;	   //reseting the counter to check if line starts with #
			index=0;		   //reseting index to zero
		}
	}

	printf("\n");
	printf("Coulmn \t Mean \t\t Standard Deviation\n");   //printing header row

	for(int i=0;i<10;i++)            //iteration to print the calculated values
	{
		printf("%2d  %le  %le\n",(i+1), numbers_sum[i]/line_count, sqrt(((numbers_square[i]/line_count)-((numbers_sum[i]*numbers_sum[i])/(line_count*line_count)))));
	}
	fclose(fptr);
	printf("\n");
	char cmd[10000]="grep -v '#' ";
	const int system();   //initializing system() function
	strcat(cmd,argv[1]);
	strcat(cmd,"|awk 'BEGIN{print \"Coulmn \t Mean \t\t Standard Deviation\" }{for(i=1;i<=NF;i++){sum[i]+=$i;sumsq[i]+=$i*$i}}END{for(i=1;i<=NF;i++){print i,\" \t \" ,sum[i]/NR,\" \t \" ,sd=sqrt((sumsq[i]/NR)-(sum[i]*sum[i])/(NR*NR))}}'");
	system(cmd);  //executing grep and awk command using system()
	
	return 0;

}
