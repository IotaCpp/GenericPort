/******************************************************************************
 * Miner.cpp
 * Miner Module.
 *
 * This module is a test case for IotaCurlMiner trinary function lib.
 *
 * Ported from Java version by oriol.borrell on 30/10/2016 
 *****************************************************************************/

#include <string>
#include <cstddef>
#include <cstdio>

#include "IotaCurlMiner.h"

static const int TX_LENGTH = 2673;

int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "usage: %s [minWeightMagnitude=13]\n", argv[0]);
        fprintf(stderr, "TX_TRYTES:\n"
                        "\tThe raw transaction data expressed in trytes-encoded string.\n" 
                        "\tShould be %d letters long and must only include letters [9A-Z].\n"
                        "minWeightMagnitude:\n\tThe difficulty factor of Curl hash function.\n"
                        "\tCurrently the default value is set to 13 as in the reference implementation.\n",
					    TX_LENGTH);
						
		return EXIT_FAILURE;		
	}
	
	const std::string tx = argv[1];
	if (tx.size() != TX_LENGTH)
	{
		fprintf(stderr, "The length of TX_TRYTES is not correct!\n");
		return EXIT_FAILURE;
	}
	
	for (char c: tx)
	{
		if ( ((c < 'A') || (c > 'Z')) && (c != '9'))
		{
			fprintf(stderr, "The raw transaction data expressed in trytes-encoded string must contain only [A-Z] and 9 number (found: %c)\n", c);
			return EXIT_FAILURE;				
		}
	}
	
	const int minWeightMagnitude = (argc > 2 ? std::stoi(argv[2]) : 13);
    if(minWeightMagnitude <= 0) 
	{
        fprintf(stderr, "minWeightMagnitude (%d) should be greater than zero.\n", minWeightMagnitude);
        return EXIT_FAILURE;
    }
	
	// Show the data to mine.
    fprintf(stdout, "I: mining for:\nTX_TRYTES: %s\nminWeightMagnitude: %d", tx.c_str(), minWeightMagnitude);

	// Do PoW.
    fprintf(stdout, "I: mining... ");

	IotaCurlMiner iotacurl;
	iotacurl.iotaCurlProofOfWork(tx, minWeightMagnitude);
	fprintf(stdout, "done.");

	// Print the approvalNonce and the final transaction.
    const std::string approvalNonce = tx.substr(2430, 2430+81);
    fprintf(stdout, "approvalNonce: %s", approvalNonce.c_str());
	fprintf(stdout, "txTrytes: %s", tx.c_str());

	// Print hash.
    const std::string hash = IotaCurlHash::iotaCurlHash(tx, TX_LENGTH);
	fprintf(stdout ,"hash: %s", hash.c_str());
	
	return EXIT_SUCCESS;
}