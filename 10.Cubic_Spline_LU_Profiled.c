/***********************************
Purpose : Interpolation Error
Authors : Kirtan Patel(AE19B038)
Date    : 29/09/2020
Input   : File Path, Downsampling Constant
Output  : The Squared Error for the Input Downsampling Constant
*************************************/

/***
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
x_ds[],y_ds[]:stores Downsampled points
cntr: counter used to initialize downsampled points
n : stores number of downsampled points-1
h[],b[],v[],u[],k[]: Arrays used in calculation of LU Decomposition
mat[][]: Stores the Matrix to be decomposed
ro
e_squared : stores the value of squared error
i,j,row,column: loop variables
ctr1,ctr2,m : control variables used for LU decomposition
Y[] : Used to store (U*K) in the L*U*K=u
Q[] : temporary array  used to store values of k[]
mul[] : array used in LU Decomposition
uppertriangle[][] : stores the upper triangular matrix
lowertriangle[][] : stores the lower triangular matrix
***/

//Logic to find piece-wise cubic spline inspired from the following page:
//https://towardsdatascience.com/numerical-interpolation-natural-cubic-spline-52c1157b98ac

/***** Usage : Compile using 'gcc Cubic_Interpolation_LU.c -lm -o CLU ' ********/
/***** Usage : enter "./epsilon ~filepath~ ~Downsampling Constant~" *****/

