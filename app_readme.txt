# PHoton Arrival-time STudio (PHAST) - version 0.8 - dec. 2020
# Copyright (c) Stijn Hinterding, Utrecht University
#
# This is open-source software, licensed under the MIT license
# (https://opensource.org/licenses/MIT). The source code
# is available at: https://www.github.com/stijnhinterding/phast

PHAST is an application that can be used to conduct time-
correlated single-photon counting (TCSPC) experiments. It can:

* control time-to-digital converters (TDCs)
* process data
* visualize intensity traces, decay and correlation curves, live
* store the raw data

The program is used on a daily basis for e.g. measurements on
single luminescent nanocrystals (quantum dots) at the lab of
Freddy Rabouw at Utrecht University.


# Usage
Currently PHAST supports only quTools' quTAG and quTAU TDCs.
 Support for other TDCs (e.g. PicoQuant's HydraHarp) can be 
 added as plugins. As the author did not have access to other
 TDCs, only these two are supported.
 
Make sure you have installed the necessary drivers for your
TDC. Then simply run either phast_qutag.bat or phast_qutau.bat.
After choosing "I want to do a measurement", PHAST will try to
detect any TDCs that are connected to the computer, and you
can choose which TDC you would like to connect to.

Before conducting measurements, you should configure the
TDC channels, under the "Channels" menu. This is a two-step
process:

1) Channel addition, and setting how these are related
		via Channels->Overview
2) Setting the voltage threshold
		via Channels->Settings

Step (1) is required in all cases. Step (2) is required only if the
default voltage threshold does not work for your specific
laser driver / detector.

A "fake" time-tag device is supplied for debugging purposes,
and can also be used to practice with PHAST.


# Data storage
 Data saving is controlled by the "data writer" plugin
 (Views->Data writer). Press "Browse", select a file save
 location, and press "Start saving" when you would like to
 start saving. Any data collected from this moment onwards
 is directly written to the output files. To stop saving, simply
 press the "stop" button.
 
 
 # Tips & troubleshooting
 * PHAST supports a software 'sync divider', i.e., it has the option
 to save timestamps of a certain channel only once in every N
 events. It is useful to use this sync divider on a laser sync
 channel, as this will greatly reduce the data size.
 
 * If possible, connect the TDC to your computer using USB3,
 this has a much higher transfer rate than USB2.
 
 * If you get no signal, check if the trigger settings (voltage)
 are correct. If nothing works, check with an oscilloscope.
 
 * It is possible to change the display settings of individual
 windows using the "Plot settings" window.
 
 * If the countrate is too large, the TDC may miss some events.
 PHAST tries to detect this and will emit warnings in the
 terminal window. These warnings should only show up when
 there really is a data loss problem. However, PHAST relies on
 the TDC to communicate that there is a problem, so beware. If
 the countrate is very high (>1M cts/s), it is possible that the
 TDC is overloaded without informing PHAST. If you observe
 strange results (especially apparent as artefacts in cross-
 correlation curves), a TDC-overload is likely. Use your common
 sense.
 
 * Sometimes the connection between PHAST and the TDC can
 hang, especially for extremely large countrates. If this happens,
 simply close and restart PHAST.
 
 * In some cases it may be necessary to turn off live preview of
 FCS curves, or decreasing the maximum lag time, as calculating
 these curves is computationally expensive.
 
 
 # Data format
 To save space, data is saved in a custom binary data format.
 The data can be read in using the libtimetag Python module
 (https://www.github.com/stijnhinterding/libtimetag).
 
 
 # Data format, more details
 The file format, "short simple time-tagged" (SSTT) was
 designed to save storage space. Data of separate channels
 are saved to separate binary files, with the extensions ".sstt.cX",
 where X is the channel number. Each dataset always has a
 plain-text header file with the extension ".sstt".
 
 Inside the binary files, each time-tag event is stored in 6 bytes.
 There are two types of events: timestamps, and overflows.
 When the internal timestamp counter overflows (i.e., becomes
 too large to be represented), an overflow event is
 written, and the internal timestamp counter is reset to zero.
 
 Each time-tag event is stored in 6 bytes. The first two
 bits are reserved, and the remaining 46 bits hold the
 timestamp data, or the number of overflows that occurred, in
 the case of an overflow event. The first bit denotes the event
 type: 1 for an overflow event, 0 for a timestamp. The second
 bit is reserved and currently not used.
 
 
 # Acknowledgements
 This work was supported by The Netherlands Center for 
 Multiscale Catalytic Energy Conversion (MCEC), an NWO 
 Gravitation Programme funded by the Ministry of Education, 
 Culture and Science of the government of The Netherlands.
 