#include <iostream>
#include <thread>
#include <chrono>


void sleep(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void withdraw(int& balance, int amt) {
    int curr_bal = balance;
    curr_bal -= amt;
    sleep(1);
    balance = curr_bal;
}

void deposit(int& balance, int amt) {
    int curr_bal = balance;
    curr_bal += amt;
    sleep(1);
    balance = curr_bal;
}

int main(int argc, char* argv[]) {
    int balance = 500;

    std::thread t1 = std::thread(deposit, std::ref(balance), 200);
    std::thread t2 = std::thread(withdraw, std::ref(balance), 600);

    t1.join();
    t2.join();

    std::cout << balance << std::endl;

    return 0;
}