/**
 Since an array cannot be alloted an infinite amount of space,Downsampling constants less than :
 1.61 , when array data type used is float  (less precise)
 2.86 , when array data type used is double (more precise)
 give the Error "Segmentation Fault (Core dumped)" since the size of mat[][] exceeds limit

 Hence I've plotted:
 1. Original vs Interpolated Graph 
 2. Epsilon vs G for G>61 (Taking float)
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//https://towardsdatascience.com/numerical-interpolation-natural-cubic-spline-52c1157b98ac



int size;
float* x_true;
float* y_true;
float* x_ds;
float* y_ds;
float* y_int;
int Ds_c;
float xi;
int DownSampled_Point_count;

float Downsample()
{	//CODE TO DOWNSAMPLE THE DATA

	if(Ds_c>=size)          //checking if the Downsampling is possible
	{	printf("Cannot Downsample so much\n");   //printing error message
  		exit(1);
  	}

	y_int = (float*) calloc(size,sizeof(float));

	for(int i=0;i<size;i++)
    { 
		if(i%(Ds_c)==0)	//initializing downsampled points to original value
		{	
			y_int[i]=y_true[i];
			DownSampled_Point_count++;
		}
		if(i%(Ds_c)!=0)	//initializing non-downsampled points to zero
			y_int[i]=0.0;
	}

	if((size-1)%Ds_c!=0)	//if last point is not included, explicitly include it in the downsampled dataset
	{	y_int[(size-1)]=y_true[(size-1)];
		DownSampled_Point_count++;
	}

	//Storing Downsampled points for interpolation

	x_ds = (float*) calloc(DownSampled_Point_count,sizeof(float));
    y_ds = (float*) calloc(DownSampled_Point_count,sizeof(float));

	int cntr=0; //counter to store downsampled points
	for(int i=0;i<size;i++)
    { 	if(i%(Ds_c)==0)	//initializing downsampled points to original value
		{	
			x_ds[cntr]=x_true[i];
			y_ds[cntr]=y_true[i];
			cntr++;
		}
	}
	if((size-1)%Ds_c!=0)	//if last point is not included, explicitly include it in the downsampled dataset
	{
		x_ds[cntr]=x_true[size-1];
		y_ds[cntr]=y_true[size-1];
		cntr++;
	}


}

float Interpolate()
{
	int n;	
	n = DownSampled_Point_count-1;
	float h[n],b[n],v[n+1],u[n-2],k[n];//u[] is the RHS Matrix , k is the knot
	int i,j;
    //defining variables that we will be using
	for(i = 0; i < n; i++)
		h[i] = x_ds[i] - x_ds[i+1];
	
	
	for(i = 0; i < n; i++)
		b[i] = (y_ds[i] - y_ds[i+1])/h[i];

	for(i = 1; i < n-1; i++)
		v[i] = 2*(h[i-1]+h[i]);

	for(i = 0; i < n-2; i++)
		u[i] = 6*(b[i] - b[i+1]);

	k[0] = k[n-1] = 0;

    //Making the LHS, Matrix to be Decomposed
	float mat[n-2][n-2];
    for(int row=0;row<n-2;row++)
	{
        for(int column=0;column<n-2;column++)
			mat[row][column]=0.0;
    }
	for(i = 0; i < n-2; i++)
		mat[i][i] = v[i+1];
	

	for(i = 0; i < n-3; i++)
	{	mat[i][i+1] = h[i+1];
		mat[i+1][i] = h[i+1];
	}

	int row,column,ctr1,ctr2;
	float lowertriangle[n-2][n-2];//matrices for lower and uppper triangle
	float uppertriangle[n-2][n-2];
	
	for(row=0;row<n-2;row++)
	{	for(column=0;column<n-2;column++)
		{
			if(row>column)//initialise all elements of lower triangle in upper triangular matrix as 0
				uppertriangle[row][column]=0.0;
			if(row<column)//initialise all elements of upper triangle in lower triangular matix as 0
				lowertriangle[row][column]=0.0;
			if(row==column)//initialise all diagonal elements of lower triangular matrix as 1
				lowertriangle[row][column]=1.0;
		}
	}

    ctr2 = 0;
	int m = 1;
	while(ctr2 < n-2)//loop for LU decomposition of the matrix
	{	
		for(ctr1 = m; ctr1<n-2;ctr1++)
		{
			float mul[n-2];
			mul[ctr1] = mat[ctr1][ctr2]/mat[ctr2][ctr2];
			lowertriangle[ctr1][ctr2] = mul[ctr1];
			row = ctr1;
			for(column = ctr2;column<n-2;column++)
				mat[row][column] = mat[row][column] - mul[ctr1]*mat[ctr2][column];
		}
		m++;
		ctr2++;
	}

	for(row=0;row<n-2;row++)
	{
        for(column=0;column<n-2;column++)
        	uppertriangle[row][column] = mat[row][column];

    }

    float Y[n-2];
	for(int i=0;i<n-2;i++)
		Y[i]=0.0;
	
	Y[0]=u[0]/lowertriangle[0][0];
	for(i=1; i<n-2; i++)
	{	Y[i]=u[i];
		for(int j=0;j<i;j++)
			Y[i]=(Y[i]-(lowertriangle[i][j]*Y[j]));
		Y[i]=(Y[i]/lowertriangle[i][i]);
	}

	float Q[n-2]; //temporary matrix to avoid index confusion in k

	Q[n-3]=Y[n-3]/uppertriangle[n-3][n-3];
	for(i=n-3; i>=0; i--)
    {
        Q[i]= Y[i];
        for(j=i+1; j<=n-3; j++)
			Q[i]=Q[i]-uppertriangle[i][j]*Q[j];

		Q[i] = Q[i]/uppertriangle[i][i];
    }

	for(int i=0;i<=n-3;i++)
		k[i+1]=Q[i];


	for(j=0;j<size;j++)
    {
		for(i =0; i < n;i++)
		{	
		if((x_ds[i] < x_true[j])&&(x_true[j] < x_ds[i+1]))
		{
			y_int[j] = (k[i]/6)*(((pow((x_true[j]-x_ds[i+1]),3))/(h[i])) - ((x_true[j]-x_ds[i+1])*h[i])) - (k[i+1]/6)*(((pow((x_true[j]-x_ds[i]),3))/(h[i])) - ((x_true[j]-x_ds[i])*h[i])) + (((y_ds[i]*(x_true[j]-x_ds[i+1]))-y_ds[i+1]*(x_true[j]-x_ds[i]))/h[i]);
		}
		}
	}

	return 0.0;
}

int Error()
{
	//CODE TO FIND EPSILON SQUARED  
	float e_squared=0.0;

	for(int i=0;i<size;i++)
		e_squared = e_squared + (y_true[i]-y_int[i])*(y_true[i]-y_int[i]);
	//Downsampling by Ds_c , the error is e_squared
	printf("%d	%le \n",Ds_c,e_squared);

}



int main(int agrc, char* argv[])
{    

	FILE* fptr;
	fptr=fopen(argv[1],"r");   //opening file 
	if(fptr==NULL)          //checking if file exist or filepath is correct
	{
 		printf("FILE NOT VALID");   //printing error message
  		exit(1);
  	}


    size = 1;
    x_true = (float*) calloc(size,sizeof(float));
    y_true = (float*) calloc(size,sizeof(float));
    
	int skip_line=0,index=0,line_count=0;   //initializing variables
	char ch,*extra;
	char current_number[100];
	DownSampled_Point_count=0;

	while((ch=fgetc(fptr))!=EOF)  //accessing the file character by character
	{
		if(ch=='#')              //checking if row starts with #
			skip_line=1;         //skip line if it starts with #

		if(skip_line==0)
		{
			if(isalpha(ch)||isdigit(ch)||ch=='-'||ch=='.')   //checking if char is a part of the field
					strncat(current_number,&ch,1);	         //if yes adding it to current number
			else 
			{   float new_digits = strtod(current_number,&extra);      //if field data ends, converting it to float
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
            x_true=realloc(x_true,(size)*sizeof(float));
            y_true=realloc(y_true,(size)*sizeof(float)); 
		}
	}

	size=size-1;	//correcting the size after it is incremented one extra time in the last
    x_true=realloc(x_true,(size)*sizeof(float));	//optimizing space by discarding excess memory
    y_true=realloc(y_true,(size)*sizeof(float)); 

	Ds_c = atoi(argv[2]);  //Take Dsc=DownSampling_Constant from the user
	
	Downsample();

	Interpolate();

	Error();

    return 0;
}   
