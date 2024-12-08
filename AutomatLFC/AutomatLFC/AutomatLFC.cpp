#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "DeterministicFiniteAutomaton.h"
#include "state.h"
#include "help.h"

//int main() {
//    State q0(0), q1(1), q2(2);  
//    std::unordered_set<State, State::Hash> states = { q0, q1, q2 };
//
//    std::unordered_set<char> inputAlph = { 'a', 'b' };
//
//    std::map<StateSymbolPair, State> stateTrans = {
//        {StateSymbolPair{q0, 'a'}, q1},
//        {StateSymbolPair{q0, 'b'}, q0},
//        {StateSymbolPair{q1, 'a'}, q1},
//        {StateSymbolPair{q1, 'b'}, q2},
//        {StateSymbolPair{q2, 'a'}, q2},
//        {StateSymbolPair{q2, 'b'}, q2}
//    };
//
//    State start_state(0);
//    std::unordered_set<State, State::Hash> accStates = { q2 };
//    DeterministicFiniteAutomaton dfa(states, inputAlph, stateTrans, start_state, accStates);
//
//    if (dfa.verify()) {
//        std::cout << "[ automatul este valid ]\n";
//    }
//    else {
//        std::cout << "[ automatul nu este valid ]\n";
//        return 1;
//    }
//
//    dfa.print();
//
//    std::vector<std::string> words = { "aab", "abb", "aabb", "bba", "aaaaaaaaaaaaaaaaaaaaaaab" };
//    for (const auto& word : words) {
//        std::cout << "cuvantul \"" << word << "\" este "
//            << (dfa.checkWord(word) ? "acceptat" : "respins") << "\n";
//    }
//
//    return 0;
//}


int main()
{   
    std::ifstream f("input.txt");
    std::string regex;
    f >> regex;
    

    std::cout << regex << "\n\n";

    std::cout << "polish notation: " << polishPostfix(regex) << "\n\n";

    DeterministicFiniteAutomaton dfa{};
    dfa.createFromRegex(polishPostfix(regex));

    dfa.print();

    return 0;
}