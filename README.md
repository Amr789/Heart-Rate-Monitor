# Heart-Rate-Monitor
A heart rate monitor was designed and implemented using an ECG sensor, that records the electrical
signals that make the heart beat. The ECG signal will be sent to a comparator, to detect the
heartbeats, then scaled and shifted, so we are able to send the signal to a microcontroller
(MSP 430). We will program the microcontroller to count the heartbeats for 16 seconds and
calculate the heart rate in beats per minute. The heart rate will be displayed on a 7 segment
LCD. The heart rate will be updated every 16 seconds, and an LED toggled. Another LED
will be toggled with every heartbeat detected.
