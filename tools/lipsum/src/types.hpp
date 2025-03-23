#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>
#include <unordered_map>

enum class POS {
    NOUN,
    ADJECTIVE,
    VERB,
    ADVERB,
    PREPOSITION,
    CONJUNCTION,
};

struct Word {
    std::string text; /* The word itself */
    POS pos; /* Part of Speech */
    char gender; /* Gender for nouns / adjectives ('m', 'f', 'n'). For others, use 'x'. */
};

using Dictionary = std::unordered_map<POS, std::vector<Word>>;

enum class TokenType {
    WORD,
    PUNCTUATION
};

struct Token {
    TokenType type;
    POS pos;
    char required_gender;

    std::string text;
};

#endif
