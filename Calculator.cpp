#include "Calculator.h"
#include <cmath>
#include <regex>

using namespace std;

double GetIdentifierValue(const string& identifierValue)
{
	try
	{
		return stod(identifierValue);
	}
	catch (exception& e)
	{
		return NAN;
	}
}

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

bool IsEqual(double a, double b)
{
	return abs(a - b) < numeric_limits<double>::epsilon();
}

bool CCalculator::AddVariableWithValue(const string& variable, const string& value)
{
	Identifier newIdentifier;
	newIdentifier.identifierName = variable;
	newIdentifier.identifierValue = value;
	newIdentifier.identifierType = IdentifierType::VARIABLE;
	if (auto search = m_identifiers.find(newIdentifier);
		search != m_identifiers.end())
	{
		if (newIdentifier.identifierValue == value)
		{
			return true;
		}
		m_identifiers.erase(search);
	}
	m_identifiers.insert(newIdentifier);
	return true;
}

bool CCalculator::AddVariableWithOtherVariableValue(const string& variable, const string& otherVariable)
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
		if (search->identifierType != IdentifierType::VARIABLE)
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

double CCalculator::GetVariableValueByName(const string variableName) const
{
	Identifier identifier;
	identifier.identifierName = variableName;
	if (auto search = m_identifiers.find(identifier);
		search != m_identifiers.end())
	{
		return GetIdentifierValue(search->identifierValue);
	}
	return NAN;
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

bool CCalculator::AddFunctionWithOperation(const string& functionName, const string& operation)
{
	Identifier functionToAdd {functionName};
	if (auto search = m_identifiers.find(functionToAdd);
		search != m_identifiers.end())
	{
		m_identifiers.erase(search);
	}
	functionToAdd.identifierType = IdentifierType::FUNCTION;
	functionToAdd.identifierValue = operation;
	m_identifiers.insert(functionToAdd);
	return true;
}

double GetOperationResult(double operand1, const string& operation, double operand2)
{
	if (operation == "+")
	{
		return operand1 + operand2;
	}
	if (operation == "-")
	{
		return operand1 - operand2;
	}
	if (operation == "*")
	{
		return operand1 * operand2;
	}
	if (operation == "/")
	{
		if (IsEqual(operand2, 0))
		{
			return INFINITY;
		}
		return operand1 / operand2;
	}
}

double CCalculator::GetFunctionValue(const string& functionName) const
{
	Identifier functionToFind{functionName};
	if (auto search = m_identifiers.find(functionToFind);
		search != m_identifiers.end())
	{
		regex rgx(R"(([a-zA-Z_][a-zA-Z0-9_]*)([+-/*])([a-zA-Z_][a-zA-Z0-9_]*))");
		smatch submatch;
		if (!regex_match(search->identifierValue, submatch, rgx))
		{
			return GetVariableValueByName(search->identifierValue);
		}
		auto firstOperandType = GetIdentifierType(submatch[1]);
		double firstOperandValue, secondOperandValue;
		if (!firstOperandType)
		{
			return NAN;
		}
		if (firstOperandType == IdentifierType::VARIABLE)
		{
			firstOperandValue = GetVariableValueByName(submatch[1]);
		}
		if (firstOperandType == IdentifierType::FUNCTION)
		{
			firstOperandValue = GetFunctionValue(submatch[1]);
		}

		auto secondOperandType = GetIdentifierType(submatch[3]);
		if (!secondOperandType)
		{
			return NAN;
		}
		if (secondOperandType == IdentifierType::VARIABLE)
		{
			secondOperandValue = GetVariableValueByName(submatch[3]);
		}
		if (secondOperandType == IdentifierType::FUNCTION)
		{
			secondOperandValue = GetFunctionValue(submatch[3]);
		}
		return GetOperationResult(firstOperandValue, submatch[2], secondOperandValue);
	}
	return NAN;
}
