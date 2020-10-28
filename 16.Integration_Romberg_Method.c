/***********************************
Purpose : Integration using Romberg and Trapezoid method
Authors : Kirtan Patel(AE19B038)
Date    : 28/10/2020
Input   : File Path, Number of substeps in a 16 unit Step
Output  : Area under curve formed by Data file
*************************************/

/*********Usage : Compile using 'gcc AE19B038_Integration.c -lm -o integrate'*************/
/*****Usage : enter "./integrate ~filepath~ ~Number of substeps in a 16 unit step~"*****/


//Header files and initial declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double* x_true;
double* y_true;
int size;
int step_size;
double t_integral;
double r_integral;



double Trapezoid_Integrate(int step_t)		//step_t is the number of substeps in the 16 unit step
{
	//To take 2 substeps in 16 units, interval of steps = 16/2 = 8 units and so on for k steps , step size = 16/k;
	int step_size_t = 16/step_t;
	t_integral = 0.0;
	for(int i=1;i<size ;i++)
    {
		if(i%(step_size_t)==0)
        t_integral = t_integral + (((y_true[i]+y_true[i-step_size_t])/2)*(x_true[i]-x_true[i-step_size_t]));
		//or replace (x_true[i]-x_true[i-step]) by step_size_t*(1e-12)
    }

    return t_integral*pow(10,10);	//Returns Area in the order e-10 to get better precision

}

double Romberg_Integrate(int substep_num_r)	//step_r is the number of substeps in the 16 unit step
{
	//Calculating array size needed according to level of Romberg which we can execute according to stepsize
	int R_size = (int)(5-log2(substep_num_r));		
	
	double R[R_size][R_size];	//declaring array to be used in the Romberg Method
	

	//initializing array values to zero
	for(int j=0;j<R_size;j++)
	{
		for(int k=0;k<R_size;k++)
		{
			R[k][j]=0.0;	
		}
	}

	//initializing first column to the area found by using Trapezoid method and corresponding stepsize
	r_integral=0.0;
	int index=0;
	for(int i=substep_num_r;i<=16;i=i*2)
	{	
		R[0][index]=Trapezoid_Integrate(i);		//r[k][0] is found by trapezoid in 1e-10 for higher precision
		index++;
		
	}


	if(substep_num_r<16)
	{
		for(int j=2;j<=R_size;j++)
		{
			for(int k=j;k<=R_size;k++)
			{
				R[j-1][k-1] = R[j-2][k-1] + (1/(pow(4,j-1)-1))*(R[j-2][k-1]-R[j-2][k-2]);	//adding -1 to each indice to adjust formula for matrix indices
			}
		}

	}	

/**	USED TO PRINT OUTPUT FOR SUBMISSION FILE
	for(int j=0;j<R_size;j++)
	{
		for(int k=0;k<=j;k++)
		{
			printf("%le	",R[k][j]/pow(10,10));  // Dividing by pow(10,10) because we had multiplied with pow(10,10) in Trapezoid method to get better precision
		}
		printf("\n");
	}
**/
	
	r_integral = R[R_size-1][R_size-1];		//The final value of corrected integral is the last value in array

    return r_integral;		//returning the final value of integral

}




int main(int argc,char *argv[])  //taking filepath arguement NOTE:Code works even for oversized Data file
{	
	/***CODE TO READ THE DATA FROM THE FILE AND SAVE IT IN AN ARRAY***/

	FILE* fptr;
	fptr=fopen(argv[1],"r");   //opening file 
	if(fptr==NULL)          //checking if file exist or filepath is correct
	{
 		printf("FILE NOT VALID");   //printing error message
  		exit(1);
  	}

    size = 1;
    x_true = (double*)malloc(size*sizeof(double));
    y_true = (double*)malloc(size*sizeof(double));
    
	int skip_line=0,index=0,line_count=0;   //initializing variables
	char ch,*extra;
	char current_number[100];

	while((ch=fgetc(fptr))!=EOF)  //accessing the file character by character
	{
		if(ch=='#')              //checking if row starts with #
			skip_line=1;         //skip line if it starts with #

		if(skip_line==0)
		{
			if(isalpha(ch)||isdigit(ch)||ch=='-'||ch=='.')   //checking if char is a part of the field
					strncat(current_number,&ch,1);	         //if yes adding it to current number
			else 
			{   double new_digits = strtod(current_number,&extra);      //if field data ends, converting it to double
				if(index==0)
                    x_true[line_count]=new_digits;                      //adding number to Array of x_true
				if(index==1)
                    y_true[line_count]=new_digits;		     //adding square of number to sum of squares of corresponding column
				index++;                                         // switching to next column
				current_number[0]='\0';						     //Resetting the number to be added to zero
			}
		}

		if(ch=='\n')        //checking for next line
		{   size++;
			line_count++;      //counter to count the lines
			skip_line=0;	   //reseting the counter to check if line starts with #
			index=0;		   //reseting index to zero
            x_true=(double*)realloc(x_true,(size)*sizeof(double));
            y_true=(double*)realloc(y_true,(size)*sizeof(double)); 
		}
	}

	size=size-1;	//correcting the size after it is incremented one extra time in the last
    
    x_true=(double*)realloc(x_true,(size)*sizeof(double));	//optimizing space by discarding excess memory
    y_true=(double*)realloc(y_true,(size)*sizeof(double)); 

	int sub_step = atoi(argv[2]);  //Number of substeps in a 16 unit Step

	if(sub_step<=0 || sub_step>16 || (sub_step%2)!=0)	//condition for working of Romberg Method
	{
		if(sub_step!=1)
		{
		printf("Please enter either of {1,2,4,8,16}\n");
		exit(1);
		}
	}

	step_size = 16/sub_step;
	
	printf("%d	%fe-10\n",step_size,Romberg_Integrate(sub_step));	//printing output

	//freeing the pointers
	fclose(fptr);

	free(x_true);	
  	free(y_true);

	
	return 0;
}
