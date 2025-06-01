//final code after implementing multi-threading
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <thread>

int main(int argc, char* argv[]){

    auto start = std::chrono::high_resolution_clock::now();     //timer start; to measure runtime

    std::ifstream inFile(argv[1]);
    
    if(!inFile.is_open()){      //check if file exists
        std::cerr << "Failed to open file ";
        return 1;
    }

    std::vector<std::string> vec;      //reading from inFile and storing it in a vector
    std::string word;
    while(inFile >> word){
        vec.push_back(word);         
    }

    inFile.close();

    std::string pattern;        //get the pattern to be matched
    std::cout << "Enter the pattern to be matched: ";
    std::cin >> pattern;

    std::mutex mtx;
    std::vector<std::string> result;

    const int number_of_threads = 4;        //this is just taken as an example; can be changed on requirement
    size_t total_size = vec.size();
    size_t chunk_size = total_size / number_of_threads;         //Dividing the entire word list into chunks
    std::vector<std::thread> threads;       //this vector is to call & manage all threads at once           

    auto local_find = [&](size_t start, size_t end) {       //a lambda f() to search locally within each chunk
        std::vector<std::string> local_result;
        for(size_t i = start; i < end; i++){        
            if(vec[i].find(pattern) != std::string::npos){
                local_result.push_back(vec[i]);
            }
        }

        {
        std::lock_guard<std::mutex> lock(mtx);      
        result.insert(result.end(), local_result.begin(), local_result.end());      //Inserting the matches found from each chunk to the "result" vector
        }
    
    };

    for(int i=0; i < number_of_threads; i++){       //indexing to know where each chunk starts and ends
        size_t start_index = i * chunk_size;
        size_t end_index = (i == number_of_threads - 1) ? total_size : (i+1) * chunk_size;
        threads.emplace_back(local_find, start_index, end_index);
    }

    for(auto& t : threads){     
        t.join();
    }

    std::cout << "Number of matches found: " << result.size() <<std::endl;

    for(const std::string& q : result){
        std::cout << "Pattern found in " << q << '\n';
    }

    if(result.empty()){
        std::cout << "No matches found" << std::endl;
    }
        
    auto end = std::chrono::high_resolution_clock::now();       // stop timer; to measure runtime
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Search runtime: " << duration << " ms" << std::endl;     //print runtime

    return 0;
}

