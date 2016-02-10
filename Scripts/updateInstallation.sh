#!/bin/sh

RemoteInstallDir=/SMT/Versions
RemoteUser=installer
RemoteGroup=support
RemotePort=22
TagString=""
SkipCreatingBasePath=0

function logSection() { echo -e "\033[0;32m==========> $* <==========\033[0m" 1>&2; }
function logError(){ echo -e "\033[0;31m[$(date)][ERROR] $*\033[0m" 1>&2; }
function logInfo(){ echo -e "\033[0;36m[$(date)][INFO] $*\033[0m"; }
function logWarning(){ echo -e "\033[0;33m[$(date)][WARNING] $*\033[0m" 1>&2; }
function log(){ echo -e "\033[0m$*"; }

function usage() {
    log "updateInstall script. v0.1. This script updates installed binaries and configs automatically"
    log "Usage: $0 -H REMOTE_HOST [-p REMOTE_SSH_PORT] [-u REMOTE_SSH_USER] [-l][-s][-t][-b] [-n] [-v VERSION_STRING] [-k]" 1>&2;
    log "\t\t  -l: Update Load Balancer" 1>&2;
    log "\t\t  -b: Update Load Balancer for test" 1>&2;
    log "\t\t  -s: Update SMT Server" 1>&2;
    log "\t\t  -t: Update SMT Server for test" 1>&2;
    log "\t\t  -H: Remote host to where install" 1>&2;
    log "\t\t  -p: SSH port used to ssh to remote host (default=$RemotePort)" 1>&2;
    log "\t\t  -u: Username used to login and install into remote host (default=$RemoteUser)" 1>&2;
    log "\t\t  -g: Groupname used to login and install into remote host (default=$RemoteGroup)" 1>&2;
    log "\t\t  -v: Version string to be used instead of parent folder name. Not available using -t -b switches" 1>&2;
    log "\t\t  -c: Configure links and restart services" 1>&2;
    log "\t\t  -h: print this help" 1>&2;
    exit 2;
}

function runCommand(){
    $*
    Result=$?
    if [ $Result -ne 0 ]; then
        exit $Result;
    fi
}

function runOnRemote(){
    $SSHCommand "RemoteInstallDir='$RemoteInstallDir';
                 SkipCreatingBasePath='$SkipCreatingBasePath';
                 RemoteUser='$RemoteUser';
                 RemoteGroup='$RemoteGroup';
                 ConfigureAndRestart=$ConfigureAndRestart;
                 TagDir='$TagDir';
                 $(typeset -f);$*"
    Return=$?
    if [ $Return -gt 10 ] && [ $Return -lt 255 ]; then
        logError "function <$*> failed with code: $Return";
        exit $Return
    fi
    return $Return
}

function name2Posix(){
    echo "$1" | sed -e "s%[ /;]%_%g"
}

