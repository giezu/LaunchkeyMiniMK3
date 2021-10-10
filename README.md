# Reverse enginneered MIDI implementation of Launchkey Mini MK3 by Novation
## Intro
I've got Launchkey Mini to control my synthesizers and use it as controller for my VJ framework.
Custom modes setup in Novation Components were useful but sometimes I needed more control over Launchkey Mini.
Inspired by [this](https://www.partsnotincluded.com/how-to-control-the-leds-on-a-novation-launchkey-mini-ii/) article I tried to make something similar.
Probably all of this knowledge is available in Launchkey Mini Programmer's Guide, but it's not publicly available.

Companion spreadsheet is available [here](https://docs.google.com/spreadsheets/d/e/2PACX-1vQgwSu7S3ifJUJc8kXHBo6Be1NiIXhUXTK6S_oT_4rPPBQmic8yTu5OKbmn-la32DogcFcIzZE-TvMF/pubhtml).
It contains data about selected midi messages, pad notes and colors.
Especially colors sheet is useful because most of colors don't follow any pattern (or I couldn't see one).

In source code you can find example application and structs for midi communication.
Everything is done in C++ using [RtMidi](https://github.com/thestk/rtmidi) but it can be easily applied to any language and API since it's compatible with MIDI standard.
This project uses C3 as middle-C convention (explained [here](https://computermusicresource.com/midikeys.html)), same as Novation.

Good explanation of MIDI messages is available [here](https://users.cs.cf.ac.uk/Dave.Marshall/Multimedia/node158.html).
Message examples in this tutorial will look like this:
Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
1|Note On|60(C3)|127|Send C3 note on with 127 velocity on channel 1

## Compilation
Standard CMake compilation.
```bash
mkdir build
cd build
cmake ..
make
```

## Program usage
This program is focused purely on DAW mode. When you launch example app you will be asked to select MIDI port of Launchkey Mini MK3.
Choose **Launchkey Mini MK3 MIDI 2** for both input and output.
This port is responsible for passing data in so-called DAW mode.
If you need midi data from piano keys, custom mode knobs and pads, pitch bend and modulation then you should connect to port **Launchkey Mini MK3 MIDI 1**, not supported by this example code.

After that you will be asked to pick one of following modes:
* Color checker - simple mode which changes colors of all pads in session mode. You can change colors by pressing up and down arrow on Launchkey Mini.
* Mario - simple animation showing Mario
* Disco - each pad has randomized color from spectrum, changing every few seconds
* Touch - randomized color each time you press a pad. You can change color mode by pressing up and down arrows, choosing normal, flashing or pulsing color.
Press play button on Launchkey to safely exit application.

## MIDI implementation
### DAW mode
First after opening midi port you have to enter DAW mode to gain more control over Launchkey Mini.
This is done by sending Note On status on channel 16, note C-1 and velocity 127.
If you want to exit DAW mode (probably on application quit) you have to send the same message but with 0 velocity.

Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
16|Note On|12(C-1)|127|Enter DAW mode
16|Note On|12(C-1)|0|Exit DAW mode

### Buttons
Some special buttons send CC messages which we can handle by getting following messages:

Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
1|CC|104(G#6)|127/0|Up button press/release
1|CC|105(A6)|127/0|Down button press/release
1|CC|108(C7)|127/0|Shift button press/release
16|CC|103(G6)|127/0|Left button press/release
16|CC|102(F#6)|127/0|Right button press/release
16|CC|115(G7)|127/0|Play button press/release
16|CC|117(A7)|127/0|Record button press/release

Shift + Octave buttons sends Program Change message
Channel|Status|Data1|Info
-------|------|-----|----
1|Program Change|n|Sends program number 'n'

### Pad modes
You can change pad mode by sending CC message on channel 16.
It's the same thing as changing modes by holding shift and choosing mode with orange pads.

edit 27.09.2021: as found by [riban-bw](https://github.com/riban-bw) there are few more undocumented modes.

Modes 6-8 replace Custom mode as found [here](https://github.com/giezu/LaunchkeyMiniMK3/issues/1) until you reboot Launchkey Mini MK3.

Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
16|CC|3(D#-2)|1|Drum pad mode
16|CC|3(D#-2)|2|Session pad mode
16|CC|3(D#-2)|5|Custom pad mode
16|CC|3(D#-2)|6|Drum 2 pad mode
16|CC|3(D#-2)|7|Toggle pad mode
16|CC|3(D#-2)|8|Program Change pad mode

Device should reply with the same message.
Keep in mind that custom mode requires listening on **Launchkey Mini MK3 MIDI 1** port.
**Session** mode pads send note data on **channel 1**.
**Drum** mode pads send note data on **channel 10**.

### Knob modes
You can change knob mode by sending CC message on channel 16.
It's the same thing as changing modes by holding shift and choosing mode with cyan pads.
Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
16|CC|9(A-2)|1|Device knob mode
16|CC|9(A-2)|2|Volume knob mode
16|CC|9(A-2)|3|Pan knob mode
16|CC|9(A-2)|4|Sends A knob mode
16|CC|9(A-2)|5|Sends B knob mode
16|CC|9(A-2)|6|Custom knob mode

Device should reply with the same message.
Keep in mind that custom mode requires listening on **Launchkey Mini MK3 MIDI 1** port.
Besides custom mode, other knob modes will always send CC message on channel 16, with data1 as knob number 21-28 (from left to right) and data2 as knob value (0-127).

### Color time!
Coloring pads was the most time consuming thing.
There are 128 colors available and writing down all of them wasn't easy.
Colors 0-59 follow easy pattern, but the rest looks randomized.
You can choose color mode by sending message to different channels.

Channels 1, 2, 3 are used in **session** mode, for solid, flashing and pulsing pad respectively.

Channels 10, 11, 12 are used in **drum** mode, for solid, flashing and pulsing pad respectively.

Examples:
Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
**1**|Note On|96(C6)|9|Color top-left pad with **solid** orange in **session** mode
**2**|Note On|96(C6)|21|Color top-left pad with **flashing** green in **session** mode
**3**|Note On|103(G6)|49|Color top-right pad with **pulsing** purple in **session** mode
**10**|Note On|96(C6)|9|Color top-left pad with **solid** orange in **drum** mode
**11**|Note On|96(C6)|9|Color top-left pad with **flashing** orange in **drum** mode
**12**|Note On|96(C6)|9|Color top-left pad with **pulsing** orange in **drum** mode

Bonus: flashing pad toggles between previous color and current (flashing) color. This way you can send solid color followed by flashing color to flash between two different colors e.g. green/red/green/red... If you want to flash between disabled and color led you have to send black (0) solid color first.

### Other buttons (by [riban-bw](https://github.com/riban-bw))
You can color both Up and Down buttons by sending CC message on channel 1, 2, 3 (same as **session** color modes).

Examples:
Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
**1**|CC|104(G#6)|9|Color Up pad with **solid** orange color
**2**|CC|105(A6)|21|Color Down pad with **flashing** green color

You can change **brightness** of Up, Down, Play, Pause buttons by sending CC message on channel 16.
Possible brightness values: 0%, 25%, 50%, 75% and 100%

Examples:
Channel|Status|Data1|Data2|Info
-------|------|-----|-----|----
16|CC|104(G#6)|32|Up pad with 25% brightness
16|CC|105(A6)|48|Down pad with 50% brightness
16|CC|115(G7)|64|Play button with 75% brightness
16|CC|117(A7)|96|Record button with 100% brightness
