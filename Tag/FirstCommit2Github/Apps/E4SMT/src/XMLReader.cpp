/******************************************************************************
 * Targoman: A robust Statistical Machine Translation framework               *
 *                                                                            *
 * Copyright 2014-2015 by ITRC <http://itrc.ac.ir>                            *
 *                                                                            *
 * This file is part of Targoman.                                             *
 *                                                                            *
 * Targoman is free software: you can redistribute it and/or modify           *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * Targoman is distributed in the hope that it will be useful,                *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mohammad M. Ziabary <ziabary@targoman.com>
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
