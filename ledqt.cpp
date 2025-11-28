#include "ledqt.h"
#include <QDebug>

ledqt::ledqt()
{
    fd = -1;
}

ledqt::~ledqt()
{
    closeDev();
}

bool ledqt::init()
{
    fd = open("/dev/Led", O_RDWR);

    if (fd < 0) {
        qDebug() << "LED open file";
        return false;
    }

    qDebug() << "LED 打开成功!";
    return true;
}

void ledqt::ledOn(int led)
{
    if (fd < 0) return;

    switch (led) {
    case 1: ioctl(fd, LED1, LED_ON); break;
    case 2: ioctl(fd, LED2, LED_ON); break;
    case 3: ioctl(fd, LED3, LED_ON); break;
    case 4: ioctl(fd, LED4, LED_ON); break;
    }
}

void ledqt::ledOff(int led)
{
    if (fd < 0) return;

    switch (led) {
    case 1: ioctl(fd, LED1, LED_OFF); break;
    case 2: ioctl(fd, LED2, LED_OFF); break;
    case 3: ioctl(fd, LED3, LED_OFF); break;
    case 4: ioctl(fd, LED4, LED_OFF); break;
    }
}

void ledqt::closeDev()
{
    if (fd >= 0) {
        ::close(fd);
        fd = -1;
    }
}
