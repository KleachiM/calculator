#include "Calculator.h"

using namespace std;

bool CCalculator::AddVariable(const string& newVar)
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
	return abs(a - b) < numeric_limits<double>::epsilon();
}

bool CCalculator::AddVariableWithValue(const string& variable, const double value)
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

bool CCalculator::AddVariableWithValue(const string& variable, const string& otherVariable)
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
		if (search->identifierType == IdentifierType::VARIABLE)
		{
			Identifier newIdent;
			newIdent.identifierName = variable;
			newIdent.identifierValue = search->identifierValue;
			newIdent.identifierType = IdentifierType::VARIABLE;
			m_identifiers.insert(newIdent);
			return true;
		}
	}
	return false;
}

const std::set<Identifier>& CCalculator::GetAllVariables() const
{
	return m_identifiers;
}

double CCalculator::GetVariableValueByName(std::string variableName)
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

optional<IdentifierType> CCalculator::GetIdentifierType(const string& identifierName) const
{
	Identifier identifier;
	identifier.identifierName = identifierName;
	if (auto search = m_identifiers.find(identifier);
		search != m_identifiers.end())
	{
		return search->identifierType;
	}
	return nullopt;
}

bool CCalculator::AddFunctionWithVariable(const std::string& functionName, const std::string& variableName)
{
	if (functionName == variableName)
	{
		return false;
	}
	Identifier identifierVar;
	identifierVar.identifierName = variableName;
	if (auto search = m_identifiers.find(identifierVar);
		search != m_identifiers.end())
	{
		if (search->identifierType == IdentifierType::VARIABLE)
		{
			Identifier idenfifierFunc;
			idenfifierFunc.identifierName = functionName;
			idenfifierFunc.identifierType = IdentifierType::FUNCTION;
			idenfifierFunc.identifierValue = search->identifierValue;
			m_identifiers.insert(idenfifierFunc);
			return true;
		}
	}
	return false;
}
