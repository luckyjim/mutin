#!/bin/sh

ssh-agent -s > /tmp/ssh.keys    # pour y mettre les variables environnement
.  /tmp/ssh.keys                # Exporter les variables
rm  /tmp/ssh.keys               # Faire le ménage après
echo "add your private key: ssh-add ~/.ssh/idKeyjmc"

