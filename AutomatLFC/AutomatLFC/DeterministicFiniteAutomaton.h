#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>

class DeterministicFiniteAutomaton {
private:
    std::unordered_set<std::string> states;
    std::unordered_set<char> input_alphabet;
    std::unordered_map<std::string, std::unordered_map<char, std::string>> state_transitions;
    std::string start_state;
    std::unordered_set<std::string> accepting_states;

public:
    DeterministicFiniteAutomaton(
        const std::unordered_set<std::string>& states,
        const std::unordered_set<char>& input_alphabet,
        const std::unordered_map<std::string, std::unordered_map<char, std::string>>& state_transitions,
        const std::string& start_state,
        const std::unordered_set<std::string>& accepting_states
    ) : states(states), input_alphabet(input_alphabet), state_transitions(state_transitions),
        start_state(start_state), accepting_states(accepting_states) {
    }

    bool verify() const;

    void print() const;

    bool checkWord(const std::string& word) const;
};

