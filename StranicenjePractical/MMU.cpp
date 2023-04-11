#include "MMU.h"

void MMU::setup() {
    for (int i = 0; i < PAGE_COUNT; i++)
    {
        this->page_table[i] = FREE_SPACE;
        this->cache_table[i] = FREE_SPACE;
    }
}

void MMU::printRAM() {
    std::cout << "RAM STRUKTURA" << std::endl;
    std::cout << "--------------------" << std::endl;

    for (int i = 0; i < FRAME_COUNT; i++)
    {
        std::cout << "Frame " << i << ": ";
        if (this->active_bit[i])
        {
            for (int j = 0; j < PAGE_SIZE; j++)
            {
                std::cout << this->physical_memory[i][j] << " ";
            }
            std::cout << "(" << this->physical_memory[i][0] / 4 << ")" << std::endl;
        }
        else
        {
            std::cout << "empty";
        }
        std::cout << std::endl;
    }
}

void MMU::printPageTable()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "PAGE_TABLE (PAGE VALUE - FRAME VALUE)" << std::endl;
    std::cout << "------------------------" << std::endl;
    for (int i = 0; i < PAGE_COUNT; i++)
    {   
    
        if (page_table[i] == -1) {
            std::cout << "Page[" << i << "] " << "FREE "  <<std::endl;
            continue;
        }
        std::cout << "Page[" << i << "] " << this->page_table[i]<< std::endl;
      
    }
}

int MMU::find_free_frame()                        // funkcija za pronalazak slobodnog okvira u fizičkoj memoriji
{
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        if (!this->active_bit[i])
        {
            return i;
        }
    }
    return NO_FREE_FRAME;
}



void MMU::load_page(int page_number) // funkcija za učitavanje stranice u fizičku memoriju
{  
    int frame_number = find_free_frame(); //tražimo da li ima slobodnog mjesta da upisujemo u RAM

    if (frame_number != NO_FREE_FRAME)   // ako ima slobodnog mjesta,tj okvira, učitaj stranicu u fizičku memoriju
    {

        for (int i = 0; i < PAGE_SIZE; i++)
        {
            this->physical_memory[frame_number][i] = page_number * PAGE_SIZE + i;
        }
        this->page_table[page_number] = frame_number;
        this->active_bit[frame_number] = true;
    }
    else                                // ako nema slobodnog mjesta,tj okvira, oslobodi jedan okvir
    {

        int random_frame = rand() % FRAME_COUNT;
        int random_page;
        for (int i = 0; i < PAGE_COUNT; i++)
        {
            if (this->page_table[i] == random_frame)
            {
                random_page = i;
                break;
            }
        }

        this->page_table[random_page] = FREE_SPACE;
        this->active_bit[random_frame] = false;

        //zatim učitaj stranicu analogno istim nacinom
        for (int i = 0; i < PAGE_SIZE; i++)
        {
            this->physical_memory[random_frame][i] = page_number * PAGE_SIZE + i;
        }

        this->page_table[page_number] = random_frame;
        this->active_bit[random_frame] = true;
    }
}

void MMU::access_page(int page_number)
{
    // Provjeri keš
    if (cache_table.find(page_number) != cache_table.end() && CACHESTATE)
    {
        // Ako je stranica prisutna u kešu, koristi keš
        int frame_number = cache_table[page_number];
        std::cout << "Pristupio stranici:  " << page_number << " iz keša. Okvir broj: " << frame_number << "." <<std::endl;
    }
    else
    {
        // Ako stranica nije u kešu, proveri da li je učitana u fizičku memoriju
        if (page_table[page_number] != -1) 
        {
            int frame_number = page_table[page_number];
            std::cout << "Pristupio stranici: " << page_number << " iz RAM-a. Broj okvira: " << frame_number << "." << std::endl;
            // Dodaj stranicu u keš
            page_table[page_number] = frame_number;
        }
        else // Ako stranica nije učitana u fizičku memoriju, učitaj je
        {
            
            load_page(page_number);
            int frame_number = page_table[page_number];
            std::cout << "Pristupio stranici: " << page_number << " iz RAM-a. Broj okvira: " << frame_number << "." << std::endl;
            // Dodaj stranicu u cache
            page_table[page_number] = frame_number;
        }
    }

    this->printRAM();
    this->printPageTable();
    std::cout << "------------------------" << std::endl;
}
