#ifndef RFCARDREADER
#define RFCARDREADER

#include "RFID.h"
#include <string>
using namespace std;

typedef struct  
{
	string Number;
	string Name;
}WorkerInfo;

class RfCardReader : public RFID
{
public:
	RfCardReader(void);
	~RfCardReader(void);

	int Status();

	int WorkerCardRead(WorkerInfo &wi);
	int CreditCardRead(string &cardid);
	int M1BankCardRead(string &cardid);

private:
	int ErrCnt;

private:
    int BCD2Str(char* dst, unsigned char *src, short n);
	void Lock();
	void Unlock();

public:
	int Stat;
};

#endif
