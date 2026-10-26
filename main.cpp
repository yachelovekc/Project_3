#include <iostream>
#include <syncstream>
#include <thread>
#include <latch>
#include <chrono>

using namespace std;

latch latch_a(2);
latch latch_bd(4);
latch latch_cg(4);
latch latch_k(2);
latch latch_eh(4);
latch latch_ji(4);

void f(char x, int i) {
    this_thread::sleep_for(chrono::seconds(1));
    osyncstream(cout) << "З набору " << x << " виконано дію " << i << ".\n";
}

void worker1() {
    for (int i = 1; i <= 4; ++i) f('a', i);
    latch_a.count_down();

    for (int i = 2; i <= 3; ++i) f('d', i);
    latch_bd.count_down();

    latch_a.wait();
    for (int i = 3; i <= 4; ++i) f('c', i);
    latch_cg.count_down();

    for (int i = 1; i <= 4; ++i) f('e', i);
    latch_eh.count_down();

    latch_eh.wait();
    for (int i = 1; i <= 2; ++i) f('i', i);
    latch_ji.count_down();

    latch_k.wait();
    for (int i = 1; i <= 3; ++i) f('f', i);
}

void worker2() {
    for (int i = 5; i <= 7; ++i) f('a', i);
    latch_a.count_down();

    f('d', 1);
    for (int i = 4; i <= 5; ++i) f('d', i);
    latch_bd.count_down();

    latch_a.wait();
    for (int i = 5; i <= 6; ++i) f('c', i);
    latch_cg.count_down();

    latch_bd.wait();
    for (int i = 1; i <= 4; ++i) f('h', i);
    latch_eh.count_down();

    latch_eh.wait();
    for (int i = 3; i <= 5; ++i) f('i', i);
    latch_ji.count_down();

    latch_ji.wait();
    for (int i = 1; i <= 2; ++i) f('n', i);
}

void worker3() {
    for (int i = 1; i <= 4; ++i) f('b', i);

    for (int i = 6; i <= 7; ++i) f('d', i);
    latch_bd.count_down();

    latch_bd.wait();
    for (int i = 1; i <= 3; ++i) f('g', i);
    latch_cg.count_down();

    latch_cg.wait();
    for (int i = 1; i <= 2; ++i) f('k', i);
    latch_k.count_down();

    latch_a.wait();
    f('e', 5);
    latch_eh.count_down();

    for (int i = 2; i <= 3; ++i) f('j', i);
    latch_ji.count_down();

    latch_k.wait();
    for (int i = 4; i <= 5; ++i) f('f', i);

    latch_ji.wait();
    f('n', 3);
}

void worker4() {
    for (int i = 5; i <= 8; ++i) f('b', i);
    latch_bd.count_down();

    latch_a.wait();
    for (int i = 1; i <= 2; ++i) f('c', i);

    latch_bd.wait();
    for (int i = 4; i <= 6; ++i) f('g', i);
    latch_cg.count_down();

    latch_cg.wait();
    f('k', 3);
    latch_k.count_down();

    f('e', 6);
    latch_eh.count_down();

    f('j', 1);
    for (int i = 4; i <= 5; ++i) f('j', i);
    latch_ji.count_down();

    latch_k.wait();
    for (int i = 6; i <= 8; ++i) f('f', i);
}

int main() {
    osyncstream(cout) << "Обчислення розпочато.\n";

    auto start_time = chrono::steady_clock::now();

    {
        jthread t1(worker1);
        jthread t2(worker2);
        jthread t3(worker3);
        jthread t4(worker4);
    }

    auto end_time = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();

    osyncstream(cout) << "Обчислення завершено. Тривалість обчислення " << duration << " секунд.\n";

    return 0;
}
