#ifndef AEASYDS_STRING_FIND_HPP
#define AEASYDS_STRING_FIND_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <cinttypes>

namespace Aeasyds::StringFind
{
    
using std::string;

struct I_Find
{
    virtual void SetupText(const string& text) = 0;
    virtual std::vector<size_t> Find(const string& pattern, const string& text) = 0;
    virtual ~I_Find() = default;
};

class ShiftAnd : public I_Find
{
private:
    using Word = uint64_t;
    using BitSet = std::vector<Word>;
    using MaskMap = std::unordered_map<char, BitSet>;

    const size_t k_word_size = sizeof(Word) * 8;

    BitSet m_current_state;
    MaskMap m_states;
    string m_pattern;
    Word m_string_end;

    void ProcessPattern();

public:
    ShiftAnd()
    {

    }
    ~ShiftAnd()
    {

    }

    void SetupText(const string& text) {} // Not needed

    std::vector<size_t> Find(const string& pattern, const string& text);
};

} // namespace Aeasyds::StringFind

#endif // AEASYDS_STRING_FIND_HPP