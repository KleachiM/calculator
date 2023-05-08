#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <set>
#include <string>
#include <cmath>

enum class IdentifierType
{
	VARIABLE,
	FUNCTION
};

struct Identifier
{
	std::string identifierName;
	IdentifierType identifierType;
	double identifierValue = NAN;
};

class CCalculator
{
public:
    bool AddVariable(const std::string& newVar);

	bool AddVariableWithValue(const std::string& variable, const double value);
	bool AddVariableWithValue(const std::string& variable, const std::string& otherVariable);

	double GetVariableByName(std::string variableName);

	const std::set<Identifier>& GetAllVariables() const;
private:
    std::set<Identifier> m_identifiers;
};

#endif //CALCULATOR_CALCULATOR_H
