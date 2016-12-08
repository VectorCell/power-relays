#!/bin/bash

make

cd ~/bin
ln -s ~/Programs/power-relays/relays

cd -
if [ -e /var/www/gui ]; then
	sudo rm -rf /var/www/gui
fi
sudo rsync -r --progress gui /var/www/
sudo cp pins.conf /var/www/gui/

sudo service apache2 restart
