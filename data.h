#include <cstdint>

struct Head{
    int id;    
    int index;     
    uint64_t timestamp;  
};

struct Data{
    Head head;
    int speed;   
    float acc;
};