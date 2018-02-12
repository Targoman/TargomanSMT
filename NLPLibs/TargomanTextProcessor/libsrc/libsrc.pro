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

BasePath = ".."

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
HEADERS += \
    libTargomanTextProcessor/Private/Normalizer.h \
    libTargomanTextProcessor/TextProcessor.h \
    libTargomanTextProcessor/TextProcessor_c.h \
    libTargomanTextProcessor/Private/Unicode.hpp \
    libTargomanTextProcessor/Private/IXMLWriter.h \
    libTargomanTextProcessor/Private/SpellCorrector.h \
    libTargomanTextProcessor/Private/Configs.h \
    libTargomanTextProcessor/Private/SpellCorrectors/PersianSpellCorrector.h

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += libID.cpp \
    libTargomanTextProcessor/Private/Normalizer.cpp \
    libTargomanTextProcessor/TextProcessor.cpp \
    libTargomanTextProcessor/TextProcessor_c.cpp \
    libTargomanTextProcessor/Private/IXMLWriter.cpp \
    libTargomanTextProcessor/Private/SpellCorrector.cpp \
    libTargomanTextProcessor/Private/Configs.cpp \
    libTargomanTextProcessor/Private/SpellCorrectors/PersianSpellCorrector.cpp

OTHER_FILES += \
    conf/Abbreviations.tbl \
    conf/Normalization.conf \
    conf/PersianVerbStepPresent.tbl \
    conf/PersianVerbStemPast.tbl \
    conf/PersianStartWithNa.tbl \
    conf/PersianStartWithBi_Ba.tbl \
    conf/PersianSpace2ZWNJ.tbl \
    conf/PersianNouns.tbl \
    conf/PersianAutoCorrectTerms.tbl \
    conf/PersianHamzeOrMadAllowed.tbl \
    conf/SpellCorrectors/Persian/VerbStemPast.tbl \
    conf/SpellCorrectors/Persian/StartWithNa.tbl \
    conf/SpellCorrectors/Persian/StartWithBi_Ba.tbl \
    conf/SpellCorrectors/Persian/Space2ZWNJ.tbl \
    conf/SpellCorrectors/Persian/Nouns.tbl \
    conf/SpellCorrectors/Persian/HamzeOrMadAllowed.tbl \
    conf/SpellCorrectors/Persian/AutoCorrectTerms.tbl \
    conf/SpellCorrectors/Persian/Adjectives.tbl \
    conf/SpellCorrectors/Persian/VerbStemPast.tbl \
    conf/SpellCorrectors/Persian/StartWithNa.tbl \
    conf/SpellCorrectors/Persian/StartWithBi_Ba.tbl \
    conf/SpellCorrectors/Persian/Space2ZWNJ.tbl \
    conf/SpellCorrectors/Persian/Nouns.tbl \
    conf/SpellCorrectors/Persian/HamzeOrMadAllowed.tbl \
    conf/SpellCorrectors/Persian/AutoCorrectTerms.tbl \
    conf/SpellCorrectors/Persian/Adjectives.tbl \
    conf/SpellCorrectors/Persian/VerbStemPresent.tbl \
    conf/SpellCorrectors/Persian/StartWith_Na.tbl \
    conf/SpellCorrectors/Persian/StartWith_Bi_Ba.tbl \
    conf/SpellCorrectors/Persian/NeedHeYe.txt

################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = $$BasePath/Configs.pri
!exists($$ConfigFile){
error("**** libsrc: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = lib
TARGET = $$ProjectName
DESTDIR = $$BaseLibraryFolder
OBJECTS_DIR = $$BaseBuildFolder/obj
MOC_DIR = $$BaseBuildFolder/moc
QMAKE_CXXFLAGS_RELEASE += -fPIC
QMAKE_CXXFLAGS_DEBUG += -fPIC
INCLUDEPATH+=lib$$ProjectName

build_static {
    DEFINES += TARGOMAN_BUILD_STATIC
    CONFIG+= staticlib
}

QMAKE_POST_LINK += $$BaseOutput/linuxPostBuild.sh lib$$ProjectName $$BaseLibraryIncludeFolder $$BaseConfigFolder
