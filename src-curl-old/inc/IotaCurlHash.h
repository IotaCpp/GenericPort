#ifndef IOTA_CURL_HASH_H_
#define IOTA_CURL_HASH_H_

#include <string>

/******************************************************************************
 * IotaCurlHash.h
 * This class has an internal state, therefore it has not been designed to be
 * thread-safe. You must access it statically, the main method is
 *
 * IotaCurlMiner.iotaCurlHash(tx,len); // thread safe
 *
 * The hash core functionalities have been ported in this class,
 * utility functions and tables on IotaCurlUtils
 *****************************************************************************/
 
 class IotaCurlHash 
 {
public:

    // The length of output hash in trytes.
    static const int IOTACURL_HASH_SZ = 81;

    // The length of internal state buffer in trytes.
    static const int IOTACURL_STATE_SZ = (3 * IOTACURL_HASH_SZ);
    static const int IOTACURL_3STATE   = (3 * IOTACURL_STATE_SZ);

	IotaCurlHash();
	
    static std::string iotaCurlHash(const std::string & tx, const int len);
	
	int getCurlStateValue(int index);

    /**
     * Absorb given trytes.
     */	 
    void doAbsorb(const char * trytes, const int len);
	
    void doHashTransform(int state[], int stateLenght);

private:

    std::string doFinalize();
	
	int state_[IOTACURL_3STATE];
};

#endif 