/* === DONE === */

#ifndef SNAPSHOT_H_
#define SNAPSHOT_H_

#include "Hash.h"
#include "Transaction.h"
#include <utility>              // std::make_pair()
#include <unordered_map>
using std::unordered_map;

class Snapshot
{
public:
    using HashTransactionMap = unordered_map<Hash, long>;
    static HashTransactionMap initialState;

public:
    static void initStatics (void)
    {
        initialState.insert( std::make_pair(Hash{}, Transaction::SUPPLY) );
    }
};

#endif
