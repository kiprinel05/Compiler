#include "DeterministicFiniteAutomaton.h"
#include "LambdaNFA.h"
#include <iostream>

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton()
    : m_states{}, m_alphabet{}, m_transition{}, m_startState{ -1 }, m_finalStates{}
{
}

DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const std::unordered_set<State, State::Hash>& states,
    const std::unordered_set<char>& alphabet,
    const std::map<StateSymbolPair, State>& transitions,
    State startState,
    const std::unordered_set<State, State::Hash>& finalStates)
    : m_states{ states }, m_alphabet{ alphabet }, m_transition{ transitions }
    , m_startState{ startState }, m_finalStates{ finalStates }
{
}

void DeterministicFiniteAutomaton::createFromRegex(std::string_view regex)
{
    NFA lambdaAutomaton{};
    lambdaAutomaton.constFromRegex(regex);
    lambdaAutomaton.print();

    *this = lambdaAutomaton.toDFA();
}

void DeterministicFiniteAutomaton::print(std::ostream& os) const
{
    os << "\n- DFA -\nRESULTING TRANSITIONS:\n\n";
    for (const auto& [stateSymbol, state] : m_transition)
    {
        bool isSourceFinal = m_finalStates.contains(stateSymbol.state);
        bool isTargetFinal = m_finalStates.contains(state);

        os << "(q" << stateSymbol.state.id;
        if (isSourceFinal) os << "*";

        os << ", " << stateSymbol.symbol << "): q" << state.id;
        if (isTargetFinal) os << "*";

        os << "\n";
    }
    os << "\n";
}

bool DeterministicFiniteAutomaton::checkWord(const std::string& word) const {
    State currentState = m_startState;  

    for (char symbol : word) {
        auto it = m_transition.find(StateSymbolPair{ currentState, symbol });

        if (it == m_transition.end()) {
            return false;
        }
        currentState = it->second;
    }

    return m_finalStates.find(currentState) != m_finalStates.end();
}

bool DeterministicFiniteAutomaton::verify() const {
    for (const auto& [stateSymbol, state] : m_transition) {
        if (m_states.find(stateSymbol.state) == m_states.end()) {
            return false;
        }
        if (m_alphabet.find(stateSymbol.symbol) == m_alphabet.end()) {
            return false;
        }

        if (m_states.find(state) == m_states.end()) {
            return false;
        }
    }

    if (m_states.find(m_startState) == m_states.end()) {
        return false;
    }

    for (const State& finalState : m_finalStates) {
        if (m_states.find(finalState) == m_states.end()) {
            return false;
        }
    }

    return true;
}
