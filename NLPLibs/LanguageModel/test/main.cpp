/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of TCRL(Targoman Community Research License)
 * You can find a copy of the license file with distributed source or
 * download it from http://targoman.com/License.txt
 *
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#include <QDebug>
#include <QStringList>

#include "libTargomanCommon/CmdIO.h"
#include "libLanguageModel/clsLanguageModel.h"
#include "libLanguageModel/clsLMSentenceScorer.h"
#include "libTargomanCommon/Logger.h"

#include <fstream>
#include <ctime>

using namespace Targoman::NLPLibs;

int main(int argc, char *argv[])
{
    clsLanguageModel LM;
    try {
        Targoman::Common::TARGOMAN_IO_SETTINGS.Info.setLevel(5);
        Targoman::Common::Logger::instance().setVisible();
        Targoman::Common::Logger::instance().setActive();


        qDebug()<<"Order = "<<LM.init(argc > 1 ? argv[1] : "./test.arpa");
        clsLMSentenceScorer SS(LM);
        QString Sentence =
                QStringLiteral("ارتباط او با سپاه پاسسسداران انقلالللب اسلاملللللی اووووو را در مرکز زنجیققرهای از ترور و جنایت قرار داده که دنیا را در برگرفته است .");

        quint8 Gram;
        foreach (const QString& Word, Sentence.split(" ")){
            Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
            qDebug()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
        }
        while(1);

        return 0;
        std::ifstream File;
        File.open("./test");
        std::string LineString;
        QList<float> Times;
        if (File.is_open()){
            while (std::getline(File, LineString)) {
                quint8 Gram;
                SS.reset();
                foreach (const QString& Word, QString::fromStdString(LineString).split(" ", QString::SkipEmptyParts)){
                    clock_t start = std::clock();
                    Targoman::Common::LogP_t Prob = SS.wordProb(Word, Gram);
                    Times.append((std::clock() - start)/(float)CLOCKS_PER_SEC);

                    qDebug()<<Times.last()<<"Prob ["<<Word<<"]:Prob = "<<Prob<<" NGram = "<<Gram;
                }
                qDebug()<<"###########################33";
            }
            float SumTime = 0;
            foreach (float Time, Times)
                SumTime += Time;
            qDebug()<<"Average = "<<SumTime / (float)Times.size();
        }
    }
    catch(Targoman::Common::exTargomanBase& e) {
        qDebug() << e.what();
    }
}


