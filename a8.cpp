//one test cases pending
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <regex>
using namespace std;

bool is_number(const std::string& s) {
    std::regex pattern("^-?\\d+(\\.\\d+)?$");
    return std::regex_match(s, pattern);
}

int main(){
    
    int no_of_lines;
    cin >> no_of_lines;
    
    cin.ignore();

    bool mul_start = true;
    bool add_start = true;
    int result = 0;
    string line;
      
    for(int i = 0; i < no_of_lines; i++){
        getline(cin, line);
    
        for(size_t index = 0; index < line.length(); ){
            if(line.compare(index, string("do_mul()").length(), "do_mul()") == 0){
                mul_start = true;
                index += string("do_mul()").length();
            }
            else if(line.compare(index, string("don't_mul()").length(), "don't_mul()") == 0){
                mul_start = false;
                index += string("don't_mul()").length();
            }
            else if(line.compare(index, string("mul(").length(), "mul(") == 0){
                index += string("mul(").length();
                if(mul_start){
                    
                    size_t end = line.find(")", index);
                    if(end != std::string::npos){

                    string inside = line.substr(index, end - index);

                    size_t comma = inside.find(",");

                    if(comma != std::string::npos){
                        string a = inside.substr(0, comma);
                        string b = inside.substr(comma + 1);
                        if (is_number(a) && is_number(b)) {
                            try {
                            result += stoi(a) * stoi(b);
                            } catch (...) {
                                // This block executes if stoi() throws an exception
                            }
                        index = end + 1;                  
                        }

                    }
                    
                    }

                }
            }
            else if(line.compare(index, string("do_add()").length(), "do_add()") == 0){
                add_start = true;
                index += string("do_add()").length();
            }
            else if(line.compare(index, string("don't_add()").length(), "don't_add()") == 0){
                add_start = false;
                index += string("don't_add()").length();
            }
            else if(line.compare(index, string("add(").length(), "add(") == 0){
                index += string("add(").length();
                if(add_start){
                    
                    size_t end = line.find(")", index);
                    if(end != std::string::npos){
                    string inside = line.substr(index, end - index);

                    size_t comma = inside.find(",");

                    if(comma != std::string::npos){
                        string a = inside.substr(0, comma);
                        string b = inside.substr(comma + 1);
                        if (is_number(a) && is_number(b)) {
                            try {
                            result += stoi(a) + stoi(b);
                            } catch (...) {
                                // This block executes if stoi() throws an exception
                            }
                        index = end + 1;
                        }
                    
                    }
                    
                    }
                } 
            }  
            else {
                index ++;
            }
        
        }
    }   

    cout << result;
    return 0;
}
