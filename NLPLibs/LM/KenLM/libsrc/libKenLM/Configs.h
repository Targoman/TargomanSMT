/*************************************************************************
 * Copyright © 2012-2014, Targoman.com
 *
 * Published under the terms of both GPL vrsion 3 and
 * TCRL(Targoman Community Research License)
 * You can find a copy of TCRL license file with distributed source or
 * download them from http://targoman.com/License.txt
 *
 * This file is part of KenLM implementation in Targoman Project.
 *
 *   KenLM is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   KenLM is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with KenLM.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   KenLM code is derived from Kenneth's code.
 *   Find origianl code at: https://kheafield.com/code/kenlm/
 *************************************************************************/
/**
 @author S. Mohammad M. Ziabary <smm@ziabary.com>
 */

#ifndef TARGOMAN_NLPLIBS_LM_KENLM_CONFIGS_H
#define TARGOMAN_NLPLIBS_LM_KENLM_CONFIGS_H

#include <cstdio>
#include <string>
#include <vector>
#include <QtCore>

namespace Targoman {
namespace NLPLibs {
namespace LM {

namespace Private {
class clsEnumerateVocab;
}
struct stuConfig
{
    // EFFECTIVE FOR BOTH ARPA AND BINARY READS

    // (default true) print progress bar to messages
    bool show_progress;

    // Where to log messages including the progress bar.  Set to NULL for
    // silence.
///    std::ostream *messages;

/**    std::ostream *ProgressMessages() const {
      return show_progress ? messages : 0;
    }*/

    // This will be called with every string in the vocabulary.  See
    // enumerate_vocab.hh for more detail.  Config does not take ownership; you
    // are still responsible for deleting it (or stack allocating).
    ///clsEnumerateVocab *enumerate_vocab;


    // ONLY EFFECTIVE WHEN READING ARPA

    // What to do when <unk> isn't in the provided model.
    ///WarningAction unknown_missing;
    // What to do when <s> or </s> is missing from the model.
    // If THROW_UP, the exception will be of type util::SpecialWordMissingException.
    ///WarningAction sentence_marker_missing;

    // What to do with a positive log probability.  For COMPLAIN and SILENT, map
    // to 0.
    ///WarningAction positive_log_probability;

    // The probability to substitute for <unk> if it's missing from the model.
    // No effect if the model has <unk> or unknown_missing == THROW_UP.
    float unknown_missing_logprob;

    // Size multiplier for probing hash table.  Must be > 1.  Space is linear in
    // this.  Time is probing_multiplier / (probing_multiplier - 1).  No effect
    // for sorted variant.
    // If you find yourself setting this to a low number, consider using the
    // TrieModel which has lower memory consumption.
    float probing_multiplier;

    // Amount of memory to use for building.  The actual memory usage will be
    // higher since this just sets sort buffer size.  Only applies to trie
    // models.
    std::size_t building_memory;

    // Template for temporary directory appropriate for passing to mkdtemp.
    // The characters XXXXXX are appended before passing to mkdtemp.  Only
    // applies to trie.  If NULL, defaults to write_mmap.  If that's NULL,
    // defaults to input file name.
    const char *temporary_directory_prefix;

    // Level of complaining to do when loading from ARPA instead of binary format.
    enum ARPALoadComplain {ALL, EXPENSIVE, NONE};
    ARPALoadComplain arpa_complain;

    // While loading an ARPA file, also write out this binary format file.  Set
    // to NULL to disable.
    const char *write_mmap;

    enum WriteMethod {
      WRITE_MMAP, // Map the file directly.
      WRITE_AFTER // Write after we're done.
    };
    WriteMethod write_method;

    // Include the vocab in the binary file?  Only effective if write_mmap != NULL.
    bool include_vocab;


    // Left rest options.  Only used when the model includes rest costs.
    enum RestFunction {
      REST_MAX,   // Maximum of any score to the left
      REST_LOWER // Use lower-order files given below.
    };
    RestFunction rest_function;
    // Only used for REST_LOWER.
    std::vector<std::string> rest_lower_files;


    // Quantization options.  Only effective for QuantTrieModel.  One value is
    // reserved for each of prob and backoff, so 2^bits - 1 buckets will be used
    // to quantize (and one of the remaining backoffs will be 0).
    quint8 prob_bits, backoff_bits;

    // Bhiksha compression (simple form).  Only works with trie.
    quint8 pointer_bhiksha_bits;


    // ONLY EFFECTIVE WHEN READING BINARY

    // How to get the giant array into memory: lazy mmap, populate, read etc.
    // See util/mmap.hh for details of MapMethod.
    ///util::LoadMethod load_method;


    // Set defaults.
    stuConfig() :
        show_progress(true),
       /// messages(&std::cerr),
       /// enumerate_vocab(NULL),
       /// unknown_missing(COMPLAIN),
       /// sentence_marker_missing(THROW_UP),
       /// positive_log_probability(THROW_UP),
        unknown_missing_logprob(-100.0),
        probing_multiplier(1.5),
        building_memory(1073741824ULL), // 1 GB
        temporary_directory_prefix(NULL),
        arpa_complain(ALL),
        write_mmap(NULL),
        write_method(WRITE_AFTER),
        include_vocab(true),
        rest_function(REST_MAX),
        prob_bits(8),
        backoff_bits(8),
        pointer_bhiksha_bits(22)
      ///  load_method(util::POPULATE_OR_READ)
      {}

};

}
}
}
#endif // TARGOMAN_NLPLIBS_LM_KENLM_CONFIGS_H
