#include "OutPutDebugStringAndCheckResult.h"
#include"DebugColor.h"

HRESULT CheckResult(std::string OUTPUT_DEBUG_STRING, HRESULT RESULT) {
	
	std::string String = OUTPUT_DEBUG_STRING;

	if (RESULT == S_OK) {
		std::string SucceedString = "Ç…ê¨å˜ÇµÇ‹ÇµÇΩ\n";
		String += SucceedString;
		cout << green;
	}
	else {
		std::string FailedString = "Ç…é∏îsÇµÇ‹ÇµÇΩ\n";
		String += FailedString;
		cout << red;
	}

	const char *OutPutString = String.c_str();
	cout << OutPutString;
	cout << white;

	return RESULT;
}

void OutPutStartDebugString(std::string OUTPUT_DEBUG_STRING) {

	std::string String = OUTPUT_DEBUG_STRING;
	std::string StartString = "ÇäJénÇµÇ‹Ç∑\n";
	
	String += StartString;


	const char *OutPutString = String.c_str();
	printf(OutPutString);
}

void OutPutEndDebugString(std::string OUTPUT_DEBUG_STRING) {
	std::string String = OUTPUT_DEBUG_STRING;
	std::string StartString = "ÇèIóπÇµÇ‹Ç∑\n";

	String += StartString;

	const char *OutPutString = String.c_str();
	printf(OutPutString);
}

HRESULT CheckResult(HRESULT RESULT, std::string SUCCEED_STRING, std::string FAIL_STRING)
{
	std::string string;
	if (RESULT == S_OK) 
	{	
		cout << green;
		string = SUCCEED_STRING;
	}
	else
	{
		cout << red;
		string = FAIL_STRING;
	}

	cout << string;
	cout << white;
	return RESULT;
}

void FailCheck(std::string FAIL_STRING)
{
	cout << red;
	cout << FAIL_STRING;
	cout << white;
}

void SucceedCheck(std::string SUCCEED_STRING)
{
	cout << green;
	cout << SUCCEED_STRING;
	cout << white;
}

void ErrorCheck(std::string ERROR_STRING)
{
	cout << yellow;
	cout << ERROR_STRING;
	cout << white;
}
