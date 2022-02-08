#include <stdio.h>
#include<iostream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <chrono>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
using namespace std;

//Adhrik Chethan
vector<string> tokenize(string s, char del = ' ', bool removeQuotes = false)//splits string to vector
{
    vector<string>tokenValues;
    string temp = "";
    int quote = 0;
    for(int i = 0; i<s.length(); i++)
    {
        if(s[i] == '\"' || s[i] == '\'')
        {
            quote+=1;
            
        }
        if(s[i] == del && quote % 2 == 0)
        {
            //cout<<"STRING VAL    "<<temp;
            tokenValues.push_back(temp);
            temp = "";
        }
        else
        {
            temp+=s[i];
        }
    }
    if(temp != "")
    {
        tokenValues.push_back(temp);
    }
    //cout<<"SZE              "<<tokenValues.size();
    if(removeQuotes == true)
    {
            for(int i = 0; i<tokenValues.size(); i++)
            {
                //cout<<"STATE "<<tokenValues[i]<<endl;
                string s = tokenValues[i];
                if(s[0] == '\"')
                {
                    s.erase(remove(s.begin(), s.end(), '\"'));
                    tokenValues.erase(tokenValues.begin()+i);
                    tokenValues.push_back(s);
                }
                if(s[0] == '\'')
                {
                    s.erase(remove(s.begin(), s.end(), '\''));
                    tokenValues.erase(tokenValues.begin()+i);
                    tokenValues.push_back(s);
                }
                if(s[s.length()-1] == '\"')
                {
                    s.erase(remove(s.begin(), s.end(), '\"'));
                    tokenValues.erase(tokenValues.begin()+i);
                    tokenValues.push_back(s);
                }
                if(s[s.length() - 1] == '\'')
                {
                    s.erase(remove(s.begin(), s.end(), '\''));
                    tokenValues.erase(tokenValues.begin()+i);
                    tokenValues.push_back(s);
                }

            }
    }
    
    
    return tokenValues;
    /*int start = 0;
    int end = s.find(del);
    while (end != -1) {
       // cout << s.substr(start, end - start) << endl;
        tokenValues.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }

    //cout << s.substr(start, end - start);
    tokenValues.push_back(s.substr(start, end - start));
    return tokenValues;*/
}

vector<string> slice(vector<string>const &v, int m, int n)//makes subArray from Array
{
    auto first = v.cbegin() + m;
    auto last = v.cbegin()+n+1;
    vector<string>vec(n-m+1);
    copy(first, last, vec.begin());
    return vec;

}

string trim(string input)
{
    int i = 0;
    while (i < input.size() && input[i] == ' ')
    {
        i++;
    }
    if(i < input.size())
    {
        input = input.substr(i);
    }
    else
    {
        return "";
    }

    i = input.size() - 1;
    while(i > 0 && input[i] == ' ')
    {
        i--;
    }
    if(i >= 0)
    {
        input = input.substr(0, i+1);
    }
    else
    {
        return "";
    }
    return input;
    
}

char**vec_to_char_array(vector<string>&parts)
{
     char**result = new char*[parts.size()+1];
     for(int i = 0; i<parts.size(); i++)
     {
         result[i] = (char*)parts[i].c_str();
     }
     result[parts.size()] = NULL;
     return result;
}

int numberOfDelimeters(string inputLine, string del = " ")//counts number of occurances of delimeter
{
    int count = 0;
    for(int i = 0; i<inputLine.length(); i++)
    {
        if(inputLine[i] == del.c_str()[0])
        {
            count++;
        }
    }
    return count;
}

