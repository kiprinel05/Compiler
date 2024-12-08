#include "LambdaNFA.h"
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <stdexcept>

NFA::NFA()
    : m_transition{}, m_stateCounter{ 0 }, m_startState{ -1, '\0' }, m_finalState{ -1, '\0' }, m_alphabet{}
{
}

void NFA::constFromRegex(std::string_view regex)
{
    std::stack<NFA> automatons;

    for (char symbol : regex)
    {
        if (!isOperator(symbol))
        {
            if (!m_alphabet.contains(symbol))
            {
                m_alphabet.insert(symbol);
            }

            State startState = { m_stateCounter++ };
            State endState = { m_stateCounter++ };

            NFA charNFA{
                constructNFA(
                startState,
                endState,
                symbol)
            };
            automatons.push(charNFA);
            continue;
        }

        if (symbol == '*')
        {
            NFA automaton{ automatons.top() };
            automatons.pop();

            automatons.push(kleeneStarNFA(automaton));
            continue;
        }

        NFA second{ automatons.top() };
        automatons.pop();
        NFA first{ automatons.top() };
        automatons.pop();
        if (symbol == '.')
        {
            NFA mergedNFA{ mergeNFA(first, second) };
            automatons.push(mergedNFA);
        }
        else if (symbol == '|')
        {
            NFA alternatedNFA{ alternateNFA(first, second) };
            automatons.push(alternatedNFA);
        }
    }

    NFA result = automatons.top();
    result.m_alphabet = m_alphabet;
    automatons.pop();

    *this = result;
    std::cout << "startState: " << this->m_startState.state.id << "\n";
    std::cout << "finalState: " << this->m_finalState.state.id << "\n\n";

    auto lambdaClosure = getLambdaCls(std::unordered_set<State, State::Hash>{State{ 19 }});
    std::cout << "Lambda closure q19: ";
    for (State state : lambdaClosure)
    {
        std::cout << state.id << " ";
    }
    std::cout << "\n";

    lambdaClosure = getLambdaCls(std::unordered_set<State, State::Hash>{State{ 24 }});
    std::cout << "Lambda closure q24: ";
    for (State state : lambdaClosure)
    {
        std::cout << state.id << " ";
    }
    std::cout << "\n";

    lambdaClosure = getLambdaCls(std::unordered_set<State, State::Hash>{State{ 1 }});
    std::cout << "Lambda closure q1: ";
    for (State state : lambdaClosure)
    {
        std::cout << state.id << " ";
    }
    std::cout << "\n\n";
}

DeterministicFiniteAutomaton NFA::toDFA() const
{
    std::unordered_set<State, State::Hash> states;
    std::unordered_set<char> alphabet{ m_alphabet };
    std::map<StateSymbolPair, State> rezTrans;
    State startState;
    std::unordered_set<State, State::Hash> finalStates;

    std::vector<std::unordered_set<State, State::Hash>> rezStates;
    std::queue<std::unordered_set<State, State::Hash>> closToCheck;
    std::unordered_set<State, State::Hash> startClosure = getLambdaCls({ m_startState.state });
    closToCheck.push(startClosure);
    rezStates.push_back(closToCheck.front());

    if (startClosure.contains(m_finalState.state))
    {
        finalStates.insert(State{ 0 });
    }

    while (!closToCheck.empty())
    {
        std::unordered_set<State, State::Hash> currentClos = closToCheck.front();
        closToCheck.pop();

        for (char symbol : m_alphabet)
        {
            std::unordered_set<State, State::Hash> currentTrans;
            for (State state : currentClos)
            {
                if (m_transition.contains({ state, symbol }))
                {
                    currentTrans.insert(
                        m_transition.at({ state, symbol }).begin(),
                        m_transition.at({ state, symbol }).end()
                    );
                }
            }

            std::unordered_set<State, State::Hash> nextClos = getLambdaCls(currentTrans);

            if (nextClos.empty()) continue;

            int currentStID = std::distance(rezStates.begin(),
                std::find(rezStates.begin(), rezStates.end(), currentClos));

            int targetStID;
            auto it = std::find(rezStates.begin(), rezStates.end(), nextClos);
            if (it == rezStates.end())
            {
                targetStID = rezStates.size();
                rezStates.push_back(nextClos);
                closToCheck.push(nextClos);

                if (nextClos.contains(m_finalState.state))
                {
                    finalStates.insert(State{ targetStID });
                }
            }
            else
            {
                targetStID = std::distance(rezStates.begin(), it);
            }

            rezTrans[{State{ currentStID }, symbol}] = { State{ targetStID } };
        }
    }

    for (int resStID = 0; resStID < rezStates.size(); ++resStID)
    {
        states.insert(State{ resStID });
    }

    startState = State{ 0 };

    std::cout << "\n- LAMBDA NFA -\n RESULTING STATES\n";
    int stateID = 0;
    for (const auto& resState : rezStates)
    {
        std::cout << "q" << stateID++ << ": ";
        for (const auto& state : resState)
        {
            std::cout << state.id << " ";
        }
        std::cout << "\n";
    }

    return DeterministicFiniteAutomaton{ states, alphabet, rezTrans, startState, finalStates };
}

