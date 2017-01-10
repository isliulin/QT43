#include "KeyReader.h"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <QApplication>
#include <QKeyEvent>

using namespace Qt;

static const char _keymap[1][16] =
{
    {
        '1', '2', '3', 's',
        '4', '5', '6', '\b',
        '7', '8', '9', 'p',
        'c', '0', '.', '\r',
    },
};

static int _keyval[16] =
{
    Key_1, Key_2, Key_3, Key_S,
    Key_4, Key_5, Key_6, Key_Backspace,
    Key_7, Key_8, Key_Up, Key_Print,
    Key_PowerOff, Key_Down, Key_Escape, Key_Return,
};

KeyReader::KeyReader()
{
    curmap = 0;
}

KeyReader::~KeyReader()
{
    isrun = false;
}

bool KeyReader::open(const char *name)
{
    bool ret = true;

    fd = ::open(name, O_RDONLY);
    if (fd < 0)
    {
        ret = false;
    }

    return ret;
}

int KeyReader::read(char *buf, int size)
{
    return ::read(fd, buf, size);
}

bool KeyReader::wait(int msec)
{
    struct pollfd pf;

    pf.fd = fd;
    pf.events = POLLIN;

    ::poll(&pf, 1, msec);

    return pf.revents & POLLIN;
}

bool KeyReader::mapkey(int input, int &val, QString &text)
{
    input -= 1;
    if (input < 0 || input > 15)
        return false;

    val = _keyval[input];
    text = QString(_keymap[curmap][input]);

    return true;
}

void KeyReader::run()
{
    unsigned char buf[32];
    int size;

    if (!open("/dev/zlg7290key"))
    {
        qDebug("kbd err");
        return;
    }

    isrun = true;

    while (isrun)
    {
        if (!wait(1000))
            continue;

        size = read((char*)buf, sizeof(buf));

        if (size > 0)
        {
            int keyval;
            QString text;

            if (!mapkey(buf[size - 1], keyval, text))
                continue;

            QKeyEvent *evtp = new QKeyEvent(QEvent::KeyPress, keyval, Qt::NoModifier, text);
            QKeyEvent *evtr = new QKeyEvent(QEvent::KeyRelease, keyval, Qt::NoModifier, text);

            QObject *recv = QApplication::focusObject();

            if (recv)
            {
                QApplication::postEvent(recv, evtp);
                QApplication::postEvent(recv, evtr);
            }
        }
    }

    qDebug("kbd exit");
}
