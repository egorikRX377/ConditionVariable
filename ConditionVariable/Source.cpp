#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <future>


std::mutex mtx;
std::condition_variable cv;

int balance = 0;



void addMoney(int quantity)
{
	std::unique_lock<std::mutex> ul(mtx);
	balance += quantity;

	std:: cout << "Current balance: " << balance << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	cv.notify_one();
	return;
}


void withdrawMoney(int quantity)
{
	std::unique_lock<std::mutex> ul(mtx);
	cv.wait(ul, [](){ return (balance != 0) ? true : false; });
	if (balance < quantity)
	{
		std::cout << "You cannot withdraw that much money!" << std::endl;
		return;
	}
	balance -= quantity;

	std::cout << "Current balance: " << balance << std::endl;

	return;
}


int main()
{
	std::thread th1(withdrawMoney, 400);
	std::thread th3(withdrawMoney, 600);

	std::thread th2(addMoney, 600);

	th1.join();
	th2.join();
	th3.join();

	return 0;
}