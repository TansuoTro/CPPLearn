#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <cmath> 
#include <vector>
#include <cstdlib>
using namespace std; 

//VECTOR
double caculate(int v)
{
    if (v <= 0) {
        return v;
    }
    this_thread::sleep_for(chrono::milliseconds(10));
    return sqrt((v * v + sqrt((v - 5) * (v + 2.5)) / 2.0) / v); 
}

template<typename Iter, typename Fun>
double visitRange(thread::id id, Iter iterBegin, Iter iterEnd, Fun func)
{
    auto curId = this_thread::get_id();
    if (id == this_thread::get_id()) {
        cout << curId << " main thread\n";
    }
    else {
        cout << curId << " work thread\n";
    } 
    double v = 0;
    for (auto iter = iterBegin; iter != iterEnd; ++iter) {
        v += func(*iter);
    }
    return v;
}

int main()
{
    auto mainThreadId = std::this_thread::get_id();

    std::vector<double> v;
    for (int i = 0; i < 1000; i++) 
    {
        v.push_back(rand());
    } 
    cout << v.size() << endl;
     double value = 0.0;  
    auto st = clock();        
    for (auto & info : v)
    {
        value += caculate(info);        
    }
    auto ed = clock();
    cout << "single thread: " << value << " " << ed - st << "time" << endl;
    

    auto iterMid = v.begin() + (v.size() / 2);    
    double anotherv = 0.0;
    auto iterEnd = v.end();
    st = clock();    
    
    std::thread s([&anotherv, mainThreadId, iterMid, iterEnd]() {      
        anotherv = visitRange(mainThreadId, iterMid, iterEnd, caculate);
        
    });

    auto halfv = visitRange(mainThreadId, v.begin(), iterMid, caculate);


    s.join(); 
    
    ed = clock();
    cout << "multithreaded: " << (halfv + anotherv) << " " << ed - st << "time" << endl;
    
    return 0;
}
