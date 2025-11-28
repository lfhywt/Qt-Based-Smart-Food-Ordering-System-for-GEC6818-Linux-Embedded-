#ifndef LEDQT_H
#define LEDQT_H

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
}

#define TEST_MAGIC 'x'
#define LED1 _IO(TEST_MAGIC, 0)
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3)

#define LED_ON  0
#define LED_OFF 1

class ledqt
{
public:
    ledqt();
    ~ledqt();

    bool init();
    void ledOn(int led);
    void ledOff(int led);
    void closeDev();

private:
    int fd;
};

#endif
