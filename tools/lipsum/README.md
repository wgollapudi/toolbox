# lipsum

**lipsum** is a command‑line tool for generating customizable Lorem Ipsum text. It leverages a procedural grammar generation system to create pseudo‑Latin sentences that mimic classical Latin grammar structures.

---

## Overview

**lipsum** allows you to generate filler text with precise control over:

- **Paragraphs, Sentences, Words, Characters, or Bytes:**
  Specify the exact quantity of text you need.
- **Output Formatting:**
  Support for plain text (default), HTML, and Markdown formatting (future extension).
- **Sentence Complexity Modes:**
  Toggle between simple sentences (single‑clause, simple verb phrases, no prepositional phrases) and complex sentences (multiple clauses, complex verb phrases with object noun phrases, and mandatory prepositional phrases).
- **Reproducible Output:**
  Use a central random engine with a user‑specified seed (`--seed`) to ensure reproducibility.

---

## Procedural Grammar Generation

The core of **lipsum** is its procedural grammar generator. This system employs a recursive, context‑free grammar to construct sentence skeletons. The process is as follows:

1. **Grammar Tree Construction:**
   - The generator builds a tree structure where each node represents a grammatical construct:
     - **Sentence:** Composed of one or more clauses.
     - **Clause:** Consists of a Noun Phrase (NP) and a Verb Phrase (VP), with an optional Prepositional Phrase (PP).
     - **Noun Phrase (NP):** Optionally includes one or more adjectives (with a weighted probability of 0, 1, or 2 adjectives) followed by a noun.
     - **Verb Phrase (VP):** Can be either simple (verb only) or complex (verb followed by an object NP), depending on the selected mode.
     - **Prepositional Phrase (PP):** An optional component appended to clauses with a specified probability.

2. **Mode Selection:**
   - **Simple Mode (`--simple`):** Generates single‑clause sentences with simple verb phrases and omits prepositional phrases.
   - **Complex Mode (`--complex`):** Forces multi‑clause sentences with complex verb phrases and always includes a prepositional phrase.
   - **Normal Mode:** Uses configurable probabilities (defined in `config.hpp`) to determine clause count, adjective usage, and PP inclusion.
   - _I might add more later, who knows._

3. **Flattening & Lexicon Processing:**
   - Once the grammar tree is constructed, it is flattened into a linear sequence of tokens.
   - The **Lexicon** class then fills these tokens with actual words, ensuring that adjectives agree in gender with their associated nouns.
   - A central random engine (seedable via `--seed`) guarantees consistent and reproducible output.

---

## Dictionary Generation

The dictionary used by **lipsum** is comprised of approximately 600 of the most common Latin words, annotated with their parts of speech and grammatical gender. This dictionary is hardcoded into the tool and stored as an `unordered_map` for fast lookup.

### Scraping Methodology

The dictionary was created by scraping the webpage [Most Common Latin Words](https://travelwithlanguages.com/blog/most-common-latin-words.html). The process involved:

- **Fetching the Webpage:**
  Using Python's `requests` library to download the HTML content.
- **Parsing HTML Content:**
  Utilizing BeautifulSoup to locate and extract data from the ordered list (`<ol id="main-list">`) containing the words.
- **Data Extraction:**
  Iterating over `<li>` elements to capture the word (from `<div class="vocab">`) and its part of speech (from the class attribute).
- **Gender Guessing:**
  Applying heuristic rules to determine the grammatical gender of nouns and adjectives.
- **Output Formatting:**
  The scraped data is then output in a format suitable for initializing the C++ map.

For full details, see the [src/latin_dict_scraper.py](src/latin_dict_scraper.py) script.

---

## Usage

```
Usage:
  lipsum [options]

Options:
  -h                  Print this help message.
  --seed <number>     Set the random seed for reproducibility.
  --simple            Force simple sentences (one clause, simple VP, no PP).
  --complex           Force complex sentences (max clauses, complex VP, always PP).
  --no-paragraph      Do not insert paragraph delimiters (output is one big block).
  --no-header         Do not include the original Lorem Ipsum text.
  --history           Display the history of Lorem Ipsum and exit.
  -p <number>         Ensure at least this many paragraphs (header counts as one if present).
  -s <number>         Ensure that this many additional sentences are produced.
  -w <number>         Ensure that this many additional words are produced.
  -c <number>         Ensure that this many additional characters are produced.
  -b <number>         (Exclusive) Final output is exactly this many bytes.
```

- **Header Behavior:**
  The tool prints the original Lorem Ipsum header unless `--no-header` is specified. If output limits are provided (e.g., `-p`, `-s`, `-w`, `-c`), the header is trimmed or extended to meet the requirements.

- **Sizing Flags:**
  Flags with an argument of 0 are ignored (i.e., treated as if they were not provided). The `-b` flag is exclusive and cannot be combined with other sizing flags.

---

## Build & Installation

This project uses a Makefile for building, installing, and uninstalling the binary.

- **Compile:**

  ```bash
  make
  ```
- **Install:**

  ```bash
  make install
  ```

- **Uninstall:**

  ```bash
  make uninstall
  ```

---

## Acknowledgements

- The dictionary data is sourced from [Travel With Languages](https://travelwithlanguages.com/blog/most-common-latin-words.html).
- The Lorem Ipsum text and history are based on public domain sources.

---

Feedback, contributions, and improvements are welcome but this is mostly a useless shitter project. Email me if you find this though, I'm curious to see if anyone reads this. If you find ts, email me.
