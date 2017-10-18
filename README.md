# arduino-beat-detection
Beat detection on Arduino using analog mic input

Currently uses the free running mode to sample ~38kHz audio, then a statistical algorithm to determine whether or not the captured audio contains a "beat" based on its energy compared to recent capture energy.
