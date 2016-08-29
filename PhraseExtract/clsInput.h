#ifndef CLSINPUT_H
#define CLSINPUT_H

#include <QString>
#include <vector>
#include <QSet>
namespace MosesTraining{

class TokenInfo {
public:
    QString tag;
    QString value;

    TokenInfo(QString _val = "", QString _tag = ""){
        tag = _tag;
        value = _val;
        if(tag != "")
            tag = QString("<") + tag + QString(">");
    }
    const QString getValue() const{
//        if(tag != "")
//            return tag;
        return value;
    }

    const QString getTag() const{

        return tag;
    }
};

class clsInput
{
public:
    clsInput();

    std::vector<TokenInfo> parseRichIXML(const QString &_inputIXML);

    inline bool isSpace(const QChar& _ch){
        return _ch == ' ';
    }

private:
    QSet<QString> SpecialTags;

};

}
#endif // CLSINPUT_H
