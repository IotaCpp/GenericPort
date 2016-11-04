/******************************************************************************
 * IotaCurlUtils.cpp
 *****************************************************************************/

#include "IotaCurlUtils.h"
#include <string>

///////////////////////////////////////////////////////////////////////////////
/// Helpers
///////////////////////////////////////////////////////////////////////////////

static inline int utilsNot(int a) 
{
	return a^0b1;
}

//https://s16.postimg.org/dthtvw3id/binary_curl.png

static inline int utilsRt(int a, int b, int c, int d) 
{
	return ((a ^ d) & (utilsNot(b) | c));
}

static inline int utilsLt(int a, int b, int c, int d) 
{
	return ((b ^ c) | (utilsRt(a,b,c,d)));
}


///////////////////////////////////////////////////////////////////////////////
/// Static Arrays
///////////////////////////////////////////////////////////////////////////////

const int IotaCurlUtils::IOTACURL_TRYTE2TRITS_TBL[][3] = {
            { 0,  0,  0}, { 1,  0,  0}, {-1,  1,  0},
            { 0,  1,  0}, { 1,  1,  0}, {-1, -1,  1},
            { 0, -1,  1}, { 1, -1,  1}, {-1,  0,  1},
            { 0,  0,  1}, { 1,  0,  1}, {-1,  1,  1},
            { 0,  1,  1}, { 1,  1,  1}, {-1, -1, -1},
            { 0, -1, -1}, { 1, -1, -1}, {-1,  0, -1},
            { 0,  0, -1}, { 1,  0, -1}, {-1,  1, -1},
            { 0,  1, -1}, { 1,  1, -1}, {-1, -1,  0},
            { 0, -1,  0}, { 1, -1,  0}, {-1,  0,  0},
};

const int IotaCurlUtils::TRUTH_TABLE[] = {1, 0, -1, 0, 1, -1, 0, 0, -1, 1, 0};

///////////////////////////////////////////////////////////////////////////////
/// IotaCurlUtils
///////////////////////////////////////////////////////////////////////////////

	
void IotaCurlUtils::iotaCurlTrytes2Trits(int trits[], const int offset, const char trytes[], const int len)
{
	for(int i=0; i<len; i++) 
	{
		const int idx = (trytes[i+offset]=='9' ? 0 : trytes[i+offset]-'A'+1);
        trits[3*i+0] = IOTACURL_TRYTE2TRITS_TBL[idx][0];
        trits[3*i+1] = IOTACURL_TRYTE2TRITS_TBL[idx][1];
        trits[3*i+2] = IOTACURL_TRYTE2TRITS_TBL[idx][2];
    }
}

void IotaCurlUtils::iotaCurlTrits2Trytes(char trytes[], const int offset, const int trits[], const int len)
{
	static const std::string reference = "9ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
    for(int i=0; i<len; i+=3) 
	{
        int j = trits[i];
        if(i+1 < len) 
		{
            j += 3 * trits[i+1];
        }
        if(i+2 < len) 
		{
            j += 9 * trits[i+2];
        }
        if(j < 0) 
		{
            j += 27;
        }
        trytes[i/3+offset] = reference.at(j);
    }
}

long IotaCurlUtils::iotaCurlTritsAdd(int trits[], const int len, long n) 
{
	for(int i=0; i<len; i++) 
	{
        trits[i] += (n % 3);
        n /= 3;
        if(trits[i] > 1) 
		{
            trits[i] -= 3;
            n += 1;
        }
	}
    return n;
}


void IotaCurlUtils::iotaCurlTritsIncrement(int trits[], int len) 
{
	for(int i=0; (++trits[i]>1) && (i<len); i++) 
	{
		trits[i]=-1;
	}
}


int IotaCurlUtils::binaryTruth(int a, int b, int c, int d) 
{
	int x1 = utilsNot(utilsRt(a,b,c,d));
    int x2 = utilsLt(a,b,c,d);

	if (x1 == 1 && x2 == 0) 
	{
		return -1;
	}
	if (x1 == 1 && x2 == 1) 
	{
		return 0;
	}
	
    return 1;
}

int IotaCurlUtils::invokeBinaryTruthOn(int x1, int x2) 
{
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
		
    switch (x1) 
	{ // switch is direct O(1)
        case 0:  a=1; b=1; break;
        case 1:  a=0; b=1; break;
        default: a=1; b=0;
	}
	
    switch (x2) 
	{
		case 0:  c=1; d=1; break;
        case 1:  c=0; d=1; break;
        default: c=1; d=0;
	}

    return binaryTruth(a,b,c,d);
}
