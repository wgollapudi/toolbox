#ifndef Grammar_HPP
#define Grammar_HPP

#include "types.hpp"
#include <memory>
#include <vector>
#include <random>
#include <string>

namespace GrammarNS {

/* Modes for sentence complexity */
enum class Mode {
    SIMPLE,
    NORMAL,
    COMPLEX
};

/**
 * @brief Base class for nodes in the grammar tree
 *
 * Each Node represents a part of the structure (e.g., Sentance, Clause, NP, VP).
 * Derived classes should implement the flatten() method to convert the tree into tokens.
 */
class Node {
public:
    /* Types of nodes in the grammar tree */
    enum class Type {
        SENTENCE,
        CLAUSE,
        NP,         /* Noun Phrase */
        VP,         /* Verb Phrase */
        PP,         /* Prepositional Phrase */
        PUNCTUATION,
        SLOT        /* Placeholder for a word */
    };

    explicit Node(Type type) : m_nodeType(type) {}

    virtual ~Node() = default;

    Type get_type() const { return m_nodeType; }

    void add_child(std::unique_ptr<Node> child) {
        m_children.push_back(std::move(child));
    }

    const std::vector<std::unique_ptr<Node>>& getChildren() const { return m_children;}

    virtual void flatten(std::vector<Token>& tokens) const = 0;

protected:
    Type                               m_nodeType;
    std::vector<std::unique_ptr<Node>> m_children;
};

/**
* @brief The Grammar class builds the sentence skeleton as a tree and flattens it.
*
* It uses shared random engine to decide on clause counts, adjective distributions,
* and optional elements like prepositional phrases.
*/
class Grammar {
public:
    explicit Grammar(std::mt19937& rng, Mode mode = Mode::NORMAL);

    std::vector<Token> generate_sentence_skeleton();

private:
    std::mt19937 m_rng;
    Mode         m_mode;

    std::unique_ptr<Node> generate_sentence();
    std::unique_ptr<Node> generate_clause();
    std::unique_ptr<Node> generate_NP();
    std::unique_ptr<Node> generate_VP();
    std::unique_ptr<Node> generate_PP();

    bool chance(double probability);

    std::unique_ptr<Node> generate_punctuation(const std::string& punct);
};

} /* namespace GrammarNS */

#endif
