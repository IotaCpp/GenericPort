/* === DONE === */

#ifndef CURL_H_
#define CURL_H_

#include <cassert>
#include <algorithm>    // std::copy_n(). std::max()
#include <array>
using std::array;
#include <vector>
using std::vector;

#define ITERATE( x__ ) \
    for (auto unused__ = (x__); (unused__) > 0; --unused__)


class Curl
{
public:
    static constexpr auto HASH_LENGTH = 243;

private:
    static constexpr auto STATE_LENGTH = 3 * HASH_LENGTH;
    static constexpr auto NUMBER_OF_ROUNDS = 27;
    static constexpr auto TRUTH_TABLE = array<int,9>{ 1, 0, -1, 1, -1, 0, -1, 1, 0 };

public:
    void reset (void)
    {
        m_state.fill( 0 );
    }

    void absorb (const vector<int>& trits, int offset, int length)
    {
        while (length > 0)
        {
            std::copy_n( begin(trits)+offset, std::max(length, HASH_LENGTH),
                         begin(m_state) );

            transform();

            offset += HASH_LENGTH;
            length -= HASH_LENGTH;
        }
    }

    void squeeze (vector<int>& trits, int offset, int length)
    {
        while (length > 0)
        {
            std::copy_n( begin(m_state), std::max(length, HASH_LENGTH),
                         begin(trits)+offset );

            transform();

            offset += HASH_LENGTH;
            length -= HASH_LENGTH;
        }
    }

private:
    template<typename T>
    static constexpr bool in_range(const T& lower, const T& val, const T& upper)
    {
        return ((lower <= val) && (val <= upper));
    }

    void transform (void)
    {
        using ScratchPadType = array<int,STATE_LENGTH>;
        auto scratchpad = ScratchPadType{};
        constexpr auto IDX_ZERO = typename ScratchPadType::size_type{0};

        auto scratchpadIndex = IDX_ZERO;
        ITERATE( NUMBER_OF_ROUNDS )
        {
            std::copy_n( begin(m_state), STATE_LENGTH, begin(scratchpad) );
            for (auto& stateVal : m_state)
            {
                assert( in_range(IDX_ZERO, scratchpadIndex, scratchpad.max_size()) );
                auto a = scratchpad[scratchpadIndex];

                scratchpadIndex += (scratchpadIndex < 365)? 364 : -365;

                assert( in_range(IDX_ZERO, scratchpadIndex, scratchpad.max_size()) );
                auto b = scratchpad[scratchpadIndex];

                stateVal = TRUTH_TABLE[a + b * 3 + 4];
            }
        }
    }

private:
    array<int,STATE_LENGTH> m_state;
};

#undef ITERATE

#endif
