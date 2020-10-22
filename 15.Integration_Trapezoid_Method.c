/**
 * This Code Takes in the number of divisions in which the domain is to be divided into and gives the area under the function
 * by applying Trapezoid method in each of these divisions in the domain
 **/

//Header files and initial declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


double step;
double integral=0.0;
double a=0.0;	//Lower Limit of Integration
double b=4.0;	//Upper Limit of Integration
//INTEGRATED ANSWER : 0.5947328188387356

//NO NEED TO DOWNSAMPLE. SINCE WE ARE WORKING WITH FUNCTIONS 


double func(double x)	//The Function whose area under curve we're finding
{
	double value = exp(-x) * pow(cos(x),2);
	return value;
}

double Trapezoid_Integrate(int sub_step_t)
{
	
	integral = 0.0;
	step = (b-a)/sub_step_t;

	for(double i=a ; i< b ; i=i+step)	//or (int i=0;i<sub_step_t;i++)
    {
        integral = integral + (((func(i)+func(i+step))/2)*(step));
    }

    printf("%le	%le\n",step,integral);

    return 0;

}



int main(int argc,char *argv[])  //taking filepath arguement NOTE:Code works even for oversized Data file
{	
	int divisions = atoi(argv[1]);	//stores the number of divisions the domain is to be divided into to calculate integration

	Trapezoid_Integrate(divisions);

	return 0;
}
