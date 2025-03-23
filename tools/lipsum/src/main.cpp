#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <cstring>
#include <cstdlib>

#include "grammar.hpp"
#include "lexicon.hpp"
#include "dictionary.hpp"
#include "types.hpp"

using namespace std;
using namespace GrammarNS;

const std::string HELP_MESSAGE =
    "Usage:\n"
    "  lipsum [options]\n\n"
    "Options:\n"
    "  -h                  Print this help message.\n"
    "  --seed <number>     Set the random seed for reproducibility.\n"
    "  --simple            Force simple sentences (one clause, simple VP, no PP).\n"
    "  --complex           Force complex sentences (max clauses, complex VP, always PP).\n"
    "  --no-paragraph      Do not insert paragraph delimiters (output is one big block).\n"
    "  --no-header         Do not include the original Lorem Ipsum text.\n"
    "  --history           Display the history of Lorem Ipsum and exit.\n"
    "  -p <number>         Ensure at least this many paragraphs (header counts as one if present).\n"
    "  -s <number>         Ensure that this many additional sentences are produced.\n"
    "  -w <number>         Ensure that this many additional words are produced.\n"
    "  -c <number>         Ensure that this many additional characters are produced.\n"
    "  -b <number>         (Exclusive) Final output is exactly this many bytes.\n";

const std::string LOREM_IPSUM =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. "
    "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. "
    "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

const std::string HISTORY_MESSAGE = R"(What is Lorem Ipsum?

Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.

Where does it come from?

Contrary to popular belief, Lorem Ipsum is not simply random text. It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old. Richard McClintock, a Latin professor at Hampden-Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source. Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of "de Finibus Bonorum et Malorum" (The Extremes of Good and Evil) by Cicero, written in 45 BC. This book is a treatise on the theory of ethics, very popular during the Renaissance. The first line of Lorem Ipsum, "Lorem ipsum dolor sit amet..", comes from a line in section 1.10.32.

Why do we use it?

It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).

Source: https://www.lipsum.com/)";

std::string trim(const std::string &str) {
    const std::string whitespace = " \t\n\r";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

std::string normalize_line(const std::string& line) {
    std::istringstream iss(line);
    std::ostringstream oss;
    std::string word;
    bool first = true;
    while (iss >> word) {
        if (!first)
            oss << " ";
        oss << word;
        first  = false;
    }
    return oss.str();
}

std::string normalize_with_paragraphs(const std::string& input) {
    std::istringstream iss(input);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(iss, line)) {
        std::string normalized = normalize_line(line);
        if (!normalized.empty())
            lines.push_back(normalized);
    }
    std::ostringstream oss;
    for (size_t i = 0; i < lines.size(); i++) {
        if (i > 0)
            oss << "\n\n";
        oss << lines[i];
    }
    return oss.str();
}

std::string normalize_spaces(const std::string &str) {
    std::istringstream iss(str);
    std::ostringstream oss;
    std::string word;
    bool first = true;
    while (iss >> word) {
        if (!first) oss << " ";
        oss << word;
        first = false;
    }
    return oss.str();
}

size_t count_paragraphs(const std::string &text) {
    std::istringstream iss(text);
    std::string line;
    size_t count = 0;
    while (std::getline(iss, line)) {
        if (!trim(line).empty())
            count++;
    }
    return count;
}

size_t count_sentences(const std::string &text) {
    return std::count(text.begin(), text.end(), '.');
}

size_t count_words(const std::string &text) {
    std::istringstream iss(text);
    size_t count = 0;
    std::string word;
    while (iss >> word)
        count++;
    return count;
}

struct Options {
    bool show_help = false;
    bool history = false;
    bool no_header = false;
    bool no_paragraph = false;
    bool simple_mode = false;
    bool complex_mode = false;
    bool byte_exclusive = false;
    bool debug_mode = false;
    unsigned int seed = 0; // 0 means no seed provided; we can then use time(0) if needed.
    int paragraphs = -1;
    int sentences = -1;
    int words = -1;
    int characters = -1;
    int bytes = -1;
};

