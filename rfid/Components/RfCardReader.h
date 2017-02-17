#ifndef RFCARDREADER
#define RFCARDREADER

#include "RFID.h"

class RfCardReader : public RFID
{
public:
	RfCardReader(void);
	~RfCardReader(void);

	int Status();

    bool WorkerCardRead(QString &Name, QString &Number);
    bool CreditCardRead(QString &cardid);
    bool M1BankCardRead(QString &cardid);

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
