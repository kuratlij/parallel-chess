//
// Created by jonathan on 11.12.16.
//
#include <atomic>
class read_write_lock {

public:
    read_write_lock() {
        readers = 0;
        write_lock = false;
    }
    void write() {
     //   while (std::atomic::compare_exchange_weak(write_lock,false,true));
        while (readers!=0);
    }
    void writeEnd(){
        write_lock=false;
    }
    void read() {
        readers++;
        while (write_lock);
    }
    void readEnd(){
        readers--;
    }

    std::atomic_int readers;
    std::atomic_bool write_lock;
};
