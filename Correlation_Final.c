//Header files and initial declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//#include <complex.h>    //has I defined for complex part

int size;
double* time ;
double* data_1 ;
double* data_2 ;
double Ts = 5e-6;   //Sampling_period i.r time between  each reading
double t_start = 0 ;    //start time of Data Redding
double t_end = 0.005 ;    //end time of Data Reading
double data_1_frequency;
double data_2_frequency;
double phase1 ; 
double phase2 ;
double omega1 ;
double omega2 ;
double an1 ;
double bn1 ;
double an2 ;
double bn2 ;

double phase_diff(double p1 , double p2)
{   
    double p_d;
    if(p1-p2 > 0)
    {   //p_d = ((p1-p2)%360);      //can use % only with integers
        p_d = fmod(p1-p2,360);
        //printf("data 1 leads data 2 by : %.2f degree\n",p_d);
    }

    if(p2-p1 > 0)
    {   //p_d = ((p2-p1)%360);      //can use % only with integers
        p_d = fmod(p2-p1,360);
        //printf("data 2 leads data 1 by : %.2f degree\n",p_d);
    }

    if(p1-p2==0)
    { p_d = 0;   }
    
    return p_d;
}



int main(int argc,char *argv[])  //taking filepath arguement NOTE:Code works even for oversized Data file
{	

	/***CODE TO READ THE DATA FROM THE FILE AND SAVE IT IN AN ARRAY***/

	FILE* fptr;
	fptr=fopen("Data_4_Corr.txt","r");   //opening file 
	if(fptr==NULL)          //checking if file exist or filepath is correct
	{
 		printf("FILE NOT VALID");   //printing error message
  		exit(1);
  	}

    size = 1;
    time = (double*) calloc(size,sizeof(double));
    data_1 = (double*) calloc(size,sizeof(double));
    data_2 = (double*) calloc(size,sizeof(double));
    
	int skip_line=0,index=0,line_count=0;   //initializing variables
	char ch,*extra;
	char current_number[100];

	while((ch=fgetc(fptr))!=EOF)  //accessing the file character by character
	{
		if(ch=='#')              //checking if row starts with #
			skip_line=1;         //skip line if it starts with #

		if(skip_line==0)
		{
			if(isalpha(ch)||isdigit(ch)||ch=='-'||ch=='.'||ch=='+')   //checking if char is a part of the field
					strncat(current_number,&ch,1);	         //if yes adding it to current number
			else 
			{   double new_digits = strtod(current_number,&extra);      //if field data ends, converting it to double
				if(index==0)
                    time[line_count]=new_digits;                      //adding number to Array of time
				if(index==1)
                    data_1[line_count]=new_digits;		     //adding square of number to sum of squares of corresponding column
				if(index==2)
                    data_2[line_count]=new_digits;
                index++;                                         // switching to next column
				current_number[0]='\0';						     //Resetting the number to be added to zero
			}
		}

		if(ch=='\n'&& skip_line ==0)        //checking for next line
		{   size++;
			line_count++;      //counter to count the lines
			index=0;		   //reseting index to zero
            time = realloc(time,(size)*sizeof(double));
            data_1 = realloc(data_1,(size)*sizeof(double));
            data_2 = realloc(data_2,(size)*sizeof(double)); 
		}

        if(ch=='\n'&& skip_line == 1)
        {
            skip_line=0;	   //reseting the counter to check if line starts with #
        }
	}

	size = size-1;	//correcting the size after it is incremented one extra time in the last
    time = realloc(time,(size)*sizeof(double));	//optimizing space by discarding excess memory
    data_1 = realloc(data_1,(size)*sizeof(double));
    data_2 = realloc(data_2,(size)*sizeof(double));  



    //We only take N/2 bins,because we cant measure correctly beyond the NyQuist Limit = Sampling Frequency/2
    //if size is odd we cant take size/2 thus defining a standardizing variable N

    int N;
    if(size%2==0)
    {N=size;}
    if(size%2!=0)
    {N=size-1;}

    double * amplitude_real = (double *) calloc(N/2,sizeof(double));     //stores real part of frequency
    double * amplitude_imag = (double *) calloc(N/2,sizeof(double));     //stores real part of frequency

    //These store the frequency of of the Frequency Bin (NOT FREQUENCY)
    //We only take N/2 bins,because we cant measure correctly beyond the NyQuist Limit = Sampling Frequency/2
    //since nth frequency bin corresponds to frequency = n*(frequency resolution)
    //where frequency resolution = Sampling frequency / Number of samples

    //IMPORTANT
    //After eliminating Data beyong Nyquist frequency,we double the modulus of the data below the NyQuist Frequency
    //We then Divide the modulus by N, for normalizing it back to its original value
    //Since we are concerned with phase,we can skip the last 2 steps since the ratio of the real and img part stays same


    double f_sampling = 1/Ts;
    double df = f_sampling/N; // Frequency increment per bin OR  Frequency Resolution




    for(int k=0;k<N/2;k++)    
    {
        amplitude_real[k]=0;  //initializing
        amplitude_imag[k]=0;  //initializing
        for(int n=0;n<size;n++)     //we can still use size here since data is defined by size of file
        {
            amplitude_real[k] = amplitude_real[k] + ((data_1[n])*(cos((-1)*(2*M_PI/size)*(k*n))));  //freq resolution taken
            amplitude_imag[k] = amplitude_imag[k] + ((data_1[n])*(sin((-1)*(2*M_PI/size)*(k*n))));  ////freq resolution taken
        }

    }

//printing the frequency (NOT FREQUENCY BIN) using the above stated formulas
//Frequency for kth frequency bin = (k*Sampling Frequency)/Number of Samples


//printf("\n");
//TO RETURN Fundamental FREQUENCY
    for(int k=1;k<N/2-1;k++)
    {
        double modulus = sqrt(pow(amplitude_real[k],2)+pow(amplitude_imag[k],2));
        double modulus1 = sqrt(pow(amplitude_real[k+1],2)+pow(amplitude_imag[k+1],2));
        double modulus2 = sqrt(pow(amplitude_real[k-1],2)+pow(amplitude_imag[k-1],2));
        if(modulus>=modulus1 && modulus>=modulus2 && (k*df)<(f_sampling/2))    
            {
                //printf("The Frequency obtained by DFT on Data 1 is   %f\n",(k*df));
                
                data_1_frequency = (k*df);

                //printf("data 1 modulus : %f\n",modulus*(2/size));   //amplittude with NyQuist correction

                k=N;    //to exit loop after getting frequency for faster execution
                //REMOVE THIS LAST LINE IF MORE THAN 1 FREQUENCIES ARE PRESENT
            }   
    }








    //Finding it for Data 2
    for(int k=0;k<N/2;k++)    
    {
        amplitude_real[k]=0;  //initializing
        amplitude_imag[k]=0;  //initializing
        for(int n=0;n<size;n++)     //we can still use size here since data is defined by size of file
        {
            amplitude_real[k] = amplitude_real[k] + ((data_2[n])*(cos((-1)*(2*M_PI/size)*(k*n))));
            amplitude_imag[k] = amplitude_imag[k] + ((data_2[n])*(sin((-1)*(2*M_PI/size)*(k*n))));
            //printf("real:%f      imaginary:%f\n",(cos((-1)*(2*M_PI/N)*(k*df*n))),sin((-1)*(2*M_PI/N)*(k*df*n)));
        }

    }


//TO RETURN Fundamental FREQUENCY
    for(int k=1;k<N/2-1;k++)
    {
        double modulus = sqrt(pow(amplitude_real[k],2)+pow(amplitude_imag[k],2));
        double modulus1 = sqrt(pow(amplitude_real[k+1],2)+pow(amplitude_imag[k+1],2));
        double modulus2 = sqrt(pow(amplitude_real[k-1],2)+pow(amplitude_imag[k-1],2));
        if(modulus>=modulus1 && modulus>=modulus2 && (k*df)<(f_sampling/2))    
            {
                //printf("The Frequency obtained by DFT on Data 2 is   %f\n",(k*df));

                data_2_frequency = (k*df);
                //printf("data 2 modulus : %f\n",modulus*(2/size));   //amplittude with NyQuist correction

                k=N;    //to exit loop after getting frequency for faster execution
                //REMOVE THIS LAST LINE IF MORE THAN 1 FREQUENCIES ARE PRESENT
            }   
    }




/**
Note that a cosine term an*cos(2π*nf*t) and a sine term bn*sin(2πnf0t) (of the same frequency nf )
may be viewed as a single cosine waveform of frequency nf :an*cos(2π*nf*t) + bn*sin(2π*nf*t) = An*cos(2π*nf*t + phi)
where the amplitude An=sqrt(an^2 + bn^2) and the phase angle phi = −atan(bn/an).

We use the reverse of this to find the phase of the data signals
**/


//THIS PORTION CAN BE CHANGED TO WORK IN CASE OF MULTIPLE FREQUENCIES BEING DETECTED IN THE DATA
omega1 = 2*M_PI*data_1_frequency ;
omega2 = 2*M_PI*data_2_frequency ;
an1 = 0 ;
bn1 = 0 ;
an2 = 0 ;
bn2 = 0 ;

/** Simultaneous Equations
data1[0] = an1*cos(omega1*time[0]) + bn1*sin(omega1*time[0]);
data1[1] = an1*cos(omega1*time[1]) + bn1*sin(omega1*time[1]);

a = bx + cy;        ------(1)
d = ex + fy;        ------(2)

from (1)
x = (a-cy)/b

substituting in (2)

bd = e*(a-cy) + fby...
bd = ea - ecy + fby...
bd - ea = (fb-ec)y...

y = (bd-ea)/(fb-ec);        -----(3)
x = (a-cy)/b;           -----(4)    //for b not equal to zero //CODE can be improved to take time[n],[n+1] st. b!=0

THIS PORTION OF THE CODE CHANGES IF THERE ARE MULTIPLE FREQUENCIES PRESENT
**/

//USING (3) and (4)
bn1 = (((cos(omega1*time[0]))*(data_1[1])) - (((cos(omega1*time[1]))*data_1[0])))/ ( ((sin(omega1*time[1]))*(cos(omega1*time[0]))) - ((cos(omega1*time[1]))*(sin(omega1*time[0]))) ) ;
an1 = ( (data_1[0]) - (bn1*sin(omega1*time[0])) ) / cos(omega1*time[0]);

//Similarly for data 2
bn2 = (((cos(omega2*time[0]))*(data_2[1])) - (((cos(omega2*time[1]))*data_2[0])))/ ( ((sin(omega2*time[1]))*(cos(omega2*time[0]))) - ((cos(omega2*time[1]))*(sin(omega2*time[0]))) ) ;
an2 = ( (data_2[0]) - (bn2*sin(omega2*time[0])) ) / cos(omega2*time[0]);


//phase in degree
phase1 = (-1)*atan(bn1/an1)*(180/M_PI);
phase2 = (-1)*atan(bn2/an2)*(180/M_PI);

double mod1 = sqrt(pow(an1,2)+pow(bn1,2));
double mod2 = sqrt(pow(an2,2)+pow(bn2,2));

double phase_Difference_data1_data2 = phase_diff(phase1,phase2);  //enter phase in degrees

//Finding Phase Difference using Spearman Correlation Coeff (rho) by the formula phase_difference = acos(rho)
//Where rho is calculated with delay_parameter = 0;
//basically something like dot product dot product...
    double coeff1 = 0.0;
    double coeff2 = 0.0;
    double coeff3 = 0.0;
    for(int i = 0;i<size;i++)   //for other values dataset1() returns zero anyways sinze zero padded
    {
       coeff1 = coeff1 + data_1[i]*data_2[i] ;
       coeff2 = coeff2 + data_1[i]*data_1[i] ;
       coeff3 = coeff3 + data_2[i]*data_2[i] ;
    }
    double rho = (coeff1 / (sqrt(coeff2*coeff3)));

    double phase_using_Corr = acos(rho)*(180/M_PI);  //phase in degree



//Relation Ratio T0/Ts
double ratio_data1 = f_sampling/data_1_frequency;
double ratio_data2 = f_sampling/data_2_frequency;





//CORRELATION COEFFICIENT
for(int delay_parameter=0;delay_parameter<1001;delay_parameter++)
{
    double corr_coeff = 0.0;
    for(int i = 0;i<size-delay_parameter;i++)   //for other values it returns zero anyways sinze zero padded
    {
       corr_coeff = corr_coeff + ((data_1[i])*(data_2[i+delay_parameter])) ;
    }
    corr_coeff = corr_coeff*Ts;
    printf("%d  %f\n",delay_parameter,corr_coeff);
}

    return 0;
}


