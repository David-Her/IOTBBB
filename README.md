# IOTBBB

## Executable as Service
(Executing a script on startup using BeagleBone Black)
https://stackoverflow.com/questions/28854705/executing-a-script-on-startup-using-beaglebone-black


1. Compile the required code.

2. Create a bash script that will launch the code at boot/ startup

        cd /usr/bin/
        Type: nano scriptname.sh

        #!/bin/bash
        /home/root/name_of_compiled_code


Save and grant execute permission

    chmod u+x /usr/bin/scriptname.sh

3. Create the service

        nano /lib/systemd/scriptname.service

4. Edit the above file as necessary to invoke the different functionalities like network. Enable these only if the code needs that particular service. Disable unwanted ones to decrease boot time.

[Unit]
Description=description of code
After=syslog.target network.target
[Service]
Type=simple
ExecStart=/usr/bin/scriptname.sh
[Install]
WantedBy=multi-user.target

5. Create a symbolic link to let the device know the location of the service.

        cd /etc/systemd/system/
        ln /lib/systemd/scriptname.service scriptname.service

6. Make systemd reload the configuration file, start the service immediately (helps to see if the service is functioning properly) and enable the unit files specified in the command line.

        systemctl daemon-reload
        systemctl start scriptname.service
        systemctl enable scriptname.service

Restart BBB immediately to see if it runs as intended.

7- reboot
(All credit goes to http://mybeagleboneblackfindings.blogspot.com/2013/10/running-script-on-beaglebone-black-boot.html)
