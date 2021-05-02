
/*
 * anomaly_detection_util.cpp
 *
 * Author: 318948809
 */

#include <math.h>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
	float sum = 0; 
    for(int i = 0; i < size; i++) 
        sum = sum + x[i]; 
    return sum / size;
	}

// returns the variance of X and Y
float var(float* x, int size){
	float u=avg(x,size),sum=0;
    for (int i = 0; i < size; i++)
    {
        sum=sum+(x[i]*x[i]);
    }
	return (sum/size)-(u*u);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float sum = 0; 
	float ax=avg(x, size);
	float ay=avg(y,size);
    for(int i = 0; i < size; i++) { 
        sum=sum+ ((x[i] -ax ) * (y[i] -ay )); 
	}
    return sum / size; 
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	float covar=cov(x,y,size);
	float sx=sqrt (var(x,size));
	float sy=sqrt (var(y,size));
	return covar/(sx*sy);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
	float* ax,* ay;
	ax=new float[size];
	ay=new float[size];
	for (int i=0;i<size;i++){
	ax[i]=points[i]->x;
	ay[i]=points[i]->y;
	}
	float avgx,avgy,a,b;
	a=(cov(ax,ay,size)/var(ax,size));
	avgx=avg(ax,size);
	avgy=avg(ay,size);
	b=avgy-(a*avgx);
	delete[] ax;
	delete[] ay;
	return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	Line l=linear_reg(points,size);
	float y=l.f(p.x);
	float d=fabs(y-p.y);
	return d;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	float y=l.f(p.x);
	float d=fabs(y-p.y);
	return d;
}




