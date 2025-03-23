#!/usr/bin/env python3
"""
latin_dict_scraper.py

This script scrapes the "Most Common Latin Words" webpage from:
    https://travelwithlanguages.com/blog/most-common-latin-words.html

It extracts Latin words and their parts of speech, applies simple heuristics to guess
gender for nouns and adjectives, and formats the results as C++ dictionary initializers.
The formatted output is written to a file "dict.txt".

Usage:
    python latin_dict_scraper.py
"""

import requests
from bs4 import BeautifulSoup
import pandas as pd


def fetch_webpage(url: str) -> bytes:
    """
    Fetch the content of the webpage from the given URL.

    Args:
        url (str): The URL of the webpage.

    Returns:
        bytes: The content of the webpage.

    Raises:
        requests.HTTPError: If the request fails.
    """
    response = requests.get(url)
    response.raise_for_status()
    return response.content


def parse_latin_words(html_content: bytes) -> pd.DataFrame:
    """
    Parse HTML content to extract Latin words and their parts of speech.

    The function looks for an ordered list with id 'main-list' and then extracts
    each word (from the <div class="vocab"> element) and its POS (from the <li>
    element's class attribute).

    Args:
        html_content (bytes): The HTML content of the webpage.

    Returns:
        pd.DataFrame: DataFrame containing 'word' and 'pos' columns.

    Raises:
        ValueError: If the expected ordered list is not found.
    """
    soup = BeautifulSoup(html_content, "html.parser")
    ol = soup.find("ol", id="main-list")
    if not ol:
        raise ValueError("Could not find <ol id='main-list'> on the page.")

    word_list = []
    for li in ol.find_all("li"):
        classes = li.get("class", [])
        pos = classes[0] if classes else "unknown"
        vocab_div = li.find("div", class_="vocab")
        if vocab_div:
            word_text = vocab_div.get_text(strip=True)
            word_list.append({"word": word_text, "pos": pos})
        else:
            print("Warning: No <div class='vocab'> found in one of the <li> elements.")

    df = pd.DataFrame(word_list)
    df['pos'] = df['pos'].replace('proper', 'noun')
    return df


def guess_gender(word: str, pos: str) -> str:
    """
    Guess the gender of a word based on simple heuristics.

    For nouns and adjectives:
      - Words ending in "a" are assumed to be feminine.
      - Words ending in "us", "er", or "or" are assumed to be masculine.
      - Words ending in "um" are assumed to be neuter.
      - Otherwise, return 'x' (unknown).

    For other parts of speech, returns 'x'.

    Args:
        word (str): The Latin word.
        pos (str): The part of speech.

    Returns:
        str: 'm' for masculine, 'f' for feminine, 'n' for neuter, or 'x' for unknown.
    """
    w = word.strip().lower()
    if pos in ['noun', 'adjective']:
        if w.endswith("a"):
            return "f"
        elif w.endswith("us") or w.endswith("er") or w.endswith("or"):
            return "m"
        elif w.endswith("um"):
            return "n"
        else:
            return "x"
    else:
        return "x"


def process_dataframe(df: pd.DataFrame) -> pd.DataFrame:
    """
    Filter the DataFrame by allowed parts of speech and apply gender heuristics.

    Allowed parts of speech are: verb, noun, adjective, adverb, preposition, and conjunction.

    Args:
        df (pd.DataFrame): DataFrame with columns 'word' and 'pos'.

    Returns:
        pd.DataFrame: Processed DataFrame with an added 'gender' column.
    """
    allowed_pos = {'verb', 'noun', 'adjective', 'adverb', 'preposition', 'conjunction'}
    df = df[df['pos'].isin(allowed_pos)]
    df['gender'] = df.apply(lambda row: guess_gender(row['word'], row['pos']), axis=1)
    return df


def format_dictionary_entries(df: pd.DataFrame) -> str:
    """
    Format the DataFrame into C++ dictionary initializer entries.

    Iterates over each allowed part of speech and formats the words into a string.

    Args:
        df (pd.DataFrame): DataFrame with columns 'word', 'pos', and 'gender'.

    Returns:
        str: The formatted dictionary as a single string.
    """
    allowed_pos = {'verb', 'noun', 'adjective', 'adverb', 'preposition', 'conjunction'}
    output_lines = []

    for pos in allowed_pos:
        group = df[df['pos'].str.strip().str.lower() == pos]
        pos_upper = pos.upper()  # Convert to uppercase to match C++ enum naming convention.
        output_lines.append(f"dict[POS::{pos_upper}] = {{")

        for _, row in group.iterrows():
            word = row['word']
            gender = row['gender']
            output_lines.append(f'    {{"{word}", POS::{pos_upper}, \'{gender}\' }},')

        output_lines.append("};\n")

    return "\n".join(output_lines)


def main():
    """
    Main function to scrape Latin words and write the formatted dictionary to 'dict.txt'.
    """
    url = "https://travelwithlanguages.com/blog/most-common-latin-words.html"
    try:
        html_content = fetch_webpage(url)
    except Exception as e:
        print(f"Error fetching webpage: {e}")
        return

    try:
        df = parse_latin_words(html_content)
    except ValueError as ve:
        print(f"Error parsing HTML: {ve}")
        return

    df = process_dataframe(df)
    dictionary_str = format_dictionary_entries(df)

    output_filename = "dict.txt"
    try:
        with open(output_filename, "w", encoding="utf-8") as f:
            f.write(dictionary_str)
        print(f"Dictionary successfully written to {output_filename}")
    except IOError as io_err:
        print(f"Error writing to file {output_filename}: {io_err}")


if __name__ == '__main__':
    main()

