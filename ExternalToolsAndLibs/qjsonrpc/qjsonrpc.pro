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
################################################################################
#                       DO NOT CHANGE ANYTHING BELOW                           #
################################################################################
ConfigFile = ./Configs.pri
!exists($$ConfigFile){
error("**** $$ProjectName: Unable to find Configuration file $$ConfigFile ****")
}
include ($$ConfigFile)

TEMPLATE = subdirs
CONFIG += ordered
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-#
addSubdirs(libsrc)
addSubdirs(tests, libsrc)

