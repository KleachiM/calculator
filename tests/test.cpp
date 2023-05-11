#include "catch2/catch_test_macros.hpp"
#include <catch2/catch_approx.hpp>

#include "../Calculator.h"
#include "../IOControl.h"

#include <sstream>
#include <cmath>

using namespace std;

TEST_CASE("Declaration variable")
{
    CCalculator calc;
	stringstream inpStr;
	stringstream outStr;
	CControl ctrl(calc, inpStr, outStr);
	string command;

	SECTION("Declaration valid identifier")
	{
		command = "var x ";
		inpStr << command;
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(calc.GetAllVariables().size() == 1);
	}

	SECTION("Declaration too many identifier")
	{
		command = "var x y";
		inpStr << command;
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE_FALSE(ctrl.HandleCommand());
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE(outStr.str() == "Too many identifiers\n"s);
	}

	SECTION("Declaration without variable name")
	{
		command = "var";
		inpStr << command;
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE_FALSE(ctrl.HandleCommand());
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE(outStr.str() == "No variable to declare\n");
	}

	SECTION("Variable after declaration is NAN")
	{
		command = "var a";
		inpStr << command;
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(calc.GetAllVariables().size() == 1);
		REQUIRE(std::isnan(calc.GetVariableValueByName("x")));
	}

	SECTION("Declaration not valid identifier name")
	{
		command = "var 1a";
		inpStr << command;
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE_FALSE(ctrl.HandleCommand());
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE(outStr.str() == "Not valid identifier name\n");
	}
}

SCENARIO("Declaration already exists variable")
{
	GIVEN("Calculator with variable a")
	{
		CCalculator calc;
		stringstream inpStr;
		stringstream outStr;
		CControl ctrl(calc, inpStr, outStr);
		string command;

		calc.AddVariable("a");
		REQUIRE(calc.GetAllVariables().size() == 1);

		WHEN("Declare another variable with name a")
		{
			inpStr << "var a"s;
			REQUIRE_FALSE(ctrl.HandleCommand());
			REQUIRE(outStr.str() == "Variable already exist\n");

			THEN("Variable with exists name not added to variables list")
			{
				REQUIRE(calc.GetAllVariables().size() == 1);
			}
		}
	}
}

TEST_CASE("Assignment variable")
{
	CCalculator calc;
	stringstream inpStr;
	stringstream outStr;
	CControl ctrl(calc, inpStr, outStr);
	string command;

	SECTION("Assignment double to variable")
	{
		command = "let a=1";
		inpStr << command;
		REQUIRE(calc.GetAllVariables().empty());
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(calc.GetAllVariables().size() == 1);
		REQUIRE(calc.GetVariableValueByName("a") == Catch::Approx(1));
	}

	SECTION("Not valid assignment")
	{
		command = "let a==1";
		inpStr << command;
		REQUIRE_FALSE(ctrl.HandleCommand());
		REQUIRE(outStr.str() == "Not valid expression\n");
	}
}

SCENARIO("Assignment variable to other variable")
{
	GIVEN("Calc with variable a with value 1")
	{
		CCalculator calc;
		stringstream inpStr;
		stringstream outStr;
		CControl ctrl(calc, inpStr, outStr);
		string command;

		calc.AddVariableWithValue("a", "1.0");

		WHEN("Assign variable to other variable")
		{
			inpStr << "let b=a"s;
			REQUIRE(ctrl.HandleCommand());

			THEN("Variable b has value 1")
			{
				REQUIRE(calc.GetVariableValueByName("b") == Catch::Approx(1));
			}
		}
	}
}

SCENARIO("Assignment not existing variable to other variable")
{
	GIVEN("Calculator with variable a with value 1")
	{
		CCalculator calc;
		stringstream inpStr;
		stringstream outStr;
		CControl ctrl(calc, inpStr, outStr);
		string command;

		calc.AddVariableWithValue("a", "1.0");
		REQUIRE(calc.GetAllVariables().size() == 1);

		WHEN("Assign not existing variable to other variable")
		{
			inpStr << "let b=c"s;
			REQUIRE_FALSE(ctrl.HandleCommand());

			THEN("Error message, no assignment")
			{
				REQUIRE(calc.GetAllVariables().size() == 1);
				REQUIRE(outStr.str() == "Assignment not possible\n");
			}
		}
	}
}

SCENARIO("Double assignment variable to other variable")
{
	GIVEN("Calc with variable a with value 1 and b = a")
	{
		CCalculator calc;
		stringstream inpStr;
		stringstream outStr;
		CControl ctrl(calc, inpStr, outStr);
		string command;

		calc.AddVariableWithValue("a", "1.0");
		calc.AddVariableWithOtherVariableValue("b", "a");

		WHEN("Double assign variable to other variable")
		{
			inpStr << "let c=b"s;
			REQUIRE(ctrl.HandleCommand());

			THEN("Variable c has value 1")
			{
				REQUIRE(calc.GetVariableValueByName("b") == Catch::Approx(1));
				REQUIRE(calc.GetVariableValueByName("c") == Catch::Approx(1));
			}
		}
	}
}

TEST_CASE("Not valid expression in assignment")
{
	CCalculator calc;
	stringstream inpStr;
	stringstream outStr;
	CControl ctrl(calc, inpStr, outStr);

	inpStr << "let a=b c=d\n"s;
	REQUIRE_FALSE(ctrl.HandleCommand());
	REQUIRE(outStr.str() == "Not valid expression\n");
	outStr.str(string());
	inpStr << "let a=1b\n"s;
	REQUIRE_FALSE(ctrl.HandleCommand());
	REQUIRE(outStr.str() == "Not valid expression\n");
}

