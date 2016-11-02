#ifndef TRANSACTION_H_
#define TRANSACTION_H_

#include <gsl/gsl>      // gsl::byte
using gsl::byte;
#include <vector>
using std::vector;

class Transaction
{
public:
#if 0   // TODO
    static constexpr long SUPPLY = 2779530283277761L; // = (3^33 - 1) / 2

    static constexpr int SIZE = 1604;

    static constexpr int TYPE_OFFSET = 0;
    static constexpr int TYPE_SIZE = Byte.BYTES;

    static constexpr int HASH_OFFSET = TYPE_OFFSET + TYPE_SIZE + ((Long.BYTES - (TYPE_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int HASH_SIZE = 46;

    static constexpr int BYTES_OFFSET = HASH_OFFSET + HASH_SIZE + ((Long.BYTES - (HASH_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int BYTES_SIZE = SIZE;

    static constexpr int ADDRESS_OFFSET = BYTES_OFFSET + BYTES_SIZE + ((Long.BYTES - (BYTES_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int ADDRESS_SIZE = 49;

    static constexpr int VALUE_OFFSET = ADDRESS_OFFSET + ADDRESS_SIZE + ((Long.BYTES - (ADDRESS_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int VALUE_SIZE = Long.BYTES;

    static constexpr int TAG_OFFSET = VALUE_OFFSET + VALUE_SIZE + ((Long.BYTES - (VALUE_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int TAG_SIZE = 17;

    static constexpr int CURRENT_INDEX_OFFSET = TAG_OFFSET + TAG_SIZE + ((Long.BYTES - (TAG_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int CURRENT_INDEX_SIZE = Long.BYTES;

    static constexpr int LAST_INDEX_OFFSET = CURRENT_INDEX_OFFSET + CURRENT_INDEX_SIZE + ((Long.BYTES - (CURRENT_INDEX_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int LAST_INDEX_SIZE = Long.BYTES;

    static constexpr int BUNDLE_OFFSET = LAST_INDEX_OFFSET + LAST_INDEX_SIZE + ((Long.BYTES - (LAST_INDEX_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int BUNDLE_SIZE = 49;

    static constexpr int TRUNK_TRANSACTION_OFFSET = BUNDLE_OFFSET + BUNDLE_SIZE + ((Long.BYTES - (BUNDLE_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int TRUNK_TRANSACTION_SIZE = HASH_SIZE;

    static constexpr int BRANCH_TRANSACTION_OFFSET = TRUNK_TRANSACTION_OFFSET + TRUNK_TRANSACTION_SIZE + ((Long.BYTES - (TRUNK_TRANSACTION_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int BRANCH_TRANSACTION_SIZE = HASH_SIZE;

    static constexpr int VALIDITY_OFFSET = BRANCH_TRANSACTION_OFFSET + BRANCH_TRANSACTION_SIZE + ((Long.BYTES - (BRANCH_TRANSACTION_SIZE & (Long.BYTES - 1))) & (Long.BYTES - 1));
    static constexpr int VALIDITY_SIZE = 1;
#endif

    static constexpr auto SUPPLY = 2779530283277761L; // = (3^33 - 1) / 2

#if 0   // TODO
    static constexpr int SIGNATURE_MESSAGE_FRAGMENT_TRINARY_OFFSET = 0;
    static constexpr int SIGNATURE_MESSAGE_FRAGMENT_TRINARY_SIZE = 6561;

    static constexpr int ADDRESS_TRINARY_OFFSET = SIGNATURE_MESSAGE_FRAGMENT_TRINARY_OFFSET + SIGNATURE_MESSAGE_FRAGMENT_TRINARY_SIZE;
    static constexpr int ADDRESS_TRINARY_SIZE = 243;

    static constexpr int VALUE_TRINARY_OFFSET = ADDRESS_TRINARY_OFFSET + ADDRESS_TRINARY_SIZE, VALUE_TRINARY_SIZE = 81;
    static constexpr int VALUE_USABLE_TRINARY_SIZE = 33;

    static constexpr int TAG_TRINARY_OFFSET = VALUE_TRINARY_OFFSET + VALUE_TRINARY_SIZE;
    static constexpr int TAG_TRINARY_SIZE = 81;

    static constexpr int TIMESTAMP_TRINARY_OFFSET = TAG_TRINARY_OFFSET + TAG_TRINARY_SIZE;
    static constexpr int TIMESTAMP_TRINARY_SIZE = 27;

    static constexpr int CURRENT_INDEX_TRINARY_OFFSET = TIMESTAMP_TRINARY_OFFSET + TIMESTAMP_TRINARY_SIZE;
    static constexpr int CURRENT_INDEX_TRINARY_SIZE = 27;

    static constexpr int LAST_INDEX_TRINARY_OFFSET = CURRENT_INDEX_TRINARY_OFFSET + CURRENT_INDEX_TRINARY_SIZE;
    static constexpr int LAST_INDEX_TRINARY_SIZE = 27;

    static constexpr int BUNDLE_TRINARY_OFFSET = LAST_INDEX_TRINARY_OFFSET + LAST_INDEX_TRINARY_SIZE;
    static constexpr int BUNDLE_TRINARY_SIZE = 243;

    static constexpr int TRUNK_TRANSACTION_TRINARY_OFFSET = BUNDLE_TRINARY_OFFSET + BUNDLE_TRINARY_SIZE;
    static constexpr int TRUNK_TRANSACTION_TRINARY_SIZE = 243;

    static constexpr int BRANCH_TRANSACTION_TRINARY_OFFSET = TRUNK_TRANSACTION_TRINARY_OFFSET + TRUNK_TRANSACTION_TRINARY_SIZE;
    static constexpr int BRANCH_TRANSACTION_TRINARY_SIZE = 243;

    static constexpr int NONCE_TRINARY_OFFSET = BRANCH_TRANSACTION_TRINARY_OFFSET + BRANCH_TRANSACTION_TRINARY_SIZE;
    static constexpr int NONCE_TRINARY_SIZE = 243;


    static constexpr int TRINARY_SIZE = NONCE_TRINARY_OFFSET + NONCE_TRINARY_SIZE;

    static constexpr int ESSENCE_TRINARY_OFFSET = ADDRESS_TRINARY_OFFSET;
    static constexpr int ESSENCE_TRINARY_SIZE = ADDRESS_TRINARY_SIZE + VALUE_TRINARY_SIZE + TAG_TRINARY_SIZE + TIMESTAMP_TRINARY_SIZE + CURRENT_INDEX_TRINARY_SIZE + LAST_INDEX_TRINARY_SIZE;

    static constexpr int MIN_WEIGHT_MAGNITUDE = 18;

public:
    Transaction (const vector<int>& trits)
    {
        m_trits = trits;
        m_bytes = Converter.bytes(trits);

        Curl curl;
        curl.absorb(trits, 0, TRINARY_SIZE);

        vector<int> hashTrits(Curl.HASH_LENGTH);
        curl.squeeze(hashTrits, 0, hashTrits.length);

        m_hash = Arrays.copyOf(Converter.bytes(hashTrits), HASH_SIZE);

        m_address = Converter.bytes(trits, ADDRESS_TRINARY_OFFSET, ADDRESS_TRINARY_SIZE);
        m_value = Converter.longValue(trits, VALUE_TRINARY_OFFSET, VALUE_USABLE_TRINARY_SIZE);
        System.arraycopy(Converter.bytes(trits, TAG_TRINARY_OFFSET, TAG_TRINARY_SIZE), 0, m_tag = new byte[TAG_SIZE], 0, TAG_SIZE);
        m_currentIndex = Converter.longValue(trits, CURRENT_INDEX_TRINARY_OFFSET, CURRENT_INDEX_TRINARY_SIZE);
        m_lastIndex = Converter.longValue(trits, LAST_INDEX_TRINARY_OFFSET, LAST_INDEX_TRINARY_SIZE);
        System.arraycopy(Converter.bytes(trits, BUNDLE_TRINARY_OFFSET, BUNDLE_TRINARY_SIZE), 0, m_bundle = new byte[BUNDLE_SIZE], 0, BUNDLE_SIZE);
        System.arraycopy(Converter.bytes(trits, TRUNK_TRANSACTION_TRINARY_OFFSET, TRUNK_TRANSACTION_TRINARY_SIZE), 0, m_trunkTransaction = new byte[TRUNK_TRANSACTION_SIZE], 0, TRUNK_TRANSACTION_SIZE);
        System.arraycopy(Converter.bytes(trits, BRANCH_TRANSACTION_TRINARY_OFFSET, BRANCH_TRANSACTION_TRINARY_SIZE), 0, m_branchTransaction = new byte[BRANCH_TRANSACTION_SIZE], 0, BRANCH_TRANSACTION_SIZE);

        m_type = Storage::FILLED_SLOT;

        m_trunkTransactionPointer = 0;
        m_branchTransactionPointer = 0;
        m_validity = 0;

        m_pointer = 0;
    }

    Transaction(const vector<byte>& bytes, const vector<int>& trits, const Curl& curl)
    {
        m_bytes = Arrays.copyOf(bytes, BYTES_SIZE);
        Converter.getTrits(m_bytes, m_trits = trits);

        for (int i = VALUE_TRINARY_OFFSET + VALUE_USABLE_TRINARY_SIZE; i < VALUE_TRINARY_OFFSET + VALUE_TRINARY_SIZE; i++)
        {
            if (trits[i] != 0)
            {
                throw new RuntimeException("Invalid transaction value");
            }
        }

        curl.reset();
        curl.absorb(trits, 0, TRINARY_SIZE);
        vector<int> hashTrits(Curl.HASH_LENGTH);
        curl.squeeze(hashTrits, 0, hashTrits.length);

        m_hash = Converter.bytes(hashTrits);
        if (m_hash[Hash.SIZE_IN_BYTES - 4] != 0 || m_hash[Hash.SIZE_IN_BYTES - 3] != 0 || m_hash[Hash.SIZE_IN_BYTES - 2] != 0 || m_hash[Hash.SIZE_IN_BYTES - 1] != 0)
        {
            throw new RuntimeException("Invalid transaction hash");
        }

        m_weightMagnitude = MIN_WEIGHT_MAGNITUDE;
        while (m_weightMagnitude < Curl.HASH_LENGTH && hashTrits[Curl.HASH_LENGTH - m_weightMagnitude - 1] == 0)
        {
            m_weightMagnitude++;
        }

        m_address = Converter.bytes(trits, ADDRESS_TRINARY_OFFSET, ADDRESS_TRINARY_SIZE);
        m_value = Converter.longValue(trits, VALUE_TRINARY_OFFSET, VALUE_USABLE_TRINARY_SIZE);
        System.arraycopy(Converter.bytes(trits, TAG_TRINARY_OFFSET, TAG_TRINARY_SIZE), 0, m_tag = new byte[TAG_SIZE], 0, TAG_SIZE);
        m_currentIndex = Converter.longValue(trits, CURRENT_INDEX_TRINARY_OFFSET, CURRENT_INDEX_TRINARY_SIZE);
        m_lastIndex = Converter.longValue(trits, LAST_INDEX_TRINARY_OFFSET, LAST_INDEX_TRINARY_SIZE);
        System.arraycopy(Converter.bytes(trits, BUNDLE_TRINARY_OFFSET, BUNDLE_TRINARY_SIZE), 0, m_bundle = new byte[BUNDLE_SIZE], 0, BUNDLE_SIZE);
        System.arraycopy(Converter.bytes(trits, TRUNK_TRANSACTION_TRINARY_OFFSET, TRUNK_TRANSACTION_TRINARY_SIZE), 0, m_trunkTransaction = new byte[TRUNK_TRANSACTION_SIZE], 0, TRUNK_TRANSACTION_SIZE);
        System.arraycopy(Converter.bytes(trits, BRANCH_TRANSACTION_TRINARY_OFFSET, BRANCH_TRANSACTION_TRINARY_SIZE), 0, m_branchTransaction = new byte[BRANCH_TRANSACTION_SIZE], 0, BRANCH_TRANSACTION_SIZE);

        m_type = Storage::FILLED_SLOT;

        m_trunkTransactionPointer = 0;
        m_branchTransactionPointer = 0;
        m_validity = 0;

        m_pointer = 0;
    }

    Transaction(const vector<byte>& mainBuffer, long pointer)
    {
        m_type = mainBuffer[TYPE_OFFSET];
        System.arraycopy(mainBuffer, HASH_OFFSET, m_hash = new byte[HASH_SIZE], 0, HASH_SIZE);

        System.arraycopy(mainBuffer, BYTES_OFFSET, m_bytes = new byte[BYTES_SIZE], 0, BYTES_SIZE);

        System.arraycopy(mainBuffer, ADDRESS_OFFSET, m_address = new byte[ADDRESS_SIZE], 0, ADDRESS_SIZE);
        m_value = Storage.value(mainBuffer, VALUE_OFFSET);
        System.arraycopy(mainBuffer, TAG_OFFSET, m_tag = new byte[TAG_SIZE], 0, TAG_SIZE);
        m_currentIndex = Storage.value(mainBuffer, CURRENT_INDEX_OFFSET);
        m_lastIndex = Storage.value(mainBuffer, LAST_INDEX_OFFSET);
        System.arraycopy(mainBuffer, BUNDLE_OFFSET, m_bundle = new byte[BUNDLE_SIZE], 0, BUNDLE_SIZE);
        System.arraycopy(mainBuffer, TRUNK_TRANSACTION_OFFSET, m_trunkTransaction = new byte[TRUNK_TRANSACTION_SIZE], 0, TRUNK_TRANSACTION_SIZE);
        System.arraycopy(mainBuffer, BRANCH_TRANSACTION_OFFSET, m_branchTransaction = new byte[BRANCH_TRANSACTION_SIZE], 0, BRANCH_TRANSACTION_SIZE);

        m_trunkTransactionPointer = Storage.transactionPointer(m_trunkTransaction);
        if (m_trunkTransactionPointer < 0)
        {
            m_trunkTransactionPointer = -m_trunkTransactionPointer;
        }
        m_branchTransactionPointer = Storage.transactionPointer(m_branchTransaction);
        if (m_branchTransactionPointer < 0)
        {
            m_branchTransactionPointer = -m_branchTransactionPointer;
        }

        m_validity = mainBuffer[VALIDITY_OFFSET];

        m_pointer = pointer;
    }

    synchronized int[] trits()
    {
        if (m_trits == null)
        {
            m_trits = new int[TRINARY_SIZE];
            Converter.getTrits(m_bytes, m_trits);
        }

        return m_trits;
    }

    static void dump(final byte[] mainBuffer, final byte[] hash, final Transaction transaction)
    {
        System.arraycopy(Storage.ZEROED_BUFFER, 0, mainBuffer, 0, Storage.CELL_SIZE);
        System.arraycopy(hash, 0, mainBuffer, HASH_OFFSET, HASH_SIZE);

        if (transaction == null)
        {
            mainBuffer[TYPE_OFFSET] = Storage.PREFILLED_SLOT;
        }
        else
        {
            mainBuffer[TYPE_OFFSET] = (byte)transaction.m_type;

            System.arraycopy(transaction.m_bytes, 0, mainBuffer, BYTES_OFFSET, BYTES_SIZE);
            System.arraycopy(transaction.m_address, 0, mainBuffer, ADDRESS_OFFSET, ADDRESS_SIZE);
            Storage.setValue(mainBuffer, VALUE_OFFSET, transaction.m_value);
            final int[] trits = transaction.m_trits();
            System.arraycopy(Converter.bytes(trits, TAG_TRINARY_OFFSET, TAG_TRINARY_SIZE), 0, mainBuffer, TAG_OFFSET, TAG_SIZE);
            Storage.setValue(mainBuffer, CURRENT_INDEX_OFFSET, transaction.m_currentIndex);
            Storage.setValue(mainBuffer, LAST_INDEX_OFFSET, transaction.m_lastIndex);
            System.arraycopy(Converter.bytes(trits, BUNDLE_TRINARY_OFFSET, BUNDLE_TRINARY_SIZE), 0, mainBuffer, BUNDLE_OFFSET, BUNDLE_SIZE);
            System.arraycopy(transaction.m_trunkTransaction, 0, mainBuffer, TRUNK_TRANSACTION_OFFSET, TRUNK_TRANSACTION_SIZE);
            System.arraycopy(transaction.m_branchTransaction, 0, mainBuffer, BRANCH_TRANSACTION_OFFSET, BRANCH_TRANSACTION_SIZE);

            long approvedTransactionPointer = Storage.transactionPointer(transaction.m_trunkTransaction);
            if (approvedTransactionPointer == 0)
            {
                Storage.approvedTransactionsToStore[Storage.numberOfApprovedTransactionsToStore++] = transaction.m_trunkTransaction;

            }
            else
            {
                if (approvedTransactionPointer < 0)
                {
                    approvedTransactionPointer = -approvedTransactionPointer;
                }

                final long index = (approvedTransactionPointer - (Storage.CELLS_OFFSET - Storage.SUPER_GROUPS_OFFSET)) >> 11;
                Storage.transactionsTipsFlags.put((int)(index >> 3), (byte)(Storage.transactionsTipsFlags.get((int)(index >> 3)) & (0xFF ^ (1 << (index & 7)))));
            }
            if (!Arrays.equals(transaction.m_branchTransaction, transaction.m_trunkTransaction))
            {

                approvedTransactionPointer = Storage.transactionPointer(transaction.m_branchTransaction);
                if (approvedTransactionPointer == 0)
                {
                    Storage.approvedTransactionsToStore[Storage.numberOfApprovedTransactionsToStore++] = transaction.m_branchTransaction;
                }
                else
                {
                    if (approvedTransactionPointer < 0)
                    {
                        approvedTransactionPointer = -approvedTransactionPointer;
                    }

                    final long index = (approvedTransactionPointer - (Storage.CELLS_OFFSET - Storage.SUPER_GROUPS_OFFSET)) >> 11;
                    Storage.transactionsTipsFlags.put((int) (index >> 3), (byte) (Storage.transactionsTipsFlags.get((int) (index >> 3)) & (0xFF ^ (1 << (index & 7)))));
                }
            }
        }
    }

private:
    int m_type;
    vector<byte> m_hash;

    vector<byte> m_bytes;

    vector<byte> m_address;
    long m_value;
    vector<byte> m_tag;
    long m_currentIndex;
    long m_lastIndex;
    vector<byte> m_bundle;
    vector<byte> m_trunkTransaction;
    vector<byte> m_branchTransaction;

    int m_validity;

    long m_pointer;

    long m_trunkTransactionPointer;
    long m_branchTransactionPointer;
    vector<int> m_trits;
    int m_weightMagnitude;
#endif
};

#endif
