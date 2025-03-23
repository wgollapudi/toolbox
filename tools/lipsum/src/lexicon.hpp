#ifndef LEXICON_HPP
#define LEXICON_HPP

#include "types.hpp"
#include <random>
#include <vector>
#include <string>

class Lexicon {
public:
    Lexicon(std::mt19937& rng, const Dictionary& dict);

    /**
     * This function processes the flattened token list and returns a completed sentance.
     * It handles adjective-noun gender agreement and capitalizes the first word.
     * */
    std::string fillTokens(const std::vector<Token>& tokens);

private:
    std::mt19937&     m_rng;
    const Dictionary& m_dictionary;

    /**
     * Selects a random word from the dictionary for a given part of speech and gender.
     * If a word matching the criteria isn't found, a warning is logged and a fallback is used.*/
    Word select_word(POS part_of_speech, char required_gender);

    /**
     * Logs a warning message when a word isn't found
     **/
    void log_warning(const std::string& location, POS part_of_speech, char required_gender,
                     const Word& fallback);
};

#endif
