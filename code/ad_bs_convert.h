// ad_bs_convert.h
//
// Last-Edit-By: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef AD_BS_CONVERT_H
#define AD_BS_CONVERT_H 1

#define AD_MIN 1944
#define AD_MAX 2033

#define BS_MIN 2000
#define BS_MAX 2090

#include <stdlib.h>
#include <stdio.h>		/* for printf testing only */


int is_leap_year(int);
int* ad_to_bs(int, int, int);
int* bs_to_ad(int, int, int);

int get_max_ad_day(int, int);
int get_max_bs_day(int, int);

const char* ad_month_name(int);
const char* bs_month_name(int);
const char* get_day_name(int);

#endif // AD_BS_CONVERT_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
