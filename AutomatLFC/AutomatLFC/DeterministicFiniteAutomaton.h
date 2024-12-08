#pragma once
#include "state.h"
#include <unordered_set>
#include <string>
#include <map>
#include <string_view>
#include <set>

class DeterministicFiniteAutomaton
{
public:
    DeterministicFiniteAutomaton();
    DeterministicFiniteAutomaton(const std::unordered_set<State, State::Hash>& states,
        const std::unordered_set<char>& alphabet,
        const std::map<StateSymbolPair, State>& transitions,
        State startState,
        const std::unordered_set<State, State::Hash>& finalStates);

    void createFromRegex(std::string_view regex);
    void print(std::ostream& os) const;

    bool checkWord(const std::string& word) const;
    bool verify() const;

private:
    std::unordered_set<State, State::Hash> m_states;
    std::unordered_set<char> m_alphabet;
    std::map<StateSymbolPair, State> m_transition;
    State m_startState;
    std::unordered_set<State, State::Hash> m_finalStates;
};