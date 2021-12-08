#include "cunit.h"
#include <stdlib.h>
#include <intrin.h>

#define is ==
#define null NULL

static bool RunTest(Test test, FILE* stream);
static Test CreateTest(char* name, bool(*Method)(FILE*));

static bool RunSuite(TestSuite suite);
static void Append(TestSuite suite, char* name, bool(*method)(FILE*));
static void Dispose(TestSuite suite);

TestSuite CreateSuite(char* name)
{
	if (name is null)
	{
		fprintf(stderr, "parameter: name, must be specified");
		__debugbreak();
	}

	TestSuite suite = malloc(sizeof(struct testSuite));

	if (suite is null)
	{
		__debugbreak();
	}

	suite->Name = name;
	suite->Count = 0;
	suite->OutputStream = stdout;

	suite->Head = suite->Tail = null;

	suite->Dispose = &Dispose;
	suite->Append = &Append;
	suite->Run = &RunSuite;

	return suite;
}

static bool RunSuite(TestSuite suite)
{
	if (suite is null)
	{
		__debugbreak();
	}

	if (suite->Name is null)
	{
		__debugbreak();
	}

	fprintf(suite->OutputStream, SuiteStartFormat, suite->Name, suite->Count);

	Test head = suite->Head;

	size_t passCount = 0;

	Benchmark(
		while (head != null)
		{
			fprintf(suite->OutputStream, "[%s]\n", head->Name);

			if (RunTest(head, suite->OutputStream))
			{
				++passCount;
			}

			head = head->Next;
		}
	fprintf(suite->OutputStream, SuiteEndFormat, suite->Name, passCount, suite->Count - passCount);

	, suite->OutputStream);

	return passCount >= suite->Count;
}

static void Append(TestSuite suite, char* name, bool(*method)(FILE*))
{
	if (suite is null)
	{
		__debugbreak();
	}

	Test newTest = CreateTest(name, method);

	++(suite->Count);

	if (suite->Head is null)
	{
		suite->Head = suite->Tail = newTest;
		return;
	}

	suite->Tail->Next = newTest;
	suite->Tail = newTest;
}

static void Dispose(TestSuite suite)
{
	if (suite is null)
	{
		__debugbreak();
	}

	Test head = suite->Head;

	while (head != null)
	{
		Test tmp = head;

#pragma warning(disable : 6001)
		head = head->Next;
#pragma warning(default : 6001)

		free(tmp);
	}

	free(suite);
}

static bool RunTest(Test test, FILE* stream)
{
	bool pass = false;

	Benchmark(

		pass = test->Method(stream);
	fprintf(stream, "[%s][%s] ", pass ? "PASS" : "FAIL", test->Name);

	, stream);

	fputc('\n', stream);

	return pass;
}

static Test CreateTest(char* name, bool(*Method)(FILE*))
{
	Test newTest = malloc(sizeof(struct _test));

	if (newTest is null)
	{
		__debugbreak();
	}

	newTest->Name = name;
	newTest->Method = Method;
	newTest->Next = null;

	return newTest;
}

#undef is
#undef null