/* 

   ad_bs_convert_test.cpp

   Author: Saroj Rai @ CharichaSoftwares 
   Created On: Thursday, 17 September 2020.

*/

// %Log:$ //
//


#include <stdio.h>
#include <assert.h>
#include "ad_bs_convert.h"

int test_cases[1][2][3] = {
    { { 2057, 1, 1}, {2000, 4, 13} }
};

int main(){

    // int* ans = bs_to_ad(test_cases[1][1][0], test_cases[1][1][1], test_cases[1][1][2]);
    // int* ans2 = ad_to_bs(test_cases[1][2][0], test_cases[1][2][1], test_cases[1][2][2]);

    int* ans = bs_to_ad(2057, 4, 20);
    int* ans2 = bs_to_ad(2007, 1, 1);
    // int* ans2 = ad_to_bs(2000, 4, 13);
    // printf("ans: %d, %d, %d\n", ans[0], ans[1], ans[2]);
    // printf("ans2: %d, %d, %d\n", ans2[0], ans2[1], ans2[2]);
    // printf("SUCCESS");
    free(ans);
    free(ans2);

    printf("%d\n", get_max_bs_day(2057, 4));

    return 0;
}


