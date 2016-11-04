/******************************************************************************
 * IotaCurlMiner.cpp
 *****************************************************************************/

#include "IotaCurlMiner.h"
#include "IotaCurlHash.h"
#include "IotaCurlUtils.h"

#include <cstring>
#include <cassert>

// TODO: Use fixed types int32_t and int64_t for this module
//
static_assert(sizeof(int)  == 4, "Expecting int being 32 bits");
static_assert(sizeof(long) == 8, "Expecting int being 32 bits");

const long IotaCurlMiner::MAP[3]    = { (0b11), (0b01), (0b10) };	
const long IotaCurlMiner::MAP_EX[3] = { LMASK3, LMASK1, LMASK2 };
 

IotaCurlMiner::IotaCurlMiner()
{
#ifndef NDEBUG
	::memset(midState_, 0x00, sizeof(midState_));
    ::memset(approvalNonce_, 0x00, sizeof(approvalNonce_));
    ::memset(trunkTransaction_, 0x00, sizeof(trunkTransaction_));
    ::memset(branchTransaction_, 0x00, sizeof(branchTransaction_));	
#endif		
}

std::string IotaCurlMiner::iotaCurlProofOfWork(const std::string & tx, const int minWeightMagnitude) 
{
	return doCurlPowSingleThread(tx, minWeightMagnitude);
}

inline long IotaCurlMiner::lc(long a) 
{
    return ((((a) ^ ((a)>>1)) & LMASK1) | (((a)<<1) & LMASK2));
}

inline long IotaCurlMiner::ld(long b, long c) 
{	
    return (c) ^ (LMASK2 & (b) & (((b) & (c))<<1)) ^ (LMASK1 & ~(b) & (((b) & (c))>>1));
}

void IotaCurlMiner::doPowAbsorb(long state[], const int trits[]) 
{
    for (int i=0; i<HASH_SIZE; i++) 
	{
        state[i] = MAP_EX[trits[i]+1];
    }
}

void IotaCurlMiner::doPowTransform(long state[], size_t stateLenght) 
{
    long * state1 = state; 
    long internalState[stateLenght];
	::memcpy(internalState, state, sizeof(internalState));		
	long * state2 = internalState;

    for(int r=0; r<27; r++) 
	{            
        const long a = state2[0];
        const long b = state2[364];
        const long c = lc(a);
        state1[0] = ld(b, c);
            
        for (int i = 0; i < (STATE_SIZE / 2); i++) 
		{
            const long a3 = state2[364 - i - 1];
            const long a1 = state2[364 - i];
            const long a2 = state2[729 - i - 1];
            const long c1 = lc(a1);
            const long c2 = lc(a2);
            state1[2 * i + 1] = ld(a2, c1);
            state1[2 * i + 2] = ld(a3, c2);
        }
			
		long * t = state1;
		state1   = state2;
		state2   = t;			
    }
}
	
void IotaCurlMiner::powFinalize(char txar[], long result) 
{
    IotaCurlUtils::iotaCurlTritsAdd(approvalNonce_, HASH_SIZE, result);
    IotaCurlUtils::iotaCurlTrits2Trytes(txar, TX_HEADER_SZ, approvalNonce_, HASH_SIZE);
}
	
std::string IotaCurlMiner::doCurlPowSingleThread(const std::string & tx, const int minWeightMagnitude) 
{
	char trax[tx.size() + 1];
	powInit(tx, trax, tx.size());

    long offset = 0;
	long result = 0;
    while ((result = doWork(minWeightMagnitude, offset)) == 0) 
	{
        offset += PARALLEL;
    }
    powFinalize(trax, result);
	
	trax[tx.size()] = '\0';
    return std::string(trax);
}

long IotaCurlMiner::doWork(const int minWeightMagnitude, long offset) 
{
    int an[HASH_SIZE];
	::memcpy(an, approvalNonce_, sizeof(an));
        
	long state[MID_STATE_SIZE];
	::memcpy(state, midState_, sizeof(state));

	static_assert(sizeof(an) == sizeof(approvalNonce_), "Wrong sizes");
	static_assert(sizeof(state) == sizeof(midState_),   "Wrong sizes");

    IotaCurlUtils::iotaCurlTritsAdd(an, HASH_SIZE, offset);

    // Search. Process approvalNonce.
    for(int i=0; i<PARALLEL; i++) 
	{
        for(int j=0; j<HASH_SIZE; j++) 
		{
            state[j] |= ((MAP[an[j]+1]) << (i*2));
        }
        IotaCurlUtils::iotaCurlTritsIncrement(an, HASH_SIZE);
    }

    doPowTransform(state, MID_STATE_SIZE);

    // Process trunkTransaction/branchTransaction.
    doPowAbsorb(state, trunkTransaction_);
    doPowTransform(state, MID_STATE_SIZE);

    doPowAbsorb(state, branchTransaction_);
    doPowTransform(state, MID_STATE_SIZE);

    // Check if work is done.
    for(int i=0; i<PARALLEL; i++) 
	{
        bool complete = true;

        for(int j=HASH_SIZE-minWeightMagnitude; j<HASH_SIZE; j++) 
		{
            long n = ( state[j] >> ((2*i)) ) & (MAP[0]);
            complete = complete && (n == (MAP[1]));
        }
        if(!complete) 
		{
            continue;
        }
        return (offset + i); // If the solution has been found.
    }
    return 0;
}
	
void IotaCurlMiner::powInit(const std::string & tx, char trx[], size_t trxSize) 
{
	assert(trxSize >= tx.size());
	
    IotaCurlHash ctx;
	::memcpy(trx, tx.c_str(), tx.size());
    ctx.doAbsorb(trx, TX_HEADER_SZ);

    for (int i = 0; i < STATE_SIZE; i++) 
	{
		midState_[i] = (i < HASH_SIZE) ? 0L : (MAP_EX[ctx.getCurlStateValue(i) + 1]);
    }

    IotaCurlUtils::iotaCurlTrytes2Trits(approvalNonce_,     7290 / 3, trx, IotaCurlHash::IOTACURL_HASH_SZ);
    IotaCurlUtils::iotaCurlTrytes2Trits(trunkTransaction_,  7533 / 3, trx, IotaCurlHash::IOTACURL_HASH_SZ);
    IotaCurlUtils::iotaCurlTrytes2Trits(branchTransaction_, 7776 / 3, trx, IotaCurlHash::IOTACURL_HASH_SZ);
}

