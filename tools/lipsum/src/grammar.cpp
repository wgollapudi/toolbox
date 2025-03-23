#include "grammar.hpp"
#include "config.hpp"
#include "types.hpp"
#include <memory>
#include <vector>
#include <random>

namespace GrammarNS {

class CompositeNode : public Node {
public:
    explicit CompositeNode(Type type) : Node(type) {}

    void flatten(std::vector<Token>& tokens) const override {
        for (const auto& child : m_children) {
            child->flatten(tokens);
        }
    }
};

class SlotNode : public Node {
public:
    SlotNode(POS part_of_speech, char gender)
        : Node(Node::Type::SLOT), m_pos(part_of_speech), m_required_gender(gender) {}

    void flatten(std::vector<Token>& tokens) const override {
        Token token;
        token.type = TokenType::WORD;
        token.pos = m_pos;
        token.required_gender = m_required_gender;
        tokens.push_back(token);
    }

private:
    POS m_pos;
    char m_required_gender;
};

class PunctuationNode : public Node {
public:
    explicit PunctuationNode(const std::string& text)
        : Node(Node::Type::PUNCTUATION), m_text(text) {}

    void flatten(std::vector<Token>& tokens) const override {
        Token token;
        token.type = TokenType::PUNCTUATION;
        token.text = m_text;
        tokens.push_back(token);
    }

private:
    std::string m_text;
};

Grammar::Grammar(std::mt19937& rng, Mode mode) : m_rng(rng), m_mode(mode) {}

std::vector<Token> Grammar::generate_sentence_skeleton() {
    auto sentence_node = generate_sentence();
    std::vector<Token> tokens;
    sentence_node->flatten(tokens);
    return tokens;
}

bool Grammar::chance(double probability) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(m_rng) < probability;
}

std::unique_ptr<Node> Grammar::generate_punctuation(const std::string& punct) {
    return std::make_unique<PunctuationNode>(punct);
}

/**
 * Generates a Noun Phrase (NP)
 * NP -> [Adjective(s)] Noun
 **/
std::unique_ptr<Node> Grammar::generate_NP() {
    auto np = std::make_unique<CompositeNode>(Node::Type::NP);

    char genders[] = { 'm', 'f', 'n' };
    std::uniform_int_distribution<int> gender_dist(0, 2);
    char noun_gender = genders[gender_dist(m_rng)];

    std::discrete_distribution<int> adj_dist({ Config::PROB_ZERO_ADJ,
                                               Config::PROB_ONE_ADJ,
                                               Config::PROB_TWO_ADJ });
    int num_adjectives = adj_dist(m_rng);

    for (int i = 0; i < num_adjectives; i++) {
         np->add_child(std::make_unique<SlotNode>(POS::ADJECTIVE, noun_gender));
    }
    np->add_child(std::make_unique<SlotNode>(POS::NOUN, noun_gender));

    return np;
}

/**
 * Generates a Verb Phrase (VP)
 * VP can be either simple (just a verb) or complex (verb + object NP).
 **/
std::unique_ptr<Node> Grammar::generate_VP() {
    auto vp = std::make_unique<CompositeNode>(Node::Type::VP);
    vp->add_child(std::make_unique<SlotNode>(POS::VERB, 'x'));

    if (m_mode == Mode::SIMPLE) {
    } else if (m_mode == Mode::COMPLEX) {
        vp->add_child(generate_NP());
    } else {
        if (chance(Config::PROB_COMPLEX_VP)) {
            vp->add_child(generate_NP());
        }
    }
    return vp;
}

/**
 * Generates a Prepositional Phrase (PP)
 * PP -> Preposition NP
 **/
std::unique_ptr<Node> Grammar::generate_PP() {
    auto pp = std::make_unique<CompositeNode>(Node::Type::PP);
    pp->add_child(std::make_unique<SlotNode>(POS::PREPOSITION, 'x'));
    pp->add_child(generate_NP());
    return pp;
}

/**
 * Generates a Clause:
 * Clause -> NP VP [Optional PP]
 **/
std::unique_ptr<Node> Grammar::generate_clause() {
    auto clause = std::make_unique<CompositeNode>(Node::Type::CLAUSE);
    clause->add_child(generate_NP());
    clause->add_child(generate_VP());

    if (m_mode == Mode::SIMPLE) {
    } else if (m_mode == Mode::COMPLEX) {
        clause->add_child(generate_PP());
    } else {
        if (chance(Config::PROB_PP_INCLSION)) {
            clause->add_child(generate_PP());
        }
    }
    return clause;
}

/**
 * Generates a complete Sentence:
 * Sentence -> Clause ["," Conjunction Clause]* "."
 **/
std::unique_ptr<Node> Grammar::generate_sentence() {
    auto sentence = std::make_unique<CompositeNode>(Node::Type::SENTENCE);

    /* Always add the first clause */
    sentence->add_child(generate_clause());

    if (m_mode == Mode::SIMPLE) {
    } else if (m_mode == Mode::COMPLEX) {
        sentence->add_child(generate_punctuation(","));
        sentence->add_child(std::make_unique<SlotNode>(POS::CONJUNCTION, 'x'));
        sentence->add_child(generate_clause());
        sentence->add_child(generate_punctuation(","));
        sentence->add_child(std::make_unique<SlotNode>(POS::CONJUNCTION, 'x'));
        sentence->add_child(generate_clause());
    } else {
        if (chance(Config::PROB_SECOND_CLAUSE)) {
            sentence->add_child(generate_punctuation(","));
            sentence->add_child(std::make_unique<SlotNode>(POS::CONJUNCTION, 'x'));
            sentence->add_child(generate_clause());

            if (chance(Config::PROB_THIRD_CLAUSE)) {
                sentence->add_child(generate_punctuation(","));
                sentence->add_child(std::make_unique<SlotNode>(POS::CONJUNCTION, 'x'));
                sentence->add_child(generate_clause());
            }
        }
    }

    sentence->add_child(generate_punctuation("."));

    return sentence;
}

} /* namespace GrammarNS */
