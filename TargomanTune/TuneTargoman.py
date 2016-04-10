#!/usr/bin/env python

import  argparse
import sys
import re
import subprocess
import Common
import os.path

#usage = "%(prog)s sourceDevFile targetDevFile decoderBinFile decoderLibFile decoderConfigFile --mert-dir MERT_DIR [options]"
usage = "%(prog)s --src sourceDevFile --trg targetDevFile --bin decoderBinFile --lib-dir decoderLibDir -c decoderConfigFile --mert-dir MERT_DIR [options]"
parser = argparse.ArgumentParser(prog='TuneTargoman.py', usage=usage);

#parser.add_argument("sourceDevFile", help="required, input text to decode")
#parser.add_argument("targetDevFile", help="required, basename of references files")
#parser.add_argument("decoderBinFile", help="required, pathname to the decoder executable")
#parser.add_argument("decoderLibFile", help="required, pathname to the decoder lib directory")
#parser.add_argument("decoderConfigFile", help="required, pathname to startup config file")


parser.add_argument("--src", required=True, dest="sourceDevFile", help="required, input text to decode")
parser.add_argument("--trg", required=True, dest="targetDevFile", help="required, basename of references files")
parser.add_argument("--bin", required=True, dest="decoderBinFile", help="required, pathname to the decoder executable")
parser.add_argument("--lib-dir", required=True, dest="decoderLibDir", help="required, pathname to the decoder lib directory")
parser.add_argument("-c", "--config", required=True, dest="decoderConfigFile", help="required, pathname to startup config file")
parser.add_argument("--mert-dir", required=True, help="required, directory with zmert.jar")

parser.add_argument("--working-dir", default=os.getcwd(), help="working directory")

parser.add_argument("--nbest", default=100, type=int, help="size of nbest list to generate [default: 100]")

parser.add_argument("--maxIt", default=None, type=int, help="maximum number of zmert iterations [default: 20]")
parser.add_argument("--minIt", default=None, type=int, help="minimum number of iterations before considering an early exit [default: 5]")
parser.add_argument("--save", default=3, type=int, help="save intermediate decoder configuration files (1) or decoder outputs (2) " +
								"or both (3) or neither (0). [default: 3]")
parser.add_argument("--norm", default="none", help="select normalization for zmert")

parser.add_argument("--metric", default="BLEU 4 shortest", help="metric name for optimization with metric parameters " +
			"such as 'BLEU 4 shortest' or 'SemPOS 0 1'.")

parser.add_argument("--fixed", default="UnknownWordPenalty/ScalingFactor", 
		help="comma-separated list of features that are fixed to the starting values [default: UnknownWordPenalty/ScalingFactor]" +
		" [example: UnknownWordPenalty/ScalingFactor,LexicalReordering/ForwardSwap,PhraseTable/ScalingFactors/PhraseFeature0 ]")

args = parser.parse_args()
print args

fixed_weights = args.fixed.split(",");

params_file = args.working_dir + "/params.txt";
zmert_cfg = args.working_dir + "/zmert_cfg.txt";
decoder_cfg = args.working_dir + "/decoder_cfg.txt";
decoder_cmd = args.working_dir + "/decoder_cmd.py";
targoman_cfg = args.working_dir + "/targoman.conf";
iter_file = args.working_dir + "/iteration";
lambdas_file = args.working_dir + "/finalWeights.txt";
nbest_file = args.working_dir + "/zmert.best" + str(args.nbest) + ".out";

cmd = "cp " + args.decoderConfigFile + " " + targoman_cfg;
print "running command: \t" + cmd;

subprocess.call(cmd, shell=True);

#############################################
##### writing params_file & decoder_cfg #####
#############################################

pfile = open(params_file, 'w');
dfile = open(decoder_cfg, 'w');

