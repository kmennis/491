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
    // cout << "ARGC number " << argc << endl;
    // cout << argv[1] << endl;
   // cout << "Command Line given directory " << endl;
    if(argc == 1){
        //Use default directory

    }else{
        //Use Given Directory
        theDirectory = argv[1];



    }
    hFind = FindFirstFile(theDirectory.c_str(), &ffd);
   // cout << "The first file " << ffd.cFileName << endl;
   // cout << "I will use the " << theDirectory << " directory " << endl;
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
                //                cout << "Extension: " << ext << endl;
                //                cout << "I am the file "<< fileName << endl;
                //                cout << theDirectory << "/" << ffd.cFileName << endl;
                //Find entropy

                //   ifstream content(fileName.c_str(), ifstream::binary);
                ifstream content;
                fullPath = theDirectory.substr(0, theDirectory.size()-1);
                fullPath = fullPath+fileName;
                content.open(fullPath.c_str(), ifstream::binary);
                char c;
                if(content == NULL){
                    //perror("therror");
                    //cout << "Piece of shit" << endl;
                    //cout << "fail " <<  content.fail() << endl;
                    //cout << content.rdstate() << endl;
                    //cout << "bad " << content.bad() << endl;
                    //cout << "good " << content.good() << endl;
                    return 0;
                }
                //               fileLen = content.tellg();
                //                cout << "The content" << endl;
                while(content.get(c)){
                    //   fileLen++;
                    //cout << "am i effing here " << endl;
                    theChars[c] = theChars[c] + 1.0;
                    if(isprint(c)){
                        //cout << c << endl;
                    }
                }
                content.close();

                fileLen = ffd.nFileSizeLow;
                //Finish Calculations
                //        cout << "FileLength " << fileLen << endl;
                for( map<char,double>::iterator it=theChars.begin(); it!=theChars.end() ; ++it){
                    //                    checkLen += it->second;
                    //                    printf("Character: %c \n", it->first);
                    // printf("Freq/filelen: %f", it->second);
                    // printf(" /  %d \n" , fileLen);
                    //137160.00
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
   // printf("exe number: %lf \n", numExe);
   // printf("dll number: %lf \n", numDll);
   // printf("total exe: %lf \n", avgExe);
   // printf("total dll: %lf \n", avgDll);
   // printf("Final exe: %lf \n", xAvg);
   // printf("final dll: %lf \n", dAvg);
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
