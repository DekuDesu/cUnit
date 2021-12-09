// cUnit.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <stdio.h>
#include "cunit.h"

bool RunTests();

int main()
{
	fprintf(stdout, "%s"NEWLINE, RunTests() == true ? "PASSED" : "FAILED");
}

bool ExampleTest(FILE* stream)
{
	Assert(1 == 1);
	Assert(13 != 15);

	int x = 12;
	int* ptr = &x;

	NotNull(ptr);

	ptr = NULL;

	IsNull(ptr);

	return true;
}

bool ShouldFail(FILE* stream)
{
	NotEqual(13, 22);

	IsNull(NULL);

	IsFalse(false);

	IsTrue(true);

	IsTrue(999); // any non-zero int is true

	size_t expected = 12;
	size_t actual = 24;

	Equals(expected, actual, "%lli"); // this should fail

	return true;
}

bool RunTests()
{
	// Create a test suite to add our test methods onto
	// we could chose any name for this collection of tests but i'll settle for the file name
	TestSuite suite = CreateSuite(__FILE__);

	// by default we output test results to stdout, but we could override it with
	suite->OutputStream = stdout;

	// add our test methods
	suite->Append(suite, "Should Pass", &ExampleTest);
	suite->Append(suite, "Should Fail", &ShouldFail);

	// run our tests and store whether or not all tests passed
	bool pass = suite->Run(suite);

	// dispose of any unmanaged resources
	suite->Dispose(suite);

	// return to caller whether or not all of the tests passed
	return pass;
}

// EXPECTED OUTPUT
/*
[~\program.c] [STARTING] [COUNT = 2]
[Should Pass]
		[PASS] [0 ticks]  in ExampleTest at ~\program.c [14]
		[PASS] [0 ticks]  in ExampleTest at ~\program.c [15]
		[PASS] [0 ticks]  in ExampleTest at ~\program.c [20]
		[PASS] [0 ticks]  in ExampleTest at ~\program.c [24]
[PASS][Should Pass] [1 ticks]
[Should Fail]
		[PASS] [0 ticks]  in ShouldFail at ~\program.c [31]
		[PASS] [0 ticks]  in ShouldFail at ~\program.c [33]
		[PASS] [0 ticks]  in ShouldFail at ~\program.c [35]
		[PASS] [0 ticks]  in ShouldFail at ~\program.c [37]
		[PASS] [0 ticks]  in ShouldFail at ~\program.c [39]
		[FAIL] [0 ticks]  in ShouldFail at ~\program.c [44]
				Expected: 12 Got: 24
[FAIL][Should Fail] [1 ticks]
[~\program.c] [FINISHED] [PASS = 1] [FAIL = 1][2 ticks] FAILED
*/