#include "ThreadSafeQueue.h"
#include <stdint.h>
#include <unistd.h>
#include <thread>

ThreadSafeQueue<int> q(10);
uint16_t count = 10000;
bool dismiss = false;

void Producer() {
    while(count > 0) {
        printf("%s product %d\n", __FUNCTION__, count);
        q.push(count--);
        if(count < 10000 - 10)
            usleep(1000000);
    }
}

void ConsumerA() {
    while(!dismiss) {
        printf("%s get: %d\n", __FUNCTION__, q.front());
        q.pop();
        usleep(500000);
    }
}

void ConsumerB() {
    while(!dismiss) {
        printf("%s get: %d\n", __FUNCTION__, q.front());
        q.pop();
        usleep(750000);
    }
}

int main() {
    std::thread producer(Producer);
    std::thread consumerA(ConsumerA);
    std::thread consumerB(ConsumerB);

    producer.join();
    dismiss = true;
    consumerA.join();
    consumerB.join();
    return 0;
}

