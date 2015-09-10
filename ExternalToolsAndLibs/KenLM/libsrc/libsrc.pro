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
    libKenLM/lm/bhiksha.hh \
    libKenLM/lm/binary_format.hh \
    libKenLM/lm/blank.hh \
    libKenLM/lm/config.hh \
    libKenLM/lm/enumerate_vocab.hh \
    libKenLM/lm/facade.hh \
    libKenLM/lm/left.hh \
    libKenLM/lm/lm_exception.hh \
    libKenLM/lm/model.hh \
    libKenLM/lm/model_type.hh \
    libKenLM/lm/ngram_query.hh \
    libKenLM/lm/quantize.hh \
    libKenLM/lm/read_arpa.hh \
    libKenLM/lm/return.hh \
    libKenLM/lm/search_hashed.hh \
    libKenLM/lm/search_trie.hh \
    libKenLM/lm/state.hh \
    libKenLM/lm/trie.hh \
    libKenLM/lm/trie_sort.hh \
    libKenLM/lm/value.hh \
    libKenLM/lm/value_build.hh \
    libKenLM/lm/virtual_interface.hh \
    libKenLM/lm/vocab.hh \
    libKenLM/lm/weights.hh \
    libKenLM/lm/word_index.hh \
    libKenLM/util/bit_packing.hh \
    libKenLM/util/check.hh \
    libKenLM/util/ersatz_progress.hh \
    libKenLM/util/exception.hh \
    libKenLM/util/file.hh \
    libKenLM/util/file_piece.hh \
    libKenLM/util/getopt.hh \
    libKenLM/util/have.hh \
    libKenLM/util/joint_sort.hh \
    libKenLM/util/mmap.hh \
    libKenLM/util/murmur_hash.hh \
    libKenLM/util/probing_hash_table.hh \
    libKenLM/util/proxy_iterator.hh \
    libKenLM/util/scoped.hh \
    libKenLM/util/sized_iterator.hh \
    libKenLM/util/sorted_uniform.hh \
    libKenLM/util/string_piece.hh \
    libKenLM/util/tokenize_piece.hh \
    libKenLM/util/usage.hh

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
SOURCES += \
    libID.cpp \
    libKenLM/lm/bhiksha.cc \
    libKenLM/lm/binary_format.cc \
    libKenLM/lm/build_binary.cc \
    libKenLM/lm/config.cc \
    libKenLM/lm/left_test.cc \
    libKenLM/lm/lm_exception.cc \
    libKenLM/lm/max_order.cc \
    libKenLM/lm/model.cc \
    libKenLM/lm/model_test.cc \
    libKenLM/lm/ngram_query.cc \
    libKenLM/lm/quantize.cc \
    libKenLM/lm/read_arpa.cc \
    libKenLM/lm/search_hashed.cc \
    libKenLM/lm/search_trie.cc \
    libKenLM/lm/trie.cc \
    libKenLM/lm/trie_sort.cc \
    libKenLM/lm/value_build.cc \
    libKenLM/lm/virtual_interface.cc \
    libKenLM/lm/vocab.cc \
    libKenLM/util/bit_packing.cc \
    libKenLM/util/bit_packing_test.cc \
    libKenLM/util/ersatz_progress.cc \
    libKenLM/util/exception.cc \
    libKenLM/util/file.cc \
    libKenLM/util/file_piece.cc \
    libKenLM/util/file_piece_test.cc \
    libKenLM/util/joint_sort_test.cc \
    libKenLM/util/mmap.cc \
    libKenLM/util/murmur_hash.cc \
    libKenLM/util/probing_hash_table_test.cc \
    libKenLM/util/sorted_uniform_test.cc \
    libKenLM/util/tokenize_piece_test.cc \
    libKenLM/util/usage.cc \
    libKenLM/util/getopt.c

CONFIG += staticlib
CONFIG += warn_off
DEFINES += "KENLM_MAX_ORDER=6"

#QMAKE_CXXFLAGS_RELEASE -= -O1
#QMAKE_CXXFLAGS_RELEASE -= -O2
#QMAKE_CXXFLAGS_RELEASE *= -O3
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
