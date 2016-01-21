#!/bin/sh

#rm /SMT/* -rf; rm /usr/lib/systemd/system/targoman*; rm /usr/sbin/rctargoman*; rm /etc/logrotate.d/targoman* ./baseInstall.sh -s -t

BaseSMTFolder=/SMT/
InstallDir=$BaseSMTFolder/Versions
User=installer
Group=support
PrepareForTest=0;

function log(){ echo -e "\033[0m$*"; }
function logSection() { echo -e "\033[0;32m==========> $* <==========\033[0m" 1>&2; }
function logError(){ echo -e "\033[0;31m[$(date)][ERROR] $*\033[0m" 1>&2; }
function logInfo(){ echo -e "\033[0;36m[$(date)][INFO] $*\033[0m"; }
function logWarning(){ echo -e "\033[0;33m[$(date)][WARNING] $*\033[0m" 1>&2; }

function probeRootPriviledges(){
    if [ "$(id -u)" != "0" ]; then
        echo "This script must be run as root" 1>&2
        exit 1
    fi
}

function usage() {
    log "Base install script which creates base paths and links. run this script on each server individualy with root access"
    log "Usage: $0 [-w][-d][-s] [-t] [-u UserName] [-g GroupName]" 1>&2;
    log "\t\t  -w: Install WebServer" 1>&2;
    log "\t\t  -d: Install Database" 1>&2;
    log "\t\t  -s: Install SMT Server" 1>&2;
    log "\t\t  -u: Username used for updatable folders (default=$User)" 1>&2;
    log "\t\t  -g: Groupname used for updatable folders (default=$Group)" 1>&2;
    log "\t\t  -t: Prepare also trunk version for test" 1>&2;
    log "\t\t  -h: print this help" 1>&2;
    exit -1;
}

function runCommand(){
    echo "$*"
    $*
    Result=$?
    if [ $Result -ne 0 ]; then
        exit $Result;
    fi
}

function installBasePackages(){
    BaseRPMs="libQt5Core5 libQt5Sql5-mysql libQt5Network5 libQt5Concurrent5 libQt5WebSockets5 "
    logInfo "Installing base required packages"
    zypper install $BaseRPMs
}

function createBasePaths(){
    logInfo "Creating BaseFolders"
    InstallationDirs="
        $InstallDir/Binaries/
        $InstallDir/Binaries/vDummy/bin
        $InstallDir/Binaries/vDummy/libs
        $InstallDir/Configs/TargomanCommon/
        $InstallDir/Configs/TargomanCommon/vDummy/
        $InstallDir/Configs/TargomanApps/
        $InstallDir/Configs/TargomanApps/vDummy/
        $InstallDir/Configs/SysConfigs/
        $InstallDir/Configs/SysConfigs/vDummy/
        $InstallDir/Models/
        $InstallDir/Models/vDummy/
        $BaseSMTFolder/scripts/services/
        $BaseSMTFolder/scripts/logrotate.d
        $BaseSMTFolder/scripts/tools
        $BaseSMTFolder/logs/
        $BaseSMTFolder/conf/
    "
    if [ $AlsoTest -eq 1 ]; then
        InstallationDirs="$InstallationDirs
            $InstallDir/Binaries/Trunk/bin
            $InstallDir/Binaries/Trunk/libs
            $InstallDir/Configs/TargomanCommon/Trunk/
            $InstallDir/Configs/TargomanApps/Trunk/
            $InstallDir/Models/Trunk/"
    fi

    runCommand install -v -o $User -g $Group -m 2775 -d $InstallationDirs
    logInfo "Creating Base Symlinks"
    runCommand cd $InstallDir/Binaries/
    ln -s vDummy Active
    runCommand cd $InstallDir/Configs/TargomanCommon/
    ln -s vDummy Active
    runCommand cd $InstallDir/Configs/TargomanApps/
    ln -s vDummy Active
    runCommand cd $InstallDir/Configs/SysConfigs/
    ln -s vDummy Active
    runCommand cd $InstallDir/Models/
    ln -s vDummy Active
    runCommand cd $BaseSMTFolder
    runCommand ln -s $InstallDir/Binaries/Active/bin
    runCommand ln -s $InstallDir/Binaries/Active/libs
    runCommand pushd conf >/dev/null 2>&1
    runCommand ln -s $InstallDir/Configs/SysConfigs/Active SysConfigs
    runCommand popd >/dev/null 2>&1
    logInfo "Configuring system"
    runCommand echo "$BaseSMTFolder/libs" > /etc/ld.so.conf.d/Targoman.conf
}

