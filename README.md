# kb25
MIDI Keyboard sample sketch for K-25m.

This project is an example of creating MIDI keyboard using Roland K-25m keyboard unit.
The project contains schematic how to connect K-25m to Arduino Uno and a sample sketch.
The sketch scans keys of K-25m and output midi note on/off messages. Velocity is note supported.
I expect this project help music instrument DIYer's to imprement well manufactured tiny musical keyboard with K-25m.

For more info, visit http://r-mono-lab.tumblr.com/products/kb-25 

Note:
This project is not authorized by Roland Corporation. Please don't ask Roland about this project.

# Libraries

This sketch uses MIDI library.

# How to connect K-25m

K-25m equipped 2x8 ribbon cable connector compliant with MIL Standard. So the box type pin header is comfort to connect. I recommend angle type pin header like Hirose HIF3F-16PA-2.54DS. See datasheet at https://www.hirose.co.jp/cataloge_hp/e61500011.pdf for your reference.

