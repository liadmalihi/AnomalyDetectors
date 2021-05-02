/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 318948809 Liad Malihi
 */
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub	

}

void HybridAnomalyDetector::learnNormal_helper(string feature1,string feature2,float corrl,vector<float> vector_f1, vector<float> vector_f2,int size){
	// go to Simple anomaly detector
	if (corrl>= myThreshold){
		SimpleAnomalyDetector::learnNormal_helper(feature1,feature2,corrl,vector_f1,vector_f2,size);
	}
	else if(corrl>=0.5){
		correlatedFeatures c_t;
		Point** p=points(vector_f1,vector_f2);
		c_t.feature1=feature1;
		c_t.feature2=feature2;
		c_t.corrlation=corrl;
		c_t.lin_reg=linear_reg(p,size);
		Circle c=findMinCircle(p,size);
		c_t.threshold=c.radius*1.1;
		c_t.x=c.center.x;
		c_t.y=c.center.y;
		cf.push_back(c_t);
		delete_points(p,size);
	}
} 

// check if the point exceeding or in range
int HybridAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c){
	// exceeding the circle
	if(c.corrlation<myThreshold){
		Point center=Point(c.x,c.y);			
		if(c.threshold>theDis(Point(x,y),center)){
			return 1;
		}
		return 0;
	}
	// exceeding the Simple Anomaly Detector
	return SimpleAnomalyDetector::isAnomalous(x,y,c);
}
HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

