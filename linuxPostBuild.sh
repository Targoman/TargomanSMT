################################################################################
#     Targoman: A robust Statistical Machine Translation framework
#
#     Copyright 2014-2015 by ITRC <http://itrc.ac.ir>
#
#     This file is part of Targoman.
#
#     Targoman is free software: you can redistribute it and/or modify
#     it under the terms of the GNU Lesser General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     Targoman is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
#     GNU Lesser General Public License for more details.
#
#     You should have received a copy of the GNU Lesser General Public License
#     along with Targoman. If not, see <http://www.gnu.org/licenses/>.
################################################################################
#!/bin/sh

BasePath=$1
IncludeTarget=$2
ConfigTarget=$3

# No need to create folders as they will be created
# when needed
#mkdir -pv $IncludeTarget
#for Dir in `find $BasePath -type d -not -name Private`
#do 
#    mkdir -pv $Dir
#done 

# Creating a symbolic link reduces the pain of ambigious changes in headers!
#cp -vrf --parents `find $BasePath -name *.h -o -name *.hpp -o -name *.hh` $IncludeTarget || :
for File in $(find "$BasePath" -name "*.h" -o -name "*.hpp" -o -name "*.hh"); do
    SrcPath=$(dirname $File);
    SrcName=$(basename $File);
    # Check if the header is private
    if echo "$SrcPath" | egrep "\bPrivate\b" 2>&1 > /dev/null; then
        echo "Ignoring private header $File ...";
    else
        TgtPath="$IncludeTarget/$BasePath/$(python -c "import os.path; print os.path.relpath('$SrcPath', '$BasePath')")";
        SrcPath="$(python -c "import os.path; print os.path.relpath('$SrcPath', '$TgtPath')")";
        if [ -r "$TgtPath/$SrcName" ]; then
            echo "Already exists $File ...";
        else
            mkdir -pv "$TgtPath" || : ;
            echo "Creating symbolic link for $File ...";
            # DO NOT USE $File BECAUSE THE SYMBOLIC LINK
            # MUST POINT TO A RELATIVE PATH
            ln -s "$SrcPath/$SrcName" "$TgtPath/$SrcName" || : ;
        fi
    fi
done
# No need to remove anything anymore!
#rm -rvf $IncludeTarget/$BasePath/Private || :

# Config files will be copied only when the exist!
if find "conf/"* 2>&1 > /dev/null; then 
    mkdir -p $ConfigTarget    || : ;
    echo "Copying config files ...";
    cp -rvf "conf/"* $ConfigTarget || : ;
else
    echo "No config files to copy ...";
fi
