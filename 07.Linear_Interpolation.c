/***********************************
Purpose : Interpolation Error
Authors : Kirtan Patel(AE19B038)
Date    : 10/09/2020
Input   : File Path, Downsampling Constant
Output  : The Squared Error for the Input Downsampling Constant
*************************************/

/*
argc : saves the number of arguements input by the user in the command line
argv : saves the arguements input by the user in the command line
fptr : pointer to access the Data file
size : stores the size of the array required to store the data from the file
x_true[],y_true[] : stores the original data accessed in the file
y_interpolated[] : stores the interpolated data
index : stores the index value of data being accessed
skip_line : counter to check if the line is to be skipped or not
line_count : stores the row number of the data being accessed
ch :  stores the character being accessed
extra : reference to an already allocated object of type char*, 
        whose value is set by the function to the next character in str after the numerical value. 
current_number[] : stores the number being accessed in the file
DownSampled_Point_count : stores the number of points after Downsampling
Ds_c : (Downsampling Constant) stores the value of the Downsampling Constant accepted from user
ip_func[i][0] : Stores the value of slope of ith linear interpolated function
ip_func[i][1] : Stores the value of constant of ith linear interpolated function
e_squared : stores the value of squared error
*/

/*********Usage : Compile using 'gcc Linear_Interpolation.c -lm -o epsilon'*************/
/*****Usage : enter "./epsilon ~filepath~ ~Downsampling Constant~"  to get the columnwise Mean and Standard Deviation*****/

//Header files and initial declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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

    int size = 1;
    double* x_true = (double*) calloc(size,sizeof(double));
    double* y_true = (double*) calloc(size,sizeof(double));
    
	int skip_line=0,index=0,line_count=0;   //initializing variables
	char ch,*extra;
	char current_number[100];
	int DownSampled_Point_count=0;

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
            x_true=realloc(x_true,(size)*sizeof(double));
            y_true=realloc(y_true,(size)*sizeof(double)); 
		}
	}

	size=size-1;	//correcting the size after it is incremented one extra time in the last
    x_true=realloc(x_true,(size)*sizeof(double));	//optimizing space by discarding excess memory
    y_true=realloc(y_true,(size)*sizeof(double)); 


	/***CODE TO DOWNSAMPLE THE DATA***/

	int Ds_c = atoi(argv[2]);  //Take Dsc=DownSampling_Constant from the user

	if(Ds_c>=size)          //checking if the Downsampling is possible
	{	printf("Cannot Downsample so much\n");   //printing error message
  		exit(1);
  	}

	double y_interpolated[size];	

	for(int i=0;i<size;i++)
    { 
		if(i%(Ds_c)==0)	//initializing downsampled points to original value
		{	DownSampled_Point_count++;
			y_interpolated[i]=y_true[i];
		}
		if(i%(Ds_c)!=0)	//initializing non-downsampled points to zero
			y_interpolated[i]=0.0;
	}

	if((size-1)%Ds_c!=0)	//if last point is not included, explicitly include it in the downsampled dataset
	{	y_interpolated[(size-1)]=y_true[(size-1)];
		DownSampled_Point_count++;
	}

	/***CODE TO FIND PIECEWISE INTERPOLATING FUNCTION***/

	double ip_func[DownSampled_Point_count-1][2];
	if((size-1)%Ds_c==0)	//if last point is present in downsampled data
	{
		for(int i=0;i<DownSampled_Point_count-1;i++) //Defining coefficients of piecewise Function.
		{	ip_func[i][0]=(y_true[(i+1)*Ds_c]-y_true[i*Ds_c])/(x_true[(i+1)*Ds_c]-x_true[i*Ds_c]);	//slope m
			ip_func[i][1]=y_true[i*Ds_c]-(x_true[i*Ds_c]*(ip_func[i][0]));	//constant c
		}	
	}

	if((size-1)%Ds_c!=0)	//if last point is not present in downsampled data
	{
		for(int i=0;i<DownSampled_Point_count-1;i++) //To find out the coefficients of Linear Interpolated Function.
		{
			if((i+1)*Ds_c<size)
			{	ip_func[i][0]=(y_true[(i+1)*Ds_c]-y_true[i*Ds_c])/(x_true[(i+1)*Ds_c]-x_true[i*Ds_c]);	//slope m
				ip_func[i][1]=y_true[i*Ds_c]-(x_true[i*Ds_c]*(ip_func[i][0]));	//constant c
			}
			if((i+1)*Ds_c>(size-1))
			{	ip_func[i][0]=(y_true[size-1]-y_true[i*Ds_c])/(x_true[size-1]-x_true[i*Ds_c]);	//slope m
				ip_func[i][1]=y_true[i*Ds_c]-(x_true[i*Ds_c]*(ip_func[i][0]));	//constant c
			}
		}
	}	

	/***CODE TO FIND Y_INTERPOLATED***/

	for(int i=0;i<size;i++)
    {
		if(i%(Ds_c)!=0)
		{	int f_num=floor(i/Ds_c);	//decide which function number to use for interpolation
			y_interpolated[i]=((ip_func[f_num][0])*(x_true[i]))+ip_func[f_num][1];	//find the interpolated value
		}
	}

	/***CODE TO FIND EPSILON SQUARED***/  
	double e_squared=0.0;

	for(int i=0;i<size;i++)
		e_squared = e_squared + (y_true[i]-y_interpolated[i])*(y_true[i]-y_interpolated[i]);
	//Downsampling by Ds_c , the error is e_squared
	printf("%d	%le \n",Ds_c,e_squared);


	
	/**This section was used to generate the .txt Datafile which was used to plot the original and interpolated graph 
		In order to achieve this the 'CODE TO FIND EPSILON SQUARED' was removed and this snippet was added.
		During execution  the '>>' command was used to append the output values in a txt file Mix_Plot.txt
		The Original Data contains 2 columns(formed by extracting First 2 column from your Data File)
		Mix_Plot.txt has a third column added to the Original Data which stores the corresponding interpolated value
		Refer to the Key in the GNU Plots in the pdf to see how they have been accessed while plotting

	for(int i=0;i<size;i++)
		printf("%le	%le	%le\n",x_true[i],y_true[i],y_interpolated[i]);
	**/

	//freeing the pointers
	fclose(fptr);
	free(x_true);	
    free(y_true);
	
	return 0;
}
