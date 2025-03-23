#ifndef CONFIG_HPP
#define CONFIG_HPP

/**
 * @file config.hpp
 * @brief Configuration constants for the Lorem Ipsum generator.
 *
 * This header defines all probability weights and other constants used in generating the
 * sentance skeleton. Each constant is annotated for customization.
 */

namespace Config {

    /* Clause Count Configuration */
    /* Every sentance starts with one clause. Additional clauses are added based on these probabilities. */
    constexpr double PROB_SECOND_CLAUSE = 0.60;
    constexpr double PROB_THIRD_CLAUSE = 0.40;

    /* Adjective Count Distrobution for Noun Phrases (NP) */
    constexpr double PROB_ZERO_ADJ = 0.40;
    constexpr double PROB_ONE_ADJ = 0.50;
    constexpr double PROB_TWO_ADJ = 1.0 - (PROB_ZERO_ADJ + PROB_ONE_ADJ);

    /* Prepositional Prase (PP) Inclusion */
    constexpr double PROB_PP_INCLSION = 0.30;

    /* Verb Phrase (VP) Complexity: */
    constexpr double PROB_SIMPLE_VP = 0.60; /* probability of simple VP (verb only) */
    constexpr double PROB_COMPLEX_VP = 1.0 - PROB_SIMPLE_VP; /* probability of complex VP (verb + object NP) */
}

#endif