function installWebServer(){
    logSection "Installing WebServer"
    logInfo "Installing web extra required packages"
    zypper install libreoffice-pyuno nginx php5-fpm

    logInfo "Creating Base WebServer Symlinks"
    runCommand cd $BaseSMTFolder
    runCommand ln -s /srv/www/vhosts/ WebUI
    runCommand pushd conf >/dev/null 2>&1
    runCommand ln -s $InstallDir/Configs/TargomanApps/Active/TargomanLoadBalancer.ini
    if [ $AlsoTest -eq 1 ]; then
        runCommand ln -s $InstallDir/Configs/TargomanApps/Trunk/TargomanLoadBalancer.ini TargomanLoadBalancer.trunk.ini
    fi
    runCommand popd >/dev/null 2>&1
    logInfo "Creating Service scripts"
    echo -e "[Unit]\n"\
        "Description=Proxifies Translation requests to Targoman Translation servers\n"\
        "After=network.target\n"\
        "\n"\
        "[Service]\n"\
        "ExecStart=$BaseSMTFolder/bin/TargomanLoadBalancer --config $BaseSMTFolder/conf/TargomanLoadBalancer.ini --log-dont-show --out-silent\n"\
        "Restart=always\n"\
        "\n"\
        "[Install]\n"\
        "WantedBy=multi-user.target\n"\
        > $BaseSMTFolder/scripts/services/targomanloadbalancer.service
    sed -i "s/^ //g" $BaseSMTFolder/scripts/services/targomanloadbalancer.service
    runCommand ln -s /sbin/service /usr/sbin/rctargomanloadbalancer

    if [ $AlsoTest -eq 1 ]; then
        echo -e "#!/bin/sh\n"\
            "export LD_LIBRARY_PATH=$InstallDir/Binaries/Trunk/libs\n"\
            "$InstallDir/Binaries/Trunk/bin/TargomanLoadBalancer \$*\n"\
            > $BaseSMTFolder/bin/TargomanLoadBalancer.trunk
        sed -i "s/^ //g" $BaseSMTFolder/bin/TargomanLoadBalancer.trunk
        echo -e "[Unit]\n"\
            "Description=Proxifies Translation requests to Targoman Translation servers for testing. This is the trunk version\n"\
            "After=network.target\n"\
            "\n"\
            "[Service]\n"\
            "ExecStart=$BaseSMTFolder/bin/TargomanLoadBalancer.trunk --config $BaseSMTFolder/conf/TargomanLoadBalancer.trunk.ini --log-dont-show --out-silent\n"\
            "Restart=always\n"\
            "\n"\
            "[Install]\n"\
            "WantedBy=multi-user.target\n"\
            > $BaseSMTFolder/scripts/services/targomanloadbalancer.trunk.service
        sed -i "s/^ //g" $BaseSMTFolder/scripts/services/targomanloadbalancer.trunk.service
        runCommand ln -s /sbin/service /usr/sbin/rctargomanloadbalancer.trunk
    fi

    logInfo "Creating soffice service scripts"
    echo -e "[Unit]\n"\
        "Description=Libre office converter service\n"\
        "After=network.target\n"\
        "\n"\
        "[Service]\n"\
        "ExecStart=/usr/bin/soffice --accept=socket,host=localhost,port=2002;urp; --invisible --headless\n"\
        "Restart=always\n"\
        "\n"\
        "[Install]\n"\
        "WantedBy=multi-user.target\n"\
        > $BaseSMTFolder/scripts/services/soffice.service
    sed -i "s/^ //g" $BaseSMTFolder/scripts/services/soffice.service
    runCommand ln -s /sbin/service /usr/sbin/soffice

    logInfo "Creating logrotate scripts"
    echo -e "$BaseSMTFolder/logs/TargomanLoadBalancer.log {
        notifempty
        maxsize
        rotate 10
        missingok
        compress
        copytruncate
        delaycompress
        nomail
        }
    " > $BaseSMTFolder/scripts/logrotate.d/targomanloadbalancer.lr
    ln -s $BaseSMTFolder/scripts/logrotate.d/targomanloadbalancer.lr /etc/logrotate.d/
    if [ $AlsoTest -eq 1 ]; then
        runCommand cp  $BaseSMTFolder/scripts/logrotate.d/targomanloadbalancer.lr $BaseSMTFolder/scripts/logrotate.d/targomanloadbalancer.trunk.lr
        sed -i "s/\.log/.trunk.log/g"
    fi
    logSection "Finished"
}

