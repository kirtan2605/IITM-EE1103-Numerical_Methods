#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define gamma -1.76e11
#define Hz 7.96e4 //100e-3*(1/Mu0)
#define omega (gamma*(100e-3)*-1)  //*-1 to get positive T0(time for one precession) omega = Mu0*Hz*gamma
#define T0 ((2*M_PI)/omega)
#define step_size 1e-16


double alpha;
double lambda=0.0;
double* theta;  //stores values for midpoint method
double* phi;
double* rktheta;    //stores values for RK45 method
double* rkphi;
double tk1=0,tk2=0,pk1=0,pk2=0;   //stores values for midpoint method
double rktk1=0,rktk2=0,rktk3=0,rktk4=0,rkpk1=0,rkpk2=0,rkpk3=0,rkpk4=0;     //stores value for RK45 Method

double t_half_step=0.0; //half step in theta
double p_half_step=0.0; //halfstep in phi
double mod = 0.0;
double tvalue=0.0;
double pvalue=0.0;
double step=0.0;
int flg = 1;
double val = M_PI / 180.0;
int n=0; //to scale theta between 0 and 360
double temp=0.0; //double to deal with negative angles

double dthetadt(double theta1,double phi1)
{
    tvalue=lambda*Hz*sin(theta1*val);
    return tvalue;
}

double dphidt(double theta2,double phi2)
{
    pvalue=dthetadt(theta2,phi2)*-1*(1/(alpha*sin(theta2*val)));
    return pvalue;
}

int main(int argc,char* argv[])
{
    alpha =atof(argv[1]);
    lambda=alpha*(gamma/(1+pow(alpha,2)));

    theta = malloc(1*(sizeof(double)));
    phi = malloc(1*(sizeof(double)));
    rktheta = malloc(1*(sizeof(double)));
    rkphi = malloc(1*(sizeof(double)));

    int theta_initial= 179 ; //in degrees
    int phi_initial = 1 ; //in degrees


/*  Using the relation:
    mx[0]=(sin(theta[0]*val))*(cos(phi[0]*val));
    my[0]=(sin(theta[0]*val))*(sin(phi[0]*val));
    mz[0]=cos(theta[0]*val);
*/
    theta[0]=theta_initial;
    phi[0]=phi_initial;
    rktheta[0]=theta_initial;
    rkphi[0]=phi_initial;


    //initial error is zero
    printf("0    0     0     0\n");     //for error in mx,my,mz
    //printf("0     0       0\n");      //for error in theta,phi

    //CALCULATING VALUES OF THETA AND PHI
    long long unsigned int size=2;
    int i=0;
    while(flg>0)
    {
        theta=realloc(theta,(size)*sizeof(double));
        phi=realloc(phi,(size)*sizeof(double));
        rktheta=realloc(rktheta,(size)*sizeof(double));
        rkphi=realloc(rkphi,(size)*sizeof(double));

        //Using Midpoint Method Method
        tk1=(dthetadt(theta[i],phi[i]))*step_size;
        pk1=(dphidt(theta[i],phi[i]))*step_size;

        t_half_step=(0.5)*tk1;
        p_half_step=(0.5)*pk1;

        tk2=(dthetadt(theta[i]+t_half_step, phi[i]+p_half_step))*step_size; 
        pk2=(dphidt(theta[i]+t_half_step, phi[i]+p_half_step))*step_size;               

        theta[i+1]=theta[i]+(tk2);
        phi[i+1]=phi[i]+(pk2);

        //Using RK45 or Fourth Order Runge-Kutta Method
        rktk1=(dthetadt(rktheta[i],rkphi[i]))*step_size;
        rkpk1=(dphidt(rktheta[i],rkphi[i]))*step_size;

        t_half_step=(0.5)*tk1;
        p_half_step=(0.5)*pk1;

        rktk2=(dthetadt(rktheta[i]+t_half_step, rkphi[i]+p_half_step))*step_size; 
        rkpk2=(dphidt(rktheta[i]+t_half_step, rkphi[i]+p_half_step))*step_size;   

        t_half_step=(0.5)*tk2;
        p_half_step=(0.5)*pk2;

        rktk3=(dthetadt(rktheta[i]+t_half_step, rkphi[i]+p_half_step))*step_size; 
        rkpk3=(dphidt(rktheta[i]+t_half_step, rkphi[i]+p_half_step))*step_size;       

        rktk4=(dthetadt(rktheta[i]+rktk3, rkphi[i]+rkpk3))*step_size; 
        rkpk4=(dphidt(rktheta[i]+rktk3, rkphi[i]+rkpk3))*step_size;                

        rktheta[i+1]=rktheta[i]+((rktk1+(2*rktk2)+(2*rktk3)+rktk4)/6);
        rkphi[i+1]=rkphi[i]+((rkpk1+(2*rkpk2)+(2*rkpk3)+rkpk4)/6);


        //scaling values down of phi from 0 to 360
        if(phi[i+1]>0&&phi[i+1]>360)
        {
            n=floor(phi[i+1]/360);
            phi[i+1]=phi[i+1]-(n*360);
        }
        if(phi[i+1]<0&&fabs(phi[i+1]>360)) 
        {   
            temp=fabs(phi[i+1]);
            n=floor(temp/360);
            temp=temp-(n*360);
            phi[i+1]=360-temp;
        }

        //scaling values down of rkphi from 0 to 360
        if(rkphi[i+1]>0&&rkphi[i+1]>360)
        {
            n=floor(rkphi[i+1]/360);
            rkphi[i+1]=rkphi[i+1]-(n*360);
        }
        if(rkphi[i+1]<0&&fabs(rkphi[i+1]>360)) 
        {   
            temp=fabs(rkphi[i+1]);
            n=floor(temp/360);
            temp=temp-(n*360);
            rkphi[i+1]=360-temp;
        }

/*      Using the following relations:
        mx[i+1]=(sin(theta[i+1]*val))*(cos(phi[i+1]*val));
        my[i+1]=(sin(theta[i+1]*val))*(sin(phi[i+1]*val));
        mz[i+1]=cos(theta[i+1]*val);
*/

        //FINDING AND PRINTING ERROR FOR EACH ITERATION(STEP)

        //error in theta,phi (if needed to find)
/*      double error_theta=(rktheta[i+1]-theta[i+1]);
        double error_phi=(rkphi[i+1]-phi[i+1]);
        printf("%le     %le     %le\n",((i+1)*step_size),pow(error_theta,2),pow(error_phi,2));
*/

        //error in <mx>,<my>,<mz>
        double error_mx=((sin(rktheta[i+1]*val))*(cos(rkphi[i+1]*val)))-((sin(theta[i+1]*val))*(cos(phi[i+1]*val)));
        double error_my=((sin(rktheta[i+1]*val))*(sin(rkphi[i+1]*val)))-((sin(theta[i+1]*val))*(sin(phi[i+1]*val)));
        double error_mz=((cos(rktheta[i+1]*val))-(cos(theta[i+1]*val)));             
        printf("%le     %le     %le     %le\n",(((i+1)*step_size)/pow(10,-15)),pow(error_mx,2),pow(error_my,2),pow(error_mz,2));
        
        //time taken = (i+1)*step_size/(10^-15) in femtoseconds
    
        //flag to stop calculating when dipole alligns with H i.e. when angle between M and H is less than 1 degree
        if(theta[i+1]<=1)
            {flg=0;}

       if(theta[i+1]>1)
       {
            i++;
            size++;
       }

    }

    //freeing memory
    free(theta);
    free(phi);
    free(rktheta);
    free(rkphi);

return 0;
}


