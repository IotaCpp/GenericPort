#ifndef IOTA_CURL_MINER_H_
#define IOTA_CURL_MINER_H_

#include <string>
#include "IotaCurlHash.h"

/******************************************************************************
 * IotaCurlMiner.h
 *****************************************************************************/
 
class IotaCurlMiner 
{
public:

    static const long LMASK1    = (0x5555555555555555l);
    static const long LMASK2    = (0xAAAAAAAAAAAAAAAAl);
    static const long LMASK3    = (0xFFFFFFFFFFFFFFFFL);
    static const long MAP[3];
	static const long MAP_EX[3];

    // The length of transaction header (before approvalNonce) in trytes.
    static const int TX_HEADER_SZ = 2430;

	IotaCurlMiner();
	
	std::string iotaCurlProofOfWork(const std::string & tx, const int minWeightMagnitude);

	void doPowTransform(long state[], size_t stateLenght);

protected:

    static long lc(long a);

    static long ld(long b, long c);

    void doPowAbsorb(long state[], const int trits[]);


	void powFinalize(char txar[], long result);
	
	std::string doCurlPowSingleThread(const std::string & tx, const int minWeightMagnitude);

	// TODO: In Java implementation, there was a multi thread version for the proof of work
	// 	std::string doCurlPowMultiThread(const std::string & tx, const int minWeightMagnitude) 
	//

private:

    long doWork(const int minWeightMagnitude, long offset);
	
	void powInit(const std::string & tx, char trx[], size_t trxSize);
	
    static const int HASH_SIZE  = 3*IotaCurlHash::IOTACURL_HASH_SZ;
    static const int STATE_SIZE = 3*IotaCurlHash::IOTACURL_STATE_SZ;
    static const int PARALLEL   = 32;
	
	static const int MID_STATE_SIZE = 3 * IotaCurlHash::IOTACURL_STATE_SZ;

    long midState_[MID_STATE_SIZE];
    int approvalNonce_[HASH_SIZE];
    int trunkTransaction_[HASH_SIZE];
    int branchTransaction_[HASH_SIZE];	
};
	
 
 #endif