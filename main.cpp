#include <iostream>
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

