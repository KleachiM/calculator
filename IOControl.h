#ifndef CALCULATOR_IOCONTROL_H
#define CALCULATOR_IOCONTROL_H

#include "Calculator.h"
#include <map>
#include <regex>

class CControl
{
public:
    CControl(CCalculator& calc, std::istream& input, std::ostream& output);
	bool HandleCommand();

	CControl& operator=(const CControl&) = delete;
private:
	bool DeclareVariable(std::istream& inpStrm);

	bool ParseCommandAndArgsForAddVariable(std::smatch& submatch);
	bool AssignValueToVariable(std::istream& inpStrm);
	static bool IsValidIdentifier(const std::string& identifierName);

	bool PrintValue(std::istream& inpStrm) const;
	bool PrintVars(std::istream& inpStrm) const;

	bool DeclareFunction(std::istream& inpStrm);
	bool ParseCommandAndArgsForAddFunction(const std::smatch& submatch);

    using Handler = std::function<bool(std::istream& args)>;
	using ActionMap = std::map<std::string, Handler>;

	CCalculator& m_calc;
	std::istream& m_input;
	std::ostream& m_output;

	const ActionMap m_actionMap;
};

#endif // CALCULATOR_IOCONTROL_H
