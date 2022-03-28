#include "MixedBuffer.h"
#include "TRandom.h"

void MixedBuffer::Add(my_event _new_event, int cen)
{
    assert((cen >= 1 && cen <= 9) && "Invalid Centrality!");
    if ((int)events[cen-1].size() <  buffer_size) events[cen-1].push_back(_new_event);
    else 
    {
        int rndm = gRandom->Integer(buffer_size);
        events[cen-1][rndm] = _new_event;
    }

    assert((int)events[cen-1].size() <= buffer_size && "Buffer overflow!");
}

my_event MixedBuffer::Sample(int cen)
{   
    assert(events[cen-1].size() != 0 && "Cannot sample from empty cell!");
    int rndm  = gRandom->Integer(events[cen-1].size());
    return events[cen-1][rndm];
}