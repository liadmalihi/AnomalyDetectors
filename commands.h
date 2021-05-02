/*
 * commands.h
 *
 * Author: 318948809
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};

// struct of data
	struct myData{
		TimeSeries* learn_normal;
		TimeSeries* detect;
		HybridAnomalyDetector* hd=new HybridAnomalyDetector();
		vector <AnomalyReport> anomaly_report;
	
    };

class Command{

protected:
	DefaultIO* dio;
	string description;
	myData* data;
public:
	Command(DefaultIO* dio,myData* data):dio(dio),data(data){}
	virtual void execute()=0;
	virtual ~Command(){}
	virtual void printDescription(){
		dio->write(description);
	}
};

// command classes

class UploadTS:public Command{
	public:
		UploadTS(DefaultIO* dio,myData* data):Command(dio,data){
			this->dio=dio;
			this->description="1.upload a time series csv file\n";
		}
		//The server reads 2 files from the client and saves the inputs as a file anomalyTrain.csv/anomalyTest.csv - respectively
		void execute(){
			string startSen="Please upload your local train CSV file.\n";
			dio->write(startSen);
			string text1,text2;
			ofstream train,test;
			train.open("anomalyTrain.csv"); //open csv file
			test.open("anomalyTest.csv"); // open csv file
			text1=this->dio->read(); // reat time series 1
			while(text1!="done"){
				train << text1<< endl;
				text1=this->dio->read();
			}
			// save the Time series csv in learn_normal in struct data
			(data->learn_normal)=new TimeSeries("anomalyTrain.csv");
			train.close(); // close file
			dio->write("Upload complete.\n");

			string secondSen="Please upload your local test CSV file.\n";
			dio->write(secondSen);

			text2=this->dio->read(); // reat time series 1
			while(text2!="done"){
				test << text2<< endl;
				text2=this->dio->read();
			}
			// save the Time series csv in detect in struct data
			(data->detect)=new TimeSeries("anomalyTest.csv"); ;
			test.close(); // close file
			dio->write("Upload complete.\n");
		}
};
class AlgoSetting:public Command{
	public:
		AlgoSetting(DefaultIO* dio,myData* data):Command(dio,data){
			this->description="2.algorithm settings\n";
		}
		//The server will show the client the correlation threshold and the client will be able to replace the threshold - 
		//if he chooses a correct value - between 0 and 1, the threshold will change.
		//If not the server will return an appropriate message.
		void execute(){
			float threshold=data->hd->getMyThreshold();
			dio->write("The current correlation threshold is ");
			dio->write(threshold);
			dio->write("\nType a new threshold\n");
			float opthr=stof(dio->read());
			// if the client choose wrong value
			while (opthr>1||opthr<0){
				dio->write("please choose a value between 0 and 1");
				dio->write("The current correlation threshold is ");
				dio->write(threshold);
				dio->write("\nType a new threshold\n"); // message for client
				opthr=stof(dio->read());
			}
			data->hd->setMyThreshold(opthr); // change threshold

		}
};
class DetectAnomalies:public Command{
	public:
		DetectAnomalies(DefaultIO* dio,myData* data):Command(dio,data){
			this->description="3.detect anomalies\n";
		}
		// the server runs the anomaly detector,learn normal on the saved files
		void execute(){
			data->hd->learnNormal(*data->learn_normal);
			data->anomaly_report=data->hd->detect(*data->detect);
			dio->write("anomaly detection complete.\n");
		}
};
class DisplayResult:public Command{
	public:
		DisplayResult(DefaultIO* dio,myData* data):Command(dio,data){
			this->description="4.display results\n";
		}
		// the server will print the anomaly reports
		void execute(){
			int size=data->anomaly_report.size();
			for (int i=0;i<size;++i){
				dio->write(data->anomaly_report.at(i).timeStep);
				dio->write("	"+data->anomaly_report.at(i).description+"\n");
			}
			dio->write("Done.\n");
		}
};
class UploadAnomaliesAnalyze:public Command{
	public:
		UploadAnomaliesAnalyze(DefaultIO* dio,myData* data):Command(dio,data){
			this->description="5.upload anomalies and analyze results\n";
		}
		// The server will pick up an anomaly file from the client, analyze it and present the results to the client
		void execute(){
			dio->write("Please upload your local anomalies file.\nUpload complete.\n");
			vector <Point> report,deviation;
			float P,N,n,sum=0,num=1;
			float TP=0; // A time frame in which there was an deviation and has a cut greater than 0 with an deviation reporting time
			float FP=0; //Reporting outside of one of the time frames in which there were anomalies
			report=get_report(); //vector of anomaly report
			deviation= get_deviation(); // vector of deviation
			P=deviation.size(); // for positive
			n=data->learn_normal->Vector(data->learn_normal->getKey().at(0)).size(); // num of line in one feature
			for(Point p:deviation){
				sum=sum+p.y-p.x+1;
			}
			N=n-sum; // for negative
			int size_devi=deviation.size();
			int i=0,arr_fn[size_devi]={0},first_tn,second_tn;
			int TN=n; // As long as there was no deviation and no reporting
			int FN=0; // Any time frame in which there was an deviation and no cut if even reported
			int d[size_devi]={0};
			// anomaly report []
			for(Point p_rep:report){
				first_tn=p_rep.x; // the first line in sequence
				second_tn=p_rep.y; // the second line in sequence
				// deviation ()
				i=0;
				for (Point p_devi:deviation){
					// []() 
					if(p_rep.y<p_devi.x){
						break;
					}
					else{
						// [(
						if (p_rep.x<=p_devi.x){
							// [()] / [(])
							if(p_rep.y>=p_devi.x){
								if (arr_fn[i]==0 || d[i]!=1){
									TP++;
									d[i]=1;
								} 
								num=0;
								arr_fn[i]++;
								second_tn=max(p_rep.y,p_devi.y);
							}
						}
						// ([
						else{
							// ([]) / ([)]
							if(p_devi.y>=p_rep.x){
								first_tn=p_devi.x;
								if (arr_fn[i]==0 || d[i]!=1){
									TP++;
									d[i]=1;
								} 
								num=0;
								arr_fn[i]++;
								second_tn=max(p_rep.y,p_devi.y);
							}
						}
					}
					++i;
				}
				TN=TN-second_tn+first_tn-1;
				FP+=num;
				num=1;
			}
			// check if there is an deviation but no report
			for(int fn=0;fn<size_devi;++fn){
				if (arr_fn[fn]==0){
					FN++;
					TN=TN-deviation.at(fn).y+deviation.at(fn).x-1;
				}
			}
			int tp=TP/P*1000;
			int fp=FP/N*1000;
			dio->write("True Positive Rate: ");
			dio->write(((float)tp/1000)); // 3 num after point
			dio->write("\nFalse Positive Rate: ");
			dio->write(((float)fp/1000)); // 3 num after point
			dio->write("\n");

		}
		// check if point 1 < point 2
		static bool sort_timestep (const Point &pt1,const Point &pt2) {
			 return (pt1.x < pt2.x);
		}
		// return vector of deviations
		vector<Point> get_deviation(){
			vector <Point> deviation;
			string devi=dio->read();
			int x,y;
			while(devi!="done"){
				string delimiter = ",";
				x = stoi(devi.substr(0, devi.find(delimiter))); // save the first num
				y = stoi(devi.substr(devi.find(delimiter)+1)); // save the second num
				deviation.push_back(Point(x,y));
				devi=dio->read();
			}
			// sort the vector
			sort(deviation.begin(), deviation.end(),sort_timestep);
			return deviation;
		}
		// return vector of anomaly report timestep
		vector<Point> get_report(){
			vector <Point> report;
			int x,y, size=data->anomaly_report.size();
			for(int i=0;i<size;++i){
				int range=1;
				x=data->anomaly_report.at(i).timeStep;
				y=data->anomaly_report.at(i).timeStep;
				while(range && i<size-1){
					// check for a sequence
					if (data->anomaly_report.at(i+1).timeStep==y+1 
					&& data->anomaly_report.at(i+1).description==data->anomaly_report.at(i).description ){
						y=data->anomaly_report.at(i+1).timeStep;
						++i;
					}
					else{
						range=0;
					}
				}
				report.push_back(Point(x,y));
			}
			// sort the vector
			sort(report.begin(), report.end(),sort_timestep);
			return report;
		}
};
class Exit:public Command{
	public:
		Exit(DefaultIO* dio,myData* data):Command(dio,data){
			this->description="6.exit\n";
		}
		// the interaction between the server and the client will end
		void execute(){
			delete data->hd;
			delete data->learn_normal;
			delete data->detect;
		}
};

#endif /* COMMANDS_H_ */
