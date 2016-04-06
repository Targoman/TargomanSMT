#!/usr/bin/env python
import re, os
import subprocess
import fileinput

working_dir = os.path.dirname(__file__);
targoman_bin = "/home/fatemeh/Desktop/Targoman2/Tools/targoman/Branch/azadi-training/out/bin/TargomanSMTConsole";
targoman_lib = "/home/fatemeh/Desktop/Targoman2/Tools/targoman/Branch/azadi-training/out/lib";
targoman_conf = working_dir + "/targoman.conf";
outFolder = working_dir;
nbest = 100;
iteration_file = working_dir + "/iteration";

lambdas = {};
feature_order = [];
iteration = 0;
nbest_file = working_dir + "/zmert.best" + str(nbest) + ".out";

if os.path.isfile(iteration_file):
	iteration = int(open(iteration_file, "r").read());

subprocess.call("mkdir " + working_dir + "/Outputs", shell=True);

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

####### ommiting feature names from nbest-list scores #####
def extractScores(line):
	array = line.split();
	reFloat = r'(^[+-]?\d+(?:\.\d+)?(?:[eE][+-]\d+)?$)';
	name = None;
	scores = {};
	for item in array:
		if re.match(".*=", item):
			name = item.split("=")[0];
		elif re.match(reFloat, item):
			if name == None:
				raise ValueError("Cannot guess nbest-list first feature score name");
			if not name in scores:
				scores[name] = "";
			scores[name] += item + " ";
		elif re.match("\S", item):
			raise ValueError("Unknown string " + name + " in nbest-list feature scores section");
	res = "";
	for key in feature_order:
		ffname = key.split("/")[0];
		if ffname in scores:
			res += scores[ffname];
			del scores[ffname];
	return res;

################################################
### reading the new weights from decoder_cfg ###
################################################
decoder_cfg_file = open(working_dir + "/decoder_cfg.txt", "r");
for line in decoder_cfg_file :
	ff = line.split();
	lambdas[ff[0]] = ff[1];
	feature_order.append(ff[0]);
	print ff;

decoder_cfg_file.close();

###############################################################################
####   store previous config file & updating targoman.conf for next run    ####
###############################################################################

targoman_conf_old = targoman_conf + ".run" + str(iteration);
if iteration == 0:
	targoman_conf_old = targoman_conf + ".orig";

subprocess.call("cp " + targoman_conf + " " + targoman_conf_old, shell=True);

for key in feature_order:
	changeConfigOption("FeatureFunctions", key, lambdas[key], targoman_conf);

#change translation and n-best-list output
changeConfigOption("App", "Output", outFolder + "/Outputs/output." + str(iteration + 1), targoman_conf);
changeConfigOption("App", "NBestFile", nbest_file, targoman_conf);

subprocess.call("rm " + nbest_file, shell=True);

##########################
###  executing decoder ###
##########################

os.environ['LD_LIBRARY_PATH'] = targoman_lib;
print "Executing: " + targoman_bin + "-c" + targoman_conf;
subprocess.call(targoman_bin + " -c " + targoman_conf + " 2> temp.log", shell=True);

## update iteration number 
iteration += 1;
subprocess.call("echo " + str(iteration) + " > iteration", shell=True);

##############################################################################
### modify the nbest-list to conform the zmert required format		   ###
### <i> ||| <candidate_translation> ||| featVal_1 featVal_2 ... featVal_m  ###
##############################################################################

nbest_file_orig = working_dir + "/zmert.best" + str(nbest) + ".out.orig";
print "mv " + nbest_file + " " + nbest_file_orig
subprocess.call("mv " + nbest_file + " " + nbest_file_orig, shell=True);

nb_file = open(nbest_file, "w");
nb_file_orig = open(nbest_file_orig, "r");

for line in nb_file_orig:
	delim = " ||| ";
	array = line.strip().split(delim);
	array[2] = extractScores(array[2]);
	nb_file.write(delim.join(array) + "\n");

nb_file.close();
nb_file_orig.close();

