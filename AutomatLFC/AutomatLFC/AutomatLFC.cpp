#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "DeterministicFiniteAutomaton.h"
#include "state.h"

int main() {
    State q0(0), q1(1), q2(2);  // Considerăm că State are un constructor ce primește un int (ID-ul stării)

    // Setul de stări
    std::unordered_set<State, State::Hash> states = { q0, q1, q2 };

    // Alfabetul de intrare
    std::unordered_set<char> input_alphabet = { 'a', 'b' };

    // Transițiile automatei
    std::map<StateSymbolPair, State> state_transitions = {
        {StateSymbolPair{q0, 'a'}, q1},
        {StateSymbolPair{q0, 'b'}, q0},
        {StateSymbolPair{q1, 'a'}, q1},
        {StateSymbolPair{q1, 'b'}, q2},
        {StateSymbolPair{q2, 'a'}, q2},
        {StateSymbolPair{q2, 'b'}, q2}
    };

    // Starea de start
    State start_state(0);

    // Stările de acceptare
    std::unordered_set<State, State::Hash> accepting_states = { q2 };

    // Crearea automatului determinist
    DeterministicFiniteAutomaton dfa(states, input_alphabet, state_transitions, start_state, accepting_states);

    // Verificarea dacă automata este validă
    if (dfa.verify()) {
        std::cout << "Automatul este valid.\n";
    }
    else {
        std::cout << "Automatul nu este valid.\n";
        return 1;
    }

    // Afișarea detaliilor despre automat
    dfa.print();

    // Verificarea unor cuvinte
    std::vector<std::string> words = { "aab", "abb", "aabb", "bba", "aaaaaaaaaaaaaaaaaaaaaaab" };
    for (const auto& word : words) {
        std::cout << "Cuvântul \"" << word << "\" este "
            << (dfa.checkWord(word) ? "acceptat" : "respins") << "\n";
    }

    return 0;
}
