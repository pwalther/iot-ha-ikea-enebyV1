# Behind Eneby Lines

Problem: 
I wanted to mount an IKEA ENEBY (30) in a difficult to reach spot. The speaker turns off automatically after a while and when it turns on, it does not retain its volume and the volume is also not synced to the bluetooth device (my phone).

Solution:
Attach a NodeMCUv3 (ESP8266). It waits for a message on MQTT and then turns the speaker on and also turns the volume up.

The pins are nicely labeled, power key and volume+/- are the important ones. The volume is a rotary encoder, so the signal needs to be emulated.
Currently the rotary encoder does not work when the ESP8266 is attached - not an issue for me.
