#ifndef BYTES_H
#define BYTES_H

#include <QByteArray>
#include <QString>

namespace roco {
inline
auto byteArrToStr16(QByteArray arr) -> QString {
    // 将QByteArray转换为16进制表示
    QString hexString = arr.toHex();

    // 每两个字符添加一个空格，以便于阅读
    for (int i = 0; i < hexString.length(); i += 2) {
        if (i > 0) {
            hexString.insert(i, ' ');
            i++;
        }
    }

    return hexString;
}
} // namespace roco

#endif // BYTES_H
