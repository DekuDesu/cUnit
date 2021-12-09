# cUnit

A simple ANSI C unit testing library.

### Requirements
This is pretty platform independent, however requires x64 architecture and assumes 8-byte alignment.

### Dependencies
This is not an embedded safe library and uses standard library headers. 
This library is super short and simple and easily modified, if you need to remove some dependencies.

Dependencies
|---|
|<stdlib.h>|
|<stdio.h>|
|<intrin.h>|
|<time.h>|
|<stdbool.h>|

### Features
- Quick Setup
- Simple API
- 2 Files (`cUnit.c`, `cUnit.h`)

### Usage
#### Creating Runtime
To actually run the tests you write you need to set up the runner that will well.. run the tests.
```C
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
```
#### Creating Tests
Once we have a way to run tests we just need to create the tests. A test method has STRICT signature requirements.
- It can either return bool or int(they're the same thing)
- Can be named anything(I encourage meaningful names)
- Can only have a single parameter that accepts a `FILE*` and it's name must be `stream`
- It must return `true` or (`1`)

Here's an example
```C
int name(FILE* stream)
{
    return 1;
}
```

He's some examples of the general API and test methods
```C
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
```

### Results
The test runner's results are streamed to either `stdout`(default) or can be streamed to whatever `FILE*` you want 
using `suite->OutputStream = yourCustomStream;`.

The output format is always as follows but can be modified in cUnit.h whenever you want.
```text
[SUITE_NAME] [STARTING] [COUNT = NUMBER_OF_TESTS]
[TEST_NAME]
    [PASS/FAIL] [TIME_TAKEN ticks] in __func__ at __FILE__ [__LINE__]
[PASS/FAIL] [TEST_NAME] [TIME_TAKEN ticks]
[SUITE_NAME] [FINISHED] [PASS = NUMBER_OF_PASSED_METHODS] [FAIL = NUMBER_OF_FAILED_METHODS] [TIME_TAKEN ticks]
```

If you run the example project your output would look similar to this:
```C
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
```