function createTagPaths(){
    logInfo "Creating tag related paths"
    InstallationDirs="
        $RemoteInstallDir/Binaries/$TagDir
        $RemoteInstallDir/Configs/TargomanCommon/$TagDir
        $RemoteInstallDir/Configs/TargomanApps/$TagDir
    "
    runCommand install -v -o $RemoteUser -g $RemoteGroup -m 2775 -d $InstallationDirs
    if [ $TagDir != "Trunk" ]; then
        runCommand cp -r $RemoteInstallDir/Configs/TargomanApps/Active/* $RemoteInstallDir/Configs/TargomanApps/$TagDir
    fi
}

function testAccess(){
    if [ ! -w $RemoteInstallDir ]; then
        logError "$RemoteInstallDir is not writeable by <$RemoteUser>."
        logError "If this is your first install please run baseInstall.sh script with root priviledges on the target server"
        return 1
    fi
}

function checkConnection(){
    runOnRemote testAccess
    if [ $? -ne 0 ];then
        logError "Invalid access using <$SSHCommand>.";
        logError "Maybe you have forgotten to create passwordless login for $RemoteUser"
        exit 2;
    fi
}

function sendLibraries(){
    logInfo "Peparing Libraries"
    runCommand mkdir -p $TempFolder/libs/
    for Lib in $*; do
        runCommand cp -a ../out/lib/lib$Lib.so* $TempFolder/libs/
    done
    logInfo "Compressing Libraries"
    pushd $TempFolder >/dev/null 2>&1
    runCommand tar -cz libs -f libs.tgz
    logInfo "Sending libraries to remote"
    runCommand scp -P ${RemotePort} libs.tgz $RemoteUser@${RemoteHost}:$RemoteInstallDir/Binaries/$TagDir/
    popd >/dev/null 2>&1
}

function sendBinaries(){
    logInfo "Peparing Binaries"
    runCommand mkdir -p $TempFolder/bin
    for Bin in $*; do
        runCommand cp -a ../out/bin/$Bin $TempFolder/bin/
    done
    logInfo "Compressing Binaries"
    runCommand pushd $TempFolder >/dev/null 2>&1
    runCommand tar -cz bin -f bin.tgz
    logInfo "Sending Binaries to remote"
    runCommand scp -P ${RemotePort} bin.tgz $RemoteUser@${RemoteHost}:$RemoteInstallDir/Binaries/$TagDir/
    popd >/dev/null 2>&1
}

function sendCommonConfigurations(){
    logInfo "Peparing ConfigFiles"
    runCommand mkdir -p $TempFolder/conf
    runCommand cp -a ../out/conf/* $TempFolder/conf/
    logInfo "Compressing Configurations"
    pushd $TempFolder/conf/ >/dev/null 2>&1
    runCommand tar -cz * -f conf.tgz
    logInfo "Sending Configurations to remote"
    runCommand scp -P ${RemotePort} conf.tgz $RemoteUser@${RemoteHost}:$RemoteInstallDir/Configs/TargomanCommon/$TagDir
    popd >/dev/null 2>&1
}


function unpackBinaries() {
    runCommand pushd $RemoteInstallDir/Binaries/$TagDir/ >/dev/null 2>&1;
    logInfo "Extracting Binary Files"
    runCommand tar -xzf "bin.tgz";
    runCommand tar -xzf "libs.tgz";
    runCommand rm "*.tgz"
    runCommand popd  >/dev/null 2>&1
}

function unpackConfigs() {
    runCommand pushd $RemoteInstallDir/Configs/TargomanCommon/$TagDir >/dev/null 2>&1
    logInfo "Extracting Config Files"
    runCommand tar -xzf "conf.tgz";
    runCommand rm -rf *.tgz
    runCommand popd >/dev/null 2>&1
}

function updateSymlinks() {
    logInfo "Updating symlinks"

    if [ $TagDir != "Trunk" ] && [ $ConfigureAndRestart -eq 1 ] ; then
        runCommand cd $RemoteInstallDir/Binaries/
        runCommand ln -sfn $TagDir Active
        runCommand cd $RemoteInstallDir/Configs/TargomanCommon/
        runCommand ln -sfn $TagDir Active
        runCommand cd $RemoteInstallDir/Configs/TargomanApps/
        runCommand ln -sfn $TagDir Active
    fi
}

function updateBasePackages(){
    BaseRPMs="libQt5Core5 libQt5Sql5-mysql libQt5Network5 libQt5Concurrent5 libQt5WebSockets5"
    if [ $RemoteUser == "root" ]; then
        logInfo "Installing base required packages"
        zypper update $BaseRPMs
    else
        logInfo "Probing required packages:"
        rpm -q $BaseRPMs
        if [ $? -ne 0 ];then
            logError "Please install missing packages or run this script with root as remote user"
            return 100
        fi
    fi
}

function doService() {
    for Service in $2; do
        if [ ! -f /usr/sbin/rc$Service ];then
            logError "service $Service not found. Please install it using baseinstall.sh script"
            exit 110
        fi
        logInfo "service $Service $1"
        sudo service $Service $1
    done
}


function updateLoadBalancer(){
    Libraries="QJsonRPC TargomanCommon"
    Binaries="TargomanLoadBalancer tsapasswd"
    if [ $TagDir == "Trunk" ];then
        Services="targomanloadbalancer.trunk"
    else
        Services="targomanloadbalancer.master targomanloadbalancer.slave"
    fi

    if [ -n "$TagString" ];then
        logSection "Installing Load Balancer [$TagString version]"
    else
        logSection "Installing Load Balancer"
    fi

    runOnRemote doService status "'$Services'"

    sendLibraries             "$Libraries"
    sendBinaries              "$Binaries"

    runOnRemote doService stop "'$Services'"
    runOnRemote unpackBinaries
    runOnRemote updateSymlinks
    runOnRemote doService start "'$Services'"
    sleep 1
    runOnRemote doService status "'$Services'"

    logSection "Finished"
}

function updateSMTServer(){
    Libraries="QJsonRPC TargomanCommon TargomanLM TargomanSMT TargomanSWT TargomanTextProcessor"
    Binaries="TargomanSMTServer TargomanSMTConsole tsapasswd"
    if [ $TagDir == "Trunk" ];then
        Services="targomansmtserver.trunk.en2fa
                  targomansmtserver.trunk.fa2en"
    else
        Services="targomansmtserver.master.en2fa
                  targomansmtserver.slave.en2fa
                  targomansmtserver.master.fa2en
                  targomansmtserver.slave.fa2en"
    fi

    if [ -n "$TagString" ];then
        logSection "Installing SMT Server [$TagString version]"
    else
        logSection "Installing SMT Server"
    fi

    runOnRemote doService status "'$Services'"

    sendLibraries             "$Libraries"
    sendBinaries              "$Binaries"

    runOnRemote doService stop "'$Services'"
    runOnRemote unpackBinaries
    runOnRemote updateSymlinks
    runOnRemote doService start "'$Services'"
    sleep 1
    runOnRemote doService status "'$Services'"

    logSection "Finished"
}

function main(){
    IsTest=0
    ConfigureAndRestart=0
    cd $(dirname $0)
    while getopts ":lbstH:p:v:u:g:nkc" o; do
        case "${o}" in
            l)  What2Install="$What2Install LoadBalancer";;
            b)  What2Install="LoadBalancer";IsTest=1;;
            s)  What2Install="$What2Install SMTServer";;
            t)  What2Install="SMTServer";IsTest=1;;
            H)  RemoteHost=${OPTARG};;
            p)  RemotePort=${OPTARG}; ((Port < 10 || Port > 65535)) || usage ;;
            u)  RemoteUser=${OPTARG};;
            g)  RemoteGroup=${OPTARG};;
            k)  SkipCreatingBasePath=1;;
            v)  TagString=${OPTARG};;
            c)  ConfigureAndRestart=1;;
            *)  usage;;
        esac
    done

    shift $((OPTIND-1))

    if [ -z "${RemoteHost}" ] || [ -z "$What2Install" ] ; then  usage; fi
    if [ -z "${RemotePort}" ];then RemotePort=22; fi

    SSHCommand="ssh ${RemoteHost} -p ${RemotePort} -l ${RemoteUser} -o ConnectTimeout=5 -oBatchMode=yes  "

    TempFolder=$(mktemp -d)
    if [ $? -ne 0 ];then
        logError "Unable to create temp directory!!!"
        exit 2;
    fi
    trap "rm -rf $TempFolder" EXIT

    checkConnection

    if [ $IsTest -eq 1 ]; then
        if [ -n "$TagString" ]; then
            logError "tag string is not supported for test installation."
            exit 2
        fi
        TagString="Trunk"
    elif [ -z "$TagString" ]; then
        TagString=`pwd | rev | cut -d '/' -f 2 |rev`
    fi

    TagDir=`name2Posix "$TagString"`

    runOnRemote updateBasePackages
    runOnRemote createTagPaths
    sendCommonConfigurations
    runOnRemote unpackConfigs

    for Ins in $What2Install; do
        update$Ins
    done
}

main $*