TEST_CASE("Print var")
{
	CCalculator calc;
	stringstream inpStr;
	stringstream outStr;
	CControl ctrl(calc, inpStr, outStr);

	SECTION("Print variable with nan")
	{
		inpStr << "var a\n"s;
		REQUIRE(ctrl.HandleCommand());
		inpStr << "print a\n"s;
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(outStr.str() == "nan\n");
	}

	SECTION("Assign value 1 to variable a and print it")
	{
		inpStr << "let b=-0.99\n"s;
		ctrl.HandleCommand();
		REQUIRE(calc.GetAllVariables().size() == 1);
		REQUIRE(calc.GetVariableValueByName("b") == Catch::Approx(-0.99));
		inpStr << "print b\n"s;
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(outStr.str() == "-0.99\n");
	}

	SECTION("Print not existing variable")
	{
		REQUIRE(calc.GetAllVariables().size() == 0);
		inpStr << "print a\n"s;
		REQUIRE_FALSE(ctrl.HandleCommand());
		REQUIRE(outStr.str() == "Variable not exist\n");
	}
}

SCENARIO("Print all variables")
{
	GIVEN("Calc without variables")
	{
		CCalculator calc;
		stringstream inpStr;
		stringstream outStr;
		CControl ctrl(calc, inpStr, outStr);

		WHEN("Print variables when none are declared")
		{
			inpStr << "printvars\n";
			REQUIRE(ctrl.HandleCommand());

			THEN("Std cout is empty")
			{
				REQUIRE(outStr.str() == ""s);
			}
		}

		WHEN("Print variables after adding two vars")
		{
			calc.AddVariableWithValue("a", "1.5");
			calc.AddVariable("c");
			calc.AddVariableWithOtherVariableValue("b", "a");

			inpStr << "printvars\n"s;
			REQUIRE(ctrl.HandleCommand());

			THEN("Printing 3 vars in cout")
			{
				REQUIRE(outStr.str() == "a:1.50\nb:1.50\nc:nan\n");
			}
		}
	}
}

TEST_CASE("Declare function")
{
	CCalculator calc;
	stringstream inpStr;
	stringstream outStr;
	CControl ctrl(calc, inpStr, outStr);

	SECTION("Assign to function variable with value = nan")
	{
		inpStr << "var a\n"s;
		ctrl.HandleCommand();
		REQUIRE(calc.GetAllVariables().size() == 1);
		inpStr << "fn firstFunc=a\n"s;
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(outStr.str() == ""s);
		REQUIRE(calc.GetAllVariables().size() == 2);
		REQUIRE(isnan(calc.GetVariableValueByName("firstFunc")));
	}

	SECTION("Assign to function variable with value = 1.5")
	{
		inpStr << "let a=1.5\n"s;
		ctrl.HandleCommand();
		REQUIRE(calc.GetAllVariables().size() == 1);
		inpStr << "fn firstFunc=a\n"s;
		REQUIRE(ctrl.HandleCommand());
		REQUIRE(outStr.str() == ""s);
		REQUIRE(calc.GetAllVariables().size() == 2);
		REQUIRE(calc.GetVariableValueByName("firstFunc") == Catch::Approx(1.5));
	}

	SECTION("Function Sum2And2")
	{
		inpStr << "let a=2\n"s;
		ctrl.HandleCommand();
		inpStr << "let b=2\n"s;
		ctrl.HandleCommand();
		inpStr << "fn Sum2And2=a+b\n"s;
		ctrl.HandleCommand();
		REQUIRE(calc.GetAllVariables().size() == 3);
		inpStr << "print Sum2And2\n"s;
		ctrl.HandleCommand();
		REQUIRE(outStr.str() == "4.00\n"s);
	}

	SECTION("Function from other functions")
	{
		inpStr << "let a=2\n"s;
		ctrl.HandleCommand();
		inpStr << "let b=3\n"s;
		ctrl.HandleCommand();
		inpStr << "fn Sum=a+b\n"s;
		ctrl.HandleCommand();
		inpStr << "fn Mult=a*b\n"s;
		ctrl.HandleCommand();
		inpStr << "fn ResFunc=Sum-Mult\n"s;
		ctrl.HandleCommand();
		inpStr << "print ResFunc\n"s;
		ctrl.HandleCommand();
		REQUIRE(outStr.str() == "-1.00\n"s);
	}
}

SCENARIO("Print all functions")
{
	GIVEN("Calc without functions")
	{
		CCalculator calc;
		stringstream inpStr;
		stringstream outStr;
		CControl ctrl(calc, inpStr, outStr);

		WHEN("Print functions when none are declared")
		{
			inpStr << "printfns\n";
			REQUIRE(ctrl.HandleCommand());

			THEN("Std cout is empty")
			{
				REQUIRE(outStr.str() == "");
			}
		}

		WHEN("Add functions")
		{
			inpStr << "let a=2\n"s;
			ctrl.HandleCommand();
			inpStr << "let b=3\n"s;
			ctrl.HandleCommand();
			inpStr << "fn Sum=a+b\n"s;
			ctrl.HandleCommand();
			inpStr << "fn Mult=a*b\n"s;
			ctrl.HandleCommand();
			inpStr << "fn ResFunc=Sum-Mult\n"s;
			ctrl.HandleCommand();
			inpStr << "printfns\n"s;
			ctrl.HandleCommand();
			REQUIRE(outStr.str() == "Mult:6.00\nResFunc:-1.00\nSum:5.00\n"s);
		}
	}
}