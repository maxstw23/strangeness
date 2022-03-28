#ifndef MixedBuffer_hh
#define MixedBuffer_hh
#include "./MixedBuffer.h"
#include "./my_event.h"

class MixedBuffer
{
private:
    std::vector<my_event> events[9]; 
    const int buffer_size;  
public:
    MixedBuffer(int _buffer_size):buffer_size(_buffer_size) {}
    virtual ~MixedBuffer() {}
    void Init() {for (int i = 0;i < 9; i++) {events[i].resize(0);}}
    void Add(my_event _new_event, int cen);
    bool IsEmpty(int cen) {return (events[cen-1].size() == 0);} 
    my_event Sample(int cen);

ClassDef(MixedBuffer,1)
};

#endif