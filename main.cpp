#include <semaphore.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <list>

using namespace std;
 
sem_t semaphore;

list <string> library;

void writer() {
    for (int i = 0; i < 5; i++) {
        cout << "Writer is now awake and waiting for control.\n";
        sem_wait(&semaphore);        
        sem_wait(&semaphore);
        // We take both points of control, so no readers would be able to interfere.
        cout << "Writer got the control and is now writing something.\n";
        sleep(1);
        // This simulates work.
        library.push_back("A poem\n");
        cout << "Writer wrote something!\n";
        sem_post(&semaphore);
        sem_post(&semaphore);
        // Then, we pass both points, so both readers can start reviewing.
        cout << "Writer passed the control and fell asleep for 3 seconds.\n";
        sleep(3);
    }
}
 
void firstReader() {
    for (int i = 0; i < 5; i++) {
        cout << "First reader is now awake and waiting for control.\n";
        sem_wait(&semaphore);
        cout << "First reader got the control and is now writing a review.\n";
        sleep(1);
        // This simulates work.
        library.push_back("A negative review of the poem\n");
        cout << "First reader wrote a review!\n";
        sem_post(&semaphore);
        cout << "First reader passed the control and fell asleep for two seconds.\n";
        sleep(2);

    }
}

void secondReader() {
    for (int i = 0; i < 5; i++) {
        cout << "Second reader is now awake and waiting for control.\n";
        sem_wait(&semaphore);
        cout << "Second reader got the control and is now writing a review.\n";
        sleep(1);
        // This simulates work.
        library.push_back("A positive review of the poem\n");
        cout << "Second reader wrote a review!\n";
        sem_post(&semaphore);
        cout << "Second reader passed the control and fell asleep for two seconds.\n";
        sleep(2);

    }
}
 
int main() {
    thread thread1 = thread(writer);
    thread thread2 = thread(firstReader);
    thread thread3 = thread(secondReader); 
    // Initiate all the threads.

    sem_init(&semaphore, 0, 2);
    // We need a counter of two to support up to two readers.


    thread1.join();
    thread2.join();
    thread3.join();

    cout << "\ncontents of the library:\n\n";
 
    while(!library.empty()){
      cout << library.front();
      library.pop_front();
    }
    // This way I output the contents of the library to check how threads interacted with it.

    sem_destroy(&semaphore);
    // Garbage collecting.

    return 0;
}