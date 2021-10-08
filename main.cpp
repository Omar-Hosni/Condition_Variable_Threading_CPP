#include <iostream>
#include <thread>
#include <stack>
#include <mutex>
#include <chrono>
#include <string>
#include <condition_variable>

using namespace std;

condition_variable cv;
mutex m;
int balance = 0;

void addMoney(int money) {
	lock_guard<mutex> lg(m);
	balance += money;
	cout << "Amount Added Current Balance: " << balance << endl;
	cv.notify_one();
}


void withdrawMoney(int money) {
	unique_lock<mutex>ul(m);
	cv.wait(ul, [] {return (balance != 0) ? true : false; });

	if (balance >= money) {
		balance -= money;
		cout << "Ammount Deducted: " << money << endl;

	}
	else {
		cout << "Amount can't be deducted, current balance is less than " << money << endl;
		
	}
	cout << "Current balance is: " << balance << endl;
}


int main() {
	thread t1(withdrawMoney, 500);
	thread t2(addMoney, 500);
	t1.join();
	t2.join();

	return 0;
}