Options parseArguments(int argc, char* argv[]) {
    Options opts;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-h") {
            opts.show_help = true;
        } else if (arg == "--history") {
            opts.history = true;
        } else if (arg == "--seed") {
            if (i + 1 < argc) {
                opts.seed = std::stoul(argv[++i]);
            } else {
                std::cerr << "Error: --seed requires a number.\n";
                exit(EXIT_FAILURE);
            }
        } else if (arg == "--simple") {
            opts.simple_mode = true;
        } else if (arg == "--complex") {
            opts.complex_mode = true;
        } else if (arg == "--no-paragraph") {
            opts.no_paragraph = true;
        } else if (arg == "--no-header") {
            opts.no_header = true;
        } else if (arg == "--debug") {
            opts.debug_mode = true;
        } else if (arg == "-p") {
            if (i + 1 < argc) {
                opts.paragraphs = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: -p requires a number.\n";
                exit(EXIT_FAILURE);
            }
        } else if (arg == "-s") {
            if (i + 1 < argc) {
                opts.sentences = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: -s requires a number.\n";
                exit(EXIT_FAILURE);
            }
        } else if (arg == "-w") {
            if (i + 1 < argc) {
                opts.words = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: -w requires a number.\n";
                exit(EXIT_FAILURE);
            }
        } else if (arg == "-c") {
            if (i + 1 < argc) {
                opts.characters = std::stoi(argv[++i]);
            } else {
                std::cerr << "Error: -c requires a number.\n";
                exit(EXIT_FAILURE);
            }
        } else if (arg == "-b") {
            if (i + 1 < argc) {
                opts.bytes = std::stoi(argv[++i]);
                opts.byte_exclusive = true;
            } else {
                std::cerr << "Error: -b requires a number.\n";
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            std::cerr << HELP_MESSAGE;
            exit(EXIT_FAILURE);
        }
    }
    if (opts.byte_exclusive && (opts.paragraphs != -1 || opts.sentences != -1 || opts.words != -1 || opts.characters != -1)) {
        std::cerr << "Error: -b flag is exclusive and cannot be used with -p, -s, -w, or -c.\n";
        exit(EXIT_FAILURE);
    }
    if (opts.simple_mode && opts.complex_mode) {
        std::cerr << "Error: --simple and --complex cannot be used together.\n";
        exit(EXIT_FAILURE);
    }
    return opts;
}

std::string generateParagraph(GrammarNS::Grammar& grammar, Lexicon& lexicon, std::mt19937& rng) {
    std::uniform_int_distribution<int> sentenceCountDist(4, 7);
    int sentenceCount = sentenceCountDist(rng);
    std::ostringstream paragraph;
    for (int i = 0; i < sentenceCount; ++i) {
        auto tokens = grammar.generate_sentence_skeleton();
        std::string sentence = lexicon.fillTokens(tokens);
        paragraph << sentence << " ";
    }
    return trim(paragraph.str());
}

std::string appendParagraphsUntil(const std::string &base, int targetCount,
                                  std::function<int(const std::string&)> countFunc,
                                  const std::string &separator,
                                  GrammarNS::Grammar &grammar,
                                  Lexicon &lexicon,
                                  std::mt19937 &rng) {
    std::string output = base;
    while (countFunc(output) < targetCount) {
        std::string paragraph = generateParagraph(grammar, lexicon, rng);
        if (!output.empty())
            output += separator;
        output += paragraph;
    }
    return output;
}

std::string trimToCharacters(const std::string &text, int targetChars) {
    if ((int)text.size() <= targetChars)
        return text;
    return text.substr(0, targetChars);
}

std::string trimToBytes(const std::string &text, int targetBytes) {
    if ((int)text.size() <= targetBytes)
        return text;
    return text.substr(0, targetBytes);
}

