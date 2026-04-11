#ifndef AEASYDS_STRING_FIND_HPP
#define AEASYDS_STRING_FIND_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <cinttypes>

namespace Aeasyds::StringFind
{
using std::string;

/* Declaration here */

/**
 * @brief Interface that all string matching classes must abide to
 * @details The class must provide a method for finding a pattern in a given text. It 
 * must also provide a setup function for preparing the text, to decrease the ammount
 * of work for follow-up searches (although not necessary for all of the classes, which
 * can opt for ignoring it)
 */
struct I_Find
{
    /**
     * @brief Prepares text for follow-up searches
     * @param text The text that will be used
     * @details This method can be implemented as empty for classes that do not need preprocessing of the text
     */
    virtual void SetupText(const string& text) = 0;
    /**
     * @brief Finds all the occourences of the pattern in the text
     * 
     * @param pattern The string that must be searched for
     * @param text The text to be searched in. Must match the text given to "SetupText()" if that is used
     * @return "std::vector<size_t>" A vector with the index of the occourence of the match (at the match end position)
     */
    virtual std::vector<size_t> Find(const string& pattern, const string& text) = 0;
    /**
     * @brief Destroy the I_Find object
     */
    virtual ~I_Find() = default;
};

/**
 * @brief String match implementation using the Shift-And strategy
 * @details See more at https://en.wikipedia.org/wiki/Bitap_algorithm
 */
class ShiftAnd : public I_Find
{
private:
    using Word = uint64_t;
    using BitSet = std::vector<Word>;
    using MaskMap = std::unordered_map<char, BitSet>;

    const size_t k_word_size = sizeof(Word) * 8; // The size of the used word, in bits

    BitSet m_current_state; // The bitset of the current step of processing
    MaskMap m_states; // The transition table for each symbol
    Word m_string_end; // The bitset that represents the end of the pattern

    void ProcessPattern(const string& pattern);

public:
    /**
     * @brief Construct a new Shift And object
     */
    ShiftAnd(){}
    /**
     * @brief Destroy the Shift And object
     */
    ~ShiftAnd(){}

    // From I_Find (Unused)
    void SetupText(const string& text) {} 
    // From I_Find
    std::vector<size_t> Find(const string& pattern, const string& text);
};

} // namespace Aeasyds::StringFind

#endif // AEASYDS_STRING_FIND_HPP