/**
 * This Code Takes in the number of divisions in which the domain is to be divided into and Order of Romberg Method to use
 * gives the area under the function by applying Trapezoid method in each of these divisions in the domain
 **/


//Header files and initial declarations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int divisions;
int order;
double a = 0;	//a is the lower limit of integration
double b = 4;	//b is the upper limit of integration
double step_size;
double t_integral;
double r_integral;


double func(double x)
{
	double value = exp(-x) * pow(cos(x),2);
	return value;
}


double Trapezoid_Integrate(int sub_step_t)
{
	t_integral = 0.0;
	double step = (b-a)/sub_step_t;

	for(double i=a ; i< b ; i=i+step)	
	{
        t_integral = t_integral + (((func(i)+func(i+step))/2)*(step));
    }

    return t_integral;
}

double Romberg_Integrate(int divisions1,int order)	//divisions1
{
	int R_size = order;
	double R[R_size][R_size];
	

	r_integral=0.0;
	for(int i=0;i<order;i++)
	{	
		R[i][0]=Trapezoid_Integrate(divisions1*pow(2,i));		//r[k][0] is R k 1
				
	}

	if(order>1)
	{
		for(int j=2;j<=R_size;j++)
		{
			for(int k=j;k<=R_size;k++)
			{
				R[k-1][j-1] = R[k-1][j-2] + (1/(pow(4,j-1)-1))*(R[k-1][j-2]-R[k-2][j-2]);	//adding -1 to each indice to adjust formula for matrix indices
			}
		}

	}	
	
	r_integral = R[R_size-1][R_size-1];

    return r_integral;

}




int main(int argc,char *argv[])
{	
	
	int divisions = atoi(argv[1]);	//stores the number of divisions the domain is to be divided into to calculate integration
	int order_of_Romberg = atoi(argv[2]);	//specifies which order of Romberg we have to use
	//First order Romberg = Trapezoid

	step_size=(b-a)/divisions;	
	printf("%le	%le\n",step_size,Romberg_Integrate(divisions,order_of_Romberg));

	return 0;
}
