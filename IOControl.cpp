#include "IOControl.h"
#include "Calculator.h"
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>

using namespace std;
CControl::CControl(CCalculator& calc, std::istream& input, std::ostream& output)
	: m_calc(calc), m_input(input), m_output(output),
	m_actionMap({
		{"var", [this](istream& strm) {
			 return DeclareVariable(strm);
		 }},
		{"let", [this](istream& strm) {
			 return AssignValueToVariable(strm);
		 }},
		{"print", [this](istream& strm) {
			 return PrintValue(strm);
		 }},
		{"printvars", [this](istream& strm) {
			 return PrintVars(strm);
		 }}
	})
{}

bool CControl::HandleCommand()
{
	string commandLine;
	getline(m_input, commandLine);
	istringstream strm(commandLine);

	string action;
	strm >> action;
	auto it = m_actionMap.find(action);
	if (it != m_actionMap.end())
	{
		return it->second(strm);
	}
	return false;
}

bool CControl::DeclareVariable(istream& inpStrm)
{
	string variableName;
	inpStrm >> variableName;
	if (variableName.empty())
	{
		m_output << "No variable to declare" << endl;
		return false;
	}
	if (!inpStrm.eof())
	{
		string restOfCommand;
		inpStrm >> restOfCommand;
		if (!restOfCommand.empty())
		{
			m_output << "Too many identifiers" << endl;
			return false;
		}
	}
	if (!IsValidIdentifier(variableName))
	{
		m_output << "Not valid identifier name" << endl;
		return false;
	}
	if (!m_calc.AddVariable(variableName))
	{
		m_output << "Variable already exist" << endl;
		return false;
	}
	return true;
}

bool CControl::IsValidIdentifier(const string& identifierName)
{
	const regex pattern(R"d(^[a-zA-Z_][a-zA-Z0-9_]*$)d");
	if (regex_match(identifierName, pattern))
	{
		return true;
	}
	return false;
}

bool IsRestOfCommandEmpty(istream& inpStrm)
{
	if (inpStrm.eof())
	{
		return true;
	}
	string restOfCommand;
	inpStrm >> restOfCommand;
	if (restOfCommand.empty())
	{
		return true;
	}
	return false;
}

bool CControl::AssignValueToVariable(istream& inpStrm)
{
	string assignment;
	inpStrm >> assignment;
	if (!IsRestOfCommandEmpty(inpStrm))
	{
		m_output << "Not valid expression" << endl;
		return false;
	}
	regex rgx("^([a-zA-Z_][a-zA-Z0-9_]*)=(?:(-?[0-9][0-9.]*)|([a-zA-Z_][a-zA-Z0-9_]*))$");
	smatch submatch;
	if (!regex_match(assignment, submatch, rgx))
	{
		m_output << "Not valid expression" << endl;
		return false;
	}
	try
	{
		double value = stod(submatch[2]);
		return m_calc.AddVariableWithValue(submatch[1], value);
	}
	catch (invalid_argument& e)
	{
		if (!m_calc.AddVariableWithValue(submatch[1], submatch[3]))
		{
			m_output << "Assignment not possible" << endl;
			return false;
		}
		return true;
	}
	catch (out_of_range& e)
	{
		return false;
	}
}

bool CControl::PrintValue(istream& inpStrm) const
{
	string identifier;
	inpStrm >> identifier;
	double value = m_calc.GetVariableByName(identifier);
	if (isinf(value))
	{
		m_output << "Variable not exist" << endl;
		return false;
	}
	m_output << fixed << setprecision(2) << value << endl;
	return true;
}

bool CControl::PrintVars(istream& inpStrm) const
{
	auto& allVariables = m_calc.GetAllVariables();
	if (allVariables.size() > 0)
	{
		for (auto item: allVariables)
		{
			if (item.identifierType == IdentifierType::VARIABLE)
			{
				m_output << item.identifierName << ":"
						 << fixed << setprecision(2)
						 << item.identifierValue << endl;
			}
		}
	}
	return true;
}
