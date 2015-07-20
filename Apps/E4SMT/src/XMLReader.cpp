/*************************************************************************
 * Copyright © 2012-2015, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include "XMLReader.h"
#include <QXmlStreamReader>

namespace Targoman {
namespace Apps {

void XMLReader::xmlErrorHandler(void *_userData, xmlErrorPtr _error)
{
    *((QString*)_userData) =
            QString("%1 on Line: %2").arg(QString(_error->message).trimmed()).arg(_error->line);
    throw exInvalidXML(*((QString*)_userData));
}

bool XMLReader::isValid(const QString &_inFile)
{
    LIBXML_TEST_VERSION
    //reset the generic error handler
    initGenericErrorDefaultFunc(NULL);
    QString ErrorString;
    //supply custom error handler
    xmlSetStructuredErrorFunc(&ErrorString, XMLReader::xmlErrorHandler);

    xmlParserCtxtPtr Context; /* the parser context */
    xmlDocPtr Doc; /* the resulting document tree */

    /* create a parser context */
    Context = xmlNewParserCtxt();
    if (Context == NULL)
      throw exInvalidXML("Failed to allocate parser context");

    QFile File(_inFile);
    File.open(QIODevice::ReadOnly);
    int FD = File.handle();

    /* parse the file, activating the DTD validation option */
    Doc = xmlCtxtReadFd(Context, FD, ".", NULL, XML_PARSE_DTDVALID);

    /* check if parsing suceeded */
    if (Doc == NULL)
        throw exInvalidXML("Unable to read XML Buffer");
    else
      xmlFreeDoc(Doc);
    /* free up the parser context */
    xmlFreeParserCtxt(Context);

    return true;
}

QStringList XMLReader::getContext(const QString &_inFile, bool _keepTitles)
{
    QStringList Output;
    QFile File(_inFile);
    File.open(QIODevice::ReadOnly);
    if (File.isReadable() == false)
        throw exAppE4SMT("Unable to open <" + _inFile + "> for reading.");

    QXmlStreamReader XMLReader(&File);

      bool Report = false;
      while (!XMLReader.atEnd())
      {
        XMLReader.readNext();
        switch(XMLReader.tokenType())
        {
        case QXmlStreamReader::StartElement:
          if (XMLReader.name().toString().toLower() == "paragraph" ||
              (XMLReader.name().toString().toLower() == "title" &&
               _keepTitles))
            Report = true;
          else
            Report = false;
          break;
        case QXmlStreamReader::Characters:
          if (Report)
            Output.append(
                        XMLReader.text().toString().replace(
                            "&gt;",">").replace(
                            "&lt;","<").replace(
                            "&amp;","&").replace(
                            "\n"," ").replace(
                            "-\n",""));
          Report = false;
          break;
        case QXmlStreamReader::StartDocument:
        case QXmlStreamReader::NoToken:
        case QXmlStreamReader::Invalid:
        case QXmlStreamReader::EndDocument:
        case QXmlStreamReader::EndElement:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::DTD:
        case QXmlStreamReader::EntityReference:
        case QXmlStreamReader::ProcessingInstruction:
          Report = false;
          break;
        }
      }
      return Output;
}

}
}
