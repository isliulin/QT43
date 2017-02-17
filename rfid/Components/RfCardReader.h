#ifndef RFCARDREADER
#define RFCARDREADER

#include "RFID.h"

class RfCardReader : public RFID
{
public:
	RfCardReader(void);
	~RfCardReader(void);

	int Status();

    bool WorkerCardRead(string &Name, string &Number);
    bool CreditCardRead(string &cardid);
    bool M1BankCardRead(string &cardid);

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
