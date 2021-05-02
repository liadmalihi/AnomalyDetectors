/*
 * SimpleAnomalyDetector.h
 *
 * Author: 318948809 Liad Malihi
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float x;
	float y;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float myThreshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
	float findThreshold(Point** ps,size_t len,Line rl);
	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	Point** points(vector<float> x, vector<float> y);
	virtual int isAnomalous(float x, float y,correlatedFeatures c);
	void delete_points(Point** p,int size);
	virtual void learnNormal_helper(string feature1,string feature2,float corrl,vector<float> vector_f1, vector<float> vector_f2,int size);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
	float getMyThreshold();
	void setMyThreshold(float newT);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
