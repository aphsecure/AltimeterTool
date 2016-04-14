#include <QDataStream>
#include <QDebug>

#include "murmagics.h"
#include "murutils.h"

#include "altimeterhandler.h"

bool AltimeterHandler::prepareRequestData(QByteArray &data, const unsigned int address)
{
    QDataStream in(&data, QIODevice::WriteOnly);

    in << ::first_package_header_value;
    in << ::second_package_header_value;
    in << ::service_info_size;
    in << ::main_id;
    in << static_cast<uint8_t>(address);
    in << ::with_confirmation;
    in << ::request_altimeter_status;

    uint16_t crc = calculateCRC(data.data(), data.size());

    in << crc;
    in << ::end_of_message_value;

    return true;
}

bool AltimeterHandler::processReplyData(QByteArray &data, unsigned int &altitude, unsigned int &from)
{
    static uint8_t valToCheck= 0;
    static uint8_t addressFrom = 0;
    static uint16_t altitudeToSet = 0;
    static uint16_t crc = 0;
    static uint16_t expectedCrc= 0;
    QDataStream out(&data, QIODevice::ReadOnly);

    //!First header.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::first_package_header_value)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> wrong FIRST header.";
        return false;
    }

    //!Second header.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::second_package_header_value)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> wrong SECOND header.";
        return false;
    }

    //! Message size.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != data.size())) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Buffer size != package size.";
    }

    //! Address from.
    out >> addressFrom;
    from = static_cast<unsigned int>(addressFrom);

    //! Address too.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::main_id)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Its weard that destination address:" << valToCheck << "!=" << ::main_id;
    }

    //! Confirmation flag.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::is_replay)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Wrong CONFIRMATION flag:";
    }

    //! Message type.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::altimeter_status)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Its weard that message type:" << valToCheck << "!=" << ::altimeter_status;
    }

    //! Data.
    out >> altitudeToSet;
    altitude = static_cast<unsigned int>(altitudeToSet);
    //! CRC.
    out >> crc;
    expectedCrc = calculateCRC(data.data(), (size_t)data.at(::package_size_position) - 3);
    if (crc != expectedCrc) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Wrong CRC";
        return false;
    }

    //!End of data.
    out >> valToCheck;

    if (valToCheck != ::end_of_message_value) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Wrong end of data";
        return false;
    }

    return true;
}

bool AltimeterHandler::processReplyType(QByteArray &data, unsigned int &type, unsigned int &from)
{
    static uint8_t valToCheck= 0;
    static uint8_t addressFrom = 0;
    static uint8_t typeToSet = 0;
    static uint16_t crc = 0;
    static uint16_t expectedCrc= 0;
    QDataStream out(&data, QIODevice::ReadOnly);

    //!First header.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::first_package_header_value)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> wrong FIRST header.";
        return false;
    }

    //!Second header.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::second_package_header_value)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> wrong SECOND header.";
        return false;
    }

    //! Message size.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != data.size())) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Buffer size != package size.";
    }

    //! Address from.
    out >> addressFrom;
    from = static_cast<unsigned int>(addressFrom);

    //! Address too.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::main_id)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Its weard that destination address:" << valToCheck << "!=" << ::main_id;
    }

    //! Confirmation flag.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::is_replay)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Wrong CONFIRMATION flag:";
    }

    //! Message type.
    out >> valToCheck;
    if (Q_UNLIKELY(valToCheck != ::device_type_message)) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Its weard that message type:" << valToCheck << "!=" << ::altimeter_status;
    }

    //! Data.
    out >> typeToSet;
    type = static_cast<unsigned int>(typeToSet);

    //! CRC.
    out >> crc;
    expectedCrc = calculateCRC(data.data(), (size_t)data.at(::package_size_position) - 3);

    if (crc != expectedCrc) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Wrong CRC";
        return false;
    }

    //!End of data.
    out >> valToCheck;

    if (valToCheck != ::end_of_message_value) {
        qWarning() << "WARNING: AltimeterHandler::processReply -> Wrong end of data";
        return false;
    }

    return true;
}

bool AltimeterHandler::setNewAddress(QByteArray &data, unsigned int newAddress, unsigned int oldAddress)
{
    if (Q_UNLIKELY(newAddress == oldAddress)) {
        qWarning() << "WARNING:  AltimeterHandler::setNewAddress -> Its weard that news address == old address";
    }
    QDataStream in(&data, QIODevice::WriteOnly);

    in << ::first_package_header_value;
    in << ::second_package_header_value;
    in << ::service_info_size + sizeof(uint8_t); //! uint8_t -> new address size.
    in << ::main_id;
    in << static_cast<uint8_t>(oldAddress);
    in << ::without_confirmation;
    in << ::setup_new_address;
    in << static_cast<uint8_t>(newAddress);
    uint16_t crc = calculateCRC(data.data(), data.size());

    in << crc;
    in << ::end_of_message_value;

    return true;
}

bool AltimeterHandler::prepareRequestType(QByteArray &data, const unsigned int address)
{
    QDataStream in(&data, QIODevice::WriteOnly);

    in << ::first_package_header_value;
    in << ::second_package_header_value;
    in << ::service_info_size;
    in << ::main_id;
    in << static_cast<uint8_t>(address);
    in << ::with_confirmation;
    in << ::request_device_type;

    uint16_t crc = calculateCRC(data.data(), data.size());

    in << crc;
    in << ::end_of_message_value;

    return true;
}

AltimeterHandler::~AltimeterHandler()
{
}
