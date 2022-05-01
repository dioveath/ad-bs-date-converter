/* 

   leap_year_test.cpp

   Author: Saroj Rai @ CharichaSoftwares 
   Created On: Thursday, 17 September 2020.

*/

// %Log:$ //
//


#include <stdio.h>
#include <assert.h>

#include "ad_bs_convert.h"

int leap_year_test[23] =
    { 
	1944, 
	1948, 
	1952, 
	1956, 
	1960, 
	1964, 
	1968, 
	1972, 
	1976, 
	1980, 
	1984, 
	1988, 
	1992, 
	1996, 
	2000, 
	2004, 
	2008, 
	2012, 
	2016, 
	2020, 
	2024, 
	2028, 
	2032
    };



int main(){

    for(int year = 1944; year <= 2032; year++){
	int result = is_leap_year(year);
	int to_comp = 0;
	for(int j = 0; j < 23; j++)
	    if(year == leap_year_test[j]) to_comp = 1;
	assert(result == to_comp);
    }

    printf("SUCCESS! \n");
    return 0;
}
