#pragma once

#include "midi.hpp"

class Application
{
    private:

        int midiOutPort =   0;
        int midiInPort  =   0;
        int numColor    =   0;
        int mode = 0;
        std::shared_ptr<MidiOut>    midiOut =   nullptr;
        std::shared_ptr<MidiIn>     midiIn  =   nullptr;
        bool isPlaying = true;

        void
        InitMIDI(void);

        void
        ColorPicker(std::optional<MidiMessage> message);

        void
        Mario(void);

        void
        Disco(void);

        void
        Touch(std::optional<MidiMessage> message);

        void
        MIDILoop(void);

    public:

        Application(){};
        ~Application(){};

        void
        Init(void);
        
        void
        Run(void);
};
