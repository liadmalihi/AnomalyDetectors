/*
 * timeseries.h
 *
 * Author: 318948809 Liad Malihi
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include <map>
#include<string>
using namespace std;


class TimeSeries{    
    map<string, vector<float> > m_myMap;
    vector<string> m_key;
public:
    
	TimeSeries(const char* CSVfileName);
    vector<string> MyKey (const char* CSVfileName);
    int size() const;
    vector<float> Vector(string key) const;
    vector<string> getKey() const;


};



#endif /* TIMESERIES_H_ */
