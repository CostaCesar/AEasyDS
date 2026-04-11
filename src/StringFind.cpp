#include "StringFind.hpp"

namespace Aeasyds::StringFind
{
using std::string;

void ShiftAnd::ProcessPattern()
{
    size_t pattern_size = m_pattern.length();
    for (size_t i = 0; i < pattern_size; i++)
    {
        if (!m_states.contains(m_pattern[i]))
            m_states.insert({m_pattern[i], BitSet((pattern_size / k_word_size) + 1)});

        m_states[m_pattern[i]][i / k_word_size] |= 1 << (i % k_word_size);
        if (i == pattern_size - 1)
        {
            m_string_end = 1 << (i % k_word_size);
        }
    }
}

std::vector<size_t> ShiftAnd::Find(const string &pattern, const string &text)
{
    m_pattern = pattern;
    ProcessPattern();

    std::vector<size_t> output;
    m_current_state = BitSet((m_pattern.length() / k_word_size) + 1, 0);

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
                && symbol == m_pattern.back()                // Reading the last letter of pattern
                && (m_current_state[j] & m_string_end) != 0) // State is reached
            {
                output.push_back(i);
                m_current_state[j] &= ~m_string_end;
            }
        }
    }
    return output;
}

};