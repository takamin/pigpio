#!/bin/sh
NAME=pedsignal
DSCR=/etc/init.d/${NAME}d
DEXE=/usr/local/sbin/${NAME}
PIDF=/var/run/${NAME}d.pid

sudo ${DSCR} stop

sudo cp ./pedsignald ${DSCR}
sudo chown root.root ${DSCR}
sudo chmod 755 ${DSCR}

sudo cp ../build/pedsignal ${DEXE}
sudo chown root.root ${DEXE}

sudo update-rc.d ${NAME}d defaults

sudo ${DSCR} start
