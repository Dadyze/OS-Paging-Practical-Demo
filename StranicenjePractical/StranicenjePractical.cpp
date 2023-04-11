#include <iostream>
#include "MMU.h"
#include "StranicenjePractical.h"

using namespace std;

int main()
{
    MMU MMU;
    MMU.setup();   // inicijalizacija tabele stranica

    MMU.load_page(0);
    MMU.load_page(1);
    MMU.load_page(2);
    MMU.load_page(3);
    MMU.load_page(4);
    MMU.load_page(5);
    MMU.load_page(6);
    MMU.load_page(7);

    
    MMU.access_page(4);
    MMU.access_page(7);
    MMU.access_page(1);
    return 0;
}