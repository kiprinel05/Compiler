#pragma once
#include "state.h"
#include "DeterministicFiniteAutomaton.h"
#include <map>
#include <set>
#include <unordered_set>
#include <string_view>

class LambdaNFA
{
public:
    LambdaNFA();

    void constructFromRegex(std::string_view regex);

    DeterministicFiniteAutomaton convertToDFA() const;
    void print() const;

private:
    LambdaNFA constructCharNFA(State startSate, State finalState, char symbol);
    LambdaNFA mergeNFA(const LambdaNFA& first, const LambdaNFA& second);
    LambdaNFA alternateNFA(const LambdaNFA& first, const LambdaNFA& second);
    LambdaNFA kleeneStarNFA(const LambdaNFA& automaton);

    static bool isOperator(char symbol);
    std::unordered_set<State, State::Hash> getLambdaClosure(const std::unordered_set<State, State::Hash>& states) const;
private:
    static constexpr char lambda = '\0';
private:
    std::map<StateSymbolPair, std::unordered_set<State, State::Hash>> m_transition;
    std::unordered_map<State, State, State::Hash> m_equivalentStates;
    std::unordered_set<char> m_alphabet;
    StateSymbolPair m_startState;
    StateSymbolPair m_finalState;
    int m_stateCounter;
};