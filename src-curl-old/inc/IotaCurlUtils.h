#ifndef IOTA_CURL_UTILS_H_
#define IOTA_CURL_UTILS_H_

/******************************************************************************
 * IotaCurlUtils.h
 *****************************************************************************/

class IotaCurlUtils
{
public:

    static const int IOTACURL_TRYTE2TRITS_TBL[][3];
	
	static const int TRUTH_TABLE[];

    /**
     * Convert trytes into trits.
     *
     * @param trits Output.
     * @param trytes Input.
     * @param len The number of trytes to load.
     */
    static void iotaCurlTrytes2Trits(int trits[], const int offset, const char trytes[], const int len);

    /**
     *
     * Convert trits into trytes.
     *
     * @param trytes
     * @param trits
     * @param len
     */
    static void iotaCurlTrits2Trytes(char trytes[], const int offset, const int trits[], const int len);

    static long iotaCurlTritsAdd(int trits[], const int len, long n);

    static void iotaCurlTritsIncrement(int trits[], int len);

    // strict min
    // Math.min is (a <= b) ? a : b;
	// inlined for efficiency
    static int smin(int a, int b) 
	{
        return (a < b) ? a : b;
    }

    static int binaryTruth(int a, int b, int c, int d);

    static int invokeBinaryTruthOn(int x1, int x2);

private:
};

#endif // IOTA_CURL_UTILS_H_