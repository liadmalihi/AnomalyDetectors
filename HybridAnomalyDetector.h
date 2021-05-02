/*
 * HybridAnomalyDetector.h
 *
 * Author: 318948809 Liad Malihi
 */

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual int isAnomalous(float x, float y,correlatedFeatures c);
	virtual void learnNormal_helper(string feature1,string feature2,float corrl,vector<float> vector_f1, vector<float> vector_f2,int size);
	virtual ~HybridAnomalyDetector();

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
