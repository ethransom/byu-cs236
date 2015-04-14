#include "database.h"

#include <stdio.h>

void Database::flatten(std::ostream& os) const {
	for (auto relation : relations) {
		os << relation.first << " ";
		relation.second->flatten(os);
	}
}

void Database::create(Scheme& scheme) {
	relations[scheme.identifier] = 	new Relation(scheme.param_list);
}

void Database::insert(Fact& fact) {
	relations[fact.identifier]->insert(fact.param_list);
}

bool Database::interpret(Rule& rule) {
	Relation result = query(rule.predicate_list[0]);

	for (uint i = 1; i < rule.predicate_list.size(); i++) {
		Relation intermediate = query(rule.predicate_list[i]);

		result.join(intermediate);
	}

	return relations[rule.scheme.identifier]->merge(result, rule.scheme.param_list);
}

Relation Database::query(Predicate& query) {
	// copy? hopefully?
	Relation rel = *relations[query.identifier];

	std::vector<std::string*> projects;

	uint pos = 0;
	for (auto& param : query.param_list) {
		if (param->type == IDENTIFIER) {
			projects.push_back(&param->identifier.str);
		} else if (param->type == LITERAL) {
			rel.select(&param->literal.str, pos);
			projects.push_back(NULL);
		} else if (param->type == EXPRESSION) {
			std::cout << "I was promised no expressions!" << std::endl;
			throw "EXPRESSION! :( ";
		}

		pos++;
	}

	for (uint pos = 0; pos < projects.size(); pos++) {
		if (projects[pos] != NULL) {
			rel.rename(projects[pos], pos);

			// have we encountered this before?
			for (int i = pos - 1; i >= 0; i--) {
				if (projects[i] != NULL && *projects[i] == *projects[pos]) {
					// select between
					rel.select(i, pos);

					projects.erase(projects.begin() + pos);
					pos--;
					rel.project(pos);
				}
			}
		}
	}

	for (int i = projects.size() - 1; i >= 0; i--) {
		if (projects.at(i) == NULL)
			rel.project(i);
	}

	return rel;
}