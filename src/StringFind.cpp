#include "StringFind.hpp"
#include <cstddef>
#include <vector>

namespace Aeasyds::StringFind
{
using std::string;

/* START ShiftAnd */

void ShiftAnd::ProcessPattern(const string& pattern)
{
    size_t pattern_size = pattern.length();
    for (size_t i = 0; i < pattern_size; i++)
    {
        if (!m_states.contains(pattern[i]))
            m_states.insert({pattern[i], BitSet((pattern_size / k_word_size) + 1)});

        m_states[pattern[i]][i / k_word_size] |= 1 << (i % k_word_size);
        if (i == pattern_size - 1)
        {
            m_string_end = 1 << (i % k_word_size);
        }
    }
}

std::vector<size_t> ShiftAnd::Find(const string &pattern, const string &text)
{
    ProcessPattern(pattern);

    std::vector<size_t> output;
    m_current_state = BitSet((pattern.length() / k_word_size) + 1, 0);

    for (size_t i = 0; i < text.length(); i++)
    {
        char symbol = text[i];
        if (!m_states.contains(symbol))
        {
            for (auto &&c : m_current_state)
                c = 0;
            continue;
        }

        for (size_t j = 0; j < m_current_state.size(); j++)
        {
            if (m_current_state[j] >= (1u << (k_word_size - 1)) && (j + 1) < m_current_state.size())
            {
                m_current_state[j + 1] |= 1;
            }
            m_current_state[j] <<= 1;
        }

        m_current_state[0] |= 1;
        for (size_t j = 0; j < m_current_state.size(); j++)
        {
            m_current_state[j] &= m_states[symbol][j];
            if (j == (m_current_state.size() - 1)            // Last chunk of bitset
                && symbol == pattern.back()                // Reading the last letter of pattern
                && (m_current_state[j] & m_string_end) != 0) // State is reached
            {
                output.push_back(i);
                m_current_state[j] &= ~m_string_end;
            }
        }
    }
    return output;
}

/* END  ShiftAnd */

/* START Horspool */

void Horspool::ProcessPattern(const string& pattern)
{
    for (size_t i = 0; i < pattern.length() - 1; i++)
    {
        size_t shift = pattern.length() - 1 - i;
        if(!m_shift_table.contains(pattern[i]))
            m_shift_table.insert({pattern[i], shift});
        else m_shift_table[pattern[i]] = shift;
    }
}
std::vector<size_t> Horspool::Find(const string& pattern, const string& text)
{
    m_shift_table.clear();
    ProcessPattern(pattern);

    std::vector<size_t> output;
    size_t pattern_size = pattern.length();

    for (size_t i = pattern_size - 1; i < text.length(); i = i * 1)
    {
        for (size_t j = 0; j <= pattern_size; j++)
        {
            if(j == pattern_size)
            {
                output.push_back(i);
                i += GetShiftSize(pattern, pattern[pattern_size - 1]);
                break;
            }

            const char& text_char = text[i - j];
            const char& pattern_char = pattern[pattern_size - j - 1];
            if(text_char != pattern_char)
            {
                i += GetShiftSize(pattern, text_char);
                break;
            }
        }
    }

    return output;
}

/* END Horspool */

/* START Kmp */

void KMP::ProcessPattern(const string& pattern)
{
    m_match_retry.resize(pattern.length());
    m_match_retry[0] = 0;

    size_t best_match = 0;
    for (size_t i = 1; i < pattern.length(); i++)
    {
        while (best_match > 0 && pattern[best_match] != pattern[i])
            best_match = m_match_retry[best_match - 1];

        if(pattern[best_match] == pattern[i])
            best_match++;

        m_match_retry[i] = best_match;
    }
}

std::vector<size_t> KMP::Find(const string& pattern, const string& text)
{
    m_match_retry.clear();
    ProcessPattern(pattern);

    std::vector<size_t> output;

    size_t best_match = 0;
    for (size_t i = 0; i < text.length(); i++)
    {
        while (best_match > 0 && pattern[best_match] != text[i])
            best_match = m_match_retry[best_match - 1];
        if(pattern[best_match] == text[i])
            best_match++;
        if(best_match == pattern.length())
        {
            output.push_back(i);
            best_match = m_match_retry[best_match - 1];
        }
    }

    return output;
}


/* END Kmp */

};