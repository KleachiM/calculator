#ifndef CALCULATOR_CALCULATOR_H
#define CALCULATOR_CALCULATOR_H

#include <set>
#include <string>
#include <cmath>
#include <optional>

enum class IdentifierType
{
	VARIABLE,
	FUNCTION
};

struct Identifier
{
	std::string identifierName;
	IdentifierType identifierType;
//	double identifierValue = NAN;
	std::string identifierValue = "nan";

	bool operator<(const Identifier& left) const
	{
		return identifierName < left.identifierName;
	}
};

class CCalculator
{
public:
    bool AddVariable(const std::string& newVar);

	bool AddVariableWithValue(const std::string& variable, const std::string& value);
	bool AddVariableWithOtherVariableValue(const std::string& variable, const std::string& otherVariable);
//	double GetIdentifierValue(const std::string& identifierName) const;
	double GetVariableValueByName(const std::string variableName) const;

	bool AddFunctionWithVariable(const std::string& functionName, const std::string& variableName);
	bool AddFunctionWithOperation(const std::string& functionName, const std::string& operation);
	double GetFunctionValue(const std::string& functionName) const;
	
	[[nodiscard]] std::optional<IdentifierType> GetIdentifierType(const std::string& identifier) const;
	[[nodiscard]] const std::set<Identifier>& GetAllVariables() const;
private:
    std::set<Identifier> m_identifiers;
};

#endif //CALCULATOR_CALCULATOR_H
