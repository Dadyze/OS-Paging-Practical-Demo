#pragma once
#include "GlobalConfig.h"
#include <iostream>
#include <unordered_map>

class MMU
{
public:
    int physical_memory[FRAME_COUNT][PAGE_SIZE]; // fizička memorija (RAM)
    int page_table[PAGE_COUNT]; // tabela stranica (PAGE TABLE)
    bool active_bit[FRAME_COUNT] = { false };   // oznake zauzeća stranica ,(AKTIVNE STRANICE) u RAM-u
    bool enableCache = CACHESTATE;
    std::unordered_map<int, int> cache_table; // keš tabela

    void setup();
    int find_free_frame();


    void load_page(int page_number);
    void access_page(int page_number);


    void printRAM();
    void printPageTable();

};

