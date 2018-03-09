#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>

enum DelimiterTypes {
  begin, begin_print, end
};

void run(std::istream &in, std::ostream &out, std::vector<std::string> delimiters = {"<%", "=", "%>"}) {
  out << "#ifndef OUT" << std::endl;
  out << "#define OUT out" << std::endl;
  out << "#endif" << std::endl;
  out << "{" << std::endl; // separate scope for the file
  std::string buf;
  in >> std::noskipws;
  char c;
  bool code_active = false;
  bool print_active = false;
  while (in >> c) {
    if (!code_active && !print_active) {
      if (c == '\n') {
        buf += "\\n";
      } else if (c == '"') {
        buf += "\\\"";
      } else {
        buf.push_back(c);
      }
    } else {
      buf.push_back(c);
    }
    for (int type = 0; type < 3; ++type) {
      std::string &delim = delimiters[type];
      // Prevents mathcing normal = inside code
      if (type == begin_print && buf.size() != delim.size()) continue;
      bool match = 0;
      if (
        buf.size() >= delim.size() &&
        buf.compare(buf.size()-delim.size(), delim.size(), delim) == 0 &&
        // escaping
        !(buf.size() > delim.size() && buf[buf.size()-delim.size()-1] == '\\')
      ) {
        match = true;
      }
      if (match) {
        switch (type) {
          case begin:
            if (code_active || print_active) break;
            buf.resize(buf.size()-delim.size());
            code_active = true;
            out << "OUT << u8\"" << buf << "\";" << std::endl;
            buf.clear();
          break;
          case begin_print:
            if (!code_active || print_active) break;
            code_active = false;
            print_active = true;
            buf.clear();
          break;
          case end:
            buf.resize(buf.size()-delim.size());
            if (code_active) {
              code_active = false;
              out << buf << ";" << std::endl;
              buf.clear();
            } else if (print_active) {
              print_active = false;
              out << "OUT << (" << buf << ");" << std::endl;
              buf.clear();
            }
          break;
        }
      }
    }
  }
  // Print rest of file
  out << "OUT << u8\"" << buf << "\";" << std::endl;
  buf.clear();
  // Close the scope
  out << "}" << std::endl;
  out << "#undef OUT" << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Please specify files to parse (or --)." << std::endl;
    return 1;
  }
  bool dnext = false;
  bool dset = false;
  std::vector<std::string> d;
  for (int vi = 1; vi < argc; ++vi) {
    std::string arg = argv[vi];
    if (dnext) {
      dnext = false;
      dset = true;
      d.clear();
      d.push_back("");
      for (char c : arg) {
        if (c == ' ') {
          d.push_back("");
        } else {
          d.back().push_back(c);
        }
      }
      continue;
    }
    if (arg == "-d" || arg == "--delimiters") {
      dnext = true;
    } else if (arg == "--") {
      if (dset) run(std::cin, std::cout, d);
      else run(std::cin, std::cout);
    } else {
      std::ifstream in(arg);
      std::ofstream out(arg + ".cpp");
      if (dset) run(in, out, d);
      else run(in, out);
    }
  }
}
