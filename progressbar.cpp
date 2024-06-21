#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <chrono>
using namespace std::literals::chrono_literals;


class Progressbar {
    private:
    std::string arrow;
    int start;
    int final;
    int count_operat_;
    std::chrono::duration<double> diff_;
    public:
    Progressbar() {
        arrow = ">";
        start = 0;
        final = 20;
        count_operat_ = 0;
    }
    void print_bar() {
        int bar_step = final / count_operat_;

        for (int i = start; i < count_operat_; i++) {
            for (int j = start; j < bar_step; j++) {
                if (j == bar_step - 1) {
                    std::cout << arrow << "\033[1C" << "\033[1D";
                    std::this_thread::sleep_for(100ms);
                } else {
                    std::cout << "-" << "\033[1C" << "\033[1D";  
                }
            }
        }
    }

    void put_count_operat(int count_operat) {
        this->count_operat_ = count_operat;
    }

    void put_time(std::chrono::duration<double> diff) {
        this->diff_ = diff;
    }

    void print_time() {
        std::cout << diff_.count();
    }

};



std::mutex m;
Progressbar p;

void func1(int num) {
    std::unique_lock<std::mutex> lk(m);
    lk.unlock();
    auto start = std::chrono::steady_clock::now();
    int count_operat = 4;
    for (int i = 0; i < count_operat; i++) {
        num *= 10;
        num *= 2;
        num += 4;
    }
    std::this_thread::sleep_for(100ms);
    lk.lock();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    p.put_time(diff);
    p.put_count_operat(count_operat);
    std::cout << 1 << "\t\t" << std::this_thread::get_id() << "\t\t";
    p.print_bar();
    std::this_thread::sleep_for(100ms);
    std::cout << "\t\t";
    p.print_time();
    std::cout << "\n";

}

void func2(int num) {
    std::unique_lock<std::mutex> lk(m);
    lk.unlock();
    auto start = std::chrono::steady_clock::now();
    int count_operat = 5;
    for (int i = 0; i < count_operat; i++) {
        num += 4;
        num *= 100;
    }
    std::this_thread::sleep_for(100ms);
    lk.lock();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    p.put_time(diff);
    p.put_count_operat(count_operat);
    std::cout << 2 << "\t\t" << std::this_thread::get_id() << "\t\t";
    p.print_bar();
    std::this_thread::sleep_for(100ms);
    std::cout << "\t\t";
    p.print_time();
    std::cout << "\n";
}

void func3(int num) {
    std::unique_lock<std::mutex> lk(m);
    lk.unlock();
    auto start = std::chrono::steady_clock::now();
    int count_operat = 10;
    for (int i = 0; i < count_operat; i++) {
        num /= 2;
        num -= 15;
    }
    std::this_thread::sleep_for(100ms);
    lk.lock();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    p.put_time(diff);
    p.put_count_operat(count_operat);
    std::cout << 3 << "\t\t" << std::this_thread::get_id() << "\t\t";
    p.print_bar();
     std::this_thread::sleep_for(100ms);
    std::cout << "\t\t";
    p.print_time();
    std::cout << "\n";
}






int main(int argc, const char * argv[]) {

    std::cout << "#\t\t" << "id\t\t\t" <<  "Progress bar" << "\t\t\t" << "Time" << "\n";
    
    std::vector<std::thread> v;

    std::thread th1(func1, 3);
    std::thread th2(func2, 4);
    std::thread th3(func3, 50);
    v.push_back(std::move(th1));
    v.push_back(std::move(th2));
    v.push_back(std::move(th3));

    for (auto &el : v) {
        el.join();
    }

    return 0;
}