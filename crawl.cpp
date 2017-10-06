/******************
Kristen Ennis
Malware CMSC 491
Given a directory, this program will crawl each .ddl and .exe in
the directory and find its entropy. Then, it will order those files
from bigget to smallest entropy.
******************/
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <map>
#include <math.h>
using namespace std;


int main( int argc, char *argv[], char *envp[]){

    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
//     string theDirectory = "D://Development/441/group3/*";
    string theDirectory = "C://Windows/System32/*";
    string fullPath ;
    string fileName="begin";
    ofstream theFile;
    char fileContent;
    map<char,double> theChars;
    map<double,string> listEnt;
    double freq;
    unsigned long fileLen;
    double ent = 0.0;
    double checkLen;
    double maxEnt = 0.0;
    double minEnt = 0.0;
    double numDll = 0.0 ;
    double numExe =  0.0;
    double avgDll = 0.0;
    double avgExe = 0.0;
    if(argc == 1){
        //Use default directory

    }else{
        //Use Given Directory
        theDirectory = argv[1];



    }
    hFind = FindFirstFile(theDirectory.c_str(), &ffd);
    if( hFind == INVALID_HANDLE_VALUE){
        return 0;
    }

    //Loop Through files
    do{
        if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY == 0){
           // cout << "I am a directory " << ffd.cFileName << endl;
            break;
        }else{

            // fileName = string(ffd.cFileName);
            fileName=string(ffd.cFileName);
            int period = fileName.find_last_of(".");

            string ext = fileName.substr(period +1);
            if( ext == "exe" | ext == "dll"){
                if(ext == "exe"){
                    numExe = numExe + 1.0;
                }else{
                    numDll = numDll +1.0;
                }

                //Find entropy

                //   ifstream content(fileName.c_str(), ifstream::binary);
                ifstream content;
                fullPath = theDirectory.substr(0, theDirectory.size()-1);
                fullPath = fullPath+fileName;
                content.open(fullPath.c_str(), ifstream::binary);
                char c;
                if(content == NULL){
                    //perror("therror");
                  
                    //cout << "fail " <<  content.fail() << endl;
                    //cout << content.rdstate() << endl;
                    //cout << "bad " << content.bad() << endl;
                    //cout << "good " << content.good() << endl;
                    return 0;
                }
                //               fileLen = content.tellg();
                
                while(content.get(c)){
                 
                    theChars[c] = theChars[c] + 1.0;
                    if(isprint(c)){
                        //cout << c << endl;
                    }
                }
                content.close();

                fileLen = ffd.nFileSizeLow;
                //Finish Calculations
                for( map<char,double>::iterator it=theChars.begin(); it!=theChars.end() ; ++it){
                    
                    //137160.00
                    // freq/filelen
                    freq = (1.0*it->second) / double(fileLen);
                    // cout << setprecision(20) << freq << endl;
                    // printf("== Freq: %lf \n", freq);
                    //    printf("log stuff %lf \n", (log(freq)/log(2)));
                    ent += freq * (log(freq)/log(2));
                    // printf("Ent: %lf \n",ent);
                    //  printf("Ent: %d --- Freq: %d", ent,freq);
                }
                ent = ent * -1.0;
                if(ent > maxEnt){
                    maxEnt = ent;
                }
                if(minEnt == 0){
                    minEnt = ent;   
                }else{
                    if(ent < minEnt){
                        minEnt = ent;
                    }
                }
                //Insert into map of all entropies and file names
                listEnt[ent]= fileName;
                //Clear all variables
                theChars.clear();
                if(ext == "exe"){
                    avgExe = avgExe+ ent;
                }else{
                    avgDll = avgDll+ ent;
                }
                freq = 0.0;
                ent = 0.0;

            }
        }

    }while(FindNextFile(hFind, &ffd) != 0 );

    //Close Seek
    FindClose(hFind);
    double tenPerc;
    tenPerc = floor( listEnt.size()*.1);
    //Find top 10 results
    map<double,string>::reverse_iterator rit;
    int toTen = 0;
    double xAvg = avgExe / numExe;
    double dAvg = avgDll / numDll;

    ofstream results("results.txt");
    if(results.is_open()){
        results << "Directory: " << theDirectory << "\n";
        results << "Number of exe: " << numExe << "\n";
        results << "Average entropy for exe: " << xAvg << "\n";
        results << "------------------------------" << " \n";
        results << "Number of dll: " << numDll << "\n";
        results << "Average entropy for dll: " << dAvg << "\n";
        results << "------------------------------" << " \n";
        results << "Overall Entropy Range: " << minEnt << " - " << maxEnt << "\n";
        results << "------------------------------" << " \n";
        results << "10% Highest Entropies------ \n";
    }
    for(  rit=listEnt.rbegin(); rit!=listEnt.rend() && toTen < tenPerc; ++rit){
        toTen++;

        if(results.is_open()){

            results << "Number: " << toTen  << " ----------- \n";
            results << "File Name: " << rit->second << "\n";
            // printf("File Ent: %lf \n", rit->first);
            results << "Entropy: " << rit->first << "\n";
            results << "------------- \n"; 
        }
    }
    results.close();
    return 0;

}