std::unordered_set<State, State::Hash> NFA::getLambdaCls(const std::unordered_set<State, State::Hash>& states) const
{
    std::unordered_set<State, State::Hash> lambdaClosure;
    std::stack<State> toCheck;
    for (State state : states)
    {
        toCheck.push(state);
        if (m_eqStates.contains(state))
        {
            toCheck.push(m_eqStates.at(state));
        }
    }

    while (!toCheck.empty())
    {
        State state = toCheck.top();
        lambdaClosure.insert(state);
        toCheck.pop();

        if (m_transition.find({ state, lambda }) == m_transition.end())
        {
            continue;
        }

        std::unordered_set<State, State::Hash> reachable = m_transition.at({ state, lambda });
        for (State state : reachable)
        {
            if (lambdaClosure.contains(state))
            {
                continue;
            }

            toCheck.push(state);
        }
    }

    return lambdaClosure;
}

void NFA::print() const
{
    for (const auto& [stateSymbol, states] : m_transition)
    {
        std::cout << "(q" << stateSymbol.state.id << ", " << stateSymbol.symbol << "): ";
        for (const auto& state : states)
        {
            std::cout << "q" << state.id << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

NFA NFA::constructNFA(State startSate, State finalState, char symbol)
{
    NFA result;
    result.m_transition[{startSate, symbol}].insert(finalState);

    result.m_startState = { startSate, symbol };
    result.m_finalState = { finalState, lambda };

    return result;
}

NFA NFA::mergeNFA(const NFA& first, const NFA& second)
{
    NFA result{ first };
    result.m_eqStates.insert(first.m_eqStates.begin(), first.m_eqStates.end());
    result.m_eqStates.insert(second.m_eqStates.begin(), second.m_eqStates.end());

    result.m_transition[{result.m_finalState.state, second.m_startState.symbol}] = second.m_transition.at(second.m_startState);

    for (const auto& [stateSymbol, states] : second.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    for (const auto& [stateSymbol, states] : first.m_transition)
    {
        if (states.contains(first.m_finalState.state))
        {
            result.m_transition[stateSymbol].insert(second.m_startState.state);
        }
    }

    result.m_startState = first.m_startState;
    result.m_finalState = second.m_finalState;

    result.m_eqStates[first.m_finalState.state] = second.m_startState.state;
    result.m_eqStates[second.m_startState.state] = first.m_finalState.state;

    return result;
}

NFA NFA::alternateNFA(const NFA& first, const NFA& second)
{
    NFA result;
    result.m_eqStates.insert(first.m_eqStates.begin(), first.m_eqStates.end());
    result.m_eqStates.insert(second.m_eqStates.begin(), second.m_eqStates.end());

    result.m_startState = { m_stateCounter++, lambda };
    result.m_finalState = { m_stateCounter++, lambda };

    result.m_transition[result.m_startState].insert(first.m_startState.state);
    result.m_transition[result.m_startState].insert(second.m_startState.state);

    result.m_transition[first.m_finalState].insert(result.m_finalState.state);
    result.m_transition[second.m_finalState].insert(result.m_finalState.state);

    for (const auto& [stateSymbol, states] : first.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    for (const auto& [stateSymbol, states] : second.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    return result;
}

NFA NFA::kleeneStarNFA(const NFA& automaton)
{
    NFA result;
    result.m_eqStates.insert(automaton.m_eqStates.begin(), automaton.m_eqStates.end());

    result.m_eqStates = automaton.m_eqStates;
    result.m_startState = { m_stateCounter++, lambda };
    result.m_finalState = { m_stateCounter++, lambda };

    for (const auto& [stateSymbol, states] : automaton.m_transition)
    {
        result.m_transition.insert({ stateSymbol, states });
    }

    result.m_transition[result.m_startState].insert(automaton.m_startState.state);
    result.m_transition[result.m_startState].insert(result.m_finalState.state);
    result.m_transition[automaton.m_finalState].insert(automaton.m_startState.state);
    result.m_transition[automaton.m_finalState].insert(result.m_finalState.state);

    return result;
}

bool NFA::isOperator(char symbol)
{
    std::string operators = "|.*";
    return operators.find(symbol) != std::string::npos;
}
