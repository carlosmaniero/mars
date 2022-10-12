#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "mclexer/lexer.h"
#include "mcparser/parser.h"
#include "spdlog/spdlog.h"

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug);

    std::string compilerName = argv[0];

    if (argc < 2) {
        spdlog::error("you must provide a filename: " + compilerName + " filename.mars");
        return 1;
    }

    std::ifstream inFile;
    inFile.open(argv[1]);

    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string sourceStr = strStream.str();

    spdlog::info("mc parser");

    spdlog::info("Parsing: " + sourceStr);

    mclexer::Lexer lexer(&sourceStr);
    mcparser::Parser parser;

    auto tokens = lexer.tokenize();
    parser.parse(std::move(tokens));

    if (parser.getErrors().size() > 0) {
        for (int i = 0; i < parser.getErrors().size(); i++) {
            auto error = parser.getErrors().at(i);
            spdlog::error("Parsing error: ");
            spdlog::error(error.message);
            spdlog::error("Line: " + std::to_string(error.location.line));
            spdlog::error("Column: " + std::to_string(error.location.column));
        }
        return 1;
    }
}
