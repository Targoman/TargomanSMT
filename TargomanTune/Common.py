import re, os
import fileinput

default_weights = {
    "LexicalReordering": [ [ 1.0, 0.0, 2.0 ],    # lexicalized reordering model
			   [ 1.0, 0.0, 2.0 ],
			   [ 1.0, 0.0, 2.0 ],
			   [ 1.0, 0.0, 2.0 ],
			   [ 1.0, 0.0, 2.0 ],
			   [ 1.0, 0.0, 2.0 ] ],

    "LanguageModel": [ [ 1.0, 0.0, 2.0 ] ],  # language model

    "PhraseTable": [ [ 0.3, 0.0, 0.5 ],    # translation model
		[ 0.2, 0.0, 0.5 ],
		[ 0.3, 0.0, 0.5 ],
		[ 0.2, 0.0, 0.5 ],
		[ 0.0,-1.0, 1.0 ] ],  # ... last weight is phrase penalty

    "ReorderingJump": [ [ 1.0, 0.0, 2.0] ],

    "WordPenalty": [ [ 0.0, -1.0, 1.0 ] ], 
    "UnknownWordPenalty": [ [ 1.0, 0.0, 2.0 ] ],
};

feature_names = {
	"LexicalReordering": [ "/ForwardMonotone", "/ForwardSwap", "/ForwardDiscontinous", 
				"/BackwardMonotone", "/BackwardSwap", "/BackwardDiscontinous" ],
	"LanguageModel": [ "/ScalingFactor" ],

	"PhraseTable": [ "/ScalingFactors/PhraseFeature0", "/ScalingFactors/PhraseFeature1", "/ScalingFactors/PhraseFeature2",
				"/ScalingFactors/PhraseFeature3", "/ScalingFactors/PhraseFeature4" ],

	"ReorderingJump": [ "/ScalingFactor" ],

	"WordPenalty": [ "/ScalingFactor" ],
	"UnknownWordPenalty": [ "/ScalingFactor" ],
};


def changeConfigOption(section, key, value, cfg_address):

	this_section = 0;
	for line in fileinput.input(cfg_address, inplace=True): 
		if line.strip() == "[" + section + "]":
			this_section = 1;
			print line.strip()
		elif this_section == 1:
			if re.match("\;*"+ str(key) + "\s*=.*", line.strip()):
				print str(key) + "=" + str(value);
				this_section = 0;
			elif re.match("\[.*\]", line.strip()):
				x = line.strip();
				print str(key) + "=" + str(value) + "\n" + x;
				this_section = 0;
			else:
				print line.strip();
		else:
			print line.strip()
	if this_section == 1:
		print str(key) + "=" + str(value) + "\n";

	fileinput.close()

