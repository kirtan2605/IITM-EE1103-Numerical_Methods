
/*********Usage : Compile using 'gcc Trapezoid.c -lm -o trapezoid'*************/
/*****Usage : enter "./trapezoid ~filepath~ ~Downsampling Constant~"*****/



//Header files and initial declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double* x_true;
double* y_true;
int size;
int step;
double integral=0.0;

//NO NEED TO DOWNSAMPLE. SINCE INPUT IS 2^n , WE WORK USING INDICES 

int Trapezoid_Integrate(int sub_step_t)
{
	step = 16/sub_step_t;
	for(int i=1;i<size ;i++)
    {
		if(i%(step)==0)
        integral = integral + (((y_true[i]+y_true[i-step])/2)*((x_true[i]-x_true[i-step])));
    }

    printf("%d	%fe-10\n",step,integral*(pow(10,10)));

    return 0;

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

	int sub_step = atoi(argv[2]);  //Number of substeps in a 2^4 Step

	if(sub_step<=0 || sub_step>16 || (sub_step%2)!=0)	//condition for working of Romberg Method
	{
		if(sub_step!=1)
		{
		printf("Please enter either of {1,2,4,8,16}\n");
		exit(1);
		}
	}


/**
    //step_t is the number of substeps in the 16 unit step
	printf("%fe-10\n",Trapezoid_Integrate(1));		//LEAST Precise
	printf("%fe-10\n",Trapezoid_Integrate(2));
	printf("%fe-10\n",Trapezoid_Integrate(4));
	printf("%fe-10\n",Trapezoid_Integrate(8));
	printf("%fe-10\n",Trapezoid_Integrate(16));	   //MOST Precise
**/

    
	Trapezoid_Integrate(sub_step);

	//freeing the pointers
	fclose(fptr);
	free(x_true);	
    free(y_true);

	
	return 0;
}
