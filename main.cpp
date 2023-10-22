#include <algorithm>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>

bool match_chars_in_group(const std::string &group, const char c) {
  for (char d : group) {
    if (c == d) {
      return true;
    }
  }
  return false;
}

bool match_pattern(const std::string &input_line, const std::string &pattern) {
  // We finished the pattern, so we're done
  if (pattern.empty()) {
    return true;
  }

  // We finished the input line, but not the pattern, so we're done
  if (input_line.empty()) {
    return false;
  }

  if (pattern[pattern.size() - 1] == '$') {
    std::string trimmed_pattern = pattern.substr(0, pattern.size() - 1);

    if (input_line.size() >= trimmed_pattern.size() &&
        input_line.compare(input_line.size() - trimmed_pattern.size(),
                           trimmed_pattern.size(), trimmed_pattern) == 0) {
      return true;
    }

    return false;
  }

  if (pattern.substr(0, 1) == "^") {
    if (input_line.rfind(pattern.substr(1), 0) == std::string::npos) {
      return false;
    }

    return true;
  }

  if (pattern.substr(0, 2) == "\\d") {
    if (std::isdigit(input_line[0]) == 0) {
      return match_pattern(input_line.substr(1), pattern);
    }

    return match_pattern(input_line.substr(1), pattern.substr(2));
  } else if (pattern.substr(0, 2) == "\\w") {
    if (std::isalpha(input_line[0]) == 0) {
      return match_pattern(input_line.substr(1), pattern);
    }

    return match_pattern(input_line.substr(1), pattern.substr(2));
  } else if (pattern.substr(0, 2) == "[^") {
    size_t index = pattern.find_first_of("]");
    if (index == std::string::npos) {
      throw std::runtime_error("Invalid pattern");
    }

    if (match_chars_in_group(pattern.substr(2, index - 2), input_line[0])) {
      return match_pattern(input_line.substr(1), pattern);
    }

    return match_pattern(input_line.substr(1), pattern.substr(index + 1));
  } else if (pattern.substr(0, 1) == "[") {
    size_t index = pattern.find_first_of("]");
    if (index == std::string::npos) {
      throw std::runtime_error("Invalid pattern");
    }
    if (!match_chars_in_group(pattern.substr(1, index - 1), input_line[0])) {
      return match_pattern(input_line.substr(1), pattern);
    }

    return match_pattern(input_line.substr(1), pattern.substr(index + 1));
  } else if (pattern.substr(1, 1) == "+") {
    char c = pattern[0];
    size_t i = 0;

    if (input_line[i] != c) {
      return false;
    }

    while (i < input_line.size() && input_line[i] == c) {
      i++;
    }

    return match_pattern(input_line.substr(i), pattern.substr(2));
  } else if (pattern.substr(1, 1) == "?") {
    char c = pattern[0];

    if (input_line[0] == c) {
      if (input_line[1] == c) {
        return false;
      }

      return match_pattern(input_line.substr(1), pattern.substr(2));
    }

    return match_pattern(input_line, pattern.substr(2));
  } else if (pattern.substr(0, 1) == ".") {
    return match_pattern(input_line.substr(1), pattern.substr(1));
  } else if (pattern.substr(0, 1) == "(") {
    auto index = pattern.find_first_of(")");
    if (index == std::string::npos) {
      throw std::runtime_error("Invalid pattern");
    }

    // strtok the words, spacer is "|"
    std::string words = pattern.substr(1, index - 1);
    std::stringstream ss(words);
    std::string token;
    while (std::getline(ss, token, '|')) {
      if (match_pattern(input_line, token)) {
        return true;
      }
    }
  }

  if (input_line[0] != pattern[0]) {
    return match_pattern(input_line.substr(1), pattern);
  }

  return match_pattern(input_line.substr(1), pattern.substr(1));
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Expected two arguments" << std::endl;
    return 1;
  }

  std::string flag = argv[1];
  std::string pattern = argv[2];

  if (flag != "-E") {
    std::cerr << "Expected first argument to be '-E'" << std::endl;
    return 1;
  }

  std::string input_line;
  std::getline(std::cin, input_line);

  try {
    if (match_pattern(input_line, pattern)) {
      return 0;
    } else {
      return 1;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

