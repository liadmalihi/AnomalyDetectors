/*
 * timeseries.cpp
 *
 * Author: 318948809 Liad Malihi
 */
#include "timeseries.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iterator>

    
    //the function get type of file and return and create a map
	TimeSeries::TimeSeries(const char* CSVfileName){
        m_key = TimeSeries::MyKey(CSVfileName);
        int size= m_key.size();
        ifstream file;
		file.open(CSVfileName);
        string first_line;
        getline(file,first_line);
		
        vector<float> arr [size];
        while(file.good()){
        string line,new_line; 
        getline(file,line);
        stringstream s2(line);
        if(line!=""){
            for (int i=0;i<size;i++){
                getline(s2,new_line,',');
                arr[i].push_back(stof(new_line));
            }
        }
        }
        file.close();
        vector<float> v;
        for(int i=0;i<size;i++){
            m_myMap.insert(pair <string,vector<float>>(m_key[i],arr[i]));
        } 
    }
    //the function get type of file and return vector of strings-the key in first line
    vector<string> TimeSeries::MyKey(const char* CSVfileName){
        ifstream my_file;
		my_file.open(CSVfileName);
		string key_line;
		if(my_file.is_open()){
			getline(my_file,key_line);
		}
        my_file.close();
        int count=0;	
        stringstream s(key_line);
        vector<string> key;
		while (s.good()) {
            string myKey;
            getline(s,myKey,',');
            key.push_back(myKey); 
		}
        return key;
    }
    //the function return the size of map
    int TimeSeries::size() const{
        return m_myMap.size();

    }
    //the function get key and return the vector value's of this key
    vector<float> TimeSeries::Vector(string key) const{
        return m_myMap.find(key)->second;
    }
    //the function return the key of map
    vector<string> TimeSeries::getKey() const {
        return m_key;
    }

