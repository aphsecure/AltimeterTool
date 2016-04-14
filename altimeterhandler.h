#ifndef ALTIMETERHANDLER_H
#define ALTIMETERHANDLER_H

#include <QtGlobal>

class AltimeterHandler
{
public:
    ~AltimeterHandler();
    static bool prepareRequestData(QByteArray &data, const unsigned int address);
    static bool prepareRequestType(QByteArray &data, const unsigned int address);

    static bool processReplyData(QByteArray &data, unsigned int &altitude, unsigned int &from);
    static bool processReplyType(QByteArray &data, unsigned int &type, unsigned int &from);

    static bool setNewAddress(QByteArray &data, unsigned int newAddress, unsigned int oldAddress);
private:
    AltimeterHandler() = delete;
    AltimeterHandler(const AltimeterHandler& other) = delete;
    AltimeterHandler& operator=(const AltimeterHandler&) = delete;
};

#endif // ALTIMETERHANDLER_H