int main(int argc, char* argv[]) {
    Options opts = parseArguments(argc, argv);
    if (opts.show_help) {
        std::cout << HELP_MESSAGE;
        return EXIT_SUCCESS;
    }
    if (opts.paragraphs == 0) opts.paragraphs = -1;
    if (opts.sentences == 0)  opts.sentences  = -1;
    if (opts.words == 0)      opts.words      = -1;
    if (opts.characters == 0) opts.characters = -1;

    if (opts.history) {
        std::cout << HISTORY_MESSAGE << std::endl;
        return EXIT_SUCCESS;
    }

    std::mt19937 rng;
    if (opts.seed != 0) {
        rng.seed(opts.seed);
    } else {
        rng.seed(1);
    }

    Mode mode = Mode::NORMAL;
    if (opts.simple_mode)
        mode = Mode::SIMPLE;
    else if (opts.complex_mode)
        mode = Mode::COMPLEX;

    Grammar grammar(rng, mode);
    Dictionary dict = get_test_dictionary();
    if (opts.debug_mode)
        dict = get_debug_dictionary();

    Lexicon lexicon(rng, dict);

    std::string output;
    if (!opts.no_header) {
        output = LOREM_IPSUM;
    }

    if (opts.paragraphs != -1) {
        size_t headerParagraphs = count_paragraphs(output);
        if ((int)headerParagraphs < opts.paragraphs) {
            std::string separator = "\n\n";
            output = appendParagraphsUntil(output, opts.paragraphs, count_paragraphs, separator, grammar, lexicon, rng);
        } else if ((int)headerParagraphs > opts.paragraphs) {
            std::istringstream iss(output);
            std::ostringstream oss;
            std::string line;
            int count = 0;
            while (std::getline(iss, line) && count < opts.paragraphs) {
                if (!trim(line).empty()) {
                    if (count > 0)
                        oss << (opts.no_paragraph ? " " : "\n");
                    oss << trim(line);
                    count++;
                }
            }
            output = oss.str();
        }
    }

    if (opts.sentences != -1) {
        int currentSentences = count_sentences(output);
        if (currentSentences > opts.sentences) {
            std::istringstream iss(output);
            std::ostringstream oss;
            std::string line;
            int sentencesCounted = 0;
            while (std::getline(iss, line)) {
                for (char ch : line) {
                    oss << ch;
                    if (ch == '.') {
                        sentencesCounted++;
                        if (sentencesCounted >= opts.sentences)
                            break;
                    }
                }
                if (sentencesCounted >= opts.sentences)
                    break;
                if (!opts.no_paragraph)
                    oss << "\n";
            }
            output = oss.str();
        } else if (currentSentences < opts.sentences) {
            int extra_needed = opts.sentences - currentSentences;
            std::string extra_sentences;
            while ((int)count_sentences(extra_sentences) < extra_needed) {
                auto tokens = grammar.generate_sentence_skeleton();
                std::string sentence = lexicon.fillTokens(tokens);
                if (!extra_sentences.empty())
                    extra_sentences += " ";
                extra_sentences += sentence;
            }
            if (!output.empty()) {
                output += opts.no_paragraph ? " " : "\n\n";
            }
            output += extra_sentences;
        }
    }

    if (opts.words != -1) {
        int currentWords = count_words(output);
        if (currentWords > opts.words) {
            std::istringstream iss(output);
            std::ostringstream oss;
            std::string word;
            int count = 0;
            while (iss >> word && count < opts.words) {
                if (count > 0) oss << " ";
                oss << word;
                count++;
            }
            output = oss.str();
        } else if (currentWords < opts.words) {
            while (count_words(output) < (size_t)opts.words) {
                auto tokens = grammar.generate_sentence_skeleton();
                std::string sentence = lexicon.fillTokens(tokens);
                output += " " + sentence;
            }
        }
    }

    if (opts.characters != -1) {
        if ((int)output.size() > opts.characters) {
            output = trimToCharacters(output, opts.characters);
        } else if ((int)output.size() < opts.characters) {
            while ((int)output.size() < opts.characters) {
                auto tokens = grammar.generate_sentence_skeleton();
                std::string sentence = lexicon.fillTokens(tokens);
                if (output.empty())
                    output += sentence;
                else
                    output += " " + sentence;
            }
            output = trimToCharacters(output, opts.characters);
        }
    }

    if (opts.byte_exclusive) {
        if ((int)output.size() > opts.bytes) {
            output = trimToBytes(output, opts.bytes);
        } else if ((int)output.size() < opts.bytes) {
            while ((int)output.size() < opts.bytes) {
                auto tokens = grammar.generate_sentence_skeleton();
                std::string sentence = lexicon.fillTokens(tokens);
                output += " " + sentence;
            }
            output = trimToBytes(output, opts.bytes);
        }
    }

    if (opts.no_paragraph) {
        std::replace(output.begin(), output.end(), '\n', ' ');
        output = normalize_spaces(output);
    } else
        output = normalize_with_paragraphs(output);

    if (opts.no_header && output.empty() &&
        opts.paragraphs == -1 && opts.sentences == -1 &&
        opts.words == -1 && opts.characters == -1 && !opts.byte_exclusive) {
            output = generateParagraph(grammar, lexicon, rng);
    }

    std::cout << output << std::endl;
    return EXIT_SUCCESS;
}
