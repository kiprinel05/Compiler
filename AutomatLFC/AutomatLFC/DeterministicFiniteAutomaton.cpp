#include "DeterministicFiniteAutomaton.h"

bool DeterministicFiniteAutomaton::verify() const {
	if (states.find(start_state) == states.end()) {
		return false;
	}

	for (const auto& state : accepting_states) {
		if (states.find(state) == states.end()) {
			return false;
		}
	}

	for (const auto& [state, transitions_for_state] : state_transitions) {
		if (states.find(state) == states.end()) {
			return false;
		}
		for (const auto& [symbol, target_state] : transitions_for_state) {
			if (input_alphabet.find(symbol) == input_alphabet.end() || states.find(target_state) == states.end()) {
				return false;
			}
		}
	}

	return true;
}

void DeterministicFiniteAutomaton::print() const {
	std::cout << "Multimea starilor: { ";
	for (const auto& state : states) {
		std::cout << state << " ";
	}
	std::cout << "}\n";

	std::cout << "Alfabetul dde intrare: { ";
	for (const auto& symbol : input_alphabet) {
		std::cout << symbol << " ";
	}
	std::cout << "}\n";

	std::cout << "Tranzitii: \n";
	for (const auto& [state, transitions_for_state] : state_transitions) {
		for (const auto& [symbol, target_state] : transitions_for_state) {
			std::cout << "   (" << state << ", " << symbol << ") => " << target_state << "\n";
		}
	}

	std::cout << "Starea Initiala: " << start_state << "\n";

	std::cout << "Starile Finale: { ";
	for (const auto& state : accepting_states) {
		std::cout << state << " ";
	}
	std::cout << "}\n";
}
bool DeterministicFiniteAutomaton::checkWord(const std::string& word) const
{
	std::string current_state = start_state;

	for (char symbol : word) {
		if (input_alphabet.find(symbol) == input_alphabet.end()) {
			return false;
		}

		if (state_transitions.at(current_state).find(symbol) != state_transitions.at(current_state).end()) {
			current_state = state_transitions.at(current_state).at(symbol);
		}
		else {
			return false;
		}
	}

	return accepting_states.find(current_state) != accepting_states.end();
}