#include "Calculator.h"

bool operator<(const Identifier& left, const Identifier& right)
{
	return left.identifierName < right.identifierName;
}

bool CCalculator::AddVariable(const std::string& newVar)
{
    Identifier newIdentifier;
	newIdentifier.identifierName = newVar;
	if (auto search = m_identifiers.find(newIdentifier);
		search != m_identifiers.end())
	{
		return false; // variable already exist
	}
	newIdentifier.identifierType = IdentifierType::VARIABLE;
	m_identifiers.insert(newIdentifier);
    return true;
}

bool isEqual(double a, double b)
{
	return std::abs(a - b) < std::numeric_limits<double>::epsilon();
}

bool CCalculator::AddVariableWithValue(const std::string& variable, const double value)
{
	Identifier newIdentifier;
	newIdentifier.identifierName = variable;
	newIdentifier.identifierValue = value;
	newIdentifier.identifierType = IdentifierType::VARIABLE;
	if (auto search = m_identifiers.find(newIdentifier);
		search != m_identifiers.end())
	{
		if (isEqual(newIdentifier.identifierValue, value))
		{
			return true;
		}
		m_identifiers.erase(search);
	}
	m_identifiers.insert(newIdentifier);
	return true;
}

bool CCalculator::AddVariableWithValue(const std::string& variable, const std::string& otherVariable)
{
	if (variable == otherVariable)
	{
		return true;
	}
	Identifier oldIdent;
	oldIdent.identifierName = otherVariable;
	if (auto search = m_identifiers.find(oldIdent);
		search != m_identifiers.end())
	{
		if (search->identifierType == IdentifierType::FUNCTION)
		{
			return false;
		}
		Identifier newIdent;
		newIdent.identifierName = variable;
		newIdent.identifierValue = search->identifierValue;
		newIdent.identifierType = IdentifierType::VARIABLE;
		m_identifiers.insert(newIdent);
		return true;
	}
	return false;
}

const std::set<Identifier>& CCalculator::GetAllVariables() const
{
	return m_identifiers;
}

double CCalculator::GetVariableByName(std::string variableName)
{
	Identifier identifier;
	identifier.identifierName = variableName;
	if (auto search = m_identifiers.find(identifier);
		search != m_identifiers.end())
	{
		return search->identifierValue;
	}
	return INFINITY;
}

