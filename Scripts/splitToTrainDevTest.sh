#!/bin/bash

FILES=($@)

if [ "${FILES[*]}" == "--help" ]; then
	echo "Usage: $0 <File1> [File2 [File3 ...]]" >&2
	exit 0
fi

if [ ${#FILES[@]} == 0 ]; then
	echo "Invalid usage. At least one file name must be specified." >&2
	echo "Usage: $0 <File1> [File2 [File3 ...]]" >&2
	exit 1
fi

# Check if all the "files" have same number of lines
N=
for ((I=0; I < ${#FILES[@]}; ++I)); do
	if [ -z "$N" ]; then
		N=$(wc -l < "${FILES[I]}")
		echo "'${FILES[I]}' has $N lines." >&2
	else
		if [ ! $N == $(wc -l < "${FILES[I]}") ]; then
			echo "'${FILES[I]}' does not have same number of lines as other files." >&2
			exit 1
		else
			echo "'${FILES[I]}' has proper number of lines." >&2
		fi
	fi
done

if [ $N -lt 700 ]; then
	echo "This corpora has too few lines ($N) to be split. Use it as a whole." >&2
	exit 1
fi

# Decide on the number of lines for each part (train, development and test)
DEV_LINES=500
TEST_LINES=1500
if [ $N -le 2000 ]; then
	DEV_LINES=100
	TEST_LINES=150
elif [ $N -le 5000 ]; then
	DEV_LINES=200
	TEST_LINES=250
elif [ $N -le 10000 ]; then
	DEV_LINES=500
	TEST_LINES=1000
fi
DEV_TEST_LINES=$((DEV_LINES+TEST_LINES))
TRAIN_LINES=$((N-DEV_TEST_LINES))

OUTDIR=$(dirname ${FILES[0]})
mkdir -p "$OUTDIR"/tmp

# Randomize the sentences so if the corpora consists of mixture of topics, we probably will take viable number of samples from each
paste "${FILES[@]}" | sort --random-sort > "$OUTDIR"/tmp/randomized.tmp
for ((I=0; I < ${#FILES[@]}; ++I)); do
	cut -f$((I+1)) < "$OUTDIR"/tmp/randomized.tmp > "$OUTDIR/tmp/$I"
done


# Create output structure and split the files
mkdir -p "$OUTDIR/Splitted/Train"
mkdir -p "$OUTDIR/Splitted/Dev"
mkdir -p "$OUTDIR/Splitted/Test"

for ((I=0; I < ${#FILES[@]}; ++I)); do
	BASE_NAME=$(basename ${FILES[I]})
	head -n $TRAIN_LINES "$OUTDIR/tmp/$I" > "$OUTDIR/Splitted/Train/$BASE_NAME"
	tail -n $DEV_TEST_LINES "$OUTDIR/tmp/$I" | head -n $DEV_LINES > "$OUTDIR/Splitted/Dev/$BASE_NAME"
	tail -n $TEST_LINES "$OUTDIR/tmp/$I" > "$OUTDIR/Splitted/Test/$BASE_NAME"
done
