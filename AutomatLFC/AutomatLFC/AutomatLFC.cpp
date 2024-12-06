#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include "DeterministicFiniteAutomaton.h"

int main() {
    std::unordered_set<std::string> states = { "q0", "q1", "q2" };
    std::unordered_set<char> input_alphabet = { 'a', 'b' };
    std::unordered_map<std::string, std::unordered_map<char, std::string>> state_transitions = {
        {"q0", {{'a', "q1"}, {'b', "q0"}}},
        {"q1", {{'a', "q1"}, {'b', "q2"}}},
        {"q2", {{'a', "q2"}, {'b', "q2"}}}
    };
    std::string start_state = "q0";
    std::unordered_set<std::string> accepting_states = { "q2" };

    DeterministicFiniteAutomaton dfa(states, input_alphabet, state_transitions, start_state, accepting_states);

    if (dfa.verify()) {
        std::cout << "automatul este valid\n";
    }
    else {
        std::cout << "automatul nu este valid\n";
        return 1;
    }

    dfa.print();

    std::vector<std::string> words = { "aab", "abb", "aabb", "bba" };
    for (const auto& word : words) {
        std::cout << "cuvantul \"" << word << "\" este "
            << (dfa.checkWord(word) ? "acceptat" : "respins") << "\n";
    }

    return 0;
}
