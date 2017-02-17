#include "RfCardReader.h"

RfCardReader::RfCardReader(void)
{
	ErrCnt = 0;
	Stat = 0;
}

RfCardReader::~RfCardReader(void)
{
}

int RfCardReader::Status()
{
    return Stat;
}

bool RfCardReader::WorkerCardRead(string &Name, string &Number)
{
	unsigned char pwd[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	unsigned char buf[20];
    bool ret = false;

	memset(buf, 0, sizeof(buf));

    Lock();

    if (!CardScan())
	{
		goto EXIT;
	}

	if (!Authen(1, TYPE_A, pwd, 6))
	{
		goto EXIT;
	}
    if (!BlockRead(1, buf, 16))
	{
		goto EXIT;
	}

    Number = (const char*)buf;
    if (Number.empty())
        goto EXIT;

    if (!BlockRead(2, buf, 16))
	{
		goto EXIT;
	}
    
    Name = (const char*)buf;
    if (Name.empty())
		goto EXIT;

    ret = true;

EXIT:
    Unlock();
	
    return ret;
}

bool RfCardReader::M1BankCardRead(string &cardid)
{
	unsigned char pwd[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bool ret = false;
	unsigned char buf[20];
	char id[33];

    Lock();

    if (!CardScan())
	{
		goto EXIT;
	}

    if (!Authen(4, TYPE_A, pwd, 6))
	{
		goto EXIT;
	}

	if (!BlockRead(4, buf, 16))
	{
		goto EXIT;
	}

    memset(id, 0, sizeof(id));
	BCD2Str(id, buf, 16);
    cardid = id;
    ret = true;

EXIT:
    Unlock();

	return ret;
}

bool RfCardReader::CreditCardRead(string &cardid)
{
	unsigned char buf[128];
	unsigned char cos1[] = {0x00, 0xA4, 0x04, 0x00, 0x08, 0xA0, 0x00, 0x00, 0x03, 0x33, 0x01, 0x01, 0x01};
	unsigned char cos2[] = {0x00, 0xB2, 0x01, 0x14, 0x00};
	short size;
	char str[24];
    bool ret = false;

    Lock();

    if (!CardScan())
	{
		goto EXIT;
	}
	
	if (!CpuCardMode())
	{    
		goto EXIT;
	}

    CosSend(cos1, sizeof(cos1));
	size = AckRecv(buf, 19);

    CosSend(cos2, sizeof(cos2));    
	size = AckRecv(buf, sizeof(buf));

	if (size != 0)
	{
        int i;

		for (i = 0; i < size; i ++)
		{
			if (buf[i] == 0x5A)
				break;
		}

		if (i == size)
            goto EXIT;
		
		i ++;

		size = BCD2Str(str, &buf[i+1], buf[i]);
        if ((size+1)/2 != buf[i])
			goto EXIT;

		cardid = str;

        ret = true;
	}

EXIT:
    Unlock();
	
	return ret;
}

void RfCardReader::Lock()
{

}

void RfCardReader::Unlock()
{

}

int RfCardReader::BCD2Str(char* dst, unsigned char *src, short n)
{
	unsigned char h, l;
	int i, j;
	int len = 0;

	for (i = 0, j = 0; i < n; i ++, j += 2)
	{
		h = src[i] >> 4;
		l = src[i] &0xF;

		if (h > 9)
		{
            dst[j] = 0;
			break;
		}
		else
		{
			len ++;
            dst[j] = h + '0';
		}

		if (l > 9)
		{
			dst[j+1] = 0;
			break;
		}
		else
		{
			len ++;
			dst[j+1] = l + '0';
		}
	}

	dst[len] = 0;

	return len;
}
