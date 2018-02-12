################################################################################
#   Targoman: A robust Statistical Machine Translation framework
#
#   Copyright 2014-2015 by ITRC <http://itrc.ac.ir>
#
#   This file is part of Targoman.
#
#   Targoman is free software: you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Targoman is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU Lesser General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public License
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################
BasePath = "."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    src/PhraseAlignment.h \
    src/SentenceAlignment.h \
    src/Graph.h \
    src/HoleCollection.h \
    src/score.h \
    src/XmlException.h \
    src/hierarchical.h \
    src/RuleExtractionOptions.h \
    src/PhraseTable.h \
    src/ExtractedRule.h \
    src/SentenceAlignmentWithSyntax.h \
    src/SafeGetline.h \
    src/tables-core.h \
    src/SyntaxTree.h \
    src/Hole.h \
    src/PhraseInfo.h \
    src/AlignmentPhrase.h \
    src/relax-parse.h \
    src/XmlTree.h \
    src/RuleExist.h
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    src/SentenceAlignment.cpp \
    src/SyntaxTree.cpp \
    src/tables-core.cpp \
    src/SentenceAlignmentWithSyntax.cpp \
    src/XmlTree.cpp \
    src/HoleCollection.cpp \
    src/AlignmentPhrase.cpp