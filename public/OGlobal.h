#ifndef PUBLIC_OGLOBAL_H
#define PUBLIC_OGLOBAL_H

#include<QObject>
#include <QCryptographicHash>
#include "OGlobal/protocol2.0.h"
using namespace Protocol2;

//注意，如果要操作的指针是只读的，不要使用这个宏，直接用delete关键字就行
#define DELETE(p) {delete (p);(p)=0;}

//全局默认配置文件路径
const QString PUBLIC_CONFIG_FILE = QObject::trUtf8("./config.ini");

inline QString Osha1(QString s)
{
    return QString(QCryptographicHash::hash(s.toAscii(),QCryptographicHash::Sha1).toHex());
}

#endif // PUBLIC_OGLOBAL_H