#pragma once
#include "state.h"
#include "DeterministicFiniteAutomaton.h"
#include <map>
#include <set>
#include <unordered_set>
#include <string_view>

class NFA
{
public:
    NFA();

    void constFromRegex(std::string_view regex);

    DeterministicFiniteAutomaton toDFA() const;
    void print() const;

private:
    NFA constructNFA(State startSate, State finalState, char symbol);
    NFA mergeNFA(const NFA& first, const NFA& second);
    NFA alternateNFA(const NFA& first, const NFA& second);
    NFA kleeneStarNFA(const NFA& automaton);

    static bool isOperator(char symbol);
    std::unordered_set<State, State::Hash> getLambdaCls(const std::unordered_set<State, State::Hash>& states) const;
private:
    static constexpr char lambda = '\0';
private:
    std::map<StateSymbolPair, std::unordered_set<State, State::Hash>> m_transition;
    std::unordered_map<State, State, State::Hash> m_eqStates;
    std::unordered_set<char> m_alphabet;
    StateSymbolPair m_startState;
    StateSymbolPair m_finalState;
    int m_stateCounter;
};