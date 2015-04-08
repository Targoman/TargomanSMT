#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T15:16:03
#
#-------------------------------------------------

QT       -= core gui

TARGET = KenLM
TEMPLATE = lib
CONFIG += staticlib
CONFIG += warn_off

DEFINES += "KENLM_MAX_ORDER=6"

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

SOURCES += \
    lm/bhiksha.cc \
    lm/binary_format.cc \
    lm/build_binary.cc \
    lm/config.cc \
    lm/left_test.cc \
    lm/lm_exception.cc \
    lm/max_order.cc \
    lm/model.cc \
    lm/model_test.cc \
    lm/ngram_query.cc \
    lm/quantize.cc \
    lm/read_arpa.cc \
    lm/search_hashed.cc \
    lm/search_trie.cc \
    lm/trie.cc \
    lm/trie_sort.cc \
    lm/value_build.cc \
    lm/virtual_interface.cc \
    lm/vocab.cc \
    util/bit_packing.cc \
    util/bit_packing_test.cc \
    util/ersatz_progress.cc \
    util/exception.cc \
    util/file.cc \
    util/file_piece.cc \
    util/file_piece_test.cc \
    util/joint_sort_test.cc \
    util/mmap.cc \
    util/murmur_hash.cc \
    util/probing_hash_table_test.cc \
    util/sorted_uniform_test.cc \
    util/tokenize_piece_test.cc \
    util/usage.cc \
    util/getopt.c

HEADERS += \
    lm/bhiksha.hh \
    lm/binary_format.hh \
    lm/blank.hh \
    lm/config.hh \
    lm/enumerate_vocab.hh \
    lm/facade.hh \
    lm/left.hh \
    lm/lm_exception.hh \
    lm/model.hh \
    lm/model_type.hh \
    lm/ngram_query.hh \
    lm/quantize.hh \
    lm/read_arpa.hh \
    lm/return.hh \
    lm/search_hashed.hh \
    lm/search_trie.hh \
    lm/state.hh \
    lm/trie.hh \
    lm/trie_sort.hh \
    lm/value.hh \
    lm/value_build.hh \
    lm/virtual_interface.hh \
    lm/vocab.hh \
    lm/weights.hh \
    lm/word_index.hh \
    util/bit_packing.hh \
    util/check.hh \
    util/ersatz_progress.hh \
    util/exception.hh \
    util/file.hh \
    util/file_piece.hh \
    util/getopt.hh \
    util/have.hh \
    util/joint_sort.hh \
    util/mmap.hh \
    util/murmur_hash.hh \
    util/probing_hash_table.hh \
    util/proxy_iterator.hh \
    util/scoped.hh \
    util/sized_iterator.hh \
    util/sorted_uniform.hh \
    util/string_piece.hh \
    util/tokenize_piece.hh \
    util/usage.hh
unix {
    target.path = /usr/lib
    INSTALLS += target
}

QMAKE_POST_LINK = ./post_link.sh