void execute(string inputline)
{
            inputline = trim(inputline);
            //cout<<inputline;
            vector<string>cVals = tokenize(inputline, ' ', true);
            /*for(int i = 0; i<cVals.size(); i++)
            {
                cout<<cVals[i]<<endl;
            }*/
            string writeTo = ">";
            string readFrom = "<";
            string oR = "|";
            
            
           
            
            
            if(cVals.size() > 1)
            {
                
                if(inputline.find('<') != std::string::npos  || inputline.find('>') != std::string::npos && inputline.find("echo") == std::string::npos)
                {
                     int beginIndex = 0;
                     //int greaterThanIndex = 0;
                     //int lessThanIndex = 0;
                     int fd1 = 0;
                     int fd2 = 1;
                     int indexToStopSlicing = inputline.length()-1;
                     for(int i = 0; i<inputline.length(); i++)
                     {
                         //cout<<"CHARACTER  "<<inputline[i];
                         if(inputline[i] == '>')
                         {
                            
                            //string fName = cVals[i+1];
                            vector<string>newVec = tokenize(inputline, '>', true);
                           /* for(int i = 0; i<newVec.size(); i++)
                            {
                                cout<<"DDD "<<newVec[i]<<endl;
                            }*/
                            string fName = newVec[1];
                            fName = trim(fName);
                            //cout<<"FNAME!   "<<fName;
                            fd1 = open(fName.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
                            dup2(fd1, 1);
                            if(i< indexToStopSlicing)
                            {
                                indexToStopSlicing = i;
                            }
                           // close(fd1);
                           
                         }
                         if(inputline[i] == '<')
                         {
                               
                            vector<string>newVec = tokenize(inputline, '<', false);
                            /*for(int i = 0; i<newVec.size(); i++)
                            {
                                cout<<"DDD "<<newVec[i]<<endl;
                            }*/
                            vector<string>newerVec = tokenize(newVec[1], '>', false);
                            string fName = newerVec[0];
                           // cout<<"FNAME  "<<fName;
                            fName = trim(fName);

                            //string fName = cVals[i+1];
                            fd2 = open(fName.c_str(), O_RDONLY, NULL);
                            //cout<<"FNAME "<<fName<<endl;
                            dup2(fd2, 0);
                            
                            if(i< indexToStopSlicing)
                            {
                                indexToStopSlicing = i;
                                //cout<<"INDEX  "<<indexToStopSlicing<<endl;
                            }
                         }
                         
                     }
                     string newSubstring = inputline.substr(0, indexToStopSlicing);
                     trim(newSubstring);
                     //cout<<"SSSSS"<<newSubstring<<endl;
                     vector<string>slicedVector = tokenize(newSubstring, ' ', true);
                     /*for(int i = 0; i<slicedVector.size(); i++)
                     {
                         //cout<<"SSSSS       "<<slicedVector[i]<<endl;
                     }*/
                     char**args = vec_to_char_array(slicedVector);
                     execvp(args[0], args);

                }
                else//basic commands
                {
                   // cout<<"IN STATEMENT"<<endl;
                    char**args = vec_to_char_array(cVals);
                    execvp(args[0], args);
                }
                //char**args = vec_to_char_array(cVals);
                //execvp(args[0], args);

            }
            else
            {
                char* args [] = {(char *) inputline.c_str(), NULL};  
                execvp (args [0], args);
            }
}



int main ()
{
    time_t currTime;
    time(&currTime);
    vector<int>bgs;//vector of background processes
    //vector<string>c = tokenize(s, "|");
    //cout<<"SIZEE                     "<<c.size();
    while (true)
    {
        for(int i = 0; i<bgs.size(); i++)
        {
            if(waitpid(bgs[i], 0, WNOHANG) == bgs[i])
            {
                bgs.erase(bgs.begin()+i);
                i--;
            }
        }
        
        
        cout << getenv("USER")<<":"<<ctime(&currTime);
        string inputline;
        getline (cin, inputline); 
        bool bg = false;
        if(inputline[inputline.length()-1] == '&')
        {
            bg = true;
            inputline = inputline.substr(0, inputline.length() - 1);
            //cout<<"INPUT LINE: "<<inputline<<endl;
        }
        inputline = trim(inputline);  // get a line from standard input
       // cout<<"INPUTLINE        "<<inputline.length()<<endl;
       inputline = trim(inputline);
       if(inputline.find("cd") != std::string::npos)
       {
           vector<string>cds = tokenize(inputline, ' ', false);
           string directory = cds[1];
           trim(directory);
           //cout<<"DIRECTORY"<<directory<<endl;
           if(directory == "-")
           {
               chdir("../");
           }
           else
           {
              chdir(directory.c_str());
           }
           

       }
       /*if(inputline.find("mkdir") != std::string::npos)
       {
           vector<string>cds = tokenize(inputline, ' ', false);
           string directory = cds[1];
           if((mkdir(directory.c_str(), 0777) != 0))
           {
               //cout<<"Could Not Create Directory"<<endl;
               continue;
           }
       }*/
        vector<string>c = tokenize(inputline, '|', false);
       // cout<<c.size()<<endl;
       /*for(int i = 0; i< c.size(); i++)
       {
           cout<<"CALUE"<<c[i]<<endl;
       }*/
       
        if (inputline == string("exit"))
        {
            cout << "Bye!! End of shell" << endl;
            break;
        }

        int inpfd = dup(0);
        int outfd = dup(1);
        for(int i = 0; i<c.size(); i++)
        {
            int fd[2];
            pipe(fd);
            
            int pid = fork ();
            
            
            if (pid == 0)
            { //child process
            // preparing the input command for execution
               if(i < c.size() - 1)
               {
                   dup2(fd[1], 1);
               }
               close(fd[0]);
            
            
               execute(c[i]);
            
            //char* args [] = {(char *) inputline.c_str(), NULL};  
            //execvp (args [0], args);
            }
            else
            {
                if(i == c.size() - 1 && (!bg))
                {
                    waitpid(pid, 0, 0);
                    
                }
                else
                {
                    bgs.push_back(bg);
                }
             // waitpid (pid, 0, 0); // wait for the child process 
            // we will discuss why waitpid() is preferred over wait()
            }
            dup2(fd[0], 0);
            close(fd[1]);
        }
        dup2(inpfd, 0);
        close(outfd);
        
    }
}