for key, array in Common.feature_names.iteritems():
	i = 0;
	for val in array:
		cfg_file = open(targoman_cfg, 'r')
		fname = key + val;
		state = " Opt ";
		if fname in fixed_weights:
			state = " Fix ";

		# checking if the feature is active in the targoman_cfg file 
		# then write it in params_file 

		if re.search("^" + fname, cfg_file.read(), flags=re.MULTILINE) != None:
			print fname + " ||| " + str(Common.default_weights[key][i]);
			pfile.write(fname + " ||| " + str(Common.default_weights[key][i][0]) + state + 
					str(Common.default_weights[key][i][1]) + " " + str(Common.default_weights[key][i][2]) + " " +
					str(Common.default_weights[key][i][1]) + " " + str(Common.default_weights[key][i][2]) + "\n");
			dfile.write(fname + " " + str(Common.default_weights[key][i][0]) + "\n");
		i += 1;

pfile.write("normalization = " + args.norm);
pfile.close();
dfile.close();

#######################################
##### changing the config options #####
#######################################
	
Common.changeConfigOption("App", "InputFile", args.sourceDevFile, targoman_cfg);
Common.changeConfigOption("NBestPath", "NBestPathSize", args.nbest, targoman_cfg);
Common.changeConfigOption("App", "NBestFile", "", targoman_cfg);

#######################################
#######    writing zmert_cfg    #######
#######################################

num_of_refs = 0;
if os.path.isfile(args.targetDevFile):
	num_of_refs = 1;
else :
	while os.path.isfile(args.targetDevFile + str(num_of_refs)) :
		num_of_refs += 1;
if num_of_refs == 0:
	raise argparse.ArgumentTypeError("Invalid address for target development files ...")

zm_file = open(zmert_cfg, "w");

zm_file.write("-r\t" + args.targetDevFile + "\n");
zm_file.write("-rps\t" + str(num_of_refs)+ "\n");
zm_file.write("-txtNrm\t0\n");
zm_file.write("-p\t" + params_file + "\n");
zm_file.write("-m\t" + args.metric+ "\n");
zm_file.write("-save\t" + str(args.save) + "\n");
zm_file.write("-cmd\t" + decoder_cmd+ "\n");
zm_file.write("-decOut\t" + nbest_file + "\n");
zm_file.write("-dcfg\t" + decoder_cfg + "\n");
zm_file.write("-N\t" + str(args.nbest)+ "\n");
zm_file.write("-v\t1\n");
zm_file.write("-decV\t1\n");
if args.minIt != None:
	zm_file.write("-minIt\t" + str(args.minIt) +"\n");
if args.maxIt != None:
	zm_file.write("-maxIt\t" + str(args.maxIt) +"\n");

zm_file.close();

######################################
#######  writing decoder-cmd  ########
######################################

cmd = "cp " + os.path.dirname(__file__) + "/DecoderCmd.py " + decoder_cmd;
print "running command: \t" + cmd;
subprocess.call(cmd, shell=True);

cmd = "sed -i \"s#targoman_bin = .*#targoman_bin = \\\"" + args.decoderBinFile + "\\\"#g\" " + decoder_cmd;
print "running command: \t" + cmd;
subprocess.call(cmd, shell=True);

cmd = "sed -i \"s#targoman_lib = .*#targoman_lib = \\\"" + args.decoderLibDir + "\\\"#g\" " + decoder_cmd;
print "running command: \t" + cmd;
subprocess.call(cmd, shell=True);

cmd = "sed -i \"s#^nbest = .*#nbest = " + str(args.nbest) + "#g\" " + decoder_cmd;
print "running command: \t" + cmd;
subprocess.call(cmd, shell=True);

subprocess.call("chmod +x " + decoder_cmd, shell=True);

##########################
####  running z-mert  ####
##########################

cmd = "java -cp " + args.mert_dir + "/zmert.jar ZMERT " + zmert_cfg;
print "running command: \t" + cmd;
subprocess.call(cmd, shell=True);


