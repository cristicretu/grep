#include <iostream>
#include <string>

bool match_pattern(const std::string &input_line, const std::string &pattern) {
  unsigned int input_index = 0;
  unsigned int pattern_index = 0;
  unsigned int input_size = input_line.size();
  unsigned int pattern_size = pattern.size();

  while (input_index < input_size && pattern_index < pattern_size) {
    std::string command = "";
    bool pattern_index_incremented = false;
    if (pattern[pattern_index] == '\\') {
      command = pattern.substr(pattern_index, 2);
      pattern_index = pattern_index + 2;
      pattern_index_incremented = true;
    } else if (pattern[pattern_index] == '[') {
      while (pattern[pattern_index] != ']') {
        command += pattern[pattern_index];

        pattern_index = pattern_index + 1;
      }
      pattern_index_incremented = true;
      command += ']';
    }

    if (command == "\\d") {
      if (std::isdigit(input_line[input_index]) == 0) {
        return false;
      }
    } else if (command == "\\w") {
      if (std::isalpha(input_line[input_index]) == 0) {
        return false;
      }
    } else if (command.front() == '[' && command.back() == ']') {
      bool negate = command[1] == '^';
      std::string chars_to_match =
          command.substr(negate ? 2 : 1, command.size() - (negate ? 3 : 2));
      for (const auto &c : input_line) {
        if (chars_to_match.find(c) != std::string::npos && !negate) {
          return true;
        } else if (chars_to_match.find(c) == std::string::npos && negate) {
          return true;
        }
      }
    } else {
      if (input_line[input_index] != pattern[pattern_index]) {
        return false;
      }
    }

    input_index = input_index + 1;
    pattern_index =
        pattern_index_incremented ? pattern_index : pattern_index + 1;
  }

  return true;
}

bool recursive_match_pattern(const std::string &input_line,
                             const std::string &pattern) {
  if (match_pattern(input_line, pattern)) {
    return true;
  }
  for (unsigned int i = 0; i < input_line.size(); i++) {
    if (match_pattern(input_line.substr(i), pattern)) {
      return true;
    }
  }

  return false;
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
    if (recursive_match_pattern(input_line, pattern)) {
      return 0;
    } else {
      return 1;
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

