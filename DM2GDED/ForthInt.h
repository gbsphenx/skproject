
#pragma once

#include <vector>
#include <string>
#include <locale>

#define FORTH_STKSIZ 100U

class CForthInt
{
	LPCSTR psz;
	int x, cx;

	int stk[FORTH_STKSIZ];
	int istk;

public:
	CForthInt(LPCSTR psz, int x, int cx)
		: psz(psz), x(x), cx(cx)
	{

	}

protected:
	std::string tok;

	char Cur() {
		return (x < cx) ? psz[x] : 0;
	}

	bool Next() {
		x++;
		return x < cx;
	}

	bool IsWs() {
		switch (Cur()) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				return true;
		}
		return false;
	}

	bool SkipWs() {
		while (x < cx) {
			if (IsWs()) {
				Next();
				continue;
			}
			return true;
		}
		return false;
	}

	bool ReadToken() {
		tok.erase();
		if (SkipWs()) {
			while (!IsWs()) {
				tok += Cur();
				if (Next())
					continue;
				break;
			}
			return true;
		}
		return false;
	}

public:
	bool Run() {
		istk = 0;

		try {
			while (ReadToken()) {
				{
					char *pszEnd = NULL;
					int val = strtol(tok.c_str(), &pszEnd, 10);
					if (*pszEnd == 0) {
						push(val);
						continue;
					}
				}
				if (tok == "=") {
					int rval = pop();
					int lval = pop();

					push(rval == lval);
					continue;
				}
				if (Process())
					continue;

				return false;
			}
			return true;
		}
		catch (runtime_error) {
			return false;
		}
	}

	int pop() {
		if (istk <= 0)
			throw std::underflow_error("stack underflow!");
		return stk[--istk];
	}

	void push(int val) {
		if (istk + 1 >= FORTH_STKSIZ)
			throw std::overflow_error("stack overflow!");
		stk[istk++] = val;
	}

	virtual bool Process() { return false; }
};
