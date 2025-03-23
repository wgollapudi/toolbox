#include "lexicon.hpp"
#include "types.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <cctype>

Lexicon::Lexicon(std::mt19937& rng, const Dictionary& dict)
    : m_rng(rng), m_dictionary(dict) {}

/**
 * Selects a random word from the dictionary for the given part of speech and gender.
 */
Word Lexicon::select_word(POS part_of_speech, char required_gender) {
    auto it = m_dictionary.find(part_of_speech);
    if (it == m_dictionary.end() || it->second.empty()) {
        Word fallback = {"[fallback]", part_of_speech, required_gender};
        log_warning("selectWord", part_of_speech, required_gender, fallback);
        return fallback;
    }

    const auto& words = it->second;
    std::vector<Word> filtered;
    if (required_gender != 'x') {
        for (const auto& word : words) {
            if (word.gender == required_gender) {
                filtered.push_back(word);
             }
        }
    } else {
        filtered = words;
    }

    if (filtered.empty()) {
        filtered = words;
        std::uniform_int_distribution<size_t> dist(0, filtered.size() - 1);
        Word fallback = filtered[dist(m_rng)];
        log_warning("selectWord", part_of_speech, required_gender, fallback);
        return fallback;
    }

    std::uniform_int_distribution<size_t> dist(0, filtered.size() - 1);
    return filtered[dist(m_rng)];
}

/**
 * Logs a warning message when a matching word is not found.
 **/
void Lexicon::log_warning(const std::string& location, POS part_of_speech,
                          char required_gender, const Word& fallback) {
    std::cerr << "[Warning] In " << location << ": No word found for POS "
              << static_cast<int>(part_of_speech) << " with gender '" << required_gender
              << "'. Using fallback word: \"" << fallback.text << "\"\n";
}

/**
 * Processes the flattened token list and produces the final sentence.
 **/
std::string Lexicon::fillTokens(const std::vector<Token>& tokens) {
    std::ostringstream oss;
    bool need_space = false; /* Indicates if a space should be inserted before next token */

    for (const auto& token : tokens) {
        if (token.type == TokenType::PUNCTUATION) {
            oss << token.text;
            need_space = true;
        } else if (token.type == TokenType::WORD) {
            if (need_space && oss.tellp() > 0) {
                oss << " ";
            }
            Word word = select_word(token.pos, token.required_gender);
            oss << word.text;
            need_space = true;
         }
    }

    std::string sentence = oss.str();
    if (!sentence.empty()) {
        auto it = sentence.begin();
        while (it != sentence.end() && !std::isalpha(static_cast<unsigned char>(*it))) {
            it++;
        }
        if (it != sentence.end()) {
            *it = static_cast<char>(std::toupper(*it));
        }
    }
    return sentence;
}
