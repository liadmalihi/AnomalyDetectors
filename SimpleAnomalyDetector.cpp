/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: 318948809 Liad Malihi
 */
#include "SimpleAnomalyDetector.h"
#include "timeseries.h"
#include "anomaly_detection_util.h"
#include <iterator>
#include <math.h>
#include <cmath>
#include <iostream>
#include "minCircle.h"



SimpleAnomalyDetector::SimpleAnomalyDetector() {
	myThreshold=0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

// create array of points through 2 vectors of float num
Point** SimpleAnomalyDetector::points(vector<float> x, vector<float> y){
	Point** p=new Point*[x.size()];
	for(size_t i=0;i<x.size();i++){
		p[i]=new Point(x[i],y[i]);
	}
	return p;
}
// delete the array points
void SimpleAnomalyDetector::delete_points(Point** p,int size){
	for (int l=0;l<size;l++){
			delete p[l];
		}
		delete[] p;
}

// find the max devision-threshold through array of points and regression line
float SimpleAnomalyDetector::findThreshold(Point** p,size_t len,Line line){
	float threshold=0,x,y,d;
	for(int l=0;l<len;l++){
		x=p[l]->x;
		y=p[l]->y;
		d=dev(Point(x,y),line);
		if(threshold<d){
			threshold=d;
		}
	}
	return threshold;
}

//The function receives a TimeSeries variable - it learns and for each pair of columns that are correlated
// with each other it maintains:feature1,feature2,lin_reg,corrlation,threshold and add to struct correlatedFeatures
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	vector<string> v=ts.getKey();
	int sizeOfKey=v.size();
	int sizeOfValue=ts.Vector(v.at(0)).size();
	string k1,k2;
	correlatedFeatures c_t;
	vector<float> v1,v2;
	// array of value in vector
	float vals[v.size()][sizeOfValue];
	for(int i=0;i<sizeOfKey;i++){
		for(int j=0;j<sizeOfValue;j++){
			vals[i][j]=ts.Vector(v[i])[j];
		}
	}
	for (int i=0;i<sizeOfKey;i++){

		float cor,corrl=0,threshold1=0.0,d;
		string feature1,feature2;
		k1=v.at(i); // key-feature
		v1=ts.Vector(k1); // value in feature
		
		for (int j=i+1;j<sizeOfKey;j++){
			k2=v.at(j); // key2-feature
			v2=ts.Vector(k2); // value in feature
			cor=fabs(pearson(vals[i],vals[j],sizeOfValue)); //Pearson correlation
			// save the big corrlation between 2 featurs
			if(cor>corrl){
				corrl=cor;
				feature1=k1;
				feature2=k2;
			}
		}
		if (corrl!=0){
			int exist=0;
			// check if this feature is exist in cf struct
			for(int ex=0;ex<cf.size();ex++){
				if(cf.at(ex).feature2==feature1){
					exist=1;
				}
			}
			if (exist==0){
				learnNormal_helper(feature1,feature2,corrl,ts.Vector(feature1),ts.Vector(feature2),v1.size());
			}
		}
	//return;
	}
}

// the function check if the corrlation biger from threshold- if yes, create new correlated feature and push to correlatedFeature struct
void SimpleAnomalyDetector::learnNormal_helper(string feature1,string feature2,float corrl,vector<float> vector_f1, vector<float> vector_f2,int size){
	correlatedFeatures c_t;
	if(corrl>=myThreshold){
		Point** p=points(vector_f1,vector_f2);
		c_t.feature1=feature1;
		c_t.feature2=feature2;
		c_t.corrlation=corrl;
		c_t.lin_reg=linear_reg(p,size);
		c_t.threshold=findThreshold(p,size,c_t.lin_reg)*(1.1);
		cf.push_back(c_t);
		delete_points(p,size);
	}
}

//The function receives a TimeSeries-variable and returns a list of reports - each report has a description and time point
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	string f1,f2;
	vector<float> v1,v2;
	vector<AnomalyReport> v;
	float d;
	bool ok;
	for(int i=0;i<cf.size();i++){
		f1=cf.at(i).feature1;
		f2=cf.at(i).feature2;
		v1=ts.Vector(f1);
		v2=ts.Vector(f2);
		int size=v1.size();
		for(int c=0;c<size;c++){ 
			ok=isAnomalous(v1.at(c),v2.at(c),cf.at(i)); // check if the point exceeding or in range
			if(!ok){
				AnomalyReport a_r=AnomalyReport(f1+"-"+f2,c+1);
				v.push_back(a_r); // push to anomaly report
			}

		}
	}
	return v;
}

// check if the point exceeding or in range
int SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
	if(c.threshold>dev(Point(x,y),c.lin_reg)){
		return 1;
	}
	return 0;
}

float SimpleAnomalyDetector::getMyThreshold(){
	return myThreshold;
}

void SimpleAnomalyDetector::setMyThreshold(float newT){
	this->myThreshold=newT;
}