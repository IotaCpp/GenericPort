/******************************************************************************
 * IotaCurlHash.cpp
 *****************************************************************************/

#include "IotaCurlHash.h"
#include "IotaCurlUtils.h"

#include <cstring>
#include <cassert>

///////////////////////////////////////////////////////////////////////////////
/// Helpers
///////////////////////////////////////////////////////////////////////////////

static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last  = str.find_last_not_of(' ');
	
    return str.substr(first, (last-first+1));
}

///////////////////////////////////////////////////////////////////////////////
/// IotaCurlHash
///////////////////////////////////////////////////////////////////////////////

IotaCurlHash::IotaCurlHash()
{
#ifndef NDEBUG
	::memset(state_, 0x00, sizeof(state_));
#endif	
}

std::string IotaCurlHash::iotaCurlHash(const std::string & tx, const int len) 
{
	IotaCurlHash ctx;
    ctx.doAbsorb(tx.c_str(), len);
    return ctx.doFinalize();
}
	
int IotaCurlHash::getCurlStateValue(int index) 
{
	assert(index < IOTACURL_3STATE);
    return state_[index];
}

void IotaCurlHash::doAbsorb(const char * trytes, const int len) 
{
    for(int i=0; i<len; i+=IOTACURL_HASH_SZ) 
	{
        IotaCurlUtils::iotaCurlTrytes2Trits(state_, i, trytes, IotaCurlUtils::smin(len-i, IOTACURL_HASH_SZ));
        doHashTransform(state_, IOTACURL_3STATE);
    }
}

void IotaCurlHash::doHashTransform(int state[], int stateLenght) 
{
	int * state1 = state;
    int internalState[stateLenght];
	::memcpy(internalState, state1, sizeof(internalState));
	int * state2 = internalState;

    for(int r=0; r<27; r++) 
	{
        state1[0] = IotaCurlUtils::TRUTH_TABLE[state2[0] + (state2[364] << 2) + 5];

        // state1[0] = IotaCurlUtils.invokeBinaryTruthOn(state2[0], state2[364]);

        for(int i=0; i<3*IOTACURL_STATE_SZ/2; i++) 
		{
            state1[2*i+1] = IotaCurlUtils::TRUTH_TABLE[state2[364-i] + (state2[729-(i+1)] << 2) + 5];
            state1[2*i+2] = IotaCurlUtils::TRUTH_TABLE[state2[729-(i+1)] + (state2[364-(i+1)] << 2) + 5];
        }
        
		int * t = state1;
		state1  = state2;
        state2  = t;
    }
}

std::string IotaCurlHash::doFinalize() 
{
    char hash[IOTACURL_HASH_SZ+1+1];
    IotaCurlUtils::iotaCurlTrits2Trytes(hash, 0, state_, 3*IOTACURL_HASH_SZ);
	hash[sizeof(hash) - 1] = '\0';
    return trim(std::string(hash));
}




