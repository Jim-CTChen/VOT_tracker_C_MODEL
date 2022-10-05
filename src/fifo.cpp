#include "../inc/memory_controller.h"
#include <vector>

FIFO::FIFO(){
    this->wptr = 0;
    this->rptr = 0;
    this->size = 3;
    this->full = false;
    this->empty = true;
    this->data_buffers = std::vector<std::vector<int>>(3,std::vector<int>(8,0));
}

void FIFO::write(std::vector<int> data){
    this->data_buffers[this->wptr] = data;
    this->wptr++;
    if (this->wptr >= 3) {
        this->full = true;
        this->wptr = 0;
    }
}

std::vector<int> FIFO::read(){
    std::vector<int> rdata;
    rdata = this->data_buffers[this->rptr];
    // this->data[this->rptr].clear();
    std::fill(this->data_buffers[this->rptr].begin(), this->data_buffers[this->rptr].end(), 0);
    this->full = false;
    this->rptr++;
    if (this->rptr >= 3) {
        this->empty = true;
        this->rptr = 0;
    }
    else{
        this->empty = false;
    }
    return rdata;
}