function installDatabase(){
    logError "Database installation has not been implemented yet"
}

function installSMTServer(){
    logSection "Installing SMTServer"
    logInfo "Creating Base SMTServer Symlinks"
    runCommand cd $BaseSMTFolder
    runCommand ln -s $InstallDir/Models/Active models
    runCommand pushd conf >/dev/null 2>&1
    runCommand ln -s $InstallDir/Configs/TargomanCommon/Active TargomanCommon
    runCommand ln -s $InstallDir/Configs/TargomanApps/Active/TargomanSMTServer.en2fa.ini
    runCommand ln -s $InstallDir/Configs/TargomanApps/Active/TargomanSMTServer.fa2en.ini
    if [ $AlsoTest -eq 1 ]; then
        runCommand ln -s $InstallDir/Configs/TargomanApps/Trunk/TargomanSMTServer.en2fa.ini TargomanSMTServer.en2fa.trunk.ini
        runCommand ln -s $InstallDir/Configs/TargomanApps/Trunk/TargomanSMTServer.fa2en.ini TargomanSMTServer.fa2en.trunk.ini
        runCommand ln -s $InstallDir/Configs/TargomanApps/Trunk/TargomanSMTConsole.en2fa.ini TargomanSMTConsole.en2fa.trunk.ini
        runCommand ln -s $InstallDir/Configs/TargomanApps/Trunk/TargomanSMTConsole.fa2en.ini TargomanSMTConsole.fa2en.trunk.ini
    fi
    runCommand popd >/dev/null 2>&1
    runCommand pushd bin >/dev/null 2>&1
    runCommand ln -s TargomanSMTServer TargomanSMTServer.master.en2fa
    runCommand ln -s TargomanSMTServer TargomanSMTServer.master.fa2en
    runCommand ln -s TargomanSMTServer TargomanSMTServer.slave.en2fa
    runCommand ln -s TargomanSMTServer TargomanSMTServer.slave.fa2en
    runCommand popd >/dev/null 2>&1
    logInfo "Creating Service scripts"
    echo -e "[Unit]\n"\
        "Description=Targoman English to Persian translation server [master]\n"\
        "After=network.target\n"\
        "\n"\
        "[Service]\n"\
        "ExecStart=$BaseSMTFolder/bin/TargomanSMTServer.master.en2fa --config $BaseSMTFolder/conf/TargomanSMTServer.en2fa.ini --log-dont-show --out-silent\n"\
        "Restart=always\n"\
        "\n"\
        "[Install]\n"\
        "WantedBy=multi-user.target\n"\
        > $BaseSMTFolder/scripts/services/targomansmtserver.master.en2fa.service
    sed -i "s/^ //g" $BaseSMTFolder/scripts/services/targomansmtserver.master.en2fa.service
    runCommand cp $BaseSMTFolder/scripts/services/targomansmtserver.master.en2fa.service $BaseSMTFolder/scripts/services/targomansmtserver.master.fa2en.service
    sed -i -e "s/en2fa/fa2en/g" -e "s/English to Persian/Persian to English/g" $BaseSMTFolder/scripts/services/targomansmtserver.master.fa2en.service
    runCommand cp $BaseSMTFolder/scripts/services/targomansmtserver.master.en2fa.service $BaseSMTFolder/scripts/services/targomansmtserver.slave.en2fa.service
    sed -i "s/master/slave/g" $BaseSMTFolder/scripts/services/targomansmtserver.slave.en2fa.service
    runCommand cp $BaseSMTFolder/scripts/services/targomansmtserver.master.fa2en.service $BaseSMTFolder/scripts/services/targomansmtserver.slave.fa2en.service
    sed -i "s/master/slave/g" $BaseSMTFolder/scripts/services/targomansmtserver.slave.fa2en.service

    runCommand ln -s /sbin/service /usr/sbin/rctargomansmtserver.master.en2fa
    runCommand ln -s /sbin/service /usr/sbin/rctargomansmtserver.master.fa2en
    runCommand ln -s /sbin/service /usr/sbin/rctargomansmtserver.slave.en2fa
    runCommand ln -s /sbin/service /usr/sbin/rctargomansmtserver.slave.fa2en
    runCommand ln -s $BaseSMTFolder/scripts/services/targomansmtserver.master.en2fa.service /usr/lib/systemd/system/
    runCommand ln -s $BaseSMTFolder/scripts/services/targomansmtserver.master.fa2en.service /usr/lib/systemd/system/
    runCommand ln -s $BaseSMTFolder/scripts/services/targomansmtserver.slave.en2fa.service /usr/lib/systemd/system/
    runCommand ln -s $BaseSMTFolder/scripts/services/targomansmtserver.slave.fa2en.service /usr/lib/systemd/system/

    logInfo "Creating logrotate scripts"
    echo -e "$BaseSMTFolder/logs/TargomanSMTServer.en2fa.log {
        notifempty
        maxsize
        rotate 10
        missingok
        compress
        copytruncate
        delaycompress
        nomail
        }
    " > $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.en2fa.lr
    runCommand cp $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.en2fa.lr $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.fa2en.lr
    sed -i "s/en2fa/fa2en/g" $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.fa2en.lr
    ln -s $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.en2fa.lr /etc/logrotate.d/
    ln -s $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.fa2en.lr /etc/logrotate.d/

    if [ $AlsoTest -eq 1 ]; then
        echo -e "#!/bin/sh\n"\
            "export LD_LIBRARY_PATH=$InstallDir/Binaries/Trunk/libs\n"\
            "$InstallDir/Binaries/Trunk/bin/TargomanSMTServer \$*\n"\
            > $BaseSMTFolder/bin/TargomanSMTServer.trunk
        sed -i "s/^ //g" $BaseSMTFolder/bin/TargomanSMTServer.trunk
        runCommand pushd bin >/dev/null 2>&1
        runCommand ln -s TargomanSMTServer.trunk TargomanSMTServer.trunk.en2fa
        runCommand ln -s TargomanSMTServer.trunk TargomanSMTServer.trunk.fa2en
        runCommand popd >/dev/null 2>&1
        echo -e "[Unit]\n"\
            "Description=Proxifies Translation requests to Targoman Translation servers for testing. This is the trunk version\n"\
            "After=network.target\n"\
            "\n"\
            "[Service]\n"\
            "ExecStart=$BaseSMTFolder/bin/TargomanSMTServer.trunk.en2fa --config $BaseSMTFolder/conf/TargomanSMTServer.en2fa.ini --log-dont-show --out-silent\n"\
            "Restart=always\n"\
            "\n"\
            "[Install]\n"\
            "WantedBy=multi-user.target\n"\
            > $BaseSMTFolder/scripts/services/targomansmtserver.trunk.en2fa.service
        sed -i "s/^ //g" $BaseSMTFolder/scripts/services/targomansmtserver.trunk.en2fa.service
        runCommand cp $BaseSMTFolder/scripts/services/targomansmtserver.trunk.en2fa.service $BaseSMTFolder/scripts/services/targomansmtserver.trunk.fa2en.service
        sed -i -e "s/en2fa/fa2en/g" -e "s/English to Persian/Persian to English/g" $BaseSMTFolder/scripts/services/targomansmtserver.trunk.fa2en.service
        runCommand ln -s /sbin/service /usr/sbin/rctargomansmtserver.trunk.en2fa
        runCommand ln -s /sbin/service /usr/sbin/rctargomansmtserver.trunk.fa2en
        runCommand ln -s $BaseSMTFolder/scripts/services/targomansmtserver.trunk.en2fa.service /usr/lib/systemd/system/
        runCommand ln -s $BaseSMTFolder/scripts/services/targomansmtserver.trunk.fa2en.service /usr/lib/systemd/system/

        runCommand cp $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.en2fa.lr $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.trunk.lr
        sed -i "s/en2fa/trunk/g" $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.trunk.lr
        ln -s $BaseSMTFolder/scripts/logrotate.d/targomansmtserver.trunk.lr /etc/logrotate.d/
    fi
    logSection "Finished. Now you must configure and copy model files"
}

function main(){
    AlsoTest=0
    while getopts ":wdstu:g:nkc" o; do
        case "${o}" in
            w)  What2Install="$What2Install WebServer";;
            d)  What2Install="$What2Install Database";;
            s)  What2Install="$What2Install SMTServer";;
            t)  AlsoTest=1;;
            u)  User=${OPTARG};;
            g)  Group=${OPTARG};;
            *)  usage;;
        esac
    done

    shift $((OPTIND-1))

    probeRootPriviledges
    if [ -z "$What2Install" ] ; then  usage; fi

    installBasePackages
    createBasePaths

    for Ins in $What2Install; do
        install$Ins
    done

}

main $*
