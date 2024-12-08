#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "DeterministicFiniteAutomaton.h"
#include "state.h"
#include "help.h"

int main()
{
    std::ifstream input("input.txt");
    std::ofstream output("data.txt");
    std::ofstream wordout("output_word.txt");
    std::ifstream wordinp("word.txt");
    std::string regex;

    input >> regex;
    std::cout << "Regex: " << regex << "\n\n";

    std::cout << "Polish notation: " << polishPostfix(regex) << "\n\n";
    DeterministicFiniteAutomaton dfa{};
    dfa.createFromRegex(polishPostfix(regex));

    dfa.print(std::cout);
    dfa.print(output);
    output.close();

    std::string word;
    while (wordinp >> word) {
        bool isAccepted = dfa.checkWord(word);
        wordout << "Word \"" << word << "\" is " << (isAccepted ? "accepted" : "rejected") << "\n";
    }
    std::cout<<"Checked every word in word.txt and wrote the output in output_word.txt\n";

    wordinp.close();
    wordout.close();

    return 0;
}
