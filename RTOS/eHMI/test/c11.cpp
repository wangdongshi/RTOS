#include <iostream>
#include <thread>
 
using namespace std;
 
void myFirstThread()
{
         cout << "Hello thread" << endl;
}
 
int main()
{
         thread myThread(myFirstThread);
         myThread.join();
         return 0;
}
