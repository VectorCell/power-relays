#!/bin/bash

make

cd ~/bin
ln -s ~/Programs/power-relays/relays

cd -
if [ -e /var/www/html/gui ]; then
	sudo rm -rf /var/www/html/gui
fi
sudo rsync -r --progress gui /var/www/html/
sudo cp pins.conf /var/www/html/gui/

sudo service apache2 restart
