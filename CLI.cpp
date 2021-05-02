/*
 * CLI.cpp
 *
 * Author: 318948809
 */
#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio=dio;
}

void CLI::start(){
    myData data;
    // create vector of commands
    UploadTS uploadTS=UploadTS(this->dio,&data);
    commands.push_back(&uploadTS);
    AlgoSetting algoSet=AlgoSetting(this->dio,&data);
    commands.push_back(&algoSet);
    DetectAnomalies detectA=DetectAnomalies(this->dio,&data);
    commands.push_back(&detectA);
    DisplayResult display=DisplayResult(this->dio,&data);
    commands.push_back(&display);
    UploadAnomaliesAnalyze uploadA=UploadAnomaliesAnalyze(this->dio,&data);
    commands.push_back(&uploadA);
    Exit exit=Exit(this->dio,&data);
    commands.push_back(&exit);

    int opt=0;
    // keep until opt=6
    while(opt!=6){
        //Print the menu
        string startStr="Welcome to the Anomaly Detection Server.\nPlease choose an option:\n";
        this->dio->write(startStr);
        int size=commands.size();
        for(int i=0;i<size;++i){
            commands.at(i)->printDescription();
        }
        // reat the option from the client
        opt=stoi(this->dio->read());
        commands.at(opt-1)->execute(); // go to the ececute according the opt
    } 
}


CLI::~CLI() {
}

