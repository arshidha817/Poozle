//implementing merge_sort multithreaded and singlethreaded
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>
#include <random>
#include <chrono>
#include <fstream>

auto merge_func (std::vector<int>& left, std::vector<int>& right){
    std::vector <int> resultant_vec;
    int i = 0, j = 0;
    while(i < left.size() && j < right.size()){
        if(left[i] < right[j]){
            resultant_vec.push_back(left[i++]);
        }
        else{
            resultant_vec.push_back(right[j++]);
        }
    }
    while(i<left.size()){
        resultant_vec.push_back(left[i++]);
    }
    while(j<right.size()){
        resultant_vec.push_back(right[j++]);
    }
return(resultant_vec);
}

void func_singlethread (std::vector<int>& vec){
    //base condn
    if (vec.size() <= 1){
        return;
    }        

    //dividing the array
    std::vector<int> left_vec;
    std::vector<int> right_vec;
    int middle_index = vec.size() / 2;

    left_vec.insert(left_vec.begin(), vec.begin(), vec.begin() + middle_index);
    right_vec.insert(right_vec.begin(), vec.begin() + middle_index, vec.end());

    func_singlethread(left_vec);
    func_singlethread(right_vec);
    vec = merge_func(left_vec, right_vec);

}

void func_multithread (std::vector<int>& vec, int level = 0){      //recursive function
    //"level" depicts the depth of each recursive step
    //base condn
    if (vec.size() <= 1){
        return;
    }        

    //dividing the array
    std::vector<int> left_vec;
    std::vector<int> right_vec;
    int middle_index = vec.size() / 2;

    left_vec.insert(left_vec.begin(), vec.begin(), vec.begin() + middle_index);
    right_vec.insert(right_vec.begin(), vec.begin() + middle_index, vec.end());

    const int max_level = 3;        //just to prevent spawning of too many threads; e.g., if 2 max_level --> 2^0 + 2^1 = 3 max_threads

    if(level < max_level){
        std::thread t1(func_multithread, std::ref(left_vec), level++);     //spawning threads; O(2^n) i.e. 2 threads per recursion call
        std::thread t2(func_multithread, std::ref(right_vec), level++);
        
        t1.join();
        t2.join();
    }

    else{
        func_multithread(left_vec, level++);
        func_multithread(right_vec, level++);
    }

    vec = merge_func(left_vec, right_vec);

}

void generate_random_array(std::vector<int>& arr, int size, int min_val = 0, int max_val = 1000000) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min_val, max_val);
    
    arr.resize(size);
    for (int& x : arr) {
        x = distrib(gen);
    }
}

int main(){
    
    int num_elements = 0;
    std::vector<int> initial_vec;
    
    std::cout << "Enter the number of elements or Array size\n";
    std::cin >> num_elements;
    
    generate_random_array(initial_vec, num_elements);       //to generate random input with varying array sizes
    
    std::vector<int> vec_multi = initial_vec;               //a separate copy of input is created, so that both the funcs get the same unsorted data

    //run singlethreaded merge sort
    auto start1 = std::chrono::high_resolution_clock::now();        
    func_singlethread(initial_vec);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time1 = end1 - start1;
    
    //run multithreaded merge sort
    auto start2 = std::chrono::high_resolution_clock::now();        
    func_multithread(vec_multi);
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time2 = end2 - start2;

    std::ofstream outFile("benchmark_file.csv", std::ios::app);     //storing the benchmark results

    auto is_file_empty = [](const std::string& filename){
        std::ifstream inFile(filename, std::ios::ate);
        return inFile.tellg() == 0;
    };

    if(is_file_empty("benchmark_file.csv")){
        outFile <<  "ArraySize,SingleThreadedTime,MultiThreadedTime\n";
    }

    outFile << num_elements << "," << time1.count() << "," << time2.count() << '\n';
    outFile.close();
    std::cout << "Benchmark results saved to benchmark_file.csv\n";

    return 0;
}