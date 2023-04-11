#include <iostream>
#include <unordered_map>
#include <vector>

#define PAGE_SIZE 1024
#define PHYSICAL_MEMORY_SIZE 8
#define CACHE_SIZE 2

using namespace std;

int main()
{
    vector<int> virtual_memory = { 1, 2, 3, 4, 5, 6, 7, 8 };

    unordered_map<int, int> page_table;
    
    int physical_memory[PHYSICAL_MEMORY_SIZE][PAGE_SIZE] = { 0 };
    
    bool present_bit[PHYSICAL_MEMORY_SIZE] = { false };

    // cache
    unordered_map<int, int> cache;
    vector<int> cache_list;
    int cache_size = 0;

    // simulate access to virtual memory
    for (int i = 0; i < virtual_memory.size(); i++)
    {
        int page_num = virtual_memory[i] / PAGE_SIZE;
        int offset = virtual_memory[i] % PAGE_SIZE;

        // check if page is in cache
        if (cache.find(page_num) != cache.end())
        {
            // hit in cache
            cout << "Page " << page_num << " is in cache!" << endl;
            int physical_address = cache[page_num] * PAGE_SIZE + offset;
            cout << "Physical address: " << physical_address << endl;
        }
        else
        {
            // miss in cache
            cout << "Page " << page_num << " is not in cache." << endl;

            // check if page is in physical memory
            if (page_table.find(page_num) != page_table.end())
            {
                // hit in physical memory
                cout << "Page " << page_num << " is in physical memory!" << endl;
                int frame_num = page_table[page_num];
                present_bit[frame_num] = true;

                // add page to cache
                if (cache_size < CACHE_SIZE)
                {
                    cache[page_num] = frame_num;
                    cache_list.push_back(page_num);
                    cache_size++;
                }
                else
                {
                    // remove least recently used page from cache
                    int lru_page = cache_list.front();
                    cache.erase(lru_page);
                    cache_list.erase(cache_list.begin());
                    cache[page_num] = frame_num;
                    cache_list.push_back(page_num);
                }

                int physical_address = frame_num * PAGE_SIZE + offset;
                cout << "Physical address: " << physical_address << endl;
            }
            else
            {
                // miss in physical memory
                cout << "Page " << page_num << " is not in physical memory." << endl;

                // find a free frame in physical memory
                int free_frame = -1;
                for (int j = 0; j < PHYSICAL_MEMORY_SIZE; j++)
                {
                    if (!present_bit[j])
                    {
                        free_frame = j;
                        break;
                    }
                }

                // if there is no free frame, replace a page using LRU algorithm
                if (free_frame == -1)
                {
                    int lru_frame = -1;
                    int min_age = INT_MAX;
                    for (int j = 0; j < PHYSICAL_MEMORY_SIZE; j++)
                    {
                        if (physical_memory[j][0] < min_age)
                        {
                            lru_frame = j;
                            min_age = physical_memory[j][0];
                        }
                    }

                    free_frame = lru_frame;
                    int lru_page = physical_memory[lru_frame][1];
                    page_table.erase(lru_page);
                    present_bit[lru_frame] = false;

                    // remove page from cache
                    if (cache.find(lru_page) != cache.end())
                    {
                        cache.erase(lru_page);
                        auto it = find(cache_list.begin(), cache_list.end(), lru_page);
                        if (it != cache_list.end())
                            cache_list.erase(it);
                        cache_size--;
                    }
                }

                // load page into physical memory
                page_table[page_num] = free_frame;
                present_bit[free_frame] = true;
                physical_memory[free_frame][0] = i; // set age to current time
                physical_memory[free_frame][1] = page_num;

                // add page to cache
                if (cache_size < CACHE_SIZE)
                {
                    cache[page_num] = free_frame;
                    cache_list.push_back(page_num);
                    cache_size++;
                }
                else
                {
                    // remove least recently used page from cache
                    int lru_page = cache_list.front();
                    cache.erase(lru_page);
                    cache_list.erase(cache_list.begin());
                    cache[page_num] = free_frame;
                    cache_list.push_back(page_num);
                }

                int physical_address = free_frame * PAGE_SIZE + offset;
                cout << "Physical address: " << physical_address << endl;
            }
        }
    }

    return 0